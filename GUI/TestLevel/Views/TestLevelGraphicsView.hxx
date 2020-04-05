#ifndef TESTLEVELGRAPHICSVIEW_HXX
#define TESTLEVELGRAPHICSVIEW_HXX

#include <QGraphicsView>

class TestLevelGraphicsView: public QGraphicsView {
  Q_OBJECT

public:
  TestLevelGraphicsView(QWidget* p_parent = nullptr);

  void InitView();

  void AddGraphicsItem(QGraphicsItem* p_graphicsItem);
  void RemoveGraphicsItem(QGraphicsItem* p_graphicsItem);

  void UpdateView();

Q_SIGNALS:
  void MousePressed(QMouseEvent* p_event);
  void MouseMoved(QMouseEvent* p_event);
  void MouseReleased(QMouseEvent* p_event);

protected:
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;
  void mouseReleaseEvent(QMouseEvent* p_event) override;

private:
  QGraphicsScene* m_scene;
  bool m_viewInitialized;
};

#endif
