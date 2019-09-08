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

protected:
  QList<QColor> GetColorAccordingToItemState() const;

  virtual QList<QColor> GetEnabledColors() const = 0;
  virtual QList<QColor> GetDisabledColors() const { return {QColor("#")}; }
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
};



class GraphicsTapeItem: public GraphicsObjectItem {

public:
  GraphicsTapeItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsTapeItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;

protected:
  QList<QColor> GetEnabledColors() const override;

private:
  Tape* m_tape;
};



class GraphicsMirrorItem: public GraphicsObjectItem {

public:
  GraphicsMirrorItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsMirrorItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;

protected:
  QList<QColor> GetEnabledColors() const override;

private:
  Mirror* m_mirror;
};



class GraphicsOneWayItem: public GraphicsObjectItem {

public:
  GraphicsOneWayItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsOneWayItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;

protected:
  QList<QColor> GetEnabledColors() const override;

private:
  OneWay* m_oneWay;
};



class GraphicsPortalItem: public GraphicsObjectItem {

public:
  GraphicsPortalItem(QStandardItem* p_item, QGraphicsItem* p_parent = nullptr);
  ~GraphicsPortalItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;

protected:
  QList<QColor> GetEnabledColors() const override;
  QList<QColor> GetDisabledColors() const override;

private:
  Portal* m_portal;
};

#endif
