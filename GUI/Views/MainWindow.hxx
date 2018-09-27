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
class LevelsView;
class LevelsModel;
class LevelsController;
class LoadingView;
class MainMenuView;
class OptionsView;
class PauseView;
class SaveLevelView;
class TestLevelView;
class TestLevelController;
class WorldsView;
class WorldsModel;
class WorldsController;
class QStackedWidget;

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = nullptr);
  ~MainWindow() override;

protected:
  void SetModelsToTestController();

private:
  QStateMachine m_stateMachine;

  QStackedWidget* m_centralWidget;
  AchievementsView* m_achievementsView;
  CreateLevelModel* m_createLevelModel;
  CreateLevelView* m_createLevelView;
  CreateLevelController* m_createLevelController;
  GameView* m_gameView;
  GameController* m_gameController;
  LevelsModel* m_levelsModel;
  LevelsView* m_levelsView;
  LevelsController* m_levelsController;
  LoadingView* m_loadingView;
  MainMenuView* m_mainMenuView;
  OptionsView* m_optionsView;
  PauseView* m_pauseView;
  TestLevelView* m_testLevelView;
  TestLevelController* m_testLevelController;
  SaveLevelView* m_saveLevelView;
  WorldsView* m_worldsView;
  WorldsModel* m_worldsModel;
  WorldsController* m_worldsController;
};

#endif
