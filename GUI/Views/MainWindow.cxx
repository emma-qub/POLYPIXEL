#include "MainWindow.hxx"

#include "AchievementsWidget.hxx"
#include "CreateLevelWidget.hxx"
#include "GameWidget.hxx"
#include "LevelsView.hxx"
#include "LoadingWidget.hxx"
#include "MainMenuWidget.hxx"
#include "OptionsWidget.hxx"
#include "PauseWidget.hxx"
#include "TestLevelWidget.hxx"
#include "WorldsView.hxx"
#include "MapView.hxx"

#include "GUI/Models/CreateLevelObjectsListModel.hxx"
//#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Models/LevelsModel.hxx"
#include "GUI/Models/WorldsModel.hxx"

#include "GUI/Controllers/CreateLevelController.hxx"
#include "GUI/Controllers/GameController.hxx"
#include "GUI/Controllers/LevelsController.hxx"
#include "GUI/Controllers/TestLevelController.hxx"
#include "GUI/Controllers/WorldsController.hxx"

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
  m_gameWidget(new GameWidget),
  m_gameController(new GameController(m_gameWidget, this)),
  m_loadingWidget(new LoadingWidget),
  m_mainMenuWidget(new MainMenuWidget),
  m_optionsWidget(new OptionsWidget),
  m_pauseWidget(new PauseWidget),
  m_testLevelWidget(new TestLevelWidget),
  m_testLevelController(new TestLevelController(m_testLevelWidget, this)),
  m_mapView(new MapView) {

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
  m_centralWidget->addWidget(m_mapView);
  m_centralWidget->addWidget(m_gameWidget);
  m_centralWidget->addWidget(m_loadingWidget);
  m_centralWidget->addWidget(m_mainMenuWidget);
  m_centralWidget->addWidget(m_optionsWidget);
  m_centralWidget->addWidget(m_pauseWidget);
  m_centralWidget->addWidget(m_testLevelWidget);

  setCentralWidget(m_centralWidget);

  auto loadingState = new QState(&m_stateMachine);
  connect(loadingState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_loadingWidget);
  });
  auto mainMenuState = new QState(&m_stateMachine);
  connect(mainMenuState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mainMenuWidget);
  });
  auto mapState = new QState(&m_stateMachine);
  connect(mapState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_mapView);
  });
  auto gameState = new QState(&m_stateMachine);
  connect(gameState, &QState::entered, this, [this]() {
    m_centralWidget->setCurrentWidget(m_gameWidget);
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

  loadingState->addTransition(m_loadingWidget, &LoadingWidget::LoadingDone, mainMenuState);
  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::PlayRequested, mapState);
  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::CreateLevelRequested, createLevelState);
  mainMenuState->addTransition(m_mainMenuWidget, &MainMenuWidget::OptionsRequested, optionsState);
  mapState->addTransition(m_mapView, &MapView::PlayLevelRequested, gameState);
  //worldsState->addTransition(m_worldsView, &WorldsView::AchievementsRequested, achievementsState);
  //worldsState->addTransition(m_worldsView, &WorldsView::Done, mainMenuState);
  gameState->addTransition(m_gameWidget, &GameWidget::PauseRequested, pauseState);
  gameState->addTransition(m_gameWidget, &GameWidget::GoToMapRequested, mapState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::ResumeRequested, gameState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::RestartRequested, gameState);
  pauseState->addTransition(m_pauseWidget, &PauseWidget::LevelsRequested, mapState);
  //achievementsState->addTransition(m_achievementsWidget, &AchievementsWidget::Done, worldsState);
  createLevelState->addTransition(m_createLevelWidget, &CreateLevelWidget::TestLevelRequested, testLevelState);
  createLevelState->addTransition(m_createLevelWidget, &CreateLevelWidget::CreateLevelDone, mainMenuState);
  testLevelState->addTransition(m_testLevelWidget, &TestLevelWidget::AmendLevelRequested, createLevelState);
  testLevelState->addTransition(m_testLevelWidget, &TestLevelWidget::Done, mainMenuState);
  optionsState->addTransition(m_optionsWidget, &OptionsWidget::Done, mainMenuState);

  connect(m_centralWidget, &QStackedWidget::currentChanged, this, [this]() {
    if (m_centralWidget->currentWidget() == m_createLevelWidget) {
      m_createLevelWidget->InitViews();
      m_createLevelController->UpdateView();
      m_toolbar->show();
    } else {
      m_toolbar->hide();

      if (m_centralWidget->currentWidget() == m_testLevelWidget) {
        m_testLevelWidget->InitView();
      } else if (m_centralWidget->currentWidget() == m_gameWidget) {
        m_gameWidget->InitView();
        m_gameWidget->PlayLevel(m_currentLevel);
      } else if (m_centralWidget->currentWidget() == m_mapView) {
        m_mapView->InitView();
      }
    }
  });

  connect(m_createLevelWidget, &CreateLevelWidget::TestLevelRequested, this, &MainWindow::SetModelsToTestController);
  connect(m_pauseWidget, &PauseWidget::RestartRequested, m_gameController, &GameController::RestartLevel);
  connect(m_mapView, &MapView::PlayLevelRequested, this, &MainWindow::SetCurrentLevel);
  connect(m_gameWidget, &GameWidget::GoToMapRequested, this, &MainWindow::GoToMap);

  m_stateMachine.setInitialState(loadingState);
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
  //m_testLevelController->SetPolygonModel(m_createLevelController->GetPolygonModel());
  //m_testLevelController->SetObjectModelsList(m_createLevelController->GetObjectModelsList());
  m_testLevelController->PlayLevel();
}

void MainWindow::GoToMap(bool p_moveToNextLevel) {
  if (p_moveToNextLevel) {
    //m_mapView->MoveToNextLevel();
  }
}
