#include "PlayingScribblingView.hxx"

#include "GUI/Models/PolygonModel.hxx"

#include <QPainter>
#include <QMouseEvent>

PlayingScribblingView::PlayingScribblingView(QWidget* p_parent):
  AbstractScribblingView(p_parent),
  m_scribbling(false) {
}

void PlayingScribblingView::DrawAreas(const QList<double>& p_areas)
{
  auto polygons = GetModel()->GetPolygonsList();
  assert(p_areas.size() == polygons.size());

  for (int row = 0; row < polygons.size(); ++row) {
    auto polygon = polygons.at(row);
    auto area = QString::number(p_areas.at(row));
    DrawText(polygon->Barycenter(), area, 50);
  }
}

PlayingScribblingView::~PlayingScribblingView() = default;

void PlayingScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && p_event->buttons() == Qt::LeftButton && !m_scribbling) {
    m_scribbling = true;
    Q_EMIT(Scribbling(p_event->pos()));
  }
}

void PlayingScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    Q_EMIT(Moving(p_event->pos()));
  }
}

void PlayingScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    Q_EMIT(Slicing(p_event->pos()));
    m_scribbling = false;
  }
}

void PlayingScribblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, GetImage(), dirtyRect);
}
