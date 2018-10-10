#include "OneWay.hxx"

OneWay::OneWay(double p_xa, double p_ya, double p_xb, double p_yb, double p_xd, double p_yd):
  Obstacle(),
  m_line(p_xa, p_ya, p_xb, p_yb),
  m_direction(p_xd, p_yd) {
}

OneWay::~OneWay() = default;

std::string OneWay::GetName() const {
  return "OneWay";
}

bool OneWay::Crossing(ppxl::Segment const& p_line) const {
  if (p_line.ComputeIntersection(m_line) == ppxl::Segment::Regular) {
    auto lineVector = ppxl::Vector::FromSegment(p_line);
    return (lineVector * m_direction) >= 0.;
  }

  return true;
}
