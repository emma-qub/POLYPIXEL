#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QStateMachine>

class AchievementsView;
class CreateLevelModel;
class CreateLevelView;
class CreateLevelController;
class GameView;
class PolygonModel;
class GameController;
class LoadingView;
class MainMenuView;
class OptionsView;
class PauseView;
class TestLevelView;
class TestLevelController;
class MapView;
class QStackedWidget;

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = nullptr);
  ~MainWindow() override;

protected:
  void SetModelsToTestController();
  void SetCurrentLevel(QString const& p_currentLevel) { m_currentLevel = p_currentLevel; }
  void GoToMap(bool p_moveToNextLevel);

private:
  QStateMachine m_stateMachine;

  QStackedWidget* m_centralWidget;
  AchievementsView* m_achievementsView;
  CreateLevelModel* m_createLevelModel;
  CreateLevelView* m_createLevelView;
  CreateLevelController* m_createLevelController;
  GameView* m_gameView;
  GameController* m_gameController;
  LoadingView* m_loadingView;
  MainMenuView* m_mainMenuView;
  OptionsView* m_optionsView;
  PauseView* m_pauseView;
  TestLevelView* m_testLevelView;
  TestLevelController* m_testLevelController;
  MapView* m_mapView;

  QString m_currentLevel;
};

#endif
