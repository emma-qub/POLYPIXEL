#ifndef SEGMENT_H
#define SEGMENT_H

#include "Core/Geometry/Point.hxx"

#include <string>
#include <cfloat>

namespace ppxl {

class Segment {
public:
  enum Side {
    OnLeft,
    OnSegmentInside,
    OnSegmentOutside,
    OnRight,
    IsBoundA,
    IsBoundB
  };
  enum Intersection {
    Regular,
    FirstVertexRegular,
    SecondVertexRegular,
    Edge,
    VertexUseless,
    EdgeUseless,
    None
  };

  Segment(Point const& a = Point(), Point const& b = Point());
  Segment(double p_xa, double p_ya, double p_xb, double p_yb);
  Segment(Segment const& p_segment);
  virtual ~Segment();

  inline Point GetA() const { return m_a; }
  inline Point GetB() const { return m_b; }
  inline void setA(Point const& p_a) { m_a = p_a; }
  inline void setB(Point const& p_b) { m_b = p_b; }

  Point GetCenter() const;
  Vector GetNormal() const;
  Vector GetDirection() const;
  std::vector<Point> GetVertices() const;
  static std::string GetIntersectionName(Intersection const& p_intersection);

  Point GetOtherBoundary(Point const& p_boundary);

  Side Location(Point const& p_point) const;
  bool SameSide(Point const& P, Point const& Q) const;
  Intersection ComputeIntersection(Point const& P, Point const& Q) const;
  Intersection ComputeIntersection(Segment const& p_segment) const;
  static Point IntersectionPoint(Segment const& AB, Segment const& PQ);

  bool PointIsOnSegment(Point const& C, double p_tolerence = DBL_EPSILON) const;

  void Translate(Vector const& p_direction);
  void Translate(double x, double y);

  double OrientedArea() const;

  double Length() const;

  friend bool operator==(Segment const& p_segment1, Segment const& p_segment2);
  friend bool operator!=(Segment const& p_segment1, Segment const& p_segment2);
  friend bool operator<(Segment const& p_segment1, Segment const& p_segment2);

  Segment& operator=(Segment const& p_segment);

  friend std::ostream& operator<<(std::ostream& os, Segment const& p_segment);
  friend QDebug operator<<(QDebug d, Segment const& p_segment);

private:
  Point m_a;
  Point m_b;
};

}

#endif
