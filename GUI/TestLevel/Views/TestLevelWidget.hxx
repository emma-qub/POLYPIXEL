#ifndef TESTLEVELWIDGET_HXX
#define TESTLEVELWIDGET_HXX

#include <QWidget>

class QGraphicsItem;
class QPushButton;
class CuttingLineGraphicsItem;
class TestLevelGraphicsView;

namespace ppxl {
class Segment;
}

class TestLevelWidget: public QWidget {
  Q_OBJECT

public:
  explicit TestLevelWidget(QWidget* p_parent = nullptr);
  ~TestLevelWidget() override;

  void InitView();
  void ClearGraphicsView();

  void AddGraphicsItem(QGraphicsItem* p_item);

  void CuttingStarted();
  void SetCuttingLines(std::vector<ppxl::Segment> const& p_pointsList);
  void CuttingEnded();
  void SetNoCutState();
  void SetGoodCutState();
  void SetBadCutState();

  void SetSaveButtonEnabled(bool p_enabled);

Q_SIGNALS:
  void Done();
  void MousePressed(QMouseEvent* p_event);
  void MouseMoved(QMouseEvent* p_event);
  void MouseReleased(QMouseEvent* p_event);
  void SaveRequested();
  void RetryRequested();
  void AmendRequested();

private:
  TestLevelGraphicsView* m_graphicsView;
  CuttingLineGraphicsItem* m_cuttingLinesGraphicsItem;

  QPushButton* m_saveButton;
  QPushButton* m_retryButton;
  QPushButton* m_amendButton;
};

#endif
