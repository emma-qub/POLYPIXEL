#include "TestLevelWidget.hxx"

#include "Core/Geometry/Point.hxx"

#include "GUI/CreateLevel/Models/GraphicsObjectItem.hxx"
#include "TestLevelGraphicsView.hxx"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>

TestLevelWidget::TestLevelWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_graphicsView(new TestLevelGraphicsView),
  m_cuttingLinesGraphicsItem(new CuttingLineGraphicsItem),
  m_saveButton(new QPushButton("SAVE")),
  m_retryButton(new QPushButton("RETRY")),
  m_amendButton(new QPushButton("AMEND")) {

  auto viewLayout = new QHBoxLayout;
  viewLayout->addWidget(m_graphicsView);
  viewLayout->setContentsMargins(0, 0, 0, 0);

  auto infoLayout = new QHBoxLayout;
  infoLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
  infoLayout->addWidget(m_saveButton);
  infoLayout->addWidget(m_retryButton);
  infoLayout->addWidget(m_amendButton);
  infoLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
  m_saveButton->setDisabled(true);
  int buttonWidth = 75;
  m_saveButton->setFixedWidth(buttonWidth);
  m_retryButton->setFixedWidth(buttonWidth);
  m_amendButton->setFixedWidth(buttonWidth);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(infoLayout);
  mainLayout->addLayout(viewLayout);

  setLayout(mainLayout);

  // Graphics View signals forward
  connect(m_graphicsView, &TestLevelGraphicsView::MousePressed, this, &TestLevelWidget::MousePressed);
  connect(m_graphicsView, &TestLevelGraphicsView::MouseMoved, this, &TestLevelWidget::MouseMoved);
  connect(m_graphicsView, &TestLevelGraphicsView::MouseReleased, this, &TestLevelWidget::MouseReleased);

  // Button signals
  connect(m_saveButton, &QPushButton::clicked, this, &TestLevelWidget::SaveRequested);
  connect(m_retryButton, &QPushButton::clicked, this, &TestLevelWidget::RetryRequested);
  connect(m_amendButton, &QPushButton::clicked, this, &TestLevelWidget::AmendRequested);
}

TestLevelWidget::~TestLevelWidget() {
  delete m_cuttingLinesGraphicsItem;
}

void TestLevelWidget::InitView() {
  m_graphicsView->InitView();
}

void TestLevelWidget::ClearGraphicsView() {
  m_graphicsView->ClearScene();
}

void TestLevelWidget::AddGraphicsItem(QGraphicsItem* p_item) {
  m_graphicsView->AddGraphicsItem(p_item);
}

void TestLevelWidget::CuttingStarted() {
  m_graphicsView->AddGraphicsItem(m_cuttingLinesGraphicsItem);
}

void TestLevelWidget::SetCuttingLines(std::vector<ppxl::Segment> const& p_pointsList) {
  m_cuttingLinesGraphicsItem->SetLinesList(p_pointsList);

  m_graphicsView->UpdateView();
}

void TestLevelWidget::CuttingEnded() {
  m_graphicsView->RemoveGraphicsItem(m_cuttingLinesGraphicsItem);
}

void TestLevelWidget::SetNoCutState() {
  m_cuttingLinesGraphicsItem->SetNoCut();
}

void TestLevelWidget::SetGoodCutState() {
  m_cuttingLinesGraphicsItem->SetGoodCut();
}

void TestLevelWidget::SetBadCutState() {
  m_cuttingLinesGraphicsItem->SetBadCut();
}

void TestLevelWidget::SetSaveButtonEnabled(bool p_enabled) {
  m_saveButton->setEnabled(p_enabled);
}
