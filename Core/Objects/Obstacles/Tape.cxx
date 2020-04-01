#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Geometry/Vector.hxx"

Tape::Tape(double p_x, double p_y, double p_w, double p_h):
  Obstacle(),
  m_x1(p_x),
  m_y1(p_y),
  m_x2(p_x+p_w),
  m_y2(p_y+p_h) {
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
  return m_x1-p_tolerence <= p_point.GetX() && p_point.GetX() <= m_x2+p_tolerence
      && m_y1-p_tolerence <= p_point.GetY() && p_point.GetY() <= m_y2+p_tolerence;
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

void Tape::MoveControlPoint(const ppxl::Point& p_point, Object::ControlPointType p_controlPointType) {
  auto newX = p_point.GetX();
  auto newY = p_point.GetY();

  switch (p_controlPointType) {
  case eLeft: {
    m_x1 = newX;
    break;
  } case eTopLeft: {
    m_x1 = newX;
    m_y1 = newY;
    break;
  } case eTop: {
    m_y1 = newY;
    break;
  } case eTopRight: {
    m_x2 = newX;
    m_y1 = newY;
    break;
  } case eRight: {
    m_x2 = newX;
    break;
  } case eBottomRight: {
    m_x2 = newX;
    m_y2 = newY;
    break;
  } case eBottom: {
    m_y2 = newY;
    break;
  } case eBottomLeft: {
    m_x1 = newX;
    m_y2 = newY;
    break;
  } case eCenter: {
    auto halfW = GetW()/2;
    auto halfH = GetH()/2;
    m_x1 = newX - halfW;
    m_x2 = newX + halfW;
    m_y1 = newY - halfH;
    m_y2 = newY + halfH;
    break;
  } default:
    break;
  }
}

void Tape::Translate(ppxl::Vector const& p_direction) {
  auto x = p_direction.GetX();
  auto y = p_direction.GetY();
  m_x1 += x;
  m_y1 += y;
  m_x2 += x;
  m_y2 += y;
}

ppxl::Point Tape::GetControlPoint(Object::ControlPointType p_controlPointType) const {
  switch (p_controlPointType) {
  case eTopLeft:{
    return ppxl::Point(m_x1, m_y1);
    break;
  } case eTop:{
    return ppxl::Point(m_x1 + GetW()/2., m_y1);
    break;
  } case eTopRight:{
    return ppxl::Point(m_x2, m_y1);
    break;
  } case eRight:{
    return ppxl::Point(m_x2, m_y1 + GetH()/2.);
    break;
  } case eBottomRight:{
    return ppxl::Point(m_x2, m_y2);
    break;
  } case eBottom:{
    return ppxl::Point(m_x1 + GetW()/2., m_y2);
    break;
  } case eBottomLeft:{
    return ppxl::Point(m_x1, m_y2);
    break;
  } case eLeft:{
    return ppxl::Point(m_x1, m_y1 + GetH()/2.);
    break;
  } case eCenter:{
    return ppxl::Point(m_x1 + GetW()/2., m_y1 + GetH()/2.);
    break;
  } default:
    break;
  }

  return ppxl::Point(-1, -1);
}

std::vector<ppxl::Point> Tape::GetTapeVertices() const {
  std::vector<ppxl::Point> tapeVertices({
    ppxl::Point(m_x1, m_y1),
    ppxl::Point(m_x2, m_y1),
    ppxl::Point(m_x2, m_y2),
    ppxl::Point(m_x1, m_y2)});

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
