#ifndef GAMEOVERITEM_HXX
#define GAMEOVERITEM_HXX

#include <QObject>
#include <QGraphicsRectItem>

class GameOverItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~GameOverItem() override;

  void Open(QPointF const& p_startPos, QPointF const& p_endPos);
};

#endif
