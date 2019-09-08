#include "Core/Objects/Obstacles/Tape.hxx"

Tape::Tape(double p_x, double p_y, double p_w, double p_h):
  Obstacle(),
  m_x(p_x),
  m_y(p_y),
  m_w(p_w),
  m_h(p_h) {
}

Tape::~Tape() = default;

Object::ObjectType Tape::GetObjectType() const {
  return eTape;
}

Object::CategoryType Tape::GetCategoryType() const {
  return eObstacle;
}

std::string Tape::GetName() const {
  return "Tape";
}

bool Tape::Intersect(const ppxl::Point& p_point, double p_tolerence) const
{
  return m_x-p_tolerence <= p_point.GetX() && p_point.GetX() <= m_x+m_w+p_tolerence
      && m_y-p_tolerence <= p_point.GetY() && p_point.GetY() <= m_y+m_h+p_tolerence;
}

bool Tape::Crossing(ppxl::Segment const& p_line) const {
  auto edges = GetTapeEdges();

  for (auto const& edge: edges) {
    if (edge.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
      return true;
    }
  }

  return false;
}

std::vector<ppxl::Point> Tape::GetcontrolPointsList() const {
  return {
    ppxl::Point(m_x, m_y),
    ppxl::Point(m_x + m_w/2.0, m_y),
    ppxl::Point(m_x + m_w, m_y),
    ppxl::Point(m_x + m_w, m_y + m_h/2.0),
    ppxl::Point(m_x + m_w, m_y + m_h),
    ppxl::Point(m_x + m_w/2.0, m_y + m_h),
    ppxl::Point(m_x, m_y + m_h),
    ppxl::Point(m_x, m_y + m_h/2.0),
    ppxl::Point(m_x + m_w/2, m_y + m_h/2)
  };
}

ppxl::Point Tape::GetControlPoint(Tape::ControlPointType p_type) const {
  return GetcontrolPointsList().at(p_type);
}

Tape::ControlPointType Tape::GetNearestControlPointType(const ppxl::Point& p_point) const {
  auto controlPointsList = GetcontrolPointsList();
  for (unsigned int type = 0; type < eNone; ++type) {
    auto const& controlPoint = controlPointsList.at(type);
    if (ppxl::Point::Distance(p_point, controlPoint) < 5) {
      return static_cast<ControlPointType>(type);
    }
  }
  return eNone;
}

std::vector<ppxl::Point> Tape::GetTapeVertices() const {
  std::vector<ppxl::Point> tapeVertices({
    ppxl::Point(m_x, m_y),
    ppxl::Point(m_x+m_w, m_y),
    ppxl::Point(m_x+m_w, m_y+m_h),
    ppxl::Point(m_x, m_y+m_w)});

  return tapeVertices;
}

std::vector<ppxl::Segment> Tape::GetTapeEdges() const  {
  std::vector<ppxl::Segment> tapeEdges;

  auto vertices = GetTapeVertices();
  for (unsigned int k = 0; k < vertices.size(); ++k) {
    ppxl::Point fstVertex = vertices.at(k);
    ppxl::Point sndVertex = vertices.at((k+1)%vertices.size());
    tapeEdges.push_back(ppxl::Segment(fstVertex, sndVertex));
  }

  return tapeEdges;
}
