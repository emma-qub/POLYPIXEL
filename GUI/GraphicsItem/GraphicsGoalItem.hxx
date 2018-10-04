#ifndef GRAPHICSGOALITEM_HXX
#define GRAPHICSGOALITEM_HXX

#include <QGraphicsItem>

class GraphicsGoalItem: public QGraphicsRectItem {

public:
  GraphicsGoalItem(QPixmap const& p_pixmap, QString const& p_text, QFont const& p_font, QGraphicsItem* p_parent = nullptr);
  ~GraphicsGoalItem() override;

  QRectF boundingRect() const override;

private:
  QGraphicsPixmapItem* m_pixmapItem;
  QGraphicsSimpleTextItem* m_textItem;
};

#endif
