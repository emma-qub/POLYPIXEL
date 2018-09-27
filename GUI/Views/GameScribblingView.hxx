#ifndef GAMESCRIBBLINGVIEW_HXX
#define GAMESCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView.hxx"

class PolygonModel;

class PlayingScribblingView: public AbstractScribblingView {
  Q_OBJECT

public:
  PlayingScribblingView(QWidget* parent = nullptr);
  ~PlayingScribblingView() override;

  void DrawAreas(QList<double> const& p_areas);

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  bool m_scribbling;
};

#endif // GAMESCRIBBLINGVIEW_HXX
