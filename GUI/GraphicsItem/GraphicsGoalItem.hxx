#ifndef GRAPHICSGOALITEM_HXX
#define GRAPHICSGOALITEM_HXX

#include <QGraphicsItem>

class GraphicsGoalItem: public QGraphicsRectItem {

public:
  GraphicsGoalItem(QFont const& p_font, QGraphicsItem* p_parent = nullptr);
  GraphicsGoalItem(QFont const& p_font, QPixmap const& p_pixmap, int p_goalNumber, QGraphicsItem* p_parent = nullptr);
  ~GraphicsGoalItem() override;

  void SetPixmap(const QPixmap& p_pixmap);
  void SetGoalNumber(int p_goalNumber);

  QRectF boundingRect() const override;

protected:
  void AlignPixmapAndText();

private:
  QGraphicsPixmapItem* m_pixmapItem;
  QGraphicsSimpleTextItem* m_textItem;
};

#endif
