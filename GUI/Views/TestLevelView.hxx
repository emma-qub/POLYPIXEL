#ifndef TESTLEVELVIEW_HXX
#define TESTLEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;
class PolygonModel;
class TestingView;
class TestingController;

class TestLevelView: public QWidget {
  Q_OBJECT

public:
  explicit TestLevelView(QWidget* p_parent = nullptr);

  void InitView();

  void SetModel(PolygonModel* p_model);
  void SetLinesGoal(int p_linesGoal);
  void SetPartsGoal(int p_partsGoal);
  void SetMaxGapToWin(int p_maxGapToWin);
  void SetTolerance(int p_tolerance);
  void PlayLevel();

signals:
  void Done();
  void AmendLevelRequested();

private:
  QLabel* m_testLevelLabel;
  QPushButton* m_menuButton;
  QPushButton* m_amendLevelButton;
  TestingView* m_view;
  TestingController* m_controller;
};

#endif
