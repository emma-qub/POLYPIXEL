#include "Tape.hxx"

Tape::Tape(double p_x, double p_y, double p_w, double p_h):
  Obstacle(),
  m_x(p_x),
  m_y(p_y),
  m_w(p_w),
  m_h(p_h) {
}

Object::Type Tape::GetType() const {
  return eObstacle;
}

Tape::~Tape() = default;

std::string Tape::GetName() const {
  return "Tape";
}

bool Tape::Crossing(ppxl::Segment const& p_line) const {
  auto edges = getTapeEdges();

  for (auto const& edge: edges) {
    if (edge.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
      return true;
    }
  }

  return false;
}

std::vector<ppxl::Point> Tape::getTapeVertices() const {
  std::vector<ppxl::Point> tapeVertices({
    ppxl::Point(m_x, m_y),
    ppxl::Point(m_x+m_w, m_y),
    ppxl::Point(m_x+m_w, m_y+m_h),
    ppxl::Point(m_x, m_y+m_w)});

  return tapeVertices;
}

std::vector<ppxl::Segment> Tape::getTapeEdges() const  {
  std::vector<ppxl::Segment> tapeEdges;

  auto vertices = getTapeVertices();
  for (unsigned int k = 0; k < vertices.size(); ++k) {
    ppxl::Point fstVertex = vertices.at(k);
    ppxl::Point sndVertex = vertices.at((k+1)%vertices.size());
    tapeEdges.push_back(ppxl::Segment(fstVertex, sndVertex));
  }

  return tapeEdges;
}
