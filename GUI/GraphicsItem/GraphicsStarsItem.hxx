#ifndef GRAPHICSSTARSITEM_HXX
#define GRAPHICSSTARSITEM_HXX

#include <QGraphicsItem>

class GraphicsStarsItem: public QGraphicsPixmapItem {

public:
  GraphicsStarsItem(QGraphicsItem* p_parent = nullptr);
  GraphicsStarsItem(int p_starsCount, QGraphicsItem* p_parent = nullptr);
  ~GraphicsStarsItem() override;

  void SetStarsCount(int p_starsCount);

  QRectF boundingRect() const override;
};

#endif
