#include "GameOverItem.hxx"
#include "Core/Polygon.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QList>
#include <QPointF>

GameOverItem::GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsRectItem(p_x, p_y, p_width, p_height, p_parent),
  m_polygonsList(),
  m_areas(),
  m_figureCenter() {

  setBrush(QBrush(QColor("#ffffff")));
}

GameOverItem::~GameOverItem() = default;

void GameOverItem::Open(QPointF const& p_startPos, QPointF const& p_endPos) {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(p_startPos);
  animation->setEndValue(p_endPos);
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::OutBack);
  connect(animation, &QPropertyAnimation::finished, this, &GameOverItem::DrawResult);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameOverItem::SetAreasData(QList<ppxl::Polygon*> p_polygonsList, QList<double> const& p_areas, ppxl::Point const& p_figureCenter) {
  Q_ASSERT_X(p_areas.size() == p_polygonsList.size(), "PlayingScribblingView::DrawAreas", "Not as much areas as polygons.");

  m_polygonsList = p_polygonsList;
  m_areas = p_areas;
  m_figureCenter = p_figureCenter;
}

void GameOverItem::DrawResult() {
  for (int row = 0; row < m_polygonsList.size(); ++row) {
    auto polygon = m_polygonsList.at(row);
    QVector<QPointF> qVertices;
    for (auto const& vertex: polygon->GetVertices()) {
      qVertices << QPointF(vertex.GetX(), vertex.GetY());
    }
    auto polygonItem = new QGraphicsPolygonItem(QPolygonF(qVertices), this);
    QTransform transform;
    polygonItem->setTransform(transform.scale(0.5, 0.5));

    auto area = QString::number(m_areas.at(row));
    //DrawText(polygon->Barycenter(), area, 50);
  }
}
