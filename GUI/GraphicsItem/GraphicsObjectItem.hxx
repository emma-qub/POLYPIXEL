#ifndef GRAPHICSOBJECTITEM_HXX
#define GRAPHICSOBJECTITEM_HXX

#include <QGraphicsItem>
#include <QColor>

class Object;
class Tape;
class Mirror;
class OneWay;
class Portal;
class QStandardItem;

class GraphicsObjectItem: public QGraphicsItem {

public:
  GraphicsObjectItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsObjectItem() override;

  QList<QPoint> GetControlPoints() const { return m_controlPoints; }

protected:
  QList<QColor> GetColorAccordingToItemState() const;

  virtual void DrawObject(QPainter* p_painter) = 0;
  virtual QList<QPoint> ComputeControlPoints() const = 0;
  virtual void DrawControlPoints(QPainter* p_painter);

  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;

  virtual QList<QColor> GetEnabledColors() const = 0;
  virtual QList<QColor> GetDisabledColors() const { return {QColor("#bdbdbd")}; }
  virtual QList<QColor> GetSelectedColors() const { return GetEnabledColors(); }
  virtual QList<QColor> GetHighlightedUpColors() const { return GetEnabledColors(); }
  virtual QList<QColor> GetHighlightedDownColors() const {
    QList<QColor> highlightedColors;
    for (auto color: GetEnabledColors()) {
      color.setAlpha(63);
      highlightedColors << color;
    }
    return highlightedColors;
  }

  QStandardItem* m_item;
  QList<QPoint> m_controlPoints;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsTapeItem: public GraphicsObjectItem {

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

  GraphicsTapeItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsTapeItem() override;

  QRectF boundingRect() const override;
  void DrawObject(QPainter* p_painter) override;

protected:
  QList<QPoint> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;

private:
  Tape* m_tape;

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsMirrorItem: public GraphicsObjectItem {

public:
  enum ControlPointType {
    eA,
    eB,
    eCenter,
    eNone
  };

  GraphicsMirrorItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsMirrorItem() override;

  QRectF boundingRect() const override;
  void DrawObject(QPainter* p_painter) override;

protected:
  QList<QPoint> ComputeControlPoints() const override;
  QList<QColor> GetEnabledColors() const override;

private:
  Mirror* m_mirror;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsOneWayItem: public GraphicsObjectItem {

public:
  enum ControlPointType {
    eA,
    eB,
    eCenter,
    eNone
  };

  GraphicsOneWayItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsOneWayItem() override;

  QRectF boundingRect() const override;
  void DrawObject(QPainter* p_painter) override;

protected:
  QList<QPoint> ComputeControlPoints() const override;

  QList<QColor> GetEnabledColors() const override;

private:
  OneWay* m_oneWay;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GraphicsPortalItem: public GraphicsObjectItem {

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

  GraphicsPortalItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPortalItem() override;

  QRectF boundingRect() const override;
  void DrawObject(QPainter* p_painter) override;

protected:
  QList<QPoint> ComputeControlPoints() const override;

  QList<QColor> GetEnabledColors() const override;
  QList<QColor> GetDisabledColors() const override;

private:
  Portal* m_portal;
};

#endif
