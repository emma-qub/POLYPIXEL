#include "GraphicsGoalItem.hxx"

GraphicsGoalItem::GraphicsGoalItem(QPixmap const& p_pixmap, QString const& p_text, QFont const& p_font, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_parent),
  m_pixmapItem(nullptr),
  m_textItem(nullptr) {

  m_pixmapItem = new QGraphicsPixmapItem(p_pixmap, this);
  auto pixmapRect = m_pixmapItem->boundingRect();
  m_pixmapItem->setPos(0, 0);

  m_textItem = new QGraphicsSimpleTextItem(p_text, this);
  m_textItem->setFont(p_font);
  auto textRect = m_textItem->boundingRect();
  m_textItem->setPos(0, pixmapRect.height());

  m_pixmapItem->setPos(std::min((textRect.width()-pixmapRect.width())/2., 0.), 0.);
  m_textItem->setPos(std::min((pixmapRect.width()-textRect.width())/2., 0.), pixmapRect.height());
}

QRectF GraphicsGoalItem::boundingRect() const {
  return QRectF(0, 0, std::max(m_pixmapItem->boundingRect().width(), m_textItem->boundingRect().width()), m_pixmapItem->boundingRect().height()+ m_textItem->boundingRect().height());
}

GraphicsGoalItem::~GraphicsGoalItem() = default;
