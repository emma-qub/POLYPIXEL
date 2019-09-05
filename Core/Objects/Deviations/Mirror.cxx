#include "Core/Objects/Deviations/Mirror.hxx"

#include "Core/Geometry/Vector.hxx"

#include <cmath>

Mirror::Mirror(double p_xa, double p_ya, double p_xb, double p_yb):
  Deviation(),
  m_line(p_xa, p_ya, p_xb, p_yb) {
}

Mirror::~Mirror() = default;

Object::ObjectType Mirror::GetObjectType() const {
  return eMirror;
}

Object::CategoryType Mirror::GetCategoryType() const {
  return eDeviation;
}

std::string Mirror::GetName() const {
  return "Mirror";
}

bool Mirror::Intersect(const ppxl::Point& p_point) const {
  return m_line.PointIsOnSegment(p_point, 0.1);
}

std::vector<ppxl::Segment> Mirror::DeviateLine(ppxl::Segment const& p_line) const {
  std::vector<ppxl::Segment> deviatedLines;

  if (m_line.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    ppxl::Point intersection(ppxl::Segment::IntersectionPoint(m_line, p_line));

    // mirror's bounds coordinates
    double xc = m_line.GetA().GetX();
    double yc = m_line.GetA().GetY();
    double xd = m_line.GetB().GetX();
    double yd = m_line.GetB().GetY();

    // Intersection's coordinates
    double xi = intersection.GetX();
    double yi = intersection.GetY();

    // First line's coordinates
    double xa = p_line.GetA().GetX();
    double ya = p_line.GetA().GetY();

    // System's determinant
    double det = (xd-xc)*(xd-xc) + (yd-yc)*(yd-yc);
    // det == 0 <=> mirror's bound are confounded. It shall not happen.
    assert(det != 0.);

    // Orthogonal projection of line's first bound on the line
    // normal to the mirror's line and going through th intersection
    double detxj = (xd-xc)*((xd-xc)*xi+(yd-yc)*yi) - (yd-yc)*((yc-yd)*xa+(xd-xc)*ya);
    double xj = std::round(detxj / det);
    double detyj = ((yc-yd)*xa+(xd-xc)*ya)*(xd-xc) - ((xd-xc)*xi+(yd-yc)*yi)*(yc-yd);
    double yj = std::round(detyj / det);

    // Symetry of first line intersection
    double xb = 2*xj - xa;
    double yb = 2*yj - ya;

    // Add line and reflected line
    deviatedLines.push_back(ppxl::Segment(p_line.GetA(), intersection));
    deviatedLines.push_back(ppxl::Segment(intersection, ppxl::Point(xb, yb)));
  } else {
    deviatedLines.push_back(p_line);
  }

  return deviatedLines;
}
