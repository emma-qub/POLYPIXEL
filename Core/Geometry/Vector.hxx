#ifndef VECTEUR_H_
#define VECTEUR_H_

#include <iostream>
#include <cassert>

#include <QDebug>

namespace ppxl {

class Point;
class Segment;

class Vector {
public:
  Vector(double const& p_x = 0.0, double const& p_y = 0.0);
  Vector(Point const& p_a, Point const& p_b);
  Vector(Vector const& p_vector);
  virtual ~Vector();

  inline double GetX() const { return m_x; }
  inline double GetY() const { return m_y; }

  inline void SetX(double p_x) { m_x = p_x; }
  inline void SetY(double p_y) { m_y = p_y; }

  static Vector FromSegment(Segment const& p_segment);

  friend bool operator==(Vector const& p_vector1, Vector const& p_vector2);
  friend bool operator!=(Vector const& p_vector1, Vector const& p_vector2);

  Vector const& operator+() const;
  Vector operator-() const;

  Vector& operator=(Vector const& p_vector);

  friend Vector operator+(Vector const& p_vector1, Vector const& p_vector2);
  friend Vector operator-(Vector const& p_vector1, Vector const& p_vector2);
  friend Vector operator*(Vector const& p_vector, double const& p_scalar);
  friend Vector operator*(double const& k, Vector const& p_vector);
  friend double operator*(Vector const& p_vector1, Vector const& p_vector2);
  friend Vector operator/(Vector const& p_vector, double const&p_scalar);

  Vector& operator+=(Vector const& p_vector);
  Vector& operator-=(Vector const& p_vector);
  Vector& operator*=(double const&p_scalar);
  Vector& operator/=(double const&p_scalar);

  double Norm() const;
  double SquaredNorm() const;
  Vector Normalized() const;
  Vector& Normalize();

  static double Determinant(Vector const& p_vector1, Vector const& p_vector2);
  static bool AreColinear(Vector const& p_vector1, Vector const& p_vector2);
  static bool AreOrthogonal(Vector const& p_vector1, Vector const& p_vector2);
  static double Angle(Vector const& p_vector1, Vector const& p_vector2);

  friend std::ostream& operator<<(std::ostream& p_os, Vector const& p_vector);
  friend QDebug operator<<(QDebug p_debug, Vector const& p_vector);

private:
  double m_x;
  double m_y;
};

}

#endif
