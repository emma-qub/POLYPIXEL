#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QStateMachine>

class AchievementsWidget;
class CreateLevelObjectsListModel;
class CreateLevelWidget;
class CreateLevelController;
class GameWidget;
class PolygonModel;
class GameController;
class LoadingWidget;
class MainMenuWidget;
class OptionsWidget;
class PauseWidget;
class TestLevelWidget;
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
  AchievementsWidget* m_achievementsWidget;
  CreateLevelWidget* m_createLevelWidget;
  CreateLevelController* m_createLevelController;
  GameWidget* m_gameWidget;
  GameController* m_gameController;
  LoadingWidget* m_loadingWidget;
  MainMenuWidget* m_mainMenuWidget;
  OptionsWidget* m_optionsWidget;
  PauseWidget* m_pauseWidget;
  TestLevelWidget* m_testLevelWidget;
  TestLevelController* m_testLevelController;
  MapView* m_mapView;

  QString m_currentLevel;

  QToolBar* m_toolbar;
};

#endif
