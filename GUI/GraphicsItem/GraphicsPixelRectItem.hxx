#ifndef GRAPHICSROUNDEDRECTITEM_HXX
#define GRAPHICSROUNDEDRECTITEM_HXX

#include <QGraphicsItem>
#include <QBrush>

class GraphicsPixelRectItem: public QGraphicsRectItem {
public:
  enum RadiusType {
    eFlatRadius,
    eSmallRadius,
    eMediumRadius,
    eLargeRadius
  };

  enum RadiusPosition {
    eNoRadius = 0b0000,
    eTopLeftRadius = 0b1000,
    eTopRightRadius = 0b0100,
    eBottomLeftRadius = 0b0010,
    eBottomRightRadius = 0b0001,

    eTopRadius = eTopLeftRadius | eTopRightRadius,
    eBottomRadius = eBottomLeftRadius | eBottomRightRadius,
    eLeftRadius = eTopLeftRadius | eBottomLeftRadius,
    eRightRadius = eTopRightRadius | eBottomRightRadius,
    eAllRadius = eTopLeftRadius | eTopRightRadius | eBottomLeftRadius | eBottomRightRadius

  };
  Q_DECLARE_FLAGS(RadiusPositions, RadiusPosition)

  enum BorderPosition {
    eNoBorder = 0b0000,
    eLeftBorder = 0b1000,
    eTopBorder = 0b0100,
    eRightBorder = 0b0010,
    eBottomBorder = 0b0001,

    eTopLeftBorder = eTopBorder | eLeftBorder,
    eTopRightBorder = eTopBorder | eRightBorder,
    eBottomLeftBorder = eBottomBorder | eLeftBorder,
    eBottomRightBorder = eBottomBorder | eRightBorder,

    eTopBottom = eTopBorder | eBottomBorder,
    eLeftRight = eLeftBorder | eRightBorder,

    eNoBottomBorder = eLeftBorder | eTopBorder | eRightBorder,
    eNoTopBorder = eLeftBorder | eBottomBorder | eRightBorder,
    eNoLeftBorder = eTopBorder| eRightBorder | eBottomBorder,
    eNoRightBorder = eBottomBorder | eLeftBorder | eTopBorder,

    eAllBorder = eLeftBorder | eTopBorder | eRightBorder | eBottomBorder
  };
  Q_DECLARE_FLAGS(BorderPositions, BorderPosition)

  GraphicsPixelRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPixelRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

  void SetRadiusType(RadiusType p_radiusType) { m_radiusType = p_radiusType; ComputePaths(); }
  void SetRadiusPositions(RadiusPositions p_radiusPositions) { m_radiusPositions = p_radiusPositions; ComputePaths(); }
  void SetBorderPositions(BorderPositions p_borderPositions) { m_borderPositions = p_borderPositions; ComputePaths(); }
  void SetBorderBrush(QBrush const& p_borderBrush) { m_borderBrush = p_borderBrush; }
  void SetHasShadows(bool p_hasShadows) { m_hasShadows = p_hasShadows; ComputePaths(); }
  void SetShadowBrush(QBrush const& p_shadowBrush) { m_shadowBrush = p_shadowBrush; }
  void SetIsInnerRect(bool p_isInnerRect) { m_isInnerRect = p_isInnerRect; ComputePaths(); }

protected:
  void ComputePaths();
  void ComputeShapePath();
  void ComputeBorderPath();
  void ComputeShadowPath();

private:
  qreal m_width;
  qreal m_height;
  RadiusType m_radiusType;
  RadiusPositions m_radiusPositions;
  BorderPositions m_borderPositions;
  QBrush m_borderBrush;
  bool m_hasShadows;
  QBrush m_shadowBrush;
  QPainterPath m_shapePath;
  QPainterPath m_borderPath;
  QPainterPath m_shadowPath;
  bool m_isInnerRect;
  static int const m_borderWidth;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsPixelRectItem::RadiusPositions)
Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsPixelRectItem::BorderPositions)

#endif
