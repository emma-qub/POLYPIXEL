#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include <QObject>

#include "GUI/Controllers/GameController.hxx"

class PolygonModel;
class TestLevelView;

class TestLevelController: public QObject {
  Q_OBJECT

public:
  explicit TestLevelController(TestLevelView* p_view, QObject* p_parent = nullptr);

  void SetModel(PolygonModel* p_model);
  void SetLinesGoal(int p_linesGoal);
  void SetPartsGoal(int p_partsGoal);
  void SetMaxGapToWin(int p_maxGapToWin);
  void SetTolerance(int p_tolerance);
  void PlayLevel();

private:
  TestLevelView* m_view;
};

#endif
