#include "Tape.hxx"

Tape::Tape(double p_x, double p_y, double p_w, double p_h):
  Obstacle(),
  m_x(p_x),
  m_y(p_y),
  m_w(p_w),
  m_h(p_h) {
}

Tape::~Tape() = default;

std::string Tape::getName() const {
  return "tape";
}

QList<ppxl::Point> Tape::getTapeVertices() const {
  QList<ppxl::Point> tapeVertices;
  tapeVertices << ppxl::Point(m_x, m_y);
  tapeVertices << ppxl::Point(m_x+m_w, m_y);
  tapeVertices << ppxl::Point(m_x+m_w, m_y+m_h);
  tapeVertices << ppxl::Point(m_x, m_y+m_w);

  return tapeVertices;
}

QList<ppxl::Segment> Tape::getTapeEdges() const  {
  QList<ppxl::Segment> tapeEdges;

  auto vertices = getTapeVertices();
  for (int k = 0; k < vertices.size(); ++k) {
    ppxl::Point fstVertex = vertices.at(k);
    ppxl::Point sndVertex = vertices.at((k+1)%vertices.size());
    tapeEdges << ppxl::Segment(fstVertex, sndVertex);
  }

  return tapeEdges;
}

bool Tape::crossing(ppxl::Segment const& p_line) const {
  auto edges = getTapeEdges();

  for (auto const& edge: edges) {
    if (edge.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
      return true;
    }
  }

  return false;
}
