#include "GraphicsRoundedRectItem.hxx"

#include <QPainter>



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsRoundedRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GraphicsRoundedRectItem::GraphicsRoundedRectItem(
  qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_radiusX(p_radiusX),
  m_radiusY(p_radiusY) {
}

GraphicsRoundedRectItem::~GraphicsRoundedRectItem() = default;

QRectF GraphicsRoundedRectItem::boundingRect() const {
  return QGraphicsRectItem::boundingRect();
}

void GraphicsRoundedRectItem::paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawRoundedRect(rect(), m_radiusX, m_radiusY);
  p_painter->restore();
}

void GraphicsRoundedRectItem::setRoundedRect(const QRectF& p_rect, double p_radiusX, double p_radiusY) {
  setRect(p_rect);
  m_radiusX = p_radiusX;
  m_radiusY = p_radiusY;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsTopRoundedRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsTopRoundedRectItem::GraphicsTopRoundedRectItem(
  qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_radiusX(p_radiusX),
  m_radiusY(p_radiusY),
  m_path() {

  m_path.setFillRule(Qt::WindingFill);
  m_path.addRoundedRect(QRectF(0, 0, p_width, p_height), m_radiusX, m_radiusY);
  m_path.addRect(QRectF(0, p_height-m_radiusY, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(p_width-m_radiusX, p_height-m_radiusY, m_radiusX, m_radiusY));
  m_path = m_path.simplified() ;
}

GraphicsTopRoundedRectItem::~GraphicsTopRoundedRectItem() = default;

QRectF GraphicsTopRoundedRectItem::boundingRect() const {
  return QGraphicsRectItem::boundingRect();
}

void GraphicsTopRoundedRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_path);
  p_painter->restore();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsBottomRoundedRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsBottomRoundedRectItem::GraphicsBottomRoundedRectItem(
  qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_radiusX(p_radiusX),
  m_radiusY(p_radiusY),
  m_path() {

  m_path.setFillRule(Qt::WindingFill);
  m_path.addRoundedRect(QRectF(0, 0, p_width, p_height), m_radiusX, m_radiusY);
  m_path.addRect(QRectF(0, 0, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(p_width-m_radiusX, 0, m_radiusX, m_radiusY));
  m_path = m_path.simplified() ;
}

GraphicsBottomRoundedRectItem::~GraphicsBottomRoundedRectItem() = default;

QRectF GraphicsBottomRoundedRectItem::boundingRect() const {
  return QGraphicsRectItem::boundingRect();
}

void GraphicsBottomRoundedRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_path);
  p_painter->restore();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsBottomLeftRoundedRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsBottomLeftRoundedRectItem::GraphicsBottomLeftRoundedRectItem(
  qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_radiusX(p_radiusX),
  m_radiusY(p_radiusY),
  m_path() {

  m_path.setFillRule(Qt::WindingFill);
  m_path.addRoundedRect(QRectF(0, 0, p_width, p_height), m_radiusX, m_radiusY);
  m_path.addRect(QRectF(0, 0, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(p_width-m_radiusX, 0, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(p_width-m_radiusX, p_height-m_radiusY, m_radiusX, m_radiusY));
  m_path = m_path.simplified() ;
}

GraphicsBottomLeftRoundedRectItem::GraphicsBottomLeftRoundedRectItem(
  QRectF const& p_rect, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent) {
  GraphicsBottomRightRoundedRectItem(p_rect.left(), p_rect.top(), p_rect.width(), p_rect.height(), p_radiusX, p_radiusY, p_parent);
}

GraphicsBottomLeftRoundedRectItem::~GraphicsBottomLeftRoundedRectItem() = default;

QRectF GraphicsBottomLeftRoundedRectItem::boundingRect() const {
  return QGraphicsRectItem::boundingRect();
}

void GraphicsBottomLeftRoundedRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_path);
  p_painter->restore();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsBottomRightRoundedRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsBottomRightRoundedRectItem::GraphicsBottomRightRoundedRectItem(
  qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_radiusX(p_radiusX),
  m_radiusY(p_radiusY),
  m_path() {

  m_path.setFillRule(Qt::WindingFill);
  m_path.addRoundedRect(QRectF(0, 0, p_width, p_height), m_radiusX, m_radiusY);
  m_path.addRect(QRectF(0, 0, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(p_width-m_radiusX, 0, m_radiusX, m_radiusY));
  m_path.addRect(QRectF(0, p_height-m_radiusY, m_radiusX, m_radiusY));
  m_path = m_path.simplified() ;
}

GraphicsBottomRightRoundedRectItem::GraphicsBottomRightRoundedRectItem(
  QRectF const& p_rect, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent) {
  GraphicsBottomRightRoundedRectItem(p_rect.left(), p_rect.top(), p_rect.width(), p_rect.height(), p_radiusX, p_radiusY, p_parent);
}

GraphicsBottomRightRoundedRectItem::~GraphicsBottomRightRoundedRectItem() = default;

QRectF GraphicsBottomRightRoundedRectItem::boundingRect() const {
  return QGraphicsRectItem::boundingRect();
}

void GraphicsBottomRightRoundedRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_path);
  p_painter->restore();
}
