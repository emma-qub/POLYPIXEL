#include "MainWindow.hxx"

#include "Achievements/AchievementsWidget.hxx"

#include "CreateLevel/Views/CreateLevelWidget.hxx"
#include "CreateLevel/Controllers/CreateLevelController.hxx"

#include "PlayLevel/Views/PlayLevelWidget.hxx"
#include "PlayLevel/Controllers/PlayLevelController.hxx"
#include "PlayLevel/Views/PauseWidget.hxx"

#include "TestLevel/Views/TestLevelWidget.hxx"
#include "TestLevel/Controllers/TestLevelController.hxx"

#include "ChooseLevel/Views/ChooseLevelWidget.hxx"
#include "ChooseLevel/Controllers/ChooseLevelController.hxx"

#include "MainMenu/MainMenuWidget.hxx"

#include "Options/OptionsWidget.hxx"

#include <QStackedWidget>
#include <QFontDatabase>
#include <QToolBar>

MainWindow::MainWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_stateMachine(this),
  m_centralWidget(new QStackedWidget),
  m_achievementsWidget(new AchievementsWidget),
  m_createLevelWidget(new CreateLevelWidget),
  m_createLevelController(new CreateLevelController(m_createLevelWidget, this)),
  m_playLevelWidget(new PlayLevelWidget),
  m_playLevelController(new PlayLevelController(m_playLevelWidget, this)),
  m_mainMenuWidget(new MainMenuWidget),
  m_optionsWidget(new OptionsWidget),
  m_pauseWidget(new PauseWidget),
  m_testLevelWidget(new TestLevelWidget),
  m_testLevelController(new TestLevelController(m_testLevelWidget, this)),
  m_chooseLevelWidget(new ChooseLevelWidget),
  m_chooseLevelController(new ChooseLevelController(m_chooseLevelWidget, this)) {

  m_toolbar = new QToolBar;
  addToolBar(Qt::LeftToolBarArea, m_toolbar);
  m_toolbar->setOrientation(Qt::Vertical);
  m_toolbar->setAllowedAreas(Qt::LeftToolBarArea);
  m_toolbar->setMovable(false);
  //m_toolbar->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
  //m_toolbar->move(50, 50);

  m_createLevelController->SetToolBar(m_toolbar);

  QFontDatabase::addApplicationFont(":/fonts/PICOPIXEL.ttf");

  m_centralWidget->addWidget(m_achievementsWidget);
  m_centralWidget->addWidget(m_createLevelWidget);
  m_centralWidget->addWidget(m_playLevelWidget);
  m_centralWidget->addWidget(m_mainMenuWidget);
  m_centralWidget->addWidget(m_optionsWidget);
  m_centralWidget->addWidget(m_pauseWidget);
  m_centralWidget->addWidget(m_testLevelWidget);
  m_centralWidget->addWidget(m_chooseLevelWidget);

  setCentralWidget(m_centralWidget);

  auto mainMenuState = new QState(&m_stateMachine);
  connect(mainMenuState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mainMenuWidget);
  });
  auto playLevelState = new QState(&m_stateMachine);
  connect(playLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_playLevelWidget);
  });
  auto pauseState = new QState(&m_stateMachine);
  connect(pauseState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_pauseWidget);
  });
  auto achievementsState = new QState(&m_stateMachine);
  connect(achievementsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_achievementsWidget);
  });
  auto createLevelState = new QState(&m_stateMachine);
  connect(createLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_createLevelWidget);
  });
  auto testLevelState = new QState(&m_stateMachine);
  connect(testLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_testLevelWidget);
  });
  auto optionsState = new QState(&m_stateMachine);
  connect(optionsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_optionsWidget);
  });
  auto levelState = new QState(&m_stateMachine);
  connect(levelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_chooseLevelWidget);
  });

  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::OptionsRequested, optionsState);
  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::CreateLevelRequested, createLevelState);
  createLevelState->addTransition(m_createLevelWidget, &CreateLevelWidget::TestLevelRequested, testLevelState);
  createLevelState->addTransition(m_createLevelWidget, &CreateLevelWidget::CreateLevelDone, mainMenuState);
  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::ChooseLevelRequested, levelState);
  levelState->addTransition(m_chooseLevelWidget, &ChooseLevelWidget::PlayLevelRequested, playLevelState);
  playLevelState->addTransition(m_playLevelWidget, &PlayLevelWidget::PauseRequested, pauseState);
  playLevelState->addTransition(m_playLevelWidget, &PlayLevelWidget::ChooseLevelRequested, levelState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::ResumeRequested, playLevelState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::RestartRequested, playLevelState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::ChooseLevelRequested, levelState);
  testLevelState->addTransition(m_testLevelWidget, &TestLevelWidget::AmendLevelRequested, createLevelState);
  testLevelState->addTransition(m_testLevelWidget, &TestLevelWidget::Done, mainMenuState);
  optionsState->addTransition(m_optionsWidget, &OptionsWidget::Done, mainMenuState);

  connect(m_centralWidget, &QStackedWidget::currentChanged, this, [this]() {
    m_toolbar->setVisible(m_centralWidget->currentWidget() == m_createLevelWidget || m_centralWidget->currentWidget() == m_testLevelWidget);
    if (m_centralWidget->currentWidget() == m_createLevelWidget) {
      m_createLevelWidget->InitViews();
      m_createLevelController->UpdateView();
    } else {
      if (m_centralWidget->currentWidget() == m_testLevelWidget) {
        m_testLevelWidget->InitView();
      } else if (m_centralWidget->currentWidget() == m_playLevelWidget) {
        m_playLevelWidget->InitView();
        m_playLevelWidget->PlayLevel(m_currentLevel);
      } else if (m_centralWidget->currentWidget() == m_chooseLevelWidget) {
        m_chooseLevelWidget->InitView();
      }
    }
  });

  connect(m_createLevelWidget, &CreateLevelWidget::TestLevelRequested, this, &MainWindow::SetModelsToTestController);
  connect(m_pauseWidget, &PauseWidget::RestartRequested, m_playLevelController, &PlayLevelController::RestartLevel);
  connect(m_chooseLevelWidget, &ChooseLevelWidget::PlayLevelRequested, this, &MainWindow::SetCurrentLevel);

  m_stateMachine.setInitialState(mainMenuState);
  m_stateMachine.start();

  showMaximized();
  //showFullScreen();
}

MainWindow::~MainWindow() = default;

void MainWindow::SetModelsToTestController() {
  m_centralWidget->setCurrentWidget(m_testLevelWidget);

  m_testLevelController->SetLinesGoal(m_createLevelController->GetLinesGoal());
  m_testLevelController->SetPartsGoal(m_createLevelController->GetPartsGoal());
  m_testLevelController->SetMaxGapToWin(m_createLevelController->GetMaxGapToWin());
  m_testLevelController->SetTolerance(m_createLevelController->GetTolerance());
  m_testLevelController->InitPolygonsList(m_createLevelController->GetPolygonsList());
  m_testLevelController->SetObjectModelsList(m_createLevelController->GetObjectsList());
  m_testLevelController->PlayLevel();
}

