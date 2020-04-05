#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include "Core/Slicer.hxx"

#include <QObject>
#include <QColor>

class TestLevelWidget;
class Object;
class GraphicsPolygonItem;
class QMouseEvent;

namespace ppxl {
class Polygon;
}

class TestLevelController: public QObject {
  Q_OBJECT

public:
  explicit TestLevelController(TestLevelWidget* p_testLevelWidget, QObject* p_parent = nullptr);

  void SetLinesGoal(int LinesGoal);
  void SetPartsGoal(int PartsGoal);
  void SetMaxGapToWin(int MaxGapToWin);
  void SetTolerance(int Tolerance);
  void InitPolygonsList(std::vector<ppxl::Polygon*> const& p_polygonsList);
  void SetPolygonsList(std::vector<ppxl::Polygon> const& p_polygonsList);
  void SetObjectModelsList(const std::vector<Object*>& p_objectsList);
  void PlayLevel();

  void MousePressEvent(QMouseEvent* p_event);
  void MouseMoveEvent(QMouseEvent* p_event);
  void MouseReleaseEvent(QMouseEvent* p_event);

Q_SIGNALS:

protected:
  void SetPolygonItems();
  void SetObjectItems();

private:
  TestLevelWidget* m_testLevelWidget;
  std::vector<ppxl::Polygon> m_polygonsList;
  std::vector<Object*> m_objectsList;
  Slicer m_slicer;
  std::vector<GraphicsPolygonItem*> m_graphicsPolygonItemsList;
  QColor m_polygonsColor;
  bool m_colorPicked;
};

#endif
