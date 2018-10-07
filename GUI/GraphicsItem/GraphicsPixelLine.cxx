#include "GraphicsPixelLine.hxx"

#include <QPainter>
#include <QGraphicsScene>

#include <cmath>

GraphicsPixelLine::GraphicsPixelLine(qreal p_x1, qreal p_y1, qreal p_x2, qreal p_y2, qreal p_lineWidth, QGraphicsItem* p_parent):
  QAbstractGraphicsShapeItem(p_parent),
  m_x1(p_x1),
  m_y1(p_y1),
  m_x2(p_x2),
  m_y2(p_y2),
  m_lineWidth(p_lineWidth) {

  double x1 = std::floor(std::min(m_x1, m_x2)/p_lineWidth)*p_lineWidth;
  double x2 = (std::ceil(std::max(m_x1, m_x2)/p_lineWidth))*p_lineWidth;

  double y1 = std::floor(std::min(m_y1, m_y2)/p_lineWidth)*p_lineWidth;
  double y2 = (std::ceil(std::max(m_y1, m_y2)/p_lineWidth))*p_lineWidth;

  m_startPoint = QPointF(x1, y1);
  m_endPoint = QPointF(x2, y2);
}

GraphicsPixelLine::~GraphicsPixelLine() = default;

void GraphicsPixelLine::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option)
  Q_UNUSED(p_widget)

  p_painter->save();
  p_painter->setPen(Qt::NoPen);
  p_painter->setBrush(brush());

  if ((std::abs(m_startPoint.x()-m_endPoint.x()) <= m_lineWidth) || (std::abs(m_startPoint.y()-m_endPoint.y()) <= m_lineWidth)) {
    p_painter->drawRect(boundingRect());
  } else if (std::abs(m_x1-m_x2) < std::abs(m_y1-m_y2)) {
    bool positive = (m_x1-m_x2)*(m_y1-m_y2) >= 0;
    double x1 = positive? m_startPoint.x(): m_endPoint.x();
    double y1 = m_startPoint.y();
    double x2 = positive? m_endPoint.x(): m_startPoint.x();
    double y2 = m_endPoint.y();
    double lineWidth = positive? m_lineWidth: -m_lineWidth;

    double y3 = 0.;
    double y4 = scene()->height();
    double previousY = y1;
    double x = x1 + lineWidth;
    for (; positive? x < x2: x > x2; x += lineWidth) {
      double y =  ((x1*y2 - y1*x2)*(y3 - y4) - x*(y1 - y2)*(y4 - y3)) / ((x1 - x2)*(y3 - y4));

      double nextY = 0;
      if (m_y1 <= m_y2) {
        nextY = (std::ceil(y/m_lineWidth))*m_lineWidth;
      } else {
        nextY = (std::floor(y/m_lineWidth))*m_lineWidth;
      }

      p_painter->drawRect(QRectF((positive? x-m_lineWidth: x), previousY, m_lineWidth, nextY-previousY));
      previousY = nextY;
    }
    p_painter->drawRect(QRectF((positive? x-m_lineWidth: x), previousY, m_lineWidth, y2-previousY));
  } else {
    bool positive = (m_x1-m_x2)*(m_y1-m_y2) >= 0;
    double x1 = m_startPoint.x();
    double y1 = positive? m_startPoint.y(): m_endPoint.y();
    double x2 = m_endPoint.x();
    double y2 = positive? m_endPoint.y(): m_startPoint.y();
    double lineWidth = positive? m_lineWidth: -m_lineWidth;

    double x3 = 0.;
    double x4 = scene()->width();
    double previousX = x1;
    double y = y1 + lineWidth;
    for (; positive? y < y2: y > y2; y += lineWidth) {
      double x = (y*(x1 - x2)*(x3 - x4) - (x1*y2 - y1*x2)*(x3 - x4)) / ((y1 - y2)*(x3 - x4));

      double nextX = 0;
      if (m_x1 <= m_x2) {
        nextX = (std::ceil(x/m_lineWidth))*m_lineWidth;
      } else {
        nextX = (std::floor(x/m_lineWidth))*m_lineWidth;
      }

      p_painter->drawRect(QRectF(previousX, (positive? y-m_lineWidth: y), nextX-previousX, m_lineWidth));
      previousX = nextX;
    }
    p_painter->drawRect(QRectF(previousX, (positive? y-m_lineWidth: y), x2-previousX, m_lineWidth));
  }

  p_painter->restore();
}

QRectF GraphicsPixelLine::boundingRect() const {
  return QRectF(m_startPoint, m_endPoint);
}

