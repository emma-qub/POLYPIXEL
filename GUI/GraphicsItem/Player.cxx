#include "PlayerItem.hxx"

#include <QTimer>
#include <QKeyEvent>



#include <QDebug>




Player::Player(QGraphicsItem* p_parent):
  QGraphicsPixmapItem(p_parent),
  m_animationStep(1) {

  setFlag(QGraphicsItem::ItemIsFocusable);

  setPixmap(QPixmap(":/sprites/player1.png"));

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this](){
    m_animationStep = (m_animationStep+1)%8+1;
    setPixmap(QPixmap(QString(":/sprites/player")+QString::number(m_animationStep)+QString(".png")));
  });

  timer->start(180);
}

Player::~Player() = default;

void Player::keyPressEvent(QKeyEvent* p_event) {
  if (p_event->key() == Qt::Key_Left) {
    moveBy(-10, 0);
  } else if (p_event->key() == Qt::Key_Right) {
    moveBy(10, 0);
  } else if (p_event->key() == Qt::Key_Up) {
    moveBy(0, -10);
  } else if (p_event->key() == Qt::Key_Down) {
    moveBy(0, 10);
  }
}
