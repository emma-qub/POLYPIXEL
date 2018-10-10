#include "OneWay.hxx"

OneWay::OneWay(ppxl::Point const& A, ppxl::Point const& B, ppxl::Point const& O, ppxl::Point const& D):
  Obstacle(),
  m_diodeLine(A, B),
  m_diodeDirection(O, D) {
}

OneWay::~OneWay() = default;

bool OneWay::Crossing(ppxl::Segment const& line) const {
  if (line.ComputeIntersection(m_diodeLine) == ppxl::Segment::Regular) {
    auto lineVector = ppxl::Vector::FromSegment(line);
    return (lineVector * m_diodeDirection) >= 0.;
  }

  return true;
}
