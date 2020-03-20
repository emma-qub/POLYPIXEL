#include "TestLevelWidget.hxx"

#include "GUI/Views/PlayingWidget.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Controllers/TestingController.hxx"
#include "GUI/Views/TestingWidget.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TestLevelWidget::TestLevelWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_testLevelLabel(new QLabel("Test your level")),
  m_menuButton(new QPushButton("Menu")),
  m_amendLevelButton(new QPushButton("Amend level")),
  m_view(new TestingWidget),
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

  connect(m_menuButton, &QPushButton::clicked, this, &TestLevelWidget::Done);
  connect(m_amendLevelButton, &QPushButton::clicked, this, &TestLevelWidget::AmendLevelRequested);
}

void TestLevelWidget::InitView() {
  m_view->InitView();
}

void TestLevelWidget::SetPolygonModel(PolygonModel* p_model) {
  m_controller->SetPolygonsItem(p_model);
}

void TestLevelWidget::SetObjectModelsList(const QList<ObjectModel*>& p_objectModelsList) {
  m_controller->SetObjectModelsList(p_objectModelsList);
}

void TestLevelWidget::SetLinesGoal(int p_linesGoal) {
  m_controller->SetLinesGoal(p_linesGoal);
}

void TestLevelWidget::SetPartsGoal(int p_partsGoal) {
  m_controller->SetPartsGoal(p_partsGoal);
}

void TestLevelWidget::SetMaxGapToWin(int p_maxGapToWin) {
  m_controller->SetMaxGapToWin(p_maxGapToWin);
}

void TestLevelWidget::SetTolerance(int p_tolerance) {
  m_controller->SetTolerance(p_tolerance);
}

void TestLevelWidget::PlayLevel() {
  m_controller->PlayLevel();
}
