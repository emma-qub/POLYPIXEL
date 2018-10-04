#ifndef GRAPHICSROUNDEDRECTITEM_HXX
#define GRAPHICSROUNDEDRECTITEM_HXX

#include <QGraphicsItem>

class GraphicsRoundedRectItem: public QGraphicsRectItem
{
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

class GraphicsTopRoundedRectItem: public QGraphicsRectItem
{
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

#endif
