#ifndef PLAYER_HXX
#define PLAYER_HXX

#include <QObject>
#include <QGraphicsItem>

class PlayerItem: public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  enum Direction {
    eRight,
    eUp,
    eLeft,
    eDown,
    eNone
  };

  PlayerItem(QGraphicsItem* p_parent = nullptr);
  ~PlayerItem() override;

  void keyPressEvent(QKeyEvent* p_event) override;

signals:
  void Move(Direction p_direction);
  void Enter();

private:
  int m_animationStep;
};

#endif
