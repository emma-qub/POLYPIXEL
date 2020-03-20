#include "GraphicsObjectItem.hxx"

#include "Core/Objects/Object.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Portal.hxx"
#include "GUI/Models/CreateLevelObjectsListModel.hxx"

#include <QPainter>
#include <QStandardItem>

GraphicsObjectItem::GraphicsObjectItem(QGraphicsItem* p_parent):
  QGraphicsItem(p_parent){

  SetState(eDisabledState);
}

GraphicsObjectItem::~GraphicsObjectItem() = default;

QList<QColor> GraphicsObjectItem::GetColorAccordingToItemState() const {
  switch (data(eStateRole).value<State>()) {
  case eEnabledState: {
    return GetEnabledColors();
  }
  case eDisabledState: {
    return GetDisabledColors();
  }
  case eSelectedState: {
    return GetSelectedColors();
  }
  case eHighlightUpState: {
    return GetHighlightedUpColors();
  }
  case eHighlightDownState: {
    return GetHighlightedDownColors();
  }
  default:
    return {};
  }
}

void GraphicsObjectItem::DrawControlPoints(QPainter* p_painter)
{
  p_painter->save();
  p_painter->setPen(Qt::NoPen);
  p_painter->setBrush(QColor("#000000"));
  m_controlPoints.clear();
  m_controlPoints = ComputeControlPoints();
  int k = 0;
  for (auto const& controlPointAndType: m_controlPoints) {
    auto controlPoint = controlPointAndType.first;
    QRectF pointBoundingRect(controlPoint.x()-6, controlPoint.y()-6, 12, 12);
    if (controlPointAndType.second == Object::eCenter) {
      p_painter->save();
      p_painter->setBrush(Qt::white);
      p_painter->setPen(QPen(QBrush(QColor("#000000")), 2));
      QRectF pointBoundingRect(controlPoint.x()-5, controlPoint.y()-5, 10, 10);
      p_painter->drawEllipse(pointBoundingRect);
      p_painter->restore();
    } else {
      p_painter->drawEllipse(pointBoundingRect);
    }
    ++k;
  }
  p_painter->restore();
}

void GraphicsObjectItem::UpdateControlPoints() {
  m_controlPoints = ComputeControlPoints();
}

void GraphicsObjectItem::SetState(GraphicsObjectItem::State p_state) {
  setData(eStateRole, p_state);

  Q_EMIT StateChanged();
}

GraphicsObjectItem::State GraphicsObjectItem::GetState() const {
  return data(eStateRole).value<State>();
}

bool GraphicsObjectItem::Intersect(const ppxl::Point& p_point) const {
  return m_boundingPolygon.IsPointInside(p_point);
}

QPolygonF GraphicsObjectItem::ComputeQuad(ppxl::Segment const& p_line, int p_factor) const {
  auto normal = p_line.GetNormal();
  auto lineCopy = p_line;
  lineCopy.Translate(-p_factor*normal.GetX(), -p_factor*normal.GetY());
  auto A = p_line.GetA();
  auto Ax = A.GetX();
  auto Ay = A.GetY();
  auto B = p_line.GetB();
  auto Bx = B.GetX();
  auto By = B.GetY();
  auto C = lineCopy.GetB();
  auto Cx = C.GetX();
  auto Cy = C.GetY();
  auto D = lineCopy.GetA();
  auto Dx = D.GetX();
  auto Dy = D.GetY();

  return QPolygonF({QPoint(Ax, Ay), QPoint(Bx, By), QPoint(Cx, Cy), QPoint(Dx, Dy)});
}

void GraphicsObjectItem::paint(QPainter* p_painter, const QStyleOptionGraphicsItem*, QWidget*)
{
  DrawObject(p_painter);
  if (data(eStateRole).value<State>() == eSelectedState) {
    setZValue(1000);
    DrawControlPoints(p_painter);
  } else {
    setZValue(100);
  }
}

