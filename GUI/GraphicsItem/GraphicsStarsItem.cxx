#include "GraphicsStarsItem.hxx"

GraphicsStarsItem::GraphicsStarsItem(int p_starsCount, QGraphicsItem* p_parent):
  QGraphicsPixmapItem(p_parent) {

  double xOffset = 0;

  for (int starCount = 1; starCount <= 3; ++starCount) {
    QString starFileName;
    if (starCount <= p_starsCount) {
      starFileName = ":/sprites/level/fullStar.png";
    } else {
      starFileName = ":/sprites/level/emptyStar.png";
    }

    auto starItem = new QGraphicsPixmapItem(QPixmap(starFileName), this);
    starItem->setPos(xOffset, 0.);

    xOffset += starItem->boundingRect().width();
  }
}

QRectF GraphicsStarsItem::boundingRect() const {
  auto width = 0.;
  auto height = 0.;

  for (auto const* child: childItems()) {
    width += child->boundingRect().width();
    height = std::max(height, child->boundingRect().height());
  }

  return QRectF(0, 0, width, height);
}

GraphicsStarsItem::~GraphicsStarsItem() = default;
