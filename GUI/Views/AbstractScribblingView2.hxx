#ifndef ABSTRACTSCRIBBLINGVIEW2_HXX
#define ABSTRACTSCRIBBLINGVIEW2_HXX

#include <QGraphicsView>

#include "Core/Segment.hxx"
#include "Core/Vector.hxx"

class PolygonModel;

class AbstractScribblingView2: public QGraphicsView {
  Q_OBJECT

public:
  AbstractScribblingView2(QWidget* p_parent = nullptr);
  ~AbstractScribblingView2() override;

  virtual void Init();

  virtual void SetModel(PolygonModel* p_model);
  void SetCanScribble(bool p_value);
  bool GetCanScribble() const;

  void ClearImage();

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color);
  void DrawText(ppxl::Point p_position, const QString& p_text, int p_weight, const ppxl::Vector& shiftVector = ppxl::Vector());
  virtual void DrawFromModel();

protected:
  inline PolygonModel* GetModel() const { return m_model; }
  inline void setPenWidth(int p_width) { m_penWidth = p_width; }

  void DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color);
  void DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color);

private:
  QGraphicsScene* m_scene;
  int m_penWidth;
  PolygonModel* m_model;
  QColor m_penColor;
  QPen m_pen;
  bool m_canScribble;
  bool m_viewInitialized;
};

#endif
