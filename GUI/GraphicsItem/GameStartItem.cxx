#include "GameStartItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>

#include "GUI/GraphicsItem/GraphicsStarsItem.hxx"
#include "GUI/GraphicsItem/GraphicsGoalItem.hxx"

GameStartItem::GameStartItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radius, QGraphicsItem* p_parent):
  GraphicsRoundedRectItem(p_x, p_y, p_width, p_height, p_radius, p_radius, p_parent),
  m_levelNumberItem(nullptr),
  m_levelLinesGoalItem(nullptr),
  m_levelPartsGoalItem(nullptr),
  m_levelStarsItem(nullptr),
  m_overlayItem(nullptr),
  m_startPos(),
  m_endPos(),
  m_font(":/fonts/PICOPIXEL.ttf", 36) {

  setBrush(QBrush(QColor("#ffffff")));
  setPen(Qt::NoPen);

  QRectF closeRect(0, 3.*p_height/4., p_width/4., p_height/4.);
  QRectF playRect(p_width/4., 3.*p_height/4., 3.*p_width/4., p_height/4.);

  auto topRect = new GraphicsTopRoundedRectItem(0, 0, p_width, p_height/3., p_radius, p_radius, this);
  topRect->setBrush(QBrush(QColor("#993aef")));
  topRect->setPen(Qt::NoPen);

  auto bottomCloseRect = new GraphicsBottomLeftRoundedRectItem(0, 0, p_width/4., p_height/4., p_radius, p_radius, this);
  bottomCloseRect->setBrush(QBrush(QColor("#c95489")));
  bottomCloseRect->setPen(Qt::NoPen);
  bottomCloseRect->setPos(closeRect.topLeft());

  auto bottomPlayRect = new GraphicsBottomRightRoundedRectItem(0, 0, 3.*p_width/4., p_height/4., p_radius, p_radius, this);
  bottomPlayRect->setBrush(QBrush(QColor("#ef3a89")));
  bottomPlayRect->setPen(Qt::NoPen);
  bottomPlayRect->setPos(playRect.topLeft());

  QString buttonStyle =
    "QPushButton {"
    "  font-family: ':/fonts/PICOPIXEL.ttf';"
    "  font-style: normal;"
    "  font-size: 36px;"
    "}";
  auto cancelButton = new QPushButton("X");

  auto playButton = new QPushButton("Let's play");
  playButton->setFlat(true);
  playButton->setStyleSheet(buttonStyle);
  playButton->setFixedSize(static_cast<int>(2.*playRect.width()/3.), static_cast<int>(playRect.height()/2.));
  auto proxyPlayItem = new QGraphicsProxyWidget(this);
  proxyPlayItem->setWidget(playButton);
  proxyPlayItem->setPos(playRect.center() - proxyPlayItem->rect().center());

  connect(playButton, &QPushButton::clicked, this, &GameStartItem::CloseToPlay);

  cancelButton->setFlat(true);
  cancelButton->setStyleSheet(buttonStyle);
  cancelButton->setFixedSize(static_cast<int>(closeRect.height())/2, static_cast<int>(closeRect.height())/2);
  auto proxyCancelItem = new QGraphicsProxyWidget(this);
  proxyCancelItem->setWidget(cancelButton);
  proxyCancelItem->setPos(closeRect.center() - proxyCancelItem->rect().center());

  connect(cancelButton, &QPushButton::clicked, this, &GameStartItem::CloseToCancel);
}

GameStartItem::~GameStartItem() = default;

void GameStartItem::InitItems() {
  m_levelNumberItem = new QGraphicsSimpleTextItem(this);
  m_font.setPixelSize(18);
  m_levelNumberItem->setFont(m_font);
  m_levelLinesGoalItem = new GraphicsGoalItem(m_font, this);
  m_levelPartsGoalItem = new GraphicsGoalItem(m_font, this);
  m_levelStarsItem = new GraphicsStarsItem(this);
  m_overlayItem = scene()->addRect(0, 0, scene()->width(), scene()->height(), QPen(Qt::NoPen), QBrush(Qt::black));
  m_overlayItem->setZValue(-1);
  m_overlayItem->hide();
}

