#ifndef GRAPHICSSTARSITEM_HXX
#define GRAPHICSSTARSITEM_HXX

#include <QGraphicsItem>

class GraphicsStarsItem: public QGraphicsPixmapItem {

public:
  GraphicsStarsItem(int p_starsCount, QGraphicsItem* p_parent = nullptr);
  ~GraphicsStarsItem() override;

  QRectF boundingRect() const override;
};

#endif
