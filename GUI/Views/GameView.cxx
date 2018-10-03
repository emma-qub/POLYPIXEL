#include "GameView.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/PlayingScribblingView.hxx"
#include "GUI/Views/PlayingView.hxx"
#include "GUI/Controllers/PlayingController.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameView::GameView(QWidget* p_parent):
  QWidget(p_parent),
  m_gameLabel(new QLabel("Play")),
  m_pauseButton(new QPushButton("Pause")),
  m_playingView(new PlayingView),
  m_playingController(new PlayingController(m_playingView, this)) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_gameLabel);
  menuLayout->addWidget(m_pauseButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_playingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_playingView, 1);
  setLayout(mainLayout);

  connect(m_pauseButton, &QPushButton::clicked, this, &GameView::PauseRequested);
}

void GameView::InitView() {
  m_playingView->InitView();
}

void GameView::PlayLevel(QString const& p_levelPath) {
  m_playingController->PlayLevel(p_levelPath);
}

void GameView::RestartLevel() {
  m_playingController->RestartLevel();
}
