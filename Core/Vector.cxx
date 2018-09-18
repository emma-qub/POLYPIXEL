#include "Vector.hxx"

#include "Point.hxx"
#include "Segment.hxx"

#include <cmath>    // sqrt, abs, atan2
#include <cfloat>   // DBL_EPSILON

namespace ppxl {

Vector::Vector(double const& p_x, double const& p_y) {
  m_x = p_x;
  m_y = p_y;
}

Vector::Vector(Point const& p_a, Point const& p_b) {
  m_x = p_b.GetX()-p_a.GetX();
  m_y = p_b.GetY()-p_a.GetY();
}

Vector::Vector(Vector const& p_vector) {
  m_x = p_vector.m_x;
  m_y = p_vector.m_y;
}

Vector::~Vector() = default;

Vector Vector::FromSegment(Segment const& p_segment) {
  return Vector(p_segment.GetA(), p_segment.GetB());
}

bool operator==(Vector const& p_vector1, Vector const& p_vector2) {
  return (std::abs(p_vector1.m_x - p_vector2.m_x) < DBL_EPSILON
       && std::abs(p_vector1.m_y - p_vector2.m_y) < DBL_EPSILON);
}

bool operator!=(Vector const& p_vector1, Vector const& p_vector2) {
  return !(p_vector1 == p_vector2);
}

Vector const& Vector::operator+() const {
  return *this;
}

Vector Vector::operator-() const {
  return Vector(-m_x, -m_y);
}

Vector& Vector::operator=(Vector const& p_vector) {
  m_x = p_vector.m_x;
  m_y = p_vector.m_y;
  return *this;
}

Vector operator+(Vector const& p_vector1, Vector const& p_vector2) {
  return Vector(p_vector1.m_x + p_vector2.m_x, p_vector1.m_y + p_vector2.m_y);
}

Vector operator-(Vector const& p_vector1, Vector const& p_vector2) {
  return Vector(p_vector1.m_x - p_vector2.m_x, p_vector1.m_y - p_vector2.m_y);
}

Vector operator*(Vector const& p_vector, double const& p_scalar) {
  return Vector(p_vector.m_x * p_scalar, p_vector.m_y * p_scalar);
}

Vector operator*(double const& p_scalar, Vector const& p_vector) {
  return Vector(p_vector.m_x * p_scalar, p_vector.m_y * p_scalar);
}

double operator*(Vector const& p_vector1, Vector const& p_vector2) {
  return p_vector1.m_x * p_vector2.m_x + p_vector1.m_y * p_vector2.m_y;
}

Vector operator/(Vector const& p_vector, double const& p_scalar) {
  assert(std::abs(p_scalar) > DBL_EPSILON);
  return Vector(p_vector.m_x / p_scalar, p_vector.m_y / p_scalar);
}

Vector& Vector::operator+=(Vector const& p_vector) {
  return *this = (*this + p_vector);
}

Vector& Vector::operator-=(Vector const& p_vector) {
  return *this = (*this - p_vector);
}

Vector& Vector::operator*=(double const& p_scalar) {
  return *this = (*this * p_scalar);
}

Vector& Vector::operator/=(double const& p_scalar) {
  return *this = (*this / p_scalar);
}

double Vector::Norm() const {
  return std::sqrt(m_x * m_x + m_y * m_y);
}

double Vector::SquaredNorm() const {
  return (m_x * m_x + m_y * m_y);
}


Vector Vector::Normalized() const {
  double p_vectorNorm = Norm();
  if (p_vectorNorm == 0.) {
    return Vector();
    //throw std::runtime_error("Vector is nul, cannot normalize it. (Vector Vector::normalized() const)");
  }

  double n = 1. / p_vectorNorm;
  return Vector(m_x*n, m_y*n);
}

Vector& Vector::Normalize() {
  double p_vectorNorm = Norm();
  if (p_vectorNorm == 0.)
  {
    SetX(0.);
    SetY(0.);
    return *this;
    //throw std::runtime_error("Vector is nul, cannot normalize it. (Vector& Vector::normalize())");
  }

  double n = 1. / p_vectorNorm;
  m_x *= n;
  m_y *= n;
  return *this;
}

double Vector::Determinant(Vector const& p_vector1, Vector const& p_vector2) {
  return p_vector1.m_x * p_vector2.m_y - p_vector1.m_y * p_vector2.m_x;
}

bool Vector::AreColinear(Vector const& p_vector1, Vector const& p_vector2) {
  /// Why 100?
  return std::abs(Determinant(p_vector1, p_vector2)) < 100.*DBL_EPSILON;
}

bool Vector::AreOrthogonal(Vector const& p_vector1, Vector const& p_vector2) {
  return std::abs(p_vector1 * p_vector2) < DBL_EPSILON;
}

double Vector::Angle(Vector const& p_vector1, Vector const& p_vector2) {
  double cos = p_vector1.Normalized() * p_vector2.Normalized();
  double sin = Determinant(p_vector1.Normalized(), p_vector2.Normalized());

  return std::atan2(sin, cos);
}

std::ostream& operator<<(std::ostream& p_os, Vector const& p_vector) {
  return p_os << "(Vector) (" << p_vector.m_x << "; " << p_vector.m_y << ")";
}

}
