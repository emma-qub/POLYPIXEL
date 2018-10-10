#include "GraphicsPixelRectItem.hxx"

#include <QPainter>

int const GraphicsPixelRectItem::m_borderWidth = 8;

GraphicsPixelRectItem::GraphicsPixelRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QGraphicsRectItem (p_x, p_y, p_width, p_height, p_parent),
  m_width(p_width),
  m_height(p_height),
  m_radiusType(eMediumRadius),
  m_radiusPositions(eAllRadius),
  m_borderPositions(eAllBorder),
  m_borderBrush(QBrush(QColor(42, 5, 37))),
  m_hasShadows(true),
  m_shadowBrush(m_borderBrush),
  m_shapePath(),
  m_borderPath(),
  m_shadowPath(),
  m_isInnerRect(false) {

  setPen(Qt::NoPen);
  ComputePaths();
}

GraphicsPixelRectItem::~GraphicsPixelRectItem() = default;

void GraphicsPixelRectItem::ComputePaths() {
  ComputeShapePath();
  ComputeBorderPath();
  ComputeShadowPath();
}

void GraphicsPixelRectItem::ComputeShapePath() {
  int maxCornerSide = static_cast<int>(eLargeRadius)*m_borderWidth;

  m_shapePath = QPainterPath();
  m_shapePath.setFillRule(Qt::WindingFill);

  // Back bone
  auto shift = (m_radiusPositions == eNoRadius)? 0: static_cast<int>(m_radiusType)*m_borderWidth;
  m_shapePath.addRect(0, shift, m_width, m_height-2*shift);
  m_shapePath.addRect(shift, 0, m_width-2*shift, m_height);

  if (m_radiusType != eFlatRadius && m_radiusPositions != eNoRadius) {
    // TopLeft corner
    int shiftCorner = m_radiusPositions.testFlag(eTopLeftRadius)? m_borderWidth: 0;
    m_shapePath.addRect(shiftCorner, shiftCorner, maxCornerSide, maxCornerSide);

    // TopRight corner
    shiftCorner = m_radiusPositions.testFlag(eTopRightRadius)? m_borderWidth: 0;
    m_shapePath.addRect(m_width - maxCornerSide - shiftCorner, shiftCorner, maxCornerSide, maxCornerSide);

    // BottomRight corner
    shiftCorner = m_radiusPositions.testFlag(eBottomRightRadius)? m_borderWidth: 0;
    m_shapePath.addRect(m_width - maxCornerSide -shiftCorner, m_height - maxCornerSide - shiftCorner, maxCornerSide, maxCornerSide);

    // BottomLeft corner
    shiftCorner = m_radiusPositions.testFlag(eBottomLeftRadius)? m_borderWidth: 0;
    m_shapePath.addRect(shiftCorner, m_height - maxCornerSide - shiftCorner, maxCornerSide, maxCornerSide);
  }

  m_shapePath = m_shapePath.simplified();
}

