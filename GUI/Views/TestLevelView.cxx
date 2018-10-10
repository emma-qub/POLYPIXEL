#include "TestLevelView.hxx"

#include "GUI/Views/PlayingView.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Controllers/TestingController.hxx"
#include "GUI/Views/TestingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TestLevelView::TestLevelView(QWidget* p_parent):
  QWidget(p_parent),
  m_testLevelLabel(new QLabel("Test your level")),
  m_menuButton(new QPushButton("Menu")),
  m_amendLevelButton(new QPushButton("Amend level")),
  m_view(new TestingView),
  m_controller(new TestingController(m_view, this)) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_testLevelLabel);
  menuLayout->addWidget(m_amendLevelButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_view);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_view, 1);
  setLayout(mainLayout);

  connect(m_menuButton, &QPushButton::clicked, this, &TestLevelView::Done);
  connect(m_amendLevelButton, &QPushButton::clicked, this, &TestLevelView::AmendLevelRequested);
}

void TestLevelView::InitView() {
  m_view->InitView();
}

void TestLevelView::SetModel(PolygonModel* p_model) {
  m_controller->SetPolygonsItem(p_model);
}

void TestLevelView::SetLinesGoal(int p_linesGoal) {
  m_controller->SetLinesGoal(p_linesGoal);
}

void TestLevelView::SetPartsGoal(int p_partsGoal) {
  m_controller->SetPartsGoal(p_partsGoal);
}

void TestLevelView::SetMaxGapToWin(int p_maxGapToWin) {
  m_controller->SetMaxGapToWin(p_maxGapToWin);
}

void TestLevelView::SetTolerance(int p_tolerance) {
  m_controller->SetTolerance(p_tolerance);
}

void TestLevelView::PlayLevel() {
  m_controller->PlayLevel();
}
