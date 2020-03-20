#include "PlayerItem.hxx"

#include <QTimer>
#include <QKeyEvent>

PlayerItem::PlayerItem(QGraphicsItem* p_parent):
  QGraphicsPixmapItem(p_parent),
  m_animationStep(1) {

  setFlag(QGraphicsItem::ItemIsFocusable);

  setPixmap(QPixmap(":/sprites/player/player1.png"));

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this](){
    m_animationStep = (m_animationStep+1)%8+1;
    setPixmap(QPixmap(QString(":/sprites/player/player")+QString::number(m_animationStep)+QString(".png")));
  });

  timer->start(180);
}

PlayerItem::~PlayerItem() = default;

void PlayerItem::keyPressEvent(QKeyEvent* p_event) {
  switch (p_event->key()) {
  case Qt::Key_Left: {
    Q_EMIT Move(eLeft);
    break;
  } case Qt::Key_Up: {
    Q_EMIT Move(eUp);
    break;
  } case Qt::Key_Right: {
    Q_EMIT Move(eRight);
    break;
  } case Qt::Key_Down: {
    Q_EMIT Move(eDown);
    break;
  } case Qt::Key_Return: {
    Q_EMIT Enter();
    break;
  } default:
    break;
  }
}
