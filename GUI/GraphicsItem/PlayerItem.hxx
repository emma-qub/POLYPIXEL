#ifndef PLAYER_HXX
#define PLAYER_HXX

#include <QObject>
#include <QGraphicsPixmapItem>

class Player: public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

public:
  Player(QGraphicsItem* p_parent = nullptr);
  ~Player() override;

  void keyPressEvent(QKeyEvent* p_event) override;

private:
  int m_animationStep;
};

#endif
