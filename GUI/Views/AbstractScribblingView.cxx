#include "AbstractScribblingView.hxx"

#include "GUI/Models/GameModel.hxx"

#include <QPainter>
#include <QMouseEvent>

AbstractScribblingView::AbstractScribblingView(QWidget* p_parent):
  QWidget(p_parent),
  m_model(nullptr),
  m_myPenWidth(1) {
}

AbstractScribblingView::~AbstractScribblingView() = default;

void AbstractScribblingView::SetModel(AbstractLevelModel* p_model) {
  m_model = p_model;
}

void AbstractScribblingView::DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  ppxl::Point startPoint(p_line.GetA());
  ppxl::Point endPoint(p_line.GetB());
  DrawLine(startPoint, endPoint, p_color, p_penStyle);
}

void AbstractScribblingView::DrawFromModel() {
  if (!m_model)
  {
    return;
  }

  // Draw every polygon in model color
  auto polygonItems = m_model->GetPolygonItemsList();
  for (auto polygonItem: polygonItems) {
    auto polygon = polygonItem->data(GameModel::ePolygonRole).value<ppxl::Polygon>();
    auto color = polygonItem->data(Qt::DecorationRole).value<QColor>();

    auto vertices = polygon.GetVertices();
    for (unsigned k = 0; k < vertices.size(); k++) {
      ppxl::Point A(vertices.at(k));
      ppxl::Point B(vertices.at((k+1)%vertices.size()));

      DrawLine(A, B, color);
    }
  }
}

void AbstractScribblingView::ClearImage() {
  m_image.fill(Qt::white);
}

void AbstractScribblingView::DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle) {
  QPoint startPoint(static_cast<int>(p_startPoint.GetX()), static_cast<int>(p_startPoint.GetY()));
  QPoint endPoint(static_cast<int>(p_endPoint.GetX()), static_cast<int>(p_endPoint.GetY()));
  DrawLine(startPoint, endPoint, p_color, p_penStyle);
}

void AbstractScribblingView::DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle) {
  QPainter painter(&m_image);
  painter.setPen(QPen(p_color, m_myPenWidth, p_penStyle, Qt::RoundCap, Qt::BevelJoin));
  painter.drawLine(p_startPoint, p_endPoint);

  update();
}

void AbstractScribblingView::ResizeImage(QImage* p_image, const QSize& p_newSize) {
  if (p_image->size() == p_newSize)
  {
    return;
  }

  QImage newImage(p_newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *p_image);
  *p_image = newImage;
}

void AbstractScribblingView::resizeEvent(QResizeEvent* p_event) {
  if (width() > m_image.width() || height() > m_image.height()) {
    int newWidth = qMax(width() + 128, m_image.width());
    int newHeight = qMax(height() + 128, m_image.height());
    ResizeImage(&m_image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(p_event);
}
