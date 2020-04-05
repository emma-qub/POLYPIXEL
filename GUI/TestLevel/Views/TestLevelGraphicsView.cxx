#include "TestLevelGraphicsView.hxx"

TestLevelGraphicsView::TestLevelGraphicsView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_viewInitialized(false) {

}

void TestLevelGraphicsView::InitView() {
  if (m_viewInitialized) {
    return;
  }

  // Hide scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set scene
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);
  m_scene->setSceneRect(0, 0, width(), height());

  m_viewInitialized = true;
}

void TestLevelGraphicsView::AddGraphicsItem(QGraphicsItem* p_graphicsItem) {
  m_scene->addItem(p_graphicsItem);
}

void TestLevelGraphicsView::RemoveGraphicsItem(QGraphicsItem* p_graphicsItem) {
  m_scene->removeItem(p_graphicsItem);
}

void TestLevelGraphicsView::UpdateView() {
  m_scene->update();
}

void TestLevelGraphicsView::mousePressEvent(QMouseEvent* p_event) {
  Q_EMIT MousePressed(p_event);
}

void TestLevelGraphicsView::mouseMoveEvent(QMouseEvent* p_event) {
  Q_EMIT MouseMoved(p_event);
}

void TestLevelGraphicsView::mouseReleaseEvent(QMouseEvent* p_event) {
  Q_EMIT MouseReleased(p_event);
}
