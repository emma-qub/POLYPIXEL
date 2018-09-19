#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QStateMachine>

class AchievementsView;
class CreateLevelModel;
class CreateLevelView;
class CreateLevelController;
class GameView;
class GameModel;
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
class WorldsView;
class WorldsModel;
class WorldsController;
class QStackedWidget;

class MainWindow: public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = nullptr);
  ~MainWindow() override;

private:
  QStateMachine m_stateMachine;

  QStackedWidget* m_centralWidget;
  AchievementsView* m_achievementsView;
  CreateLevelModel* m_createLevelModel;
  CreateLevelView* m_createLevelView;
  CreateLevelController* m_createLevelController;
  GameView* m_gameView;
  GameModel* m_gameModel;
  GameController* m_gameController;
  LevelsView* m_levelsView;
  LevelsModel* m_levelsModel;
  LevelsController* m_levelsController;
  LoadingView* m_loadingView;
  MainMenuView* m_mainMenuView;
  OptionsView* m_optionsView;
  PauseView* m_pauseView;
  SaveLevelView* m_saveLevelView;
  TestLevelView* m_testLevelView;
  WorldsView* m_worldsView;
  WorldsModel* m_worldsModel;
  WorldsController* m_worldsController;
};

#endif
