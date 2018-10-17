#include "GraphicsObjectItem.hxx"

GraphicsObjectItem::GraphicsObjectItem(QGraphicsItem* p_parent):
  QGraphicsItem(p_parent) {
}

GraphicsObjectItem::~GraphicsObjectItem() = default;



GraphicsTapeItem::GraphicsTapeItem(QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent) {
}

GraphicsTapeItem::~GraphicsTapeItem() = default;

QRectF GraphicsTapeItem::boundingRect() const {
  QRectF rect;

  return rect;
}

void GraphicsTapeItem::paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) {

}



GraphicsMirrorItem::GraphicsMirrorItem(QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent) {
}

GraphicsMirrorItem::~GraphicsMirrorItem() = default;

QRectF GraphicsMirrorItem::boundingRect() const {
  QRectF rect;

  return rect;
}

void GraphicsMirrorItem::paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) {

}



GraphicsOneWayItem::GraphicsOneWayItem(QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent) {
}

GraphicsOneWayItem::~GraphicsOneWayItem() = default;

QRectF GraphicsOneWayItem::boundingRect() const {
  QRectF rect;

  return rect;
}

void GraphicsOneWayItem::paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) {

}



GraphicsPortalItem::GraphicsPortalItem(QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent) {
}

GraphicsPortalItem::~GraphicsPortalItem() = default;

QRectF GraphicsPortalItem::boundingRect() const {
  QRectF rect;

  return rect;
}

void GraphicsPortalItem::paint(QPainter* p_painter, QStyleOptionGraphicsItem const* p_option, QWidget* p_widget) {

}
