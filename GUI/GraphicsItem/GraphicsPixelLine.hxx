#ifndef GRAPHICSPIXELLINE_HXX
#define GRAPHICSPIXELLINE_HXX

#include <QGraphicsItem>

class GraphicsPixelLine: public QAbstractGraphicsShapeItem {

public:
  GraphicsPixelLine(qreal p_x1, qreal p_y1, qreal p_x2, qreal p_y2, qreal p_lineWidth, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPixelLine() override;

  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;
  QRectF boundingRect() const override;

private:
  qreal m_x1;
  qreal m_y1;
  qreal m_x2;
  qreal m_y2;
  qreal m_lineWidth;
  QPointF m_startPoint;
  QPointF m_endPoint;
};

#endif
