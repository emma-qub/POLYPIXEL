#include "GraphicsRoundedRectItem.hxx"

#include <QPainter>



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsPixelRectItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int const GraphicsPixelRectItem::m_borderWidth = 4;

GraphicsPixelRectItem::GraphicsPixelRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QGraphicsRectItem (p_x, p_y, p_width, p_height, p_parent),
  m_width(p_width),
  m_height(p_height),
  m_radiusType(eMediumRadius),
  m_radiusPositions(eAllRadius),
  m_borders(eAllBorder),
  m_shadows(eBottomShadow),
  m_path() {

  ComputePath();
}

GraphicsPixelRectItem::~GraphicsPixelRectItem() = default;

void GraphicsPixelRectItem::ComputePath() {
  int maxCornerSide = static_cast<int>(eLargeRadius)*m_borderWidth;

  m_path = QPainterPath();
  m_path.setFillRule(Qt::WindingFill);

  // Back bone
  auto shift = static_cast<int>(m_radiusType)*m_borderWidth;
  m_path.addRect(0, shift, m_width, m_height-2*shift);
  m_path.addRect(shift, 0, m_width-2*shift, m_height);

  if (m_radiusType != eFlatRadius && m_radiusPositions != eFlatRadius) {
    // TopLeft corner
    int shiftCorner = m_radiusPositions.testFlag(eTopLeftRadius)? m_borderWidth: 0;
    m_path.addRect(shiftCorner, shiftCorner, maxCornerSide, maxCornerSide);

    // TopRight corner
    shiftCorner = m_radiusPositions.testFlag(eTopRightRadius)? m_borderWidth: 0;
    m_path.addRect(m_width - maxCornerSide - shiftCorner, shiftCorner, maxCornerSide, maxCornerSide);

    // BottomRight corner
    shiftCorner = m_radiusPositions.testFlag(eBottomRightRadius)? m_borderWidth: 0;
    m_path.addRect(m_width - maxCornerSide -shiftCorner, m_height - maxCornerSide - shiftCorner, maxCornerSide, maxCornerSide);

    // BottomLeft corner
    shiftCorner = m_radiusPositions.testFlag(eBottomLeftRadius)? m_borderWidth: 0;
    m_path.addRect(shiftCorner, m_height - maxCornerSide - shiftCorner, maxCornerSide, maxCornerSide);
  }

  m_path = m_path.simplified();
}

QRectF GraphicsPixelRectItem::boundingRect() const {
  QRectF rect(QGraphicsRectItem::boundingRect());
  // Don't count shadow
  if (m_borders.testFlag(eLeftBorder)) {
    rect.setLeft(rect.left()+1);
  }
  if (m_borders.testFlag(eTopBorder)) {
    rect.setTop(rect.top()+1);
  }
  if (m_borders.testFlag(eRightBorder)) {
    rect.setRight(rect.right()-1);
  }
  if (m_borders.testFlag(eBottomBorder)) {
    rect.setBottom(rect.bottom()-1);
  }
  return rect;
}

void GraphicsPixelRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_path);
  p_painter->restore();
}





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
