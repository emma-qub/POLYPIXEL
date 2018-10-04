#include "GameStartItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "GUI/GraphicsItem/GraphicsStarsItem.hxx"
#include "GUI/GraphicsItem/GraphicsGoalItem.hxx"



#include <QDebug>




CloseItem::CloseItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent) {

  //setAcceptHoverEvents(true);

  setBrush(QColor("#ffffff"));
}

CloseItem::~CloseItem() = default;

void CloseItem::hoverEnterEvent(QGraphicsSceneHoverEvent* p_event) {
  Q_UNUSED(p_event)
  setBrush(QColor("#e63939"));
}

void CloseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* p_event) {
  Q_UNUSED(p_event)
  setBrush(QColor("#ffffff"));
}



PlayItem::PlayItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent) {

  //setAcceptHoverEvents(true);

  setBrush(QColor("#ffffff"));
}

PlayItem::~PlayItem() = default;

void PlayItem::hoverEnterEvent(QGraphicsSceneHoverEvent* p_event) {
  Q_UNUSED(p_event)
  setBrush(QColor("#39e650"));
}

void PlayItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* p_event) {
  Q_UNUSED(p_event)
  setBrush(QColor("#ffffff"));
}



GameStartItem::GameStartItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_levelNumberItem(new QGraphicsSimpleTextItem(this)),
  m_levelLinesGoalItem(nullptr),
  m_levelPartsGoalItem(nullptr),
  m_levelStarsItem(nullptr),
  m_closeRectItem(new CloseItem(0, 0, p_width/4, p_height/4, this)),
  m_playRectItem(new PlayItem(0, 0, 3*p_width/4, p_height/4, this)),
  m_pen(QPen(QColor("#cccccc"))),
  m_startPos(),
  m_endPos(),
  m_font(":/fonts/PICOPIXEL.ttf", 36) {

  setBrush(QBrush(QColor("#ffffff")));

  m_closeRectItem->setPen(m_pen);
  m_closeRectItem->setPos(0, 3*p_height/4);

  m_playRectItem->setPen(m_pen);
  m_playRectItem->setPos(p_width/4, 3*p_height/4);

  auto playButton = new QPushButton("Let's go");
  playButton->setFlat(true);
  playButton->setStyleSheet("font-family: ':/fonts/PICOPIXEL.ttf';font-style: normal;font-size: 36px;");
  auto playRectItemRect = m_playRectItem->rect();
  playButton->setFixedSize(static_cast<int>(playRectItemRect.width())/2, static_cast<int>(playRectItemRect.height())/2);
  auto proxyPlayItem = new QGraphicsProxyWidget(this);
  proxyPlayItem->setWidget(playButton);
  proxyPlayItem->setPos(mapFromItem(m_playRectItem, m_playRectItem->rect().center()) - proxyPlayItem->rect().center());

  connect(playButton, &QPushButton::clicked, this, &GameStartItem::CloseToPlay);
}

void GameStartItem::SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax) {
  m_levelNumberItem->setFont(m_font);
  m_levelNumberItem->setText(tr("Level %1").arg(p_levelNumber));
  auto shiftX = rect().center().x() - m_levelNumberItem->boundingRect().center().x();
  m_levelNumberItem->setPos(shiftX, 0);

  m_font.setPixelSize(18);

  m_levelLinesGoalItem = new GraphicsGoalItem(QPixmap(":/sprites/level/lines.png"), QString::number(p_linesGoal), m_font, this);
  m_levelLinesGoalItem->setPos(rect().width()/2.-(1./2.+1./3.)*m_levelLinesGoalItem->boundingRect().width(), m_levelNumberItem->boundingRect().height()+32.);

  m_levelPartsGoalItem = new GraphicsGoalItem(QPixmap(":/sprites/level/parts.png"), QString::number(p_partsGoal), m_font, this);
  m_levelPartsGoalItem->setPos(rect().width()/2.+(1./2.+1./3.)*m_levelPartsGoalItem->boundingRect().width(), m_levelNumberItem->boundingRect().height()+32.);

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

void GameStartItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* p_event) {
  qDebug() << scene()->itemAt(p_event->scenePos(), QTransform());
}
