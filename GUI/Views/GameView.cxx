#include "GameView.hxx"

#include "GUI/Models/GameModel.hxx"
#include "GUI/Views/GameScribblingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPoint>

GameView::GameView(QWidget* p_parent):
  QWidget(p_parent),
  m_gameLabel(new QLabel("Play")),
  m_pauseButton(new QPushButton("Pause")),
  m_gameModel(nullptr),
  m_scribblingView(new GameScribblingView),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_linesCountLabel(new QLabel),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_partsCountLabel(new QLabel) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_gameLabel);
  menuLayout->addWidget(m_linesCountLabel);
  menuLayout->addWidget(m_partsCountLabel);
  menuLayout->addWidget(m_pauseButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_scribblingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_scribblingView, 1);

  setLayout(mainLayout);

  connect(m_pauseButton, &QPushButton::clicked, this, &GameView::PauseRequested);
  connect(m_scribblingView, &GameScribblingView::Scribbling, this, &GameView::Scribbling);
  connect(m_scribblingView, &GameScribblingView::Moving, this, &GameView::Moving);
  connect(m_scribblingView, &GameScribblingView::Slicing, this, &GameView::Slicing);
}

void GameView::SetModel(GameModel* p_gameModel) {
  m_gameModel = p_gameModel;
  m_scribblingView->SetModel(p_gameModel);
}

void GameView::DrawLine(const ppxl::Segment& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  m_scribblingView->DrawLine(p_line, p_color, p_penStyle);
}

void GameView::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void GameView::ClearImage() {
  m_scribblingView->ClearImage();
}

void GameView::UpdateLinesCount(int p_linesCount, int p_linesGoal) {
  if (p_linesGoal != -1)
  {
    m_linesGoal = p_linesGoal;
  }

  m_linesCount = p_linesCount;
  m_linesCountLabel->setText(tr("Lines: %1/%2").arg(m_linesCount).arg(m_linesGoal));
}

void GameView::UpdatePartsCount(int p_partsCount, int p_partsGoal) {
  if (p_partsGoal != -1)
  {
    m_partsGoal = p_partsGoal;
  }

  m_partsCount = p_partsCount;
  m_partsCountLabel->setText(tr("Parts: %1/%2").arg(m_partsCount).arg(m_partsGoal));
}