QList<QColor> GraphicsObjectItem::GetHighlightedDownColors() const {
  QList<QColor> highlightedColors;
  for (auto color: GetEnabledColors()) {
    color.setAlpha(63);
    highlightedColors << color;
  }
  return highlightedColors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// POLYGON
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsPolygonItem::GraphicsPolygonItem(ppxl::Polygon* p_polygon, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent),
  m_polygon(p_polygon) {

}

void GraphicsPolygonItem::SetColor(const QColor& p_color) {
  m_enabledColor = p_color;
}

QRectF GraphicsPolygonItem::boundingRect() const {
  auto xmin = std::numeric_limits<double>::infinity();
  auto xmax = -std::numeric_limits<double>::infinity();
  auto ymin = std::numeric_limits<double>::infinity();
  auto ymax = -std::numeric_limits<double>::infinity();

  for (auto const& vertex: m_polygon->GetVertices()) {
    xmin = std::min(vertex.GetX(), xmin);
    xmax = std::max(vertex.GetX(), xmax);
    ymin = std::min(vertex.GetY(), ymin);
    ymax = std::max(vertex.GetY(), ymax);
  }

  return QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

void GraphicsPolygonItem::DrawObject(QPainter* p_painter) {
  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(GetColorAccordingToItemState().first()), 7));
  auto vertices = m_polygon->GetVertices();
  if (vertices.size() > 1) {
    for (unsigned int vertexRow = 0; vertexRow < vertices.size(); ++vertexRow) {
      ppxl::Point A = vertices.at(vertexRow);
      unsigned int indexB = (vertexRow+1)%vertices.size();
      ppxl::Point B = vertices.at(indexB);
      p_painter->drawLine(A.GetX(), A.GetY(), B.GetX(), B.GetY());

      if (data(eStateRole).value<State>() == eSelectedState) {
        auto vertexPos = A;
        ppxl::Vector fontShift(-5, 5);
        vertexPos.Translated(fontShift);
        if (m_polygon->HasEnoughVertices()) {
          unsigned int indexZ = (vertexRow+vertices.size()-1)%vertices.size();
          ppxl::Point Z = vertices.at(indexZ);
          ppxl::Polygon tempPolygon({Z, A, B});
          auto tempBarycenter = tempPolygon.Barycenter();
          ppxl::Vector shiftVector(A, tempBarycenter);
          shiftVector.Normalize();
          shiftVector *= 20;
          m_polygon->IsPointInside(vertexPos.Translate(shiftVector-fontShift)) ?
            vertexPos.Translated(-shiftVector):
            vertexPos.Translated(shiftVector);
          p_painter->save();
          p_painter->setPen(QPen(QBrush(Qt::black), 7));
          p_painter->drawText(QPointF(vertexPos.GetX(), vertexPos.GetY()), QString(QChar(static_cast<char>('A'+vertexRow))));
          p_painter->restore();
        }
      }
    }
  }
  p_painter->restore();
}

QList<QPair<QPoint, Object::ControlPointType>> GraphicsPolygonItem::ComputeControlPoints() const {
  QList<QPair<QPoint, Object::ControlPointType>> controlPoints;
  for (auto const& vertex: m_polygon->GetVertices()) {
    controlPoints << QPair(QPoint(vertex.GetX(), vertex.GetY()), Object::eVertex);
  }
  if (m_polygon->HasEnoughVertices()) {
    auto barycenter = m_polygon->Barycenter();
    controlPoints << QPair(QPoint(barycenter.GetX(), barycenter.GetY()), Object::eCenter);
  }
  return controlPoints;
}

void GraphicsPolygonItem::ComputeBoundingPolygon() {
  m_boundingPolygon = *m_polygon;
}

GraphicsPolygonItem::~GraphicsPolygonItem() = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsTapeItem::GraphicsTapeItem(Tape* p_tape, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent),
  m_tape(p_tape) {
}

GraphicsTapeItem::~GraphicsTapeItem() = default;

QRectF GraphicsTapeItem::boundingRect() const {
  return QRectF(m_tape->GetXmin(), m_tape->GetYmin(), m_tape->GetW(), m_tape->GetH());
}

void GraphicsTapeItem::DrawObject(QPainter* p_painter) {
  auto color = GetColorAccordingToItemState().first();
  auto fadeColor = color;
  fadeColor.setAlpha(63);

  p_painter->save();
  p_painter->setBrush(QBrush(fadeColor));
  p_painter->setPen(QPen(QBrush(color), 7));
  p_painter->drawRect(m_tape->GetXmin(), m_tape->GetYmin(), m_tape->GetW(), m_tape->GetH());
  p_painter->restore();
}

void GraphicsTapeItem::ComputeBoundingPolygon() {
  auto l = m_tape->GetX1();
  auto t = m_tape->GetY1();
  auto r = m_tape->GetX2();
  auto b = m_tape->GetY2();

  ppxl::Point lt(l, t);
  ppxl::Point rt(r, t);
  ppxl::Point rb(r, b);
  ppxl::Point lb(l, b);

  m_boundingPolygon = ppxl::Polygon({lt, rt, rb, lb});
}

