#include "GameStartItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsOpacityEffect>

#include "GUI/GraphicsItem/GraphicsStarsItem.hxx"
#include "GUI/GraphicsItem/GraphicsGoalItem.hxx"



#include <QDebug>




GameStartItem::GameStartItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radius, QGraphicsItem* p_parent):
  QObject(),
  GraphicsRoundedRectItem(p_x, p_y, p_width, p_height, p_radius, p_radius, p_parent),
  m_levelNumberItem(nullptr),
  m_levelLinesGoalItem(nullptr),
  m_levelPartsGoalItem(nullptr),
  m_levelStarsItem(nullptr),
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

void GameStartItem::SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax) {
  m_levelNumberItem = new QGraphicsSimpleTextItem(this);
  m_levelNumberItem->setFont(m_font);
  m_levelNumberItem->setText(tr("Level %1").arg(p_levelNumber));
  auto shiftX = rect().center().x() - m_levelNumberItem->boundingRect().center().x();
  m_levelNumberItem->setPos(shiftX, 10);

  m_font.setPixelSize(18);

  m_levelLinesGoalItem = new GraphicsGoalItem(QPixmap(":/sprites/level/lines.png"), QString::number(p_linesGoal), m_font, this);
  m_levelLinesGoalItem->setPos(rect().width()/2.-5.*m_levelLinesGoalItem->boundingRect().width()/4., m_levelNumberItem->boundingRect().height()+32.);

  m_levelPartsGoalItem = new GraphicsGoalItem(QPixmap(":/sprites/level/parts.png"), QString::number(p_partsGoal), m_font, this);
  m_levelPartsGoalItem->setPos(rect().width()/2.+m_levelLinesGoalItem->boundingRect().width()/4., m_levelNumberItem->boundingRect().height()+32.);

  m_levelStarsItem = new GraphicsStarsItem(p_starsMax, this);
  m_levelStarsItem->setPos(rect().center() - m_levelStarsItem->boundingRect().center());
}

GameStartItem::~GameStartItem() = default;

void GameStartItem::Open(QPointF const& p_startPos, QPointF const& p_endPos) {
  m_startPos = p_startPos;
  m_endPos = p_endPos;

  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(m_startPos);
  animation->setEndValue(m_endPos);
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::OutBack);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameStartItem::CloseToPlay() {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(m_endPos);
  animation->setEndValue(QPointF(m_endPos.x(), scene()->height()));
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::InBack);

  connect(animation, &QPropertyAnimation::finished, this, &GameStartItem::StartLevelRequested);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameStartItem::CloseToCancel() {
  auto effect = new QGraphicsOpacityEffect(parent());
  this->setGraphicsEffect(effect);

  auto animation = new QPropertyAnimation(effect, "opacity", this);
  animation->setStartValue(1.);
  animation->setEndValue(0.);
  animation->setDuration(250);

  connect(animation, &QPropertyAnimation::finished, this, &GameStartItem::CancelLevelRequested);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameStartItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* p_event) {
  qDebug() << scene()->itemAt(p_event->scenePos(), QTransform());
}
