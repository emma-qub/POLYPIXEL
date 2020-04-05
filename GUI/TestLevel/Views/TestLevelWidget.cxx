#include "TestLevelWidget.hxx"

#include "Core/Geometry/Point.hxx"

#include "GUI/CreateLevel/Models/GraphicsObjectItem.hxx"
#include "TestLevelGraphicsView.hxx"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QMouseEvent>

TestLevelWidget::TestLevelWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_graphicsView(new TestLevelGraphicsView),
  m_cuttingLinesGraphicsItem(new CuttingLineGraphicsItem) {

  auto viewLayout = new QHBoxLayout;
  viewLayout->addWidget(m_graphicsView);
  viewLayout->setContentsMargins(0, 0, 0, 0);
  setLayout(viewLayout);

  // Graphics View signals forward
  connect(m_graphicsView, &TestLevelGraphicsView::MousePressed, this, &TestLevelWidget::MousePressed);
  connect(m_graphicsView, &TestLevelGraphicsView::MouseMoved, this, &TestLevelWidget::MouseMoved);
  connect(m_graphicsView, &TestLevelGraphicsView::MouseReleased, this, &TestLevelWidget::MouseReleased);
}

TestLevelWidget::~TestLevelWidget() {
  delete m_cuttingLinesGraphicsItem;
}

void TestLevelWidget::InitView() {
  m_graphicsView->InitView();
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
