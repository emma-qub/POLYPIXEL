#include "GameController.hxx"
#include "GUI/Views/GameView.hxx"

GameController::GameController(GameView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_view(p_view) {
}

GameController::~GameController() = default;

void GameController::PlayLevel(QString const& p_levelPath) {
  m_view->PlayLevel(p_levelPath);
}

void GameController::RestartLevel() {
  m_view->RestartLevel();
}
