#ifndef TESTLEVELVIEW_HXX
#define TESTLEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;
class PolygonModel;
class PlayingView;
class PlayingController;

class TestLevelView: public QWidget
{
  Q_OBJECT
public:
  explicit TestLevelView(QWidget* p_parent = nullptr);

  void SetModel(PolygonModel* p_model);
  void SetLinesGoal(int p_linesGoal);
  void SetPartsGoal(int p_partsGoal);
  void SetMaxGapToWin(int p_maxGapToWin);
  void SetTolerance(int p_tolerance);

signals:
  void Done();
  void AmendLevelRequested();
  void SaveLevelRequested();

public slots:

private:
  QLabel* m_testLevelLabel;
  QPushButton* m_menuButton;
  QPushButton* m_amendLevelButton;
  QPushButton* m_saveLevelButton;
  PlayingView* m_playingView;
  PlayingController* m_playingController;
};

#endif
