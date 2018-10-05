#ifndef GRAPHICSROUNDEDRECTITEM_HXX
#define GRAPHICSROUNDEDRECTITEM_HXX

#include <QGraphicsItem>



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

  enum Border {
    eNoBorder = 0b0000,
    eLeftBorder = 0b100,
    eTopBorder = 0b0100,
    eRightBorder = 0b0010,
    eBottomBorder = 0b0001,

    eAllBorder = eLeftBorder | eTopBorder | eRightBorder | eBottomBorder
  };
  Q_DECLARE_FLAGS(Borders, Border)

  enum Shadow {
    eNoShadow = 0b0000,
    eLeftShadow = 0b100,
    eTopShadow = 0b0100,
    eRightShadow = 0b0010,
    eBottomShadow = 0b0001,

    eAllShadow = eLeftShadow | eTopShadow  | eRightShadow | eBottomShadow
  };
  Q_DECLARE_FLAGS(Shadows, Shadow)

  GraphicsPixelRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPixelRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

  void SetRadiusType(RadiusType p_radiusType) { m_radiusType = p_radiusType; ComputePath(); }
  void SetRadiusPositions(RadiusPositions p_radiusPositions) { m_radiusPositions = p_radiusPositions; ComputePath(); }
  void SetBorders(Borders p_borders) { m_borders = p_borders; ComputePath(); }
  void SetShadows(Shadows p_shadows) { m_shadows = p_shadows; ComputePath(); }

protected:
  void ComputePath();

private:
  qreal m_width;
  qreal m_height;
  RadiusType m_radiusType;
  RadiusPositions m_radiusPositions;
  Borders m_borders;
  Shadows m_shadows;
  QPainterPath m_path;
  static int const m_borderWidth;
};





class GraphicsRoundedRectItem: public QGraphicsRectItem {

public:
  GraphicsRoundedRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  ~GraphicsRoundedRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

  void setRoundedRect(QRectF const& p_rect, double p_radiusX, double p_radiusY);

private:
  qreal m_radiusX;
  qreal m_radiusY;
};

class GraphicsTopRoundedRectItem: public QGraphicsRectItem {

public:
  GraphicsTopRoundedRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  ~GraphicsTopRoundedRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

private:
  qreal m_radiusX;
  qreal m_radiusY;
  QPainterPath m_path;
};

class GraphicsBottomRoundedRectItem: public QGraphicsRectItem
{
public:
  GraphicsBottomRoundedRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  ~GraphicsBottomRoundedRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

private:
  qreal m_radiusX;
  qreal m_radiusY;
  QPainterPath m_path;
};

class GraphicsBottomLeftRoundedRectItem: public QGraphicsRectItem
{
public:
  GraphicsBottomLeftRoundedRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  GraphicsBottomLeftRoundedRectItem(QRectF const& p_rect, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  ~GraphicsBottomLeftRoundedRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

private:
  qreal m_radiusX;
  qreal m_radiusY;
  QPainterPath m_path;
};

class GraphicsBottomRightRoundedRectItem: public QGraphicsRectItem
{
public:
  GraphicsBottomRightRoundedRectItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  GraphicsBottomRightRoundedRectItem(QRectF const& p_rect, qreal p_radiusX, qreal p_radiusY, QGraphicsItem* p_parent = nullptr);
  ~GraphicsBottomRightRoundedRectItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) override;

private:
  qreal m_radiusX;
  qreal m_radiusY;
  QPainterPath m_path;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsPixelRectItem::RadiusPositions)
Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsPixelRectItem::Borders)
Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsPixelRectItem::Shadows)

#endif
