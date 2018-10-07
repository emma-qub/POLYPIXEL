#include "GraphicsGoalItem.hxx"

GraphicsGoalItem::GraphicsGoalItem(QFont const& p_font, QGraphicsItem* p_parent):
  QGraphicsRectItem(p_parent),
  m_pixmapItem(new QGraphicsPixmapItem(this)),
  m_textItem(new QGraphicsSimpleTextItem(this)) {

  m_textItem->setFont(p_font);
}

GraphicsGoalItem::GraphicsGoalItem(QFont const& p_font, QPixmap const& p_pixmap, int p_goalNumber, QGraphicsItem* p_parent):
  GraphicsGoalItem(p_font, p_parent) {

  SetPixmap(p_pixmap);
  SetGoalNumber(p_goalNumber);
}

void GraphicsGoalItem::SetPixmap(QPixmap const& p_pixmap) {
  m_pixmapItem->setPixmap(p_pixmap);
  m_pixmapItem->setPos(0, 0);

}

void GraphicsGoalItem::SetGoalNumber(int p_gaolNumber) {
  m_textItem->setText(QString::number(p_gaolNumber));
  AlignPixmapAndText();
}

void GraphicsGoalItem::AlignPixmapAndText() {
  auto pixmapRect = m_pixmapItem->boundingRect();
  auto textRect = m_textItem->boundingRect();
  m_textItem->setPos(0, pixmapRect.height());
  if (textRect.width() < pixmapRect.width()) {
    m_textItem->setPos((pixmapRect.width()-textRect.width())/2., pixmapRect.height());
  } else {
    m_pixmapItem->setPos((textRect.width()-pixmapRect.width())/2., 0.);
  }
}

QRectF GraphicsGoalItem::boundingRect() const {
  return QRectF(0, 0, std::max(m_pixmapItem->boundingRect().width(), m_textItem->boundingRect().width()), m_pixmapItem->boundingRect().height()+ m_textItem->boundingRect().height());
}

GraphicsGoalItem::~GraphicsGoalItem() = default;
