#include "PlayingScribblingView.hxx"

#include "GUI/Models/PolygonModel.hxx"

#include <QPainter>
#include <QMouseEvent>

PlayingScribblingView::PlayingScribblingView(QWidget* p_parent):
  AbstractScribblingView(p_parent),
  m_scribbling(false),
  m_cursorPosition(0, 0) {

  setFocusPolicy(Qt::StrongFocus);
}

void PlayingScribblingView::DrawAreas(QList<double> const& p_areas) {
  auto polygons = GetModel()->GetPolygonsList();
  assert(p_areas.size() == polygons.size());

  for (int row = 0; row < polygons.size(); ++row) {
    auto polygon = polygons.at(row);
    auto area = QString::number(p_areas.at(row));
    DrawText(polygon->Barycenter(), area, 50);
  }
}

PlayingScribblingView::~PlayingScribblingView() = default;

void PlayingScribblingView::keyPressEvent(QKeyEvent* p_event) {
  if (m_scribbling && p_event->key() == Qt::Key_Control)
  {
    Q_EMIT(ControlPressed(m_cursorPosition));
  }

  QWidget::keyPressEvent(p_event);
}

void PlayingScribblingView::keyReleaseEvent(QKeyEvent* p_event) {
  if (m_scribbling && p_event->key() == Qt::Key_Control)
  {
    Q_EMIT(ControlReleased(m_cursorPosition));
  }

  QWidget::keyReleaseEvent(p_event);
}

void PlayingScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && p_event->buttons() == Qt::LeftButton && !m_scribbling) {
    m_scribbling = true;
    m_cursorPosition = p_event->pos();
    Q_EMIT(Scribbling(p_event->pos()));
  }
}

void PlayingScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    m_cursorPosition = p_event->pos();
    Q_EMIT(Moving(p_event->pos()));
  }
}

void PlayingScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    m_cursorPosition = p_event->pos();
    Q_EMIT(Slicing(p_event->pos()));
    m_scribbling = false;
  }
}

void PlayingScribblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, GetImage(), dirtyRect);
}
