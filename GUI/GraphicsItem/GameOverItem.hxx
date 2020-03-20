#ifndef GAMEOVERITEM_HXX
#define GAMEOVERITEM_HXX

#include "Core/Geometry/Polygon.hxx"
#include "GUI/GraphicsItem/GraphicsRoundedRectItem.hxx"

class GameOverItem: public GraphicsRoundedRectItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radius, QGraphicsItem *p_parent = nullptr);
  ~GameOverItem() override;

  void Open(QPointF const& p_startPos, QPointF const& p_endPos);
  void SetAreasData(QList<ppxl::Polygon*> p_polygonsList, QList<double> const& p_areas, ppxl::Point const& p_figureCenter);
  void SetLevelData(int m_linesCount, int m_linesGoal, int m_partsCount, int m_partsGoal, int m_starsMax);
  void DrawResult();

  void CloseToReplay();
  void CloseToMap(bool p_moveToNextLevel);

Q_SIGNALS:
  void ReplayRequested();
  void GoToMapRequested(bool p_moveToNextLevel);

private:
  QList<ppxl::Polygon*> m_polygonsList;
  QList<double> m_areas;
  ppxl::Point m_figureCenter;

  QGraphicsRectItem* m_overlayItem;
  QPointF m_startPos;
  QPointF m_endPos;

  int m_levelNumber;
  int m_linesCount;
  int m_linesGoal;
  int m_partsCount;
  int m_partsGoal;
  int m_starsMax;
};

#endif
