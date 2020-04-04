#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QStateMachine>

class AchievementsWidget;
class CreateLevelWidget;
class CreateLevelController;
class PlayLevelWidget;
class PlayLevelController;
class PauseWidget;
class TestLevelWidget;
class TestLevelController;
class MainMenuWidget;
class OptionsWidget;
class ChooseLevelWidget;
class ChooseLevelController;
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
  PlayLevelWidget* m_playLevelWidget;
  PlayLevelController* m_playLevelController;
  MainMenuWidget* m_mainMenuWidget;
  OptionsWidget* m_optionsWidget;
  PauseWidget* m_pauseWidget;
  TestLevelWidget* m_testLevelWidget;
  TestLevelController* m_testLevelController;
  ChooseLevelWidget* m_chooseLevelWidget;
  ChooseLevelController* m_chooseLevelController;

  QString m_currentLevel;

  QToolBar* m_toolbar;
};

#endif
