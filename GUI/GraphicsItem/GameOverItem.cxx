#include "GameOverItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>

GameOverItem::GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent) {

  setBrush(QBrush(QColor("#ffffff")));
}

GameOverItem::~GameOverItem() = default;

void GameOverItem::Open(QPointF const& p_startPos, QPointF const& p_endPos) {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(p_startPos);
  animation->setEndValue(p_endPos);
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::OutBack);

  connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);

  animation->start();
}
