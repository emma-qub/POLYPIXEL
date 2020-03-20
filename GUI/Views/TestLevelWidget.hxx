#ifndef TESTLEVELWIDGET_HXX
#define TESTLEVELWIDGET_HXX

#include <QWidget>

class PolygonModel;
class ObjectModel;
class TestingWidget;
class TestingController;
class QLabel;
class QPushButton;

class TestLevelWidget: public QWidget {
  Q_OBJECT

public:
  explicit TestLevelWidget(QWidget* p_parent = nullptr);

  void InitView();

  void SetPolygonModel(PolygonModel* p_model);
  void SetObjectModelsList(QList<ObjectModel*> const& p_objectModelsList);
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
  TestingWidget* m_view;
  TestingController* m_controller;
};

#endif
