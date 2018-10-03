#include "AbstractScribblingView2.hxx"

#include "GUI/Models/PolygonModel.hxx"

#include <QGraphicsLineItem>

AbstractScribblingView2::AbstractScribblingView2(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_penWidth(5),
  m_model(nullptr),
  m_penColor(),
  m_canScribble(false),
  m_viewInitialized(false) {

  m_pen = (QPen(QBrush(QColor("#000000")), m_penWidth));
}

void AbstractScribblingView2::Init() {
  if (m_viewInitialized) {
    return;
  }

  // Hide scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set scene
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);
  m_scene->setSceneRect(0, 0, width(), height());

  m_viewInitialized = true;
}

void AbstractScribblingView2::SetModel(PolygonModel* p_model) {
  m_model = p_model;
}

void AbstractScribblingView2::SetCanScribble(bool p_value) {
  m_canScribble = p_value;
}

bool AbstractScribblingView2::GetCanScribble() const {
  return m_canScribble;
}

void AbstractScribblingView2::ClearImage() {
  for (auto* item: m_scene->items()) {
    m_scene->removeItem(item);
  }

  update();
}

void AbstractScribblingView2::DrawLine(ppxl::Segment const& p_line, QColor const& p_color) {
  if (m_canScribble) {
    ppxl::Point p_startPoint(p_line.GetA());
    ppxl::Point p_endPoint(p_line.GetB());

    m_pen.setColor(p_color);
    m_scene->addLine(p_startPoint.GetX(), p_startPoint.GetY(), p_endPoint.GetX(), p_endPoint.GetY(), m_pen);
  }
}

void AbstractScribblingView2::DrawText(ppxl::Point p_position, const QString& p_text, int p_weight, ppxl::Vector const& shiftVector) {
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

  m_scene->addText(p_text, font)->setPos(boundingRect.topLeft());
}

void AbstractScribblingView2::DrawFromModel() {
  if (!m_model || !m_canScribble)
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

void AbstractScribblingView2::DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color) {
  m_pen.setColor(p_color);
  m_scene->addLine(p_startPoint.GetX(), p_startPoint.GetY(), p_endPoint.GetX(), p_endPoint.GetY(), m_pen);
}

void AbstractScribblingView2::DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color) {
  m_pen.setColor(p_color);
  m_scene->addLine(QLineF(p_startPoint, p_endPoint), m_pen);
}

AbstractScribblingView2::~AbstractScribblingView2() = default;
