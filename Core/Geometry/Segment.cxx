#include "Core/Geometry/Segment.hxx"

#include "Core/Geometry/Vector.hxx"

#include <QDebug>

#include <cmath>    // abs
#include <cfloat>   // DBL_EPSILON

namespace ppxl {

Segment::Segment(Point const& p_a, Point const& p_b) {
  m_a = p_a;
  m_b = p_b;
}

Segment::Segment(double p_xa, double p_ya, double p_xb, double p_yb) {
  m_a = Point(p_xa, p_ya);
  m_b = Point(p_xb, p_yb);
}

Segment::Segment(Segment const& p_segment) {
  m_a = p_segment.m_a;
  m_b = p_segment.m_b;
}

Segment::~Segment() = default;

Point Segment::GetCenter() const {
  return Point((GetA().GetX() + GetB().GetX())/2., (GetA().GetY() + GetB().GetY())/2.);
}

Vector Segment::GetNormal() const {
  return Vector(-(GetB().GetY() - GetA().GetY()), (GetB().GetX() - GetA().GetX())).Normalize();
}

Vector Segment::GetDirection() const {
  return Vector::FromSegment(*this).Normalize();
}

std::vector<Point> Segment::GetVertices() const {
  std::vector<Point> vertices;
  vertices.push_back(m_a);
  vertices.push_back(m_b);
  return vertices;
}

std::string Segment::GetIntersectionName(Segment::Intersection const& p_intersection) {
  switch (p_intersection) {
  case Edge:
  {
    return "Edge";
  }
  case EdgeUseless:
  {
    return "Wrong edge";
  }
  case FirstVertexRegular:
  {
    return "Intersect on A";
  }
  case None:
  {
    return "None";
  }
  case Regular:
  {
    return "Classic intersection";
  }
  case SecondVertexRegular:
  {
    return "Intersect on B";
  }
  case VertexUseless:
  {
    return "Wrong vertex";
  }
  }

  return "Not even an existing intersection";
}

Point Segment::GetOtherBoundary(Point const& p_boundary) {
  assert(p_boundary == m_a || p_boundary == m_b);

  if (p_boundary == m_a) {
    return m_b;
  } else { //if (p_boundary == m_b)
    return m_a;
  }
}

Segment::Side Segment::Location(Point const& p_point) const {
  Point A(m_a);
  Point B(m_b);

  if (A == p_point) {
    return IsBoundA;
  }
  if (B == p_point) {
    return IsBoundB;
  }

  Vector vAB = B - A;
  Vector vAP = p_point - A;

  double det = Vector::Determinant(vAB, vAP);
  if (Vector::AreColinear(vAB, vAP)) {
    if ((Point::Distance(A, p_point) < Point::Distance(A, B))
     && (Point::Distance(B, p_point) < Point::Distance(A, B))) {
      return OnSegmentInside;
    } else {
      return OnSegmentOutside;
    }
  } else if (det > DBL_EPSILON) {
    return OnLeft;
  }

  return OnRight;
}

bool Segment::SameSide(Point const& P, Point const& Q) const {
  Side locationP = Location(P);
  Side locationQ = Location(Q);
  return (locationP == locationQ
       && locationP != OnSegmentInside
       && locationP != OnSegmentOutside
       && locationP != IsBoundA
       && locationP != IsBoundB);
}

Segment::Intersection Segment::ComputeIntersection(Point const& P, Point const& Q) const {
  Point A = m_a;
  Point B = m_b;

  Vector vAB(B - A);
  Vector vPQ(Q - P);

  // Deal with colinear
  if (Vector::AreColinear(vAB, vPQ) || A == P || A == Q || B == P || B == Q) {
    Vector vAP(P - A);
    Vector vAQ(Q - A);
    if (Vector::AreColinear(vAP, vAQ)
     && Point::Distance(P, Q) > Point::Distance(A, B)) {
      return Edge;
    } else {
      return None;
    }
  }

  Segment PQ(P, Q);
  Side positionP = Location(P);
  Side positionQ = Location(Q);
  Side positionA = PQ.Location(A);
  Side positionB = PQ.Location(B);

  if (!SameSide(P, Q)
   && !PQ.SameSide(A, B)
   && positionP != OnSegmentInside
   && positionQ != OnSegmentInside
   && positionA != OnSegmentInside
   && positionB != OnSegmentInside) {
    return Regular;
  } else if (positionA == OnSegmentInside) {
    return FirstVertexRegular;
  } else if (positionB == OnSegmentInside) {
    return SecondVertexRegular;
  }

  return None;
}

Segment::Intersection Segment::ComputeIntersection(Segment const& p_segment) const {
  return ComputeIntersection(p_segment.GetA(), p_segment.GetB());
}

void Segment::Translate(Vector const& p_direction) {
  Translate(p_direction.GetX(), p_direction.GetY());
}

void Segment::Translate(double x, double y) {
  if (std::abs(x) < DBL_EPSILON && std::abs(y) < DBL_EPSILON)
  {
    return;
  }

  m_a.Move(x, y);
  m_b.Move(x, y);
}

double Segment::OrientedArea() const {
  double ax = m_a.GetX();
  double ay = m_a.GetY();
  double bx = m_b.GetX();
  double by = m_b.GetY();

  double height = bx - ax;
  double width = by + ay;

  return height*width/2.;
}

double Segment::Length() const {
  return std::sqrt((m_b.GetX()-m_a.GetX()) * (m_b.GetX()-m_a.GetX()) + (m_b.GetY()-m_a.GetY()) * (m_b.GetY()-m_a.GetY()));
}

Point Segment::IntersectionPoint(Segment const& AB, Segment const& PQ) {
  auto intersection = AB.ComputeIntersection(PQ);
  if (intersection != Segment::Regular) {
    std::cerr << "Error within Segment::IntersectionPoint: " << GetIntersectionName(intersection) << std::endl;
    return Point(-1, -1);
  }

  Point A(AB.GetA());
  Point B(AB.GetB());
  Vector vAB(B - A);

  Point P(PQ.GetA());
  Point Q(PQ.GetB());
  Vector vPQ(Q - P);

  double Ax = A.GetX();
  double Ay = A.GetY();
  double ABx = vAB.GetX();
  double ABy = vAB.GetY();

  double Px = P.GetX();
  double Py = P.GetY();
  double PQx = vPQ.GetX();
  double PQy = vPQ.GetY();

  double t = -(Ax*PQy - Px*PQy - PQx*Ay + PQx*Py) / (ABx*PQy - ABy*PQx);

  Vector tvAB(t*vAB);
  Point tAB(std::round(tvAB.GetX()), std::round(tvAB.GetY()));
  return Point(A + tAB);
}

bool Segment::PointIsInBoundingBox(Point const& C) const {
  return std::min(m_a.GetX(), m_b.GetX()) <= C.GetX()
    && C.GetX() <= std::max(m_a.GetX(), m_b.GetX())
    && std::min(m_a.GetY(), m_b.GetY()) <= C.GetY()
    && C.GetY() <= std::max(m_a.GetY(), m_b.GetY());
}

bool Segment::PointIsOnSegment(Point const& C, double p_tolerence) const {
  auto A = m_a;
  auto B = m_b;

  auto crossproduct = (C.GetY() - A.GetY()) * (B.GetX() - A.GetX()) - (C.GetX() - A.GetX()) * (B.GetY() - A.GetY());

  // Point C is not too far from segment
  if (std::abs(crossproduct) > p_tolerence*Length())
    return false;

  // Point C is between A and B
  return (A.GetX() - C.GetX()) * (B.GetX() - C.GetX()) + (A.GetY() - C.GetY())*(B.GetY() - C.GetY()) <= DBL_EPSILON;
//  auto dotproduct = (C.GetX() - A.GetX()) * (B.GetX() - A.GetX()) + (C.GetY() - A.GetY())*(B.GetY() - A.GetY());
//  if (dotproduct < 0)
//    return false;

//  auto squaredlengthba = (B.GetX() - A.GetX())*(B.GetX() - A.GetX()) + (B.GetY() - A.GetY())*(B.GetY() - A.GetY());
//  if (dotproduct > squaredlengthba)
//    return false;

  //  return true;
}

bool Segment::PointIsNear(Point const& M, double p_tolerance) const {
  auto t = Vector(m_a, M)*GetDirection();
  if (0 <= t && t <= Length()) {
    auto H = m_a + (GetDirection()*t);
    if (Point::Distance(M, H) < p_tolerance) {
      return true;
    }
  }
  return false;
}

bool operator==(Segment const& p_segment1, Segment const& p_segment2) {
  return (p_segment1.GetA() == p_segment2.GetA() && p_segment1.GetB() == p_segment2.GetB());
}

bool operator!=(Segment const& p_segment1, Segment const& p_segment2) {
  return !(p_segment1 == p_segment2);
}

bool operator<(Segment const& p_segment1, Segment const& p_segment2) {
  Point A = p_segment1.GetA();
  Point B = p_segment1.GetB();
  Point C = p_segment2.GetA();
  Point D = p_segment2.GetB();

  if (A != C) {
    return A < C;
  } else {
    return B < D;
  }
}

std::ostream& operator<<(std::ostream& p_os, Segment const& p_segment) {
  return p_os << "(Segment) [" << p_segment.GetA() << " " << p_segment.GetB() << "]";
}

Segment& Segment::operator=(Segment const& p_segment) {
  m_a = p_segment.m_a;
  m_b = p_segment.m_b;
  return *this;
}

QDebug operator<<(QDebug p_debug, Segment const& p_segment) {
  return p_debug << "[" << p_segment.GetA() << " " << p_segment.GetB() << "]";
}

}