QList<QPair<QPoint, Object::ControlPointType>> GraphicsTapeItem::ComputeControlPoints() const {
  auto l = m_tape->GetX1();
  auto t = m_tape->GetY1();
  auto r = m_tape->GetX2();
  auto b = m_tape->GetY2();

  QPoint lt(l, t);
  QPoint tt((l+r)/2, t);
  QPoint rt(r, t);
  QPoint rr(r, (t+b)/2);
  QPoint rb(r, b);
  QPoint bb((l+r)/2, b);
  QPoint lb(l, b);
  QPoint ll(l, (t+b)/2);
  QPoint c((l+r)/2, (t+b)/2);

  return {
    {lt, Object::eTopLeft},
    {tt, Object::eTop},
    {rt, Object::eTopRight},
    {rr, Object::eRight},
    {rb, Object::eBottomRight},
    {bb, Object::eBottom},
    {lb, Object::eBottomLeft},
    {ll, Object::eLeft},
    {c, Object::eCenter}
  };
}

QList<QColor> GraphicsTapeItem::GetEnabledColors() const {
  return {QColor("#f44336")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsMirrorItem::GraphicsMirrorItem(Mirror* p_mirror, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent),
  m_mirror(p_mirror) {
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
  auto color = GetColorAccordingToItemState().first();
  auto factor = 50;
  auto mirrorLine = m_mirror->GetLine();
  auto mirrorNormal = mirrorLine.GetNormal();
  mirrorLine.Translate(factor/2 * mirrorNormal);

  auto ComputeCrossHatching = [](ppxl::Segment const& p_line, int p_factor, int p_spacing) {
    auto ComputeCrossHatchingBounds = [](ppxl::Segment const& p_line, int p_spacing, int p_shift) {
      auto lineDirection = p_line.GetDirection();
      std::vector<ppxl::Point> crossHatchingBounds;

      if (lineDirection.IsNull()) {
        return crossHatchingBounds;
      }

      auto firstPoint = p_line.GetA();
      ppxl::Point nextPoint = firstPoint.Translated(p_shift*lineDirection).Translated(p_spacing*lineDirection);

      while (p_line.PointIsInBoundingBox(nextPoint)) {
        crossHatchingBounds.push_back(nextPoint);
        nextPoint.Translated(p_spacing*lineDirection);
      }
      return crossHatchingBounds;
    };

    std::vector<QLineF> crossHatchingList;
    auto copyLine = p_line;
    auto normal = p_line.GetNormal();
    copyLine.Translate(-p_factor*normal);

    auto firstCrossHatchingBounds = ComputeCrossHatchingBounds(p_line, p_spacing, -p_spacing);
    auto secondCrossHatchingBounds = ComputeCrossHatchingBounds(copyLine, p_spacing, 50-p_spacing);

    for (auto k = 0ul; k < std::min(firstCrossHatchingBounds.size(), secondCrossHatchingBounds.size()); ++k) {
      auto A = firstCrossHatchingBounds.at(k);
      auto B = secondCrossHatchingBounds.at(k);
      crossHatchingList.push_back(QLineF(QPointF(A.GetX(), A.GetY()), QPointF(B.GetX(), B.GetY())));
    }

    return crossHatchingList;
  };

  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(color), 7));
  p_painter->drawPolygon(ComputeQuad(mirrorLine, factor));
  p_painter->setPen(QPen(QBrush(color), 4));

  for (auto line: ComputeCrossHatching(mirrorLine, factor, 30)) {
    p_painter->drawLine(line);
  }

  p_painter->restore();
}

void GraphicsMirrorItem::ComputeBoundingPolygon() {
  m_boundingPolygon.Clear();
  auto graphicsPolygon = ComputeQuad(m_mirror->GetLine(), 50);
  for (auto const& vertex: graphicsPolygon) {
    m_boundingPolygon.AppendVertex(ppxl::Point(vertex.x(), vertex.y()));
  }
}

QList<QPair<QPoint, Object::ControlPointType>> GraphicsMirrorItem::ComputeControlPoints() const {
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

  return {{a, Object::eTopLeft}, {b, Object::eBottomRight}, {c, Object::eCenter}};
}

