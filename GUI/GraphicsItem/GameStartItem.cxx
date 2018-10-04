#include "GameStartItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>




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
  m_levelGoalsItem(new QGraphicsSimpleTextItem(this)),
  m_levelStarsItem(new QGraphicsSimpleTextItem(this)),
  m_closeRectItem(new CloseItem(0, 0, p_width/2, p_height/4, this)),
  m_playRectItem(new PlayItem(0, 0, p_width/2, p_height/4, this)),
  m_pen(QPen(QColor("#cccccc"))),
  m_startPos(),
  m_endPos() {

  setBrush(QBrush(QColor("#ffffff")));

  m_closeRectItem->setPen(m_pen);
  m_closeRectItem->setPos(0, 3*p_height/4);

  m_playRectItem->setPen(m_pen);
  m_playRectItem->setPos(p_width/2, 3*p_height/4);

  auto playButton = new QPushButton("GO");
  auto proxyPlayItem = new QGraphicsProxyWidget(this);
  proxyPlayItem->setWidget(playButton);
  proxyPlayItem->setPos(mapFromItem(m_playRectItem, m_playRectItem->rect().center()) - proxyPlayItem->rect().center());

  connect(playButton, &QPushButton::clicked, this, &GameStartItem::CloseToPlay);
}

void GameStartItem::SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax) {
  m_levelNumberItem->setText(tr("Level %1").arg(p_levelNumber));
  auto shiftX = rect().center().x() - m_levelNumberItem->boundingRect().center().x();
  m_levelNumberItem->setPos(shiftX, 0);

  m_levelGoalsItem->setText(tr("Lines: %1 Parts: %2").arg(p_linesGoal).arg(p_partsGoal));
  shiftX = rect().center().x() - m_levelGoalsItem->boundingRect().center().x();
  m_levelGoalsItem->setPos(shiftX, m_levelNumberItem->boundingRect().height());

  QString starsString;
  for (int starCount = 1; starCount <= p_starsMax; ++starCount) {
    starsString += "*";
  }
  for (int dotCount = p_starsMax+1; dotCount <= 3; ++dotCount) {
    starsString += ".";
  }
  m_levelStarsItem->setText(tr("Stars %1").arg(starsString));
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

  connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);

  animation->start();
}

void GameStartItem::CloseToPlay() {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(m_endPos);
  animation->setEndValue(QPointF(m_endPos.x(), scene()->height()));
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::InBack);

  connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);
  connect(animation, &QPropertyAnimation::finished, this, &GameStartItem::StartLevelRequested);

  animation->start();
}

void GameStartItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* p_event) {
  qDebug() << scene()->itemAt(p_event->scenePos(), QTransform());
}
