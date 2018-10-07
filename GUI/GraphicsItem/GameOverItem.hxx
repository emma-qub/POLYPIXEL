#ifndef GAMEOVERITEM_HXX
#define GAMEOVERITEM_HXX

#include <QObject>
#include <QGraphicsRectItem>

#include "Core/Polygon.hxx"

class GameOverItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~GameOverItem() override;

  void Open(QPointF const& p_startPos, QPointF const& p_endPos);
  void SetAreasData(QList<ppxl::Polygon*> p_polygonsList, QList<double> const& p_areas, ppxl::Point const& p_figureCenter);
  void DrawResult();

private:
  QList<ppxl::Polygon*> m_polygonsList;
  QList<double> m_areas;
  ppxl::Point m_figureCenter;
};

#endif