void GraphicsPixelRectItem::ComputeBorderPath() {
  int maxCornerSide = static_cast<int>(eLargeRadius)*m_borderWidth;

  m_borderPath = QPainterPath();
  m_borderPath.setFillRule(Qt::WindingFill);

  if (!m_borderPositions.testFlag(eNoBorder)) {
    // Back bone
    auto shift = (m_radiusPositions == eNoRadius)? 0: static_cast<int>(m_radiusType)*m_borderWidth;
    if (m_borderPositions.testFlag(eLeftBorder)) {
      m_borderPath.addRect(0, shift, m_borderWidth, m_height-2*shift);
    }
    if (m_borderPositions.testFlag(eTopBorder)) {
      m_borderPath.addRect(shift, 0, m_width-2*shift, m_borderWidth);
    }
    if (m_borderPositions.testFlag(eRightBorder)) {
      m_borderPath.addRect(m_width-m_borderWidth, shift, m_borderWidth, m_height-2*shift);
    }
    if (m_borderPositions.testFlag(eBottomBorder)) {
      m_borderPath.addRect(shift, m_height-m_borderWidth, m_width-2*shift, m_borderWidth);
    }

    if (m_radiusType != eFlatRadius && m_radiusPositions != eNoRadius) {
      // TopLeft corner
      auto cornerRounded = m_radiusPositions.testFlag(eTopLeftRadius);
      int shiftCorner = cornerRounded? m_borderWidth: 0;
      int cornerSide = cornerRounded? (static_cast<int>(m_radiusType)-1)*m_borderWidth: maxCornerSide;
      if (m_borderPositions.testFlag(eTopBorder)) {
        m_borderPath.addRect(shiftCorner, shiftCorner, cornerSide, m_borderWidth);
      }
      if (m_borderPositions.testFlag(eLeftBorder)) {
        m_borderPath.addRect(shiftCorner, shiftCorner, m_borderWidth, cornerSide);
      }

      // TopRight corner
      cornerRounded = m_radiusPositions.testFlag(eTopRightRadius);
      shiftCorner = cornerRounded? m_borderWidth: 0;
      cornerSide = cornerRounded? (static_cast<int>(m_radiusType)-1)*m_borderWidth: maxCornerSide;
      if (m_borderPositions.testFlag(eTopBorder)) {
        m_borderPath.addRect(m_width - cornerSide - shiftCorner, shiftCorner, cornerSide, m_borderWidth);
      }
      if (m_borderPositions.testFlag(eRightBorder)) {
        m_borderPath.addRect(m_width - m_borderWidth - shiftCorner, shiftCorner, m_borderWidth, cornerSide);
      }

      // BottomRight corner
      cornerRounded = m_radiusPositions.testFlag(eBottomRightRadius);
      shiftCorner = cornerRounded? m_borderWidth: 0;
      cornerSide = cornerRounded? (static_cast<int>(m_radiusType)-1)*m_borderWidth: maxCornerSide;
      if (m_borderPositions.testFlag(eBottomBorder)) {
        m_borderPath.addRect(m_width - cornerSide - shiftCorner, m_height - shiftCorner - m_borderWidth, cornerSide, m_borderWidth);
      }
      if (m_borderPositions.testFlag(eRightBorder)) {
        m_borderPath.addRect(m_width - m_borderWidth - shiftCorner, m_height - cornerSide - shiftCorner, m_borderWidth, cornerSide);
      }

      // BottomLeft corner
      cornerRounded = m_radiusPositions.testFlag(eBottomLeftRadius);
      shiftCorner = cornerRounded? m_borderWidth: 0;
      cornerSide = cornerRounded? (static_cast<int>(m_radiusType)-1)*m_borderWidth: maxCornerSide;
      if (m_borderPositions.testFlag(eBottomBorder)) {
        m_borderPath.addRect(shiftCorner, m_height - shiftCorner - m_borderWidth, cornerSide, m_borderWidth);
      }
      if (m_borderPositions.testFlag(eLeftBorder)) {
        m_borderPath.addRect(shiftCorner, m_height - cornerSide - shiftCorner, m_borderWidth, cornerSide);
      }
    }
  }

  m_borderPath = m_borderPath.simplified();
}

void GraphicsPixelRectItem::ComputeShadowPath() {
  m_shadowPath = QPainterPath();

  if (m_hasShadows) {
    // Back bone
    auto shift = static_cast<int>(m_radiusType)*m_borderWidth;
    m_shadowPath.addRect(shift, m_height, m_width-2*shift, m_borderWidth);

    if (m_radiusType != eFlatRadius) {
      auto cornerLeftRounded = m_radiusPositions.testFlag(eBottomLeftRadius);
      auto cornerRightRounded = m_radiusPositions.testFlag(eBottomRightRadius);

      // Extern parts
      if (!m_isInnerRect){
        // BottomLeft corner
        int firstHeightShift = cornerLeftRounded? shift: 0;
        m_shadowPath.addRect(0, m_height-firstHeightShift, m_borderWidth, m_borderWidth);

        // BottomRight corner
        int lastHeightShift = cornerRightRounded? shift: 0;
        m_shadowPath.addRect(m_width - m_borderWidth, m_height-lastHeightShift, m_borderWidth, m_borderWidth);
      }

      // Middle parts
      if (m_radiusType == eMediumRadius || m_radiusType == eLargeRadius) {
        int shadowWidth = (static_cast<int>(m_radiusType-1))*m_borderWidth;

        // BottomLeft corner
        int leftHeightShift = cornerLeftRounded? m_borderWidth: 0;
        m_shadowPath.addRect(m_borderWidth, m_height-leftHeightShift, shadowWidth, m_borderWidth);

        // BottomRight corner
        int rightHeightShift = cornerRightRounded? m_borderWidth: 0;
        m_shadowPath.addRect(m_width - m_borderWidth - shadowWidth, m_height-rightHeightShift, shadowWidth, m_borderWidth);
      }
    }
  }

  m_shadowPath = m_shadowPath.simplified();
}

QRectF GraphicsPixelRectItem::boundingRect() const {
  if (m_hasShadows)
  {
    QRectF rect(QGraphicsRectItem::boundingRect());
    rect.setBottom(rect.bottom()+m_borderWidth);
    return rect;
  }

  return QGraphicsRectItem::boundingRect();
}

void GraphicsPixelRectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget) {
  Q_UNUSED(p_option);
  Q_UNUSED(p_widget);

  p_painter->save();
  p_painter->setBrush(brush());
  p_painter->setPen(pen());
  p_painter->drawPath(m_shapePath);
  p_painter->setBrush(m_borderBrush);
  p_painter->drawPath(m_borderPath);
  p_painter->setOpacity(0.5);
  p_painter->setBrush(m_shadowBrush);
  p_painter->drawPath(m_shadowPath);
  p_painter->restore();
}
