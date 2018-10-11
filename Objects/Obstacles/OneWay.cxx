#include "OneWay.hxx"

OneWay::OneWay(double p_xa, double p_ya, double p_xb, double p_yb):
  Obstacle(),
  m_line(p_xa, p_ya, p_xb, p_yb),
  m_direction(m_line.GetNormal()) {
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

bool OneWay::Crossing(ppxl::Segment const& p_line) const {
  if (p_line.ComputeIntersection(m_line) == ppxl::Segment::Regular) {
    auto lineVector = ppxl::Vector::FromSegment(p_line);
    return (lineVector * m_direction) >= 0.;
  }

  return false;
}
