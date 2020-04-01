#ifndef GRAPHICSOBJECTITEM_HXX
#define GRAPHICSOBJECTITEM_HXX

#include <QGraphicsItem>
#include <QColor>

#include "Core/Objects/Object.hxx"
#include "Core/Geometry/Segment.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Polygon.hxx"

class Tape;
class Mirror;
class OneWay;
class Portal;
class QStandardItem;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// OBJECT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsObjectItem: public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

public:
  enum ItemRole {
    eStateRole
  };

  enum State {
    eSelectedState,
    eEnabledState,
    eDisabledState,
    eHighlightUpState,
    eHighlightDownState
  };

  GraphicsObjectItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsObjectItem() override;

  QList<QPair<QPoint, Object::ControlPointType>> GetControlPoints() const { return m_controlPoints; }
  void UpdateControlPoints();

  void SetState(State p_state);
  State GetState() const;

  virtual bool Intersect(ppxl::Point const& p_point) const;
  virtual void ComputeBoundingPolygon() = 0;
  inline ppxl::Polygon GetBoundingPolygon() { return m_boundingPolygon; }

Q_SIGNALS:
  void StateChanged();

protected:
  QList<QColor> GetColorAccordingToItemState() const;

  QPolygonF ComputeQuad(const ppxl::Segment& p_line, int p_factor) const;

  virtual void DrawObject(QPainter* p_painter) = 0;
  virtual QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const = 0;
  virtual void DrawControlPoints(QPainter* p_painter);

  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const*, QWidget* = nullptr) override;

  virtual QList<QColor> GetEnabledColors() const = 0;
  virtual QList<QColor> GetDisabledColors() const { return {QColor("#bdbdbd")}; }
  virtual QList<QColor> GetSelectedColors() const { return GetEnabledColors(); }
  virtual QList<QColor> GetHighlightedUpColors() const { return GetEnabledColors(); }
  virtual QList<QColor> GetHighlightedDownColors() const;

  QList<QPair<QPoint, Object::ControlPointType>> m_controlPoints;
  ppxl::Polygon m_boundingPolygon;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// POLYGON
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsPolygonItem: public GraphicsObjectItem {
  Q_OBJECT

public:
  GraphicsPolygonItem(ppxl::Polygon* p_polygon, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPolygonItem() override;

  void SetColor(QColor const& p_color);

  QRectF boundingRect() const override;
  void ComputeBoundingPolygon() override;

protected:
  void DrawObject(QPainter* p_painter) override;
  QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override { return {m_enabledColor}; };

private:
  ppxl::Polygon* m_polygon;
  QColor m_enabledColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsTapeItem: public GraphicsObjectItem {
  Q_OBJECT

public:
  enum ControlPointType {
    eTopLeft,
    eTop,
    eTopRight,
    eRight,
    eBottomRight,
    eBottom,
    eBottomLeft,
    eLeft,
    eCenter,
    eNone
  };

  GraphicsTapeItem(Tape* p_tape, QGraphicsItem* p_parent = nullptr);
  ~GraphicsTapeItem() override;

  QRectF boundingRect() const override;
  void ComputeBoundingPolygon() override;

protected:
  void DrawObject(QPainter* p_painter) override;
  QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;

private:
  Tape* m_tape;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsMirrorItem: public GraphicsObjectItem {
  Q_OBJECT

public:
  enum ControlPointType {
    eA,
    eB,
    eCenter,
    eNone
  };

  GraphicsMirrorItem(Mirror* p_mirror, QGraphicsItem* p_parent = nullptr);
  ~GraphicsMirrorItem() override;

  QRectF boundingRect() const override;
  void ComputeBoundingPolygon() override;

protected:
  void DrawObject(QPainter* p_painter) override;
  QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;

private:
  Mirror* m_mirror;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsOneWayItem: public GraphicsObjectItem {
  Q_OBJECT

public:
  enum ControlPointType {
    eA,
    eB,
    eCenter,
    eNone
  };

  GraphicsOneWayItem(OneWay* p_oneWay, QGraphicsItem* p_parent = nullptr);
  ~GraphicsOneWayItem() override;

  QRectF boundingRect() const override;
  void ComputeBoundingPolygon() override;

protected:
  void DrawObject(QPainter* p_painter) override;
  QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;

private:
  OneWay* m_oneWay;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsPortalItem: public GraphicsObjectItem {
  Q_OBJECT

public:
  enum ControlPointType {
    eAIn,
    eBIn,
    eCenterIn,
    eAOut,
    eBOut,
    eCenterOut,
    eNone
  };

  GraphicsPortalItem(Portal* p_portal, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPortalItem() override;

  bool Intersect(ppxl::Point const& p_point) const override;

  QRectF boundingRect() const override;
  void ComputeBoundingPolygon() override;

protected:
  void DrawObject(QPainter* p_painter) override;
  QList<QPair<QPoint, Object::ControlPointType>> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;
  QList<QColor> GetDisabledColors() const override;

private:
  Portal* m_portal;
  ppxl::Polygon m_boundingPolygonOut;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RECTANGLE SELECTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsRectangleSelectionItem: public QGraphicsRectItem {
public:
  GraphicsRectangleSelectionItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsRectangleSelectionItem() override;

protected:
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const*, QWidget* = nullptr) override;
};

Q_DECLARE_METATYPE(GraphicsObjectItem::State);

#endif
