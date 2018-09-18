#include "Mirror.hxx"

#include "Core/Vector.hxx"

#include <cmath>

Mirror::Mirror(int p_xa, int p_ya, int p_xb, int p_yb):
  Deviation(),
  m_mirrorLine(p_xa, p_ya, p_xb, p_yb) {
}

Mirror::~Mirror() = default;

std::string Mirror::GetName() const {
  return "mirror";
}

QList<ppxl::Segment> Mirror::DeviateLine(ppxl::Segment const& p_line) const {
  QList<ppxl::Segment> deviatedLines;

  if (m_mirrorLine.ComputeIntersection(p_line) == ppxl::Segment::Regular) {
    ppxl::Point intersection(ppxl::Segment::IntersectionPoint(m_mirrorLine, p_line));

    // mirror's bounds coordinates
    double xc = m_mirrorLine.GetA().GetX();
    double yc = m_mirrorLine.GetA().GetY();
    double xd = m_mirrorLine.GetB().GetX();
    double yd = m_mirrorLine.GetB().GetY();

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
    deviatedLines << ppxl::Segment(p_line.GetA(), intersection);
    deviatedLines << ppxl::Segment(intersection, ppxl::Point(xb, yb));
  } else {
    deviatedLines << p_line;
  }

  return deviatedLines;
}
