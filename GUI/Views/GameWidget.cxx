#include "GameWidget.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/PlayingScribblingView.hxx"
#include "GUI/Views/PlayingWidget.hxx"
#include "GUI/Controllers/PlayingController.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameWidget::GameWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_gameLabel(new QLabel("Play")),
  m_pauseButton(new QPushButton("Pause")),
  m_playingWidget(new PlayingWidget),
  m_playingController(new PlayingController(m_playingWidget, this)) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_gameLabel);
  menuLayout->addWidget(m_pauseButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_playingWidget);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_playingWidget, 1);
  setLayout(mainLayout);

  connect(m_pauseButton, &QPushButton::clicked, this, &GameWidget::PauseRequested);
  connect(m_playingController, &PlayingController::GoToMapRequested, this, &GameWidget::GoToMapRequested);
}

void GameWidget::InitView() {
  m_playingWidget->InitView();
}

void GameWidget::PlayLevel(QString const& p_levelPath) {
  m_playingController->PlayLevel(p_levelPath);
}

void GameWidget::RestartLevel() {
  m_playingController->RestartLevel();
}
