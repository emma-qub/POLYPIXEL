#include "Core/Objects/Deviations/Portal.hxx"

#include <cmath>

Portal::Portal(double xaIn, double p_yaIn, double p_xbIn, double p_ybIn, double p_xaOut, double p_yaOut, double p_xbOut, double p_ybOut):
  Deviation(),
  m_in(ppxl::Segment(xaIn, p_yaIn, p_xbIn, p_ybIn)),
  m_out(ppxl::Segment(p_xaOut, p_yaOut, p_xbOut, p_ybOut)),
  m_normalIn(m_in.GetNormal()),
  m_normalOut(m_out.GetNormal()) {
}

Portal::Portal(ppxl::Segment const& p_in, ppxl::Segment const& p_out):
  Deviation(),
  m_in(p_in),
  m_out(p_out),
  m_normalIn(m_in.GetNormal()),
  m_normalOut(m_out.GetNormal()) {
}

Portal::~Portal() = default;

void Portal::SetIn(ppxl::Segment const& p_in) {
  m_in = p_in;
  m_normalIn = m_in.GetNormal();
}

void Portal::SetOut(ppxl::Segment const& p_out) {
  m_out = p_out;
  m_normalOut = m_out.GetNormal();
}

Object::ObjectType Portal::GetObjectType() const {
  return ePortal;
}

Object::CategoryType Portal::GetCategoryType() const {
  return eDeviation;
}

std::string Portal::GetName() const {
  return "Portal";
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
