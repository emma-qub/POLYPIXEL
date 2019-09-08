#include "GraphicsObjectItem.hxx"

#include "Core/Objects/Object.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Portal.hxx"
#include "GUI/Models/CreateLevelModel.hxx"

#include <QPainter>
#include <QStandardItem>

GraphicsObjectItem::GraphicsObjectItem(QStandardItem* p_item, QGraphicsItem* p_parent):
  QGraphicsItem(p_parent),
  m_item(p_item) {
}

GraphicsObjectItem::~GraphicsObjectItem() = default;

QList<QColor> GraphicsObjectItem::GetColorAccordingToItemState() const {
  switch (m_item->data(CreateLevelModel::eStateRole).value<CreateLevelModel::State>()) {
  case CreateLevelModel::eEnabled: {
    return GetEnabledColors();
  }
  case CreateLevelModel::eDisabled: {
    return GetDisabledColors();
  }
  case CreateLevelModel::eSelected: {
    return GetSelectedColors();
  }
  case CreateLevelModel::eHighlightUp: {
    return GetHighlightedUpColors();
  }
  case CreateLevelModel::eHighlightDown: {
    return GetHighlightedDownColors();
  }
  }
}

void GraphicsObjectItem::DrawControlPoints(QPainter* p_painter)
{
  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(QColor("#000000")), 10));
  m_controlPoints.clear();
  m_controlPoints = ComputeControlPoints();
  for (auto const& controlPoint: m_controlPoints) {
    p_painter->drawPoint(controlPoint);
  }
  p_painter->restore();
}

void GraphicsObjectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem* p_option, QWidget* p_widget)
{
  DrawObject(p_painter);
  if (m_item->data(CreateLevelModel::eStateRole).value<CreateLevelModel::State>() == CreateLevelModel::eSelected) {
    setZValue(1000);
    DrawControlPoints(p_painter);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsTapeItem::GraphicsTapeItem(QStandardItem* p_item, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_item, p_parent),
  m_tape(static_cast<Tape*>(p_item->data(CreateLevelModel::eObjectRole).value<Object*>())) {
}

GraphicsTapeItem::~GraphicsTapeItem() = default;

QRectF GraphicsTapeItem::boundingRect() const {
  return QRectF(m_tape->GetX(), m_tape->GetY(), m_tape->GetW(), m_tape->GetH());
}

void GraphicsTapeItem::DrawObject(QPainter* p_painter) {
  auto color = GetColorAccordingToItemState().first();

  p_painter->save();
  p_painter->setBrush(QBrush(color, Qt::BDiagPattern));
  p_painter->setPen(QPen(QBrush(color), 3));
  p_painter->drawRect(m_tape->GetX(), m_tape->GetY(), m_tape->GetW(), m_tape->GetH());
  p_painter->restore();
}

QList<QPoint> GraphicsTapeItem::ComputeControlPoints() const {
  auto l = m_tape->GetLeft();
  auto t = m_tape->GetTop();
  auto r = m_tape->GetRight();
  auto b = m_tape->GetBottom();

  QPoint lt(l, t);
  QPoint tt((l+r)/2, t);
  QPoint rt(r, t);
  QPoint rr(r, (t+b)/2);
  QPoint rb(r, b);
  QPoint bb((l+r)/2, b);
  QPoint lb(l, b);
  QPoint ll(l, (t+b)/2);
  QPoint c((l+r)/2, (t+b)/2);

  return {lt, tt, rt, rr, rb, bb, lb, ll, c};
}

QList<QColor> GraphicsTapeItem::GetEnabledColors() const {
  return {QColor("#f44336")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsMirrorItem::GraphicsMirrorItem(QStandardItem* p_item, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_item, p_parent),
  m_mirror(static_cast<Mirror*>(p_item->data(CreateLevelModel::eObjectRole).value<Object*>())) {
}

GraphicsMirrorItem::~GraphicsMirrorItem() = default;

QRectF GraphicsMirrorItem::boundingRect() const {
  auto line = m_mirror->GetLine();
  auto A = line.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = line.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();

  return QRectF(qMin(Ax, Bx), qMin(Ay, By), qAbs(Bx-Ax), qAbs(By-Ay));
}

void GraphicsMirrorItem::DrawObject(QPainter* p_painter) {
  auto mirrorLine = m_mirror->GetLine();
  auto mirrorLineA = mirrorLine.GetA();
  auto mirrorLineB = mirrorLine.GetB();

  auto color = GetColorAccordingToItemState().first();

  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(color), 7));
  p_painter->drawLine(mirrorLineA.GetX(), mirrorLineA.GetY(), mirrorLineB.GetX(), mirrorLineB.GetY());
  p_painter->restore();
}

QList<QPoint> GraphicsMirrorItem::ComputeControlPoints() const {
  auto line = m_mirror->GetLine();
  auto A = line.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = line.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  QPoint a(Ax, Ay);
  QPoint b(Bx, By);
  QPoint c((Ax+Bx)/2, (Ay+By)/2);

  return {a, b, c};
}

QList<QColor> GraphicsMirrorItem::GetEnabledColors() const {
  return {QColor("#607d8b")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsOneWayItem::GraphicsOneWayItem(QStandardItem* p_item, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_item, p_parent),
  m_oneWay(static_cast<OneWay*>(p_item->data(CreateLevelModel::eObjectRole).value<Object*>())){
}

GraphicsOneWayItem::~GraphicsOneWayItem() = default;

QRectF GraphicsOneWayItem::boundingRect() const {
  auto line = m_oneWay->GetLine();
  auto A = line.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = line.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  return QRectF(qMin(Ax, Bx), qMin(Ay, By), qAbs(Bx-Ax), qAbs(By-Ay));
}

void GraphicsOneWayItem::DrawObject(QPainter* p_painter) {
  auto oneWayLine = m_oneWay->GetLine();
  auto oneWayLineA = oneWayLine.GetA();
  auto oneWayLineB = oneWayLine.GetB();

  auto color = GetColorAccordingToItemState().first();

  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(color), 7));
  p_painter->drawLine(oneWayLineA.GetX(), oneWayLineA.GetY(), oneWayLineB.GetX(), oneWayLineB.GetY());
  p_painter->restore();
}

QList<QPoint> GraphicsOneWayItem::ComputeControlPoints() const {
  auto line = m_oneWay->GetLine();
  auto A = line.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = line.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  QPoint a(Ax, Ay);
  QPoint b(Bx, By);
  QPoint c((Ax+Bx)/2, (Ay+By)/2);

  return {a, b, c};
}

QList<QColor> GraphicsOneWayItem::GetEnabledColors() const {
  return {QColor("#795548")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsPortalItem::GraphicsPortalItem(QStandardItem* p_item, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_item, p_parent),
  m_portal(static_cast<Portal*>(p_item->data(CreateLevelModel::eObjectRole).value<Object*>())) {
}

GraphicsPortalItem::~GraphicsPortalItem() = default;

QRectF GraphicsPortalItem::boundingRect() const {
  auto lineIn = m_portal->GetIn();
  auto A = lineIn.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = lineIn.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  auto lineOut = m_portal->GetOut();
  auto C = lineOut.GetA();
  auto Cx = C.GetX();
  auto Cy = C.GetY();
  auto D = lineOut.GetB();
  auto Dx = D.GetX();
  auto Dy = D.GetY();
  auto minX = qMin(qMin(Ax, Bx), qMin(Cx, Dx));
  auto maxX = qMax(qMax(Ax, Bx), qMax(Cx, Dx));
  auto minY = qMin(qMin(Ay, By), qMin(Cy, Dy));
  auto maxY = qMax(qMax(Ay, By), qMax(Cy, Dy));
  return QRectF(minX, minY, qAbs(maxX-minX), qAbs(maxY-minY));
}

void GraphicsPortalItem::DrawObject(QPainter* p_painter) {
  auto colors = GetColorAccordingToItemState();

  auto portalIn = m_portal->GetIn();
  auto portalInA = portalIn.GetA();
  auto portalInB = portalIn.GetB();
  auto colorIn = colors.at(0);

  auto portalOut = m_portal->GetOut();
  auto portalOutA = portalOut.GetA();
  auto portalOutB = portalOut.GetB();
  auto colorOut = colors.at(1);

  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(colorIn), 7));
  p_painter->drawLine(portalInA.GetX(), portalInA.GetY(), portalInB.GetX(), portalInB.GetY());
  p_painter->setPen(QPen(QBrush(colorOut), 7));
  p_painter->drawLine(portalOutA.GetX(), portalOutA.GetY(), portalOutB.GetX(), portalOutB.GetY());
  p_painter->restore();
}

QList<QPoint> GraphicsPortalItem::ComputeControlPoints() const {
  auto lineIn = m_portal->GetIn();
  auto A = lineIn.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = lineIn.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  QPoint a(Ax, Ay);
  QPoint b(Bx, By);
  QPoint c((Ax+Bx)/2, (Ay+By)/2);

  auto lineOut = m_portal->GetOut();
  auto D = lineIn.GetA();
  auto Dx = D.GetX();
  auto Dy = D.GetY();
  auto E = lineIn.GetB();
  auto Ex = E.GetX();
  auto Ey = E.GetY();
  QPoint d(Dx, Dy);
  QPoint e(Ex, Ey);
  QPoint f((Dx+Ex)/2, (Dy+Ey)/2);

  return {a, b, c, d, e, f};
}

QList<QColor> GraphicsPortalItem::GetEnabledColors() const {
  return {QColor("#ff9800"), QColor("#00bcd4")};
}

QList<QColor> GraphicsPortalItem::GetDisabledColors() const {
  return {QColor("#bdbdbd"), QColor("#9e9e9e")};
}
