#include "Core/Objects/Obstacles/OneWay.hxx"

OneWay::OneWay(double p_xa, double p_ya, double p_xb, double p_yb):
  Obstacle(),
  m_line(p_xa, p_ya, p_xb, p_yb) {
}

OneWay::~OneWay() = default;

Object::ObjectType OneWay::GetObjectType() const {
  return eOneWay;
}

Object::CategoryType OneWay::GetCategoryType() const {
  return eObstacle;
}

std::string OneWay::GetName() const {
  return "OneWay";
}

bool OneWay::Intersect(const ppxl::Point& p_point, double p_tolerence) const {
  return m_line.PointIsOnSegment(p_point, p_tolerence);
}

bool OneWay::Crossing(ppxl::Segment const& p_line) const {
  if (p_line.ComputeIntersection(m_line) == ppxl::Segment::Regular) {
    auto lineVector = ppxl::Vector::FromSegment(p_line);
    return (lineVector * GetNormal()) >= 0.;
  }

  return false;
}

void OneWay::MoveControlPoint(const ppxl::Point& p_point, Object::ControlPointType p_controlPointType) {
  auto endPoint = p_point;

  switch (p_controlPointType) {
  case eBottomRight: {
    ppxl::Point::GetDiscreteEndPoint(m_line.GetA(), p_point, endPoint);
    m_line.SetB(endPoint);
    break;
  } case eTopLeft: {
    ppxl::Point::GetDiscreteEndPoint(m_line.GetB(), p_point, endPoint);
    m_line.SetA(endPoint);
    break;
  } case eCenter: {
    m_line.Translate(ppxl::Vector(m_line.GetCenter(), p_point));
    break;
  } default:
    break;
  }
}

ppxl::Point OneWay::GetControlPoint(Object::ControlPointType p_controlPointType) const {
  switch (p_controlPointType) {
  case eTopLeft:{
    return ppxl::Point(m_line.GetA());
    break;
  }case eCenter:{
    return ppxl::Point(m_line.GetCenter());
    break;
  } case eBottomRight:{
    return ppxl::Point(m_line.GetB());
    break;
  }  default:
    break;
  }

  return ppxl::Point(-1, -1);
}
