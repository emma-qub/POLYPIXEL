#include "MainWindow.hxx"

#include "AchievementsView.hxx"
#include "CreateLevelView.hxx"
#include "GameView.hxx"
#include "LevelsView.hxx"
#include "LoadingView.hxx"
#include "MainMenuView.hxx"
#include "OptionsView.hxx"
#include "PauseView.hxx"
#include "SaveLevelView.hxx"
#include "TestLevelView.hxx"
#include "WorldsView.hxx"

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
  m_levelsModel(new LevelsModel(this)),
  m_levelsView(new LevelsView),
  m_levelsController(new LevelsController(m_levelsView, m_levelsModel, this)),
  m_loadingView(new LoadingView),
  m_mainMenuView(new MainMenuView),
  m_optionsView(new OptionsView),
  m_pauseView(new PauseView),
  m_testLevelView(new TestLevelView),
  m_testLevelController(new TestLevelController(m_testLevelView, this)),
  m_saveLevelView(new SaveLevelView),
  m_worldsView(new WorldsView),
  m_worldsModel(new WorldsModel(this)),
  m_worldsController(new WorldsController(m_worldsView, m_worldsModel, this))
{
  m_centralWidget->addWidget(m_achievementsView);
  m_centralWidget->addWidget(m_createLevelView);
  m_centralWidget->addWidget(m_gameView);
  m_centralWidget->addWidget(m_levelsView);
  m_centralWidget->addWidget(m_loadingView);
  m_centralWidget->addWidget(m_mainMenuView);
  m_centralWidget->addWidget(m_optionsView);
  m_centralWidget->addWidget(m_pauseView);
  m_centralWidget->addWidget(m_saveLevelView);
  m_centralWidget->addWidget(m_testLevelView);
  m_centralWidget->addWidget(m_worldsView);

  setCentralWidget(m_centralWidget);

  auto loadingState = new QState(&m_stateMachine);
  connect(loadingState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_loadingView);
  });
  auto mainMenuState = new QState(&m_stateMachine);
  connect(mainMenuState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mainMenuView);
  });
  auto worldsState = new QState(&m_stateMachine);
  connect(worldsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_worldsView);
  });
  auto levelsState = new QState(&m_stateMachine);
  connect(levelsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_levelsView);
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
  auto saveLevelState = new QState(&m_stateMachine);
  connect(saveLevelState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_saveLevelView);
  });
  auto optionsState = new QState(&m_stateMachine);
  connect(optionsState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_optionsView);
  });

  loadingState->addTransition(m_loadingView, &LoadingView::LoadingDone, mainMenuState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::PlayRequested, worldsState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::CreateLevelRequested, createLevelState);
  mainMenuState->addTransition(m_mainMenuView, &MainMenuView::OptionsRequested, optionsState);
  worldsState->addTransition(m_worldsView, &WorldsView::SelectLevelsRequested, levelsState);
  worldsState->addTransition(m_worldsView, &WorldsView::AchievementsRequested, achievementsState);
  worldsState->addTransition(m_worldsView, &WorldsView::Done, mainMenuState);
  levelsState->addTransition(m_levelsView, &LevelsView::PlayLevelRequested, gameState);
  levelsState->addTransition(m_levelsView, &LevelsView::WorldsSelectionRequested, worldsState);
  levelsState->addTransition(m_levelsView, &LevelsView::Done, mainMenuState);
  gameState->addTransition(m_gameView, &GameView::PauseRequested, pauseState);
  pauseState->addTransition(m_pauseView, &PauseView::ResumeRequested, gameState);
  pauseState->addTransition(m_pauseView, &PauseView::RestartRequested, gameState);
  pauseState->addTransition(m_pauseView, &PauseView::LevelsRequested, levelsState);
  achievementsState->addTransition(m_achievementsView, &AchievementsView::Done, worldsState);
  createLevelState->addTransition(m_createLevelView, &CreateLevelView::TestLevelRequested, testLevelState);
  createLevelState->addTransition(m_createLevelView, &CreateLevelView::Done, mainMenuState);
  testLevelState->addTransition(m_testLevelView, &TestLevelView::AmendLevelRequested, createLevelState);
  testLevelState->addTransition(m_testLevelView, &TestLevelView::SaveLevelRequested, saveLevelState);
  testLevelState->addTransition(m_testLevelView, &TestLevelView::Done, mainMenuState);
  saveLevelState->addTransition(m_saveLevelView, &SaveLevelView::CreateNewLevelRequested, createLevelState);
  saveLevelState->addTransition(m_saveLevelView, &SaveLevelView::Done, mainMenuState);
  optionsState->addTransition(m_optionsView, &OptionsView::Done, mainMenuState);

  connect(m_createLevelView, &CreateLevelView::TestLevelRequested, this, &MainWindow::SetModelsToTestController);
  connect(m_worldsController, &WorldsController::LevelsPathChanged, m_levelsController, &LevelsController::SetLevelsPath);
  connect(m_levelsController, &LevelsController::PlayLevelRequested, m_gameController, &GameController::PlayLevel);
  connect(m_pauseView, &PauseView::RestartRequested, m_gameController, &GameController::RestartLevel);
  connect(m_centralWidget, &QStackedWidget::currentChanged, this, [this]() {
    if (m_centralWidget->currentWidget() == m_createLevelView) {
      m_createLevelController->Redraw();
    }
  });

  m_stateMachine.setInitialState(loadingState);
  m_stateMachine.start();

  showFullScreen();
}

MainWindow::~MainWindow() = default;

void MainWindow::SetModelsToTestController() {
  m_centralWidget->setCurrentWidget(m_testLevelView);

  m_testLevelController->SetLinesGoal(m_createLevelController->GetLinesGoal());
  m_testLevelController->SetPartsGoal(m_createLevelController->GetPartsGoal());
  m_testLevelController->SetTolerance(m_createLevelController->GetTolerance());
  m_testLevelController->SetMaxGapToWin(m_createLevelController->GetMaxGapToWin());
  m_testLevelController->SetModel(m_createLevelController->GetModel());
  m_testLevelController->PlayLevel();
}
