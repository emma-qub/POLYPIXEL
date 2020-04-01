#include "Core/Objects/Deviations/Portal.hxx"

#include <cmath>

Portal::Portal(double xaIn, double p_yaIn, double p_xbIn, double p_ybIn, double p_xaOut, double p_yaOut, double p_xbOut, double p_ybOut):
  Deviation(),
  m_in(ppxl::Segment(xaIn, p_yaIn, p_xbIn, p_ybIn)),
  m_out(ppxl::Segment(p_xaOut, p_yaOut, p_xbOut, p_ybOut)),
  m_creating(true) {
}

Portal::Portal(ppxl::Segment const& p_in, ppxl::Segment const& p_out):
  Deviation(),
  m_in(p_in),
  m_out(p_out) {
}

Portal::~Portal() = default;

Object::ObjectType Portal::GetObjectType() const {
  return ePortal;
}

Object::CategoryType Portal::GetCategoryType() const {
  return eDeviation;
}

std::string Portal::GetName() const {
  return "Portal";
}

bool Portal::Intersect(const ppxl::Point& p_point, double p_tolerence) const {
  return m_in.PointIsOnSegment(p_point, p_tolerence) || m_out.PointIsOnSegment(p_point, p_tolerence);
}

std::vector<ppxl::Segment> Portal::DeviateLine(ppxl::Segment const& p_line) const {
  std::vector<ppxl::Segment> deviatedLines;

  ppxl::Segment in(m_in);
  ppxl::Segment out(m_out);
  if (m_out.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    out = m_in;
    in = m_out;
  }
  if (in.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    // Compute theta the angle between line and portal
    double theta = ppxl::Vector::Angle(in.GetDirection(), p_line.GetDirection());

    // Compute in point (I) and out point (J)
    ppxl::Point I(ppxl::Segment::IntersectionPoint(in, p_line));
    double ratio = ppxl::Point::Distance(in.GetA(), I)/out.Length();
    ppxl::Point J = out.GetA().Translate(ratio*ppxl::Vector::FromSegment(out));

    // Get second portal vector direction
    ppxl::Vector t(out.GetDirection());

    // Compute exit line vector
    double det = t.SquaredNorm(); // tx²+ty²
    assert(det != 0.);
    double tx = t.GetX();
    double ty = t.GetY();
    ppxl::Vector w((tx*std::cos(theta) - ty*std::sin(theta))/det, (tx*std::sin(theta) + ty*std::cos(theta))/det);

    // Compute other bound of exit line
    ppxl::Point P = J + ppxl::Point::Distance(p_line.GetA(), I)*w;

    // Add in line and out line
    deviatedLines.push_back(ppxl::Segment(p_line.GetA(), I));
    deviatedLines.push_back(ppxl::Segment(J, P));
  } else {
    deviatedLines.push_back(p_line);
  }

  return deviatedLines;
}

std::vector<ppxl::Segment> Portal::DeviateLine2(ppxl::Segment const& p_line) const {
  std::vector<ppxl::Segment> deviatedLines;

  ppxl::Segment in(m_in);
  ppxl::Segment out(m_out);
  if (m_out.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    out = m_in;
    in = m_out;
  }
  if (in.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    // Compute theta the rotation's angle
    double theta = ppxl::Vector::Angle(ppxl::Vector::FromSegment(in), ppxl::Vector::FromSegment(out));

    // Compute in point (I) and out point (J)
    ppxl::Point I(ppxl::Segment::IntersectionPoint(in, p_line));
    double ratio = ppxl::Point::Distance(in.GetA(), I)/out.Length();
    ppxl::Point J = out.GetA().Translate(ratio*ppxl::Vector::FromSegment(out));

    // Compute extended point
    ppxl::Point A1 = I.Translate(ppxl::Vector(p_line.GetA(), I));

    // Compute point after moving rotation's center to origin
    ppxl::Point A2 = A1.Translate(ppxl::Vector(-I.GetX(), -I.GetY()));

    // Apply rotation
    ppxl::Point A3(A2.GetX()*std::cos(theta)-A2.GetY()*std::sin(theta), A2.GetX()*std::sin(theta)+A2.GetY()*std::cos(theta));

    // Translate back
    ppxl::Point A4 = A3.Translate(ppxl::Vector(J.GetX(), J.GetY()));

    // Add in line and out line
    deviatedLines.push_back(ppxl::Segment(p_line.GetA(), I));
    deviatedLines.push_back(ppxl::Segment(J, A4));
  } else {
    deviatedLines.push_back(p_line);
  }

  return deviatedLines;
}

void Portal::MoveControlPoint(const ppxl::Point& p_point, Object::ControlPointType p_controlPointType) {
  auto endPoint = p_point;

  switch (p_controlPointType) {
  case eBottomRight: {
    auto inOut = ppxl::Point::Distance(m_in.GetA(), m_out.GetA());
    ppxl::Point::GetDiscreteEndPoint(m_in.GetA(), p_point, endPoint);

    m_in.SetB(endPoint);

    auto normalOut = GetNormalOut();
    if (!normalOut.IsNull()) {
      auto startPointOut = m_in.GetA().Translated(-inOut*normalOut);
      m_out.SetA(startPointOut);
      endPoint.Translated(-inOut*normalOut);
    }
    m_out.SetB(endPoint);

    break;
  } case eBottomRightYellow: {
    ppxl::Point::GetDiscreteEndPoint(m_in.GetA(), p_point, endPoint);
    m_in.SetB(endPoint);
    break;
  } case eCenterYellow: {
    m_in.Translate(ppxl::Vector(m_in.GetCenter(), p_point));
    break;
  } case eTopLeftYellow: {
    ppxl::Point::GetDiscreteEndPoint(m_in.GetB(), p_point, endPoint);
    m_in.SetA(endPoint);
    break;
  } case eBottomRightBlue: {
    ppxl::Point::GetDiscreteEndPoint(m_out.GetA(), p_point, endPoint);
    m_out.SetB(endPoint);
    break;
  } case eCenterBlue: {
    m_out.Translate(ppxl::Vector(m_out.GetCenter(), p_point));
    break;
  } case eTopLeftBlue: {
    ppxl::Point::GetDiscreteEndPoint(m_out.GetB(), p_point, endPoint);
    m_out.SetA(endPoint);
    break;
  } default:
    break;
  }
}

void Portal::Translate(const ppxl::Vector& p_direction) {
  m_in.Translate(p_direction);
  m_out.Translate(p_direction);
}

ppxl::Point Portal::GetControlPoint(Object::ControlPointType p_controlPointType) const {
  switch (p_controlPointType) {
  case eTopLeftYellow:{
    return ppxl::Point(m_in.GetA());
    break;
  } case eCenterYellow:{
    return ppxl::Point(m_in.GetCenter());
    break;
  } case eBottomRightYellow:{
    return ppxl::Point(m_in.GetB());
    break;
  } case eTopLeftBlue:{
    return ppxl::Point(m_out.GetA());
    break;
  } case eCenterBlue:{
    return ppxl::Point(m_out.GetCenter());
    break;
  } case eBottomRightBlue:{
    return ppxl::Point(m_out.GetB());
    break;
  } default:
    break;
  }

  return ppxl::Point(-1, -1);
}
