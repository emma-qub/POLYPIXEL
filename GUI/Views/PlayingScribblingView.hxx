#ifndef GAMESCRIBBLINGVIEW_HXX
#define GAMESCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView2.hxx"

class PolygonModel;

class PlayingScribblingView: public AbstractScribblingView2 {
  Q_OBJECT

public:
  PlayingScribblingView(QWidget* parent = nullptr);
  ~PlayingScribblingView() override;

  void DrawAreas(QList<double> const& p_areas);

  void AnimatePolygons(QList<ppxl::Vector> const& shiftVectors);

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);
  void ControlPressed(QPoint const& p_cursorPosition);
  void ControlReleased(QPoint const& p_cursorPosition);
  void PolygonsAnimationDone();

protected:
  void keyPressEvent(QKeyEvent* p_event) override;
  void keyReleaseEvent(QKeyEvent* p_event) override;
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;
  void mouseReleaseEvent(QMouseEvent* p_event) override;
  //void paintEvent(QPaintEvent* p_event) override;

private:
  bool m_scribbling;
  QPoint m_cursorPosition;
};

#endif // GAMESCRIBBLINGVIEW_HXX
