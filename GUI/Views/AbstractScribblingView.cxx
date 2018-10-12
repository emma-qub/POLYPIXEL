#include "AbstractScribblingView.hxx"

#include "Core/Point.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/GraphicsItem/GraphicsPixelLine.hxx"
#include "Objects/Object.hxx"
#include "Objects/Obstacles/Tape.hxx"
#include "Objects/Obstacles/OneWay.hxx"
#include "Objects/Deviations/Mirror.hxx"
#include "Objects/Deviations/Portal.hxx"


#include <QGraphicsLineItem>

AbstractScribblingView::AbstractScribblingView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_penWidth(5),
  m_polygonModel(nullptr),
  m_penColor(),
  m_canScribble(false),
  m_viewInitialized(false) {

  m_pen = (QPen(QBrush(QColor("#000000")), m_penWidth));
}

void AbstractScribblingView::InitView() {
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

void AbstractScribblingView::SetModel(PolygonModel* p_model) {
  m_polygonModel = p_model;
}

void AbstractScribblingView::SetObjectsList(const QList<Object*>& p_objectsList) {
  m_objectsList.clear();
  m_objectsList << p_objectsList;
}

void AbstractScribblingView::SetCanScribble(bool p_value) {
  m_canScribble = p_value;
}

bool AbstractScribblingView::GetCanScribble() const {
  return m_canScribble;
}

void AbstractScribblingView::ClearImage() {
  for (auto* item: m_scene->items()) {
    m_scene->removeItem(item);
  }

  update();
}

void AbstractScribblingView::DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  if (m_canScribble) {
    ppxl::Point p_startPoint(p_line.GetA());
    ppxl::Point p_endPoint(p_line.GetB());

    m_pen.setColor(p_color);
    m_pen.setStyle(p_penStyle);

    auto line = new GraphicsPixelLine(p_startPoint.GetX(), p_startPoint.GetY(), p_endPoint.GetX(), p_endPoint.GetY(), 5);
    line->setBrush(p_color);
    m_scene->addItem(line);

    //m_scene->addLine(p_startPoint.GetX(), p_startPoint.GetY(), p_endPoint.GetX(), p_endPoint.GetY(), m_pen);
  }
}

void AbstractScribblingView::DrawText(ppxl::Point p_position, const QString& p_text, int p_weight, ppxl::Vector const& shiftVector) {
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

void AbstractScribblingView::DrawFromModel() {
  if (!m_polygonModel || !m_canScribble)
  {
    return;
  }

  m_graphicsPolygonList.clear();
  m_pen.setStyle(Qt::SolidLine);
  auto polygonItems = m_polygonModel->GetPolygonsItem();
  for (int polygonRow = 0; polygonRow < polygonItems->rowCount(); ++polygonRow) {
    auto polygonItem = polygonItems->child(polygonRow, 0);
    m_penColor =  polygonItem->data(Qt::DecorationRole).value<QColor>();
    m_pen.setColor(m_penColor);
    auto* polygon = polygonItem->data(PolygonModel::ePolygonRole).value<ppxl::Polygon*>();
    QVector<QPointF> verticesList;
    for (auto const& vertex: polygon->GetVertices()) {
      verticesList << QPointF(vertex.GetX(), vertex.GetY());
    }
    auto graphicsPolygonItem = new GraphicsPolygonItem(QPolygonF(verticesList));
    graphicsPolygonItem->setPen(m_pen);
    m_scene->addItem(graphicsPolygonItem);
    m_graphicsPolygonList << graphicsPolygonItem;
  }
}

void AbstractScribblingView::DrawObjects() {
  for (auto* object: m_objectsList) {
    auto objectType = object->GetObjectType();
    switch (objectType) {
    case Object::eTape: {
      auto tape = static_cast<Tape*>(object);
      scene()->addRect(tape->GetX(), tape->GetY(), tape->GetW(), tape->GetH(), QPen(QBrush(QColor("#ff9900")), 3), QBrush(QColor("#ff9900"), Qt::BDiagPattern));
      break;
    } case Object::eOneWay: {
      auto oneWay = static_cast<OneWay*>(object);
      auto oneWayLine = oneWay->GetLine();
      auto oneWayLineA = oneWayLine.GetA();
      auto oneWayLineB = oneWayLine.GetB();
      scene()->addLine(oneWayLineA.GetX(), oneWayLineA.GetY(), oneWayLineB.GetX(), oneWayLineB.GetY(), QPen(QBrush(QColor("#cccccc")), 7));
      break;
    } case Object::eMirror: {
      auto mirror = static_cast<Mirror*>(object);
      auto mirrorLine = mirror->GetMirrorLine();
      auto mirrorLineA = mirrorLine.GetA();
      auto mirrorLineB = mirrorLine.GetB();
      scene()->addLine(mirrorLineA.GetX(), mirrorLineA.GetY(), mirrorLineB.GetX(), mirrorLineB.GetY(), QPen(QBrush(QColor("#0033ff")), 7));
      break;
    } case Object::ePortal: {
      auto portal = static_cast<Portal*>(object);
      auto portalIn = portal->GetIn();
      auto portalInA = portalIn.GetA();
      auto portalInB = portalIn.GetB();
      scene()->addLine(portalInA.GetX(), portalInA.GetY(), portalInB.GetX(), portalInB.GetY(), QPen(QBrush(QColor("#ff9a00")), 7));
      auto portalOut = portal->GetOut();
      auto portalOutA = portalOut.GetA();
      auto portalOutB = portalOut.GetB();
      scene()->addLine(portalOutA.GetX(), portalOutA.GetY(), portalOutB.GetX(), portalOutB.GetY(), QPen(QBrush(QColor("#27a7d8")), 7));
      break;
    } default: {
      break;
    }
    }
  }
}

void AbstractScribblingView::DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color) {
  m_pen.setColor(p_color);
  m_scene->addLine(p_startPoint.GetX(), p_startPoint.GetY(), p_endPoint.GetX(), p_endPoint.GetY(), m_pen);
}

void AbstractScribblingView::DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color) {
  m_pen.setColor(p_color);
  m_scene->addLine(QLineF(p_startPoint, p_endPoint), m_pen);
}

AbstractScribblingView::~AbstractScribblingView() = default;
