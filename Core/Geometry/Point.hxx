#ifndef POINT_H
#define POINT_H

#include <iostream>

#include <QDebug>

namespace ppxl {
class Vector;

class Point {
public:
  Point(double p_x = 0.0, double p_y = 0.0);
  Point(double p_xMin, double p_xMax, double p_yMin, double p_yMax);
  Point(Point const& p_point);
  virtual ~Point();

  inline double GetX() const { return m_x; }
  inline double GetY() const { return m_y; }

  inline void SetX(double const& p_x) { m_x = p_x; }
  inline void SetY(double const& p_y) { m_y = p_y; }

  void Move(double const& p_x = 0.0, double const& p_y = 0.0);

  void Symetry();

  void Homothetie(Point const& p_origin, double p_scale);
  Point Translate(Vector const& p_vector) const;
  Point& Translated(Vector const& p_vector);

  Point operator/(double const& p_scalar) const;
  Point& operator/=(double const& p_scalar);

  friend Point operator+(Point const& p_point1, Point const& p_point2);
  friend Point operator+(Point const& p_origin, Vector const& p_direction);
  Point& operator+=(Point const& p_point);

  friend Vector operator-(Point const& p_point1, Point const& p_point2);

  Point& operator=(Point const& p_point);

  friend bool operator==(Point const& p_point1, Point const& p_point2);
  friend bool operator!=(Point const& p_point1, Point const& p_point2);
  friend bool operator<(Point const& p_point1, Point const& p_point2);
  friend bool operator<=(Point const& p_point1, Point const& p_point2);
  friend bool operator>(Point const& p_point1, Point const& p_point2);
  friend bool operator>=(Point const& p_point1, Point const& p_point2);

  static double Distance(Point const& p_point1, Point const& p_point2);
  static Point Middle(Point const& p_point1, Point const& p_point2);

  friend std::ostream& operator<<(std::ostream& p_os, Point const& p_point);
  friend QDebug operator<<(QDebug p_debug, Point const& p_point);

private:
  double m_x;
  double m_y;
};

}

#endif
