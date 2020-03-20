#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include <QObject>

#include "GUI/Controllers/GameController.hxx"

class PolygonModel;
class ObjectModel;
class TestLevelWidget;

class TestLevelController: public QObject {
  Q_OBJECT

public:
  explicit TestLevelController(TestLevelWidget* p_view, QObject* p_parent = nullptr);

  void SetPolygonModel(PolygonModel* p_polygonModel);
  void SetObjectModelsList(const QList<ObjectModel*>& p_objectModelList);
  void SetLinesGoal(int p_linesGoal);
  void SetPartsGoal(int p_partsGoal);
  void SetMaxGapToWin(int p_maxGapToWin);
  void SetTolerance(int p_tolerance);
  void PlayLevel();

private:
  TestLevelWidget* m_view;
};

#endif
