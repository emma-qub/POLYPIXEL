#include "AbstractScribblingView.hxx"

#include "GUI/Models/PolygonModel.hxx"

#include <QPainter>
#include <QMouseEvent>

AbstractScribblingView::AbstractScribblingView(QWidget* p_parent):
  QWidget(p_parent),
  m_model(nullptr),
  m_myPenWidth(1) {
}

AbstractScribblingView::~AbstractScribblingView() = default;

void AbstractScribblingView::SetModel(PolygonModel* p_model) {
  m_model = p_model;
}

void AbstractScribblingView::DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  ppxl::Point startPoint(p_line.GetA());
  ppxl::Point endPoint(p_line.GetB());
  DrawLine(startPoint, endPoint, p_color, p_penStyle);
}

void AbstractScribblingView::DrawText(ppxl::Point p_position, const QString& p_text,  int p_weight, ppxl::Vector const& shiftVector) {
  QPainter painter(&GetImage());
  QFont font("", 12, p_weight);
  QFontMetrics fm(font);
  auto vertexSize = fm.size(Qt::TextSingleLine, p_text);
  ppxl::Point topLeft(p_position);
  topLeft.Move(-vertexSize.width()/2., -vertexSize.height()/2.);
  topLeft.Translated(shiftVector*10);
  ppxl::Point bottomRight(p_position);
  bottomRight.Move(vertexSize.width()/2., vertexSize.height()/2.);
  bottomRight.Translated(shiftVector*10);
  QRectF boundingRect(QPointF(topLeft.GetX(), topLeft.GetY()), QPointF(bottomRight.GetX(), bottomRight.GetY()));
  painter.setFont(font);
  painter.drawText(boundingRect, p_text);
}

void AbstractScribblingView::DrawFromModel() {
  if (!m_model)
  {
    return;
  }

  // Draw every polygon in model color
  auto polygonItems = m_model->GetPolygonsItem();

  for (int polygonRow = 0; polygonRow < polygonItems->rowCount(); ++polygonRow) {
    auto polygonItem = polygonItems->child(polygonRow, 0);
    for (int row = 0; row < polygonItem->rowCount(); ++row) {
      int indexA = row;
      int indexB =(row+1)%polygonItem->rowCount();
      ppxl::Point A(polygonItem->child(indexA, 1)->text().toDouble(),
                    polygonItem->child(indexA, 2)->text().toDouble());
      ppxl::Point B(polygonItem->child(indexB, 1)->text().toDouble(),
                    polygonItem->child(indexB, 2)->text().toDouble());
      auto color = polygonItem->data(Qt::DecorationRole).value<QColor>();
      DrawLine(A, B, color);
    }
  }
}

void AbstractScribblingView::ClearImage() {
  m_image.fill(Qt::white);
  update();
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
