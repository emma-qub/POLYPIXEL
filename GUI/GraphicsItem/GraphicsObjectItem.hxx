#ifndef GRAPHICSOBJECTITEM_HXX
#define GRAPHICSOBJECTITEM_HXX

#include <QGraphicsItem>

class GraphicsObjectItem: public QGraphicsItem {

public:
  GraphicsObjectItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsObjectItem() override;
};



class GraphicsTapeItem: public GraphicsObjectItem {
  GraphicsTapeItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsTapeItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;
};



class GraphicsMirrorItem: public GraphicsObjectItem {
  GraphicsMirrorItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsMirrorItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;
};



class GraphicsOneWayItem: public GraphicsObjectItem {
  GraphicsOneWayItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsOneWayItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;
};



class GraphicsPortalItem: public GraphicsObjectItem {
  GraphicsPortalItem(QGraphicsItem* p_parent = nullptr);
  ~GraphicsPortalItem() override;

  QRectF boundingRect() const override;
  void paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget = nullptr) override;
};

#endif