QList<QColor> GraphicsMirrorItem::GetEnabledColors() const {
  return {QColor("#aba5d9")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsOneWayItem::GraphicsOneWayItem(OneWay* p_oneWay, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent),
  m_oneWay(p_oneWay) {
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
  auto ComputeArrow = [](ppxl::Segment const& p_line, ppxl::Vector const& p_normal) {
    auto shift = 80;
    auto lineCopy = p_line;
    lineCopy.Translate(-shift*p_normal.GetX(), -shift*p_normal.GetY());
    auto direction = lineCopy.GetDirection();
    auto lineCopyCenter = lineCopy.GetCenter();
    auto minDemiLength = std::min(30., p_line.Length()/2.);
    auto A = lineCopyCenter.Translate(minDemiLength*direction);
    auto Ax = A.GetX();
    auto Ay = A.GetY();
    auto B = lineCopyCenter.Translate(-minDemiLength*direction);
    auto Bx = B.GetX();
    auto By = B.GetY();
    auto C = lineCopyCenter;
    C.Translated(50*p_normal);
    auto Cx = C.GetX();
    auto Cy = C.GetY();

    return QPolygonF({QPoint(Ax, Ay), QPoint(Bx, By), QPoint(Cx, Cy)});
  };

  auto color = GetColorAccordingToItemState().first();

  p_painter->save();
  p_painter->setBrush(Qt::NoBrush);
  p_painter->setPen(QPen(QBrush(color), 7));
  p_painter->drawPolygon(ComputeQuad(m_oneWay->GetLine(), 100));
  p_painter->setBrush(QBrush(color));
  p_painter->drawPolygon(ComputeQuad(m_oneWay->GetLine(), 20));
  p_painter->drawPolygon(ComputeArrow(m_oneWay->GetLine(), m_oneWay->GetNormal()));
  p_painter->restore();
}

void GraphicsOneWayItem::ComputeBoundingPolygon() {
  m_boundingPolygon.Clear();
  auto graphicsPolygon = ComputeQuad(m_oneWay->GetLine(), 100);
  for (auto const& vertex: graphicsPolygon) {
    m_boundingPolygon.AppendVertex(ppxl::Point(vertex.x(), vertex.y()));
  }
}

QList<QPair<QPoint, Object::ControlPointType>> GraphicsOneWayItem::ComputeControlPoints() const {
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

  return {{a, Object::eTopLeft}, {b, Object::eBottomRight}, {c, Object::eCenter}};
}

QList<QColor> GraphicsOneWayItem::GetEnabledColors() const {
  return {QColor("#9b0080")};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GraphicsPortalItem::GraphicsPortalItem(Portal* p_portal, QGraphicsItem* p_parent):
  GraphicsObjectItem(p_parent),
  m_portal(p_portal) {
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

void GraphicsPortalItem::ComputeBoundingPolygon() {
  auto portalIn = m_portal->GetIn();
  m_boundingPolygon.Clear();
  auto graphicsPolygonIn = ComputeQuad(portalIn, 7);
  for (auto const& vertex: graphicsPolygonIn) {
    m_boundingPolygon.AppendVertex(ppxl::Point(vertex.x(), vertex.y()));
  }

  auto portalOut = m_portal->GetOut();
  m_boundingPolygonOut.Clear();
  auto graphicsPolygonOut = ComputeQuad(portalIn, 7);
  for (auto const& vertex: graphicsPolygonOut) {
    m_boundingPolygonOut.AppendVertex(ppxl::Point(vertex.x(), vertex.y()));
  }
}

bool GraphicsPortalItem::Intersect(const ppxl::Point& p_point) const {
  return GraphicsObjectItem::Intersect(p_point) || m_boundingPolygonOut.IsPointInside(p_point);
}

QList<QPair<QPoint, Object::ControlPointType>> GraphicsPortalItem::ComputeControlPoints() const {

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
  auto D = lineOut.GetA();
  auto Dx = D.GetX();
  auto Dy = D.GetY();
  auto E = lineOut.GetB();
  auto Ex = E.GetX();
  auto Ey = E.GetY();
  QPoint d(Dx, Dy);
  QPoint e(Ex, Ey);
  QPoint f((Dx+Ex)/2, (Dy+Ey)/2);

  if (m_portal->IsCreating()) {
    return {{a, Object::eTopLeft}, {b, Object::eBottomRight}, {c, Object::eCenter},
      {d, Object::eTopLeft}, {e, Object::eBottomRight}, {f, Object::eCenter}};
  } else {
    return {{a, Object::eTopLeftYellow}, {b, Object::eBottomRightYellow}, {c, Object::eCenterYellow},
      {d, Object::eTopLeftBlue}, {e, Object::eBottomRightBlue}, {f, Object::eCenterBlue}};
  }
}

QList<QColor> GraphicsPortalItem::GetEnabledColors() const {
  return {QColor("#dfb069"), QColor("#8087bc")};
}

QList<QColor> GraphicsPortalItem::GetDisabledColors() const {
  return {QColor("#bdbdbd"), QColor("#9e9e9e")};
}
