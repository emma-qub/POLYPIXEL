#include "Core/Geometry/Polygon.hxx"

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Segment.hxx"

#include <cmath>        // round, abs
#include <cfloat>       // DBL_EPSILON
#include <cstring>      // string
#include <algorithm>    // sort
#include <iomanip>      // boolalpha

namespace ppxl {

Polygon::Polygon(std::vector<Point> const& p_vertices):
  m_vertices(p_vertices) {
}

Polygon::Polygon(int p_xMin, int p_xMax, int p_yMin, int p_yMax, unsigned int p_verticesCount) {
  if (p_verticesCount < 3)
    Polygon();

  for (int k = 0; k < 3; k++) {
    m_vertices.push_back(Point(p_xMin, p_xMax, p_yMin, p_yMax));
  }

  for (unsigned int k = 3; k < p_verticesCount; k++) {
    Point newVertex;
    do {
      newVertex = Point(p_xMin, p_xMax, p_yMin, p_yMax);
    } while (!NewPointIsGood(newVertex));
    m_vertices.push_back(newVertex);
  }
}

Polygon::Polygon(Polygon const& p_polygon):
  m_vertices(p_polygon.m_vertices) {
}

Polygon::~Polygon() = default;

void Polygon::SetVertices(std::vector<Point> const& p_vertices) {
  m_vertices = p_vertices;
}

void Polygon::InsertVertex(Point const& p_vertex, unsigned int p_position) {
  m_vertices.insert(m_vertices.begin()+p_position, p_vertex);
}

void Polygon::SetVertexAt(Point const& p_vertex, unsigned int p_position) {
  m_vertices[p_position] = p_vertex;
}

void Polygon::AppendVertex(Point const& p_vertex) {
  m_vertices.push_back(p_vertex);
}

void Polygon::RemoveVertex(unsigned int p_position) {
  m_vertices.erase(m_vertices.begin()+p_position);
}

void Polygon::ReplaceVertex(unsigned int p_position, Point const& p_newVertex) {
  m_vertices[p_position] = p_newVertex;
}

void Polygon::Translate(Vector const& p_direction) {
  Translate(p_direction.GetX(), p_direction.GetY());
}

void Polygon::Translate(double p_x, double p_y) {
  if (std::abs(p_x) < DBL_EPSILON && std::abs(p_y) < DBL_EPSILON) {
    return;
  }

  for (auto& vertex: m_vertices)
  {
    vertex.Move(p_x, p_y);
  }
}

void Polygon::Homothetie(Point const& p_origin, double p_scale) {
  for (auto& vertex: m_vertices) {
    vertex.Homothetie(p_origin, p_scale);
  }
}

bool Polygon::NewPointIsGood(Point const& p_vertex) const {
  Segment PA(p_vertex, m_vertices.at(0));
  Segment PZ(p_vertex, m_vertices.at(m_vertices.size()-1));

  for (unsigned int k = 0; k < m_vertices.size()-1; k++) {
    Segment edge(m_vertices.at(k), m_vertices.at(k+1));
    if (k != 0 && (edge.ComputeIntersection(PA) != Segment::None)) {
      return false;
    }

    if (k != m_vertices.size()-2 && (edge.ComputeIntersection(PZ) != Segment::None)) {
      return false;
    }
  }

  return true;
}

bool Polygon::IsPointInside(Point const& P) const {
  auto countVertices = m_vertices.size();
  double theta = 0.;

  for (unsigned int k = 0; k < countVertices; k++) {
    Point A(m_vertices.at(k));
    Point B(m_vertices.at((k+1)%countVertices));
    Vector vPA(A - P);
    Vector vPB(B - P);
    theta += Vector::Angle(vPA, vPB);
  }

  // Methematically, theta can only take two values:
  // - 0 if point is outside the polygon
  // - 2Pi if point is inside the polygon
  // In order to avoid any double comparison, truncate it and compare it to 6 (2Pi ~ 6.28)
  return static_cast<int>(std::abs(theta)) == 6;
}

bool Polygon::IsCrossing(Segment const& p_line) const {
  auto verticesCount = m_vertices.size();

  int regularCount = 0;
  int fstVertexCount = 0;
  int sndVertexCount = 0;
  int edgeCount = 0;
  int noneCount = 0;
  int otherCount = 0;

  for (unsigned int k = 0; k < verticesCount; k++) {
    Segment edge(m_vertices.at(k), m_vertices.at((k+1)%verticesCount));
    Segment::Intersection intersection = edge.ComputeIntersection(p_line);
    switch (intersection) {
    case Segment::Regular: {
      regularCount++;
      break;
    } case Segment::FirstVertexRegular: {
      fstVertexCount++;
      break;
    } case Segment::SecondVertexRegular: {
      sndVertexCount++;
      break;
    } case Segment::Edge: {
      edgeCount++;
      break;
    } case Segment::None: {
      noneCount++;
      break;
    } default: {
      break;
    }
    }
  }

  otherCount = fstVertexCount + sndVertexCount + regularCount;

  // Prevent from cutting through a polygon's edge since it's really not a good idea.
  return (edgeCount == 0 && otherCount > 0);
}

bool Polygon::IsGoodSegment(Segment const& p_line) const {
  return (!IsPointInside(p_line.GetA()) && IsCrossing(p_line) && !IsPointInside(p_line.GetB()));
}

bool Polygon::IsGoodPolygon() const {
  auto countVertices = m_vertices.size();
  for (unsigned int k = 0; k < countVertices; k++) {
    Point A(m_vertices.at(k));
    Point B(m_vertices.at((k+1)%countVertices));
    Point C(m_vertices.at((k+2)%countVertices));
    if (A == B) {
      return false;
    }

    Vector vAB(A, B);
    Vector vAC(A, C);
    if (Vector::AreColinear(vAB, vAC)) {
      return false;
    }

    Segment AB(A, B);
    for (unsigned int i = 0; i < countVertices; ++i) {
      if (i == k || (i-1)%countVertices == k || (i+1)%countVertices == k) {
        continue;
      }
      Point C(m_vertices.at(i));
      Point D(m_vertices.at((i+1)%countVertices));
      Segment CD(C, D);
      if (AB.ComputeIntersection(CD) == Segment::Regular) {
        return false;
      }
    }
  }
  return true;
}

double Polygon::OrientedArea() const {
  auto countVertices = m_vertices.size();
  double area = 0.0;

  for (unsigned int k = 0; k < countVertices; k++) {
    Point A(m_vertices.at(k));
    Point B(m_vertices.at((k+1)%countVertices));
    Segment edge(A, B);

    area += edge.OrientedArea();
  }

  return std::abs(area);
}

Point Polygon::Barycenter() const {
  if (m_vertices.size() < 3) {
    std::cerr << "Error within Polygon::barycenter: polygon has less than 3 vertices." << std::endl;
    return Point();
  }

  Point bary;
  double baryX = 0.;
  double baryY = 0.;

  auto vertexCount = m_vertices.size();
  for (unsigned int k = 0; k < vertexCount; ++k) {
    Point const& A = m_vertices.at(k);
    Point const& B = m_vertices.at((k+1)%vertexCount);

    double ax = A.GetX();
    double bx = B.GetX();
    double ay = A.GetY();
    double by = B.GetY();

    baryX += (ax+bx)*(ax*by - bx*ay);
    baryY += (ay+by)*(ax*by - bx*ay);
  }

  // take abs other wise barycenter depends on vertex browse direction
  bary.SetX(std::abs(baryX));
  bary.SetY(std::abs(baryY));

  /// 6?
  bary /= (6*OrientedArea());

  return bary;
}

double Polygon::ComputeAngleFromPoint(double p_x, double p_y) {
  auto countVertices = m_vertices.size();
  double res = 0.;

  for (unsigned int k = 0; k < countVertices; k++) {
    double ax = m_vertices.at(k).GetX() - p_x;
    double ay = m_vertices.at(k).GetY() - p_y;
    double bx = m_vertices.at((k+1)%countVertices).GetX() - p_x;
    double by = m_vertices.at((k+1)%countVertices).GetY() - p_y;

    res += std::atan2(ax*by - bx*ay, ax*bx + ay*by);
  }

  return res;
}

void Polygon::ComputeBoundingRect(double& p_left, double& p_top, double& p_right, double& p_bottom) const {
  p_left = std::numeric_limits<double>::infinity();
  p_top = std::numeric_limits<double>::infinity();
  p_right = -std::numeric_limits<double>::infinity();
  p_bottom = -std::numeric_limits<double>::infinity();

  for (auto const& vertex: m_vertices) {
    p_left = std::min(p_left, vertex.GetX());
    p_top = std::min(p_top, vertex.GetY());
    p_right = std::max(p_right, vertex.GetX());
    p_bottom = std::max(p_bottom, vertex.GetY());
  }
}

bool operator==(Polygon const& p_polygon1, Polygon const& p_polygon2) {
  if (p_polygon1.m_vertices.size() != p_polygon2.m_vertices.size()) {
    return false;
  } else {
    for (unsigned int i = 0; i < p_polygon1.m_vertices.size(); i++) {
      if (p_polygon1.m_vertices.at(i) != p_polygon2.m_vertices.at(i)) {
        return false;
      }
    }
  }
  return true;
}

Polygon& operator<<(Polygon& p_polygon, Point const& p_vertex) {
  bool isNotAlreadyThere = true;
  for (auto const& currVertex: p_polygon.m_vertices) {
    if (currVertex == p_vertex) {
      isNotAlreadyThere = false;
      break;
    }
  }

  if (isNotAlreadyThere) {
    p_polygon.AppendVertex(p_vertex);
  }

  return p_polygon;
}

Polygon& operator<<(Polygon& p_polygon, std::vector<Point> const& p_vertices) {
  for (unsigned int k = 0; k < p_vertices.size(); k++) {
    p_polygon << p_vertices.at(k);
  }
  return p_polygon;
}

std::ostream& operator<<(std::ostream& p_os, Polygon const& p_polygon) {
  auto const& vertices = p_polygon.m_vertices;

  p_os << vertices.size() << ";";
  for (unsigned k = 0; k < vertices.size(); k++) {
    p_os << vertices.at(k) << ";";
  }
  return p_os;
}

QDebug operator<<(QDebug p_debug, Polygon const& p_polygon) {
  auto const& vertices = p_polygon.m_vertices;

  p_debug.nospace() << vertices.size() << ";";
  for (unsigned k = 0; k < vertices.size(); k++) {
    p_debug.nospace() << vertices.at(k) << ";";
  }

  return p_debug.maybeSpace();
}

}
