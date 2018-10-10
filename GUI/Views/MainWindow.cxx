#include "MainWindow.hxx"

#include "AchievementsView.hxx"
#include "CreateLevelView.hxx"
#include "GameView.hxx"
#include "LevelsView.hxx"
#include "LoadingView.hxx"
#include "MainMenuView.hxx"
#include "OptionsView.hxx"
#include "PauseView.hxx"
#include "TestLevelView.hxx"
#include "WorldsView.hxx"
#include "MapView.hxx"

#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Models/LevelsModel.hxx"
#include "GUI/Models/WorldsModel.hxx"

#include "GUI/Controllers/CreateLevelController.hxx"
#include "GUI/Controllers/GameController.hxx"
#include "GUI/Controllers/LevelsController.hxx"
#include "GUI/Controllers/TestLevelController.hxx"
#include "GUI/Controllers/WorldsController.hxx"

#include <QStackedWidget>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_stateMachine(this),
  m_centralWidget(new QStackedWidget),
  m_achievementsView(new AchievementsView),
  m_createLevelModel(new CreateLevelModel(this)),
  m_createLevelView(new CreateLevelView),
  m_createLevelController(new CreateLevelController(m_createLevelModel, m_createLevelView, this)),
  m_gameView(new GameView),
  m_gameController(new GameController(m_gameView, this)),
  m_loadingView(new LoadingView),
  m_mainMenuView(new MainMenuView),
  m_optionsView(new OptionsView),
  m_pauseView(new PauseView),
  m_testLevelView(new TestLevelView),
  m_testLevelController(new TestLevelController(m_testLevelView, this)),
  m_mapView(new MapView) {

  QFontDatabase::addApplicationFont(":/fonts/PICOPIXEL.ttf");

  m_centralWidget->addWidget(m_achievementsView);
  m_centralWidget->addWidget(m_createLevelView);
  m_centralWidget->addWidget(m_mapView);
  m_centralWidget->addWidget(m_gameView);
  m_centralWidget->addWidget(m_loadingView);
  m_centralWidget->addWidget(m_mainMenuView);
  m_centralWidget->addWidget(m_optionsView);
  m_centralWidget->addWidget(m_pauseView);
  m_centralWidget->addWidget(m_testLevelView);

  setCentralWidget(m_centralWidget);

  auto loadingState = new QState(&m_stateMachine);
  connect(loadingState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_loadingView);
  });
  auto mainMenuState = new QState(&m_stateMachine);
  connect(mainMenuState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mainMenuView);
  });
  auto mapState = new QState(&m_stateMachine);
  connect(mapState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mapView);
  });
  auto gameState = new QState(&m_stateMachine);
  connect(gameState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_gameView);
  });
  auto pauseState = new QState(&m_stateMachine);
  connect(pauseState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_pauseView);
  });
  auto achievementsState = new QState(&m_stateMachine);
  connect(achievementsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_achievementsView);
  });
  auto createLevelState = new QState(&m_stateMachine);
  connect(createLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_createLevelView);
  });
  auto testLevelState = new QState(&m_stateMachine);
  connect(testLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_testLevelView);
  });
  auto optionsState = new QState(&m_stateMachine);
  connect(optionsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_optionsView);
  });

  loadingState->addTransition(m_loadingView, &LoadingView::LoadingDone, mainMenuState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::PlayRequested, mapState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::CreateLevelRequested, createLevelState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::OptionsRequested, optionsState);
  mapState->addTransition(m_mapView, &MapView::PlayLevelRequested, gameState);
  //worldsState->addTransition(m_worldsView, &WorldsView::AchievementsRequested, achievementsState);
  //worldsState->addTransition(m_worldsView, &WorldsView::Done, mainMenuState);
  gameState->addTransition(m_gameView, &GameView::PauseRequested, pauseState);
  gameState->addTransition(m_gameView, &GameView::GoToMapRequested, mapState);
  pauseState->addTransition(m_pauseView, &PauseView::ResumeRequested, gameState);
  pauseState->addTransition(m_pauseView, &PauseView::RestartRequested, gameState);
  pauseState->addTransition(m_pauseView, &PauseView::LevelsRequested, mapState);
  //achievementsState->addTransition(m_achievementsView, &AchievementsView::Done, worldsState);
  createLevelState->addTransition(m_createLevelView, &CreateLevelView::TestLevelRequested, testLevelState);
  createLevelState->addTransition(m_createLevelView, &CreateLevelView::Done, mainMenuState);
  testLevelState->addTransition(m_testLevelView, &TestLevelView::AmendLevelRequested, createLevelState);
  testLevelState->addTransition(m_testLevelView, &TestLevelView::Done, mainMenuState);
  optionsState->addTransition(m_optionsView, &OptionsView::Done, mainMenuState);

  connect(m_centralWidget, &QStackedWidget::currentChanged, this, [this]() {
    if (m_centralWidget->currentWidget() == m_createLevelView) {
      m_createLevelView->InitView();
      m_createLevelController->Redraw();
    } else if (m_centralWidget->currentWidget() == m_testLevelView) {
      m_testLevelView->InitView();
    } else if (m_centralWidget->currentWidget() == m_gameView) {
      m_gameView->InitView();
      m_gameView->PlayLevel(m_currentLevel);
    } else if (m_centralWidget->currentWidget() == m_mapView) {
      m_mapView->InitView();
    }
  });

  connect(m_createLevelView, &CreateLevelView::TestLevelRequested, this, &MainWindow::SetModelsToTestController);
  connect(m_pauseView, &PauseView::RestartRequested, m_gameController, &GameController::RestartLevel);
  connect(m_mapView, &MapView::PlayLevelRequested, this, &MainWindow::SetCurrentLevel);
  connect(m_gameView, &GameView::GoToMapRequested, this, &MainWindow::GoToMap);

  m_stateMachine.setInitialState(loadingState);
  m_stateMachine.start();

  showFullScreen();
}

MainWindow::~MainWindow() = default;

void MainWindow::SetModelsToTestController() {
  m_centralWidget->setCurrentWidget(m_testLevelView);

  m_testLevelController->SetLinesGoal(m_createLevelController->GetLinesGoal());
  m_testLevelController->SetPartsGoal(m_createLevelController->GetPartsGoal());
  m_testLevelController->SetMaxGapToWin(m_createLevelController->GetMaxGapToWin());
  m_testLevelController->SetTolerance(m_createLevelController->GetTolerance());
  m_testLevelController->SetModel(m_createLevelController->GetModel());
  m_testLevelController->PlayLevel();
}

void MainWindow::GoToMap(bool p_moveToNextLevel) {
  if (p_moveToNextLevel) {
    m_mapView->MoveToNextLevel();
  }
}
