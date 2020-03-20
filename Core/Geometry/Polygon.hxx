#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>

#include "Core/Geometry/Segment.hxx"

namespace ppxl {

class Point;
class Vector;

class Polygon {
public:

  Polygon(std::vector<Point> const& p_vertices = std::vector<Point>());
  Polygon(int p_xMin, int p_xMax, int p_yMin, int p_yMax, unsigned int p_verticesCount);
  Polygon(Polygon const& p_polygon);
  virtual ~Polygon();

  inline bool HasEnoughVertices() const { return m_vertices.size() > 2; }
  inline std::vector<Point>& GetVertices() { return m_vertices; }
  inline std::vector<Point> const& GetVertices() const { return m_vertices; }
  void SetVertices(std::vector<Point> const& p_vertices);
  inline unsigned long GetVerticesCount() const { return m_vertices.size(); }

  void InsertVertex(Point const& p_vertex, unsigned int p_position);
  void AppendVertex(Point const& p_vertex);
  void RemoveVertex(unsigned int p_position);
  void ReplaceVertex(unsigned int p_position, Point const& p_newVertex);

  void SetVertexAt(Point const& p_vertex, unsigned int p_position);
  void Translate(Vector const& p_direction);
  void Translate(double p_x, double p_y);
  void Homothetie(Point const& p_origin, double p_scale);

  bool NewPointIsGood(Point const& p_vertex) const;

  bool IsPointInside(Point const& P) const;

  bool IsCrossing(Segment const& p_line) const;
  bool IsGoodSegment(Segment const& p_line) const;
  bool IsGoodPolygon() const;

  inline void Clear() { m_vertices.clear(); }

  double OrientedArea() const;
  Point Barycenter() const;

  double ComputeAngleFromPoint(double p_x, double p_y);

  friend bool operator==(Polygon const& p_polygon1, Polygon const& p_polygon2);

  friend Polygon& operator<<(Polygon& p_polygon, Point const& p_vertex);
  friend Polygon& operator<<(Polygon& p_polygon, std::vector<Point> const& p_vertices);

  friend std::ostream& operator<<(std::ostream& p_os, Polygon const& p_polygon);
  friend QDebug operator<<(QDebug p_debug, Polygon const& p_model);

private:
  std::vector<Point> m_vertices;
};

}

#endif
