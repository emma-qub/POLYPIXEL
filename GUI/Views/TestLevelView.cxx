#include "TestLevelView.hxx"

#include "GUI/Views/PlayingView.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Controllers/PlayingController.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TestLevelView::TestLevelView(QWidget* p_parent):
  QWidget(p_parent),
  m_testLevelLabel(new QLabel("Test your level")),
  m_menuButton(new QPushButton("Menu")),
  m_amendLevelButton(new QPushButton("Amend level")),
  m_saveLevelButton(new QPushButton("Save level")),
  m_playingView(new PlayingView),
  m_playingController(new PlayingController(m_playingView, this))
{
  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_testLevelLabel);
  menuLayout->addWidget(m_amendLevelButton);
  menuLayout->addWidget(m_saveLevelButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_playingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_playingView, 1);
  setLayout(mainLayout);

  connect(m_menuButton, &QPushButton::clicked, this, &TestLevelView::Done);
  connect(m_amendLevelButton, &QPushButton::clicked, this, &TestLevelView::AmendLevelRequested);
  connect(m_saveLevelButton, &QPushButton::clicked, this, &TestLevelView::SaveLevelRequested);
}

void TestLevelView::SetModel(PolygonModel* p_model) {
  m_playingView->SetModel(p_model);
  m_playingController->SetPolygonsItem(p_model);
  m_playingController->PlayLevel();
}

void TestLevelView::SetLinesGoal(int p_linesGoal) {
  m_playingController->SetLinesGoal(p_linesGoal);
}

void TestLevelView::SetPartsGoal(int p_partsGoal) {
  m_playingController->SetPartsGoal(p_partsGoal);
}

void TestLevelView::SetMaxGapToWin(int p_maxGapToWin) {
  m_playingController->SetMaxGapToWin(p_maxGapToWin);
}

void TestLevelView::SetTolerance(int p_tolerance) {
  m_playingController->SetTolerance(p_tolerance);
}