void GameStartItem::SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax) {
  m_levelNumberItem->setText(tr("Level %1").arg(p_levelNumber));
  auto shiftX = rect().center().x() - m_levelNumberItem->boundingRect().center().x();
  m_levelNumberItem->setPos(shiftX, 10);

  m_levelLinesGoalItem->SetPixmap(QPixmap(":/sprites/level/lines.png"));
  m_levelLinesGoalItem->SetGoalNumber(p_linesGoal);
  m_levelLinesGoalItem->setPos(rect().width()/2.-5.*m_levelLinesGoalItem->boundingRect().width()/4., m_levelNumberItem->boundingRect().height()+32.);

  m_levelPartsGoalItem->SetPixmap(QPixmap(":/sprites/level/parts.png"));
  m_levelPartsGoalItem->SetGoalNumber(p_partsGoal);
  m_levelPartsGoalItem->setPos(rect().width()/2.+m_levelLinesGoalItem->boundingRect().width()/4., m_levelNumberItem->boundingRect().height()+32.);

  m_levelStarsItem->SetStarsCount(p_starsMax);
  m_levelStarsItem->setPos(rect().center() - m_levelStarsItem->boundingRect().center());
}

void GameStartItem::Open(QPointF const& p_startPos, QPointF const& p_endPos) {
  show();
  m_overlayItem->show();

  m_startPos = p_startPos;
  m_endPos = p_endPos;

  // Move
  auto moveAnimation = new QPropertyAnimation(this, "pos", this);
  moveAnimation->setStartValue(m_startPos);
  moveAnimation->setEndValue(m_endPos);
  moveAnimation->setDuration(500);
  moveAnimation->setEasingCurve(QEasingCurve::OutBack);

  // Fade in
  auto effect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(effect);
  auto* fadeInAnimation = new QPropertyAnimation(effect, "opacity", this);
  fadeInAnimation->setStartValue(0.);
  fadeInAnimation->setEndValue(0.5);
  fadeInAnimation->setDuration(200);

  // Group animation
  auto animation = new QParallelAnimationGroup(this);
  animation->addAnimation(moveAnimation);
  animation->addAnimation(fadeInAnimation);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameStartItem::CloseToPlay() {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(m_endPos);
  animation->setEndValue(QPointF(m_endPos.x(), scene()->height()));
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::InBack);

  connect(animation, &QPropertyAnimation::finished, this, &GameStartItem::StartLevelRequested);
  connect(animation, &QPropertyAnimation::finished, this, [this](){hide();m_overlayItem->hide();});

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameStartItem::CloseToCancel() {
  // Fade Out dialog
  auto dialogEffect = new QGraphicsOpacityEffect(parent());
  this->setGraphicsEffect(dialogEffect);
  auto dialogAnimation = new QPropertyAnimation(dialogEffect, "opacity", this);
  dialogAnimation->setStartValue(1.);
  dialogAnimation->setEndValue(0.);
  dialogAnimation->setDuration(250);

  // Fade Out overlay
  auto overlayEffect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(overlayEffect);
  auto* overlayAnimation = new QPropertyAnimation(overlayEffect, "opacity", this);
  overlayAnimation->setStartValue(0.5);
  overlayAnimation->setEndValue(0.);
  overlayAnimation->setDuration(250);

  // Group animation
  auto animation = new QParallelAnimationGroup(this);
  animation->addAnimation(dialogAnimation);
  animation->addAnimation(overlayAnimation);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
  connect(animation, &QPropertyAnimation::finished, this, &GameStartItem::CancelLevelRequested);
  connect(animation, &QPropertyAnimation::finished, this, [this](){hide();m_overlayItem->hide();});

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}
