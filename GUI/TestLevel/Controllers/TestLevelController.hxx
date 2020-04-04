#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include <QObject>

class TestLevelWidget;

class TestLevelController: public QObject {
  Q_OBJECT

public:
  explicit TestLevelController(TestLevelWidget* p_testLevelWidget, QObject* p_parent = nullptr);

  void SetLinesGoal(int LinesGoal);
  void SetPartsGoal(int PartsGoal);
  void SetMaxGapToWin(int MaxGapToWin);
  void SetTolerance(int Tolerance);
  //void SetPolygonModel(std::vector<ppxl::Polygon*> const& p_polygonsList);
  //void SetObjectModelsList(std::vector<Object*> const& p_objectsList);
  void PlayLevel();


Q_SIGNALS:

};

#endif
