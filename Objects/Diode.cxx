#include "Diode.hxx"

Diode::Diode(ppxl::Point const& A, ppxl::Point const& B, ppxl::Point const& O, ppxl::Point const& D):
  Obstacle(),
  m_diodeLine(A, B),
  m_diodeDirection(O, D) {
}

Diode::~Diode() = default;

bool Diode::crossing(ppxl::Segment const& line) const {
  if (line.ComputeIntersection(m_diodeLine) == ppxl::Segment::Regular) {
    auto lineVector = ppxl::Vector::FromSegment(line);
    return (lineVector * m_diodeDirection) >= 0.;
  }

  return true;
}
