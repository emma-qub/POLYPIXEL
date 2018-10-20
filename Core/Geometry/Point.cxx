#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"

#include <QDebug>

#include <cmath>    // sqrt, pow, abs
#include <cfloat>   // DBL_EPSILON
#include <random>

namespace ppxl {

Point::Point(double p_x, double p_y) {
  m_x = p_x;
  m_y = p_y;
}

Point::Point(double p_xMin, double p_xMax, double p_yMin, double p_yMax) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<> xDistribution(p_xMin, p_xMax);
  std::uniform_real_distribution<> yDistribution(p_yMin, p_yMax);

  m_x = xDistribution(rng);
  m_y = yDistribution(rng);
}

Point::Point(Point const& p_point) {
  m_x = p_point.m_x;
  m_y = p_point.m_y;
}

Point::~Point() = default;

void Point::Move(double const& p_x, double const& p_y) {
  m_x += p_x;
  m_y += p_y;
}

void Point::Symetry() {
  double tmp = m_y;
  m_y = m_x;
  m_x = tmp;
}

void Point::Homothetie(Point const& p_origin, double p_scale) {
  m_x = p_scale*(m_x - p_origin.m_x) + p_origin.m_x;
  m_y = p_scale*(m_y - p_origin.m_y) + p_origin.m_y;
}

Point Point::Translate(Vector const& p_vector) const {
  return Point(m_x + p_vector.GetX(), m_y + p_vector.GetY());
}

Point& Point::Translated(Vector const& p_vector) {
  m_x += p_vector.GetX();
  m_y += p_vector.GetY();
  return *this;
}

Point Point::operator/(double const& p_scalar) const {
  return Point(m_x/p_scalar, m_y/p_scalar);
}

Point& Point::operator/=(double const& p_scalar) {
  m_x /= p_scalar;
  m_y /= p_scalar;
  return *this;
}

Point operator+(Point const& p_point1, Point const& p_point2) {
  return Point(p_point1.m_x + p_point2.m_x, p_point1.m_y + p_point2.m_y);
}

Point operator+(Point const& p_origin, Vector const& p_direction) {
  return p_origin + Point(p_direction.GetX(), p_direction.GetY());
}

Point& Point::operator+=(Point const& p_point) {
  m_x += p_point.m_x;
  m_y += p_point.m_y;
  return *this;
}

Vector operator-(Point const& p_point1, Point const& p_point2) {
  return Vector(p_point2.m_x - p_point1.m_x, p_point2.m_y - p_point1.m_y);
}

Point& Point::operator=(Point const& p_point) {
  m_x = p_point.m_x;
  m_y = p_point.m_y;
  return *this;
}

bool operator==(Point const& p_point1, Point const& p_point2) {
  return std::abs(p_point1.m_x - p_point2.m_x) < DBL_EPSILON
      && std::abs(p_point1.m_y - p_point2.m_y) < DBL_EPSILON;
}

bool operator!=(Point const& p_point1, Point const& p_point2) {
  return !(p_point1 == p_point2);
}

bool operator<(Point const& p_point1, Point const& p_point2) {
  return (p_point1.m_x < p_point2.m_x)
      || ((p_point1.m_x - p_point2.m_x) < DBL_EPSILON
       && (p_point1.m_y < p_point2.m_y));
}

bool operator<=(Point const& p_point1, Point const& p_point2) {
  return (p_point1 == p_point2) || (p_point1 < p_point2);
}

bool operator>(Point const& p_point1, Point const& p_point2) {
  return !(p_point1 <= p_point2);
}

bool operator>=(Point const& p_point1, Point const& p_point2) {
  return !(p_point1 < p_point2);
}

double Point::Distance(Point const& p_point1, Point const& p_point2) {
  auto diffX = p_point2.m_x - p_point1.m_x;
  auto diffY = p_point2.m_y - p_point1.m_y;
  return std::sqrt(diffX*diffX + diffY*diffY);
}

Point Point::Middle(Point const& p_point1, Point const& p_point2) {
  return Point((p_point1.GetX() + p_point2.GetX())/2., (p_point1.GetY() + p_point2.GetY())/2.);
}

std::ostream& operator<<(std::ostream& p_os, Point const& p_point) {
  return p_os << "(Point) (" << p_point.m_x << "; " << p_point.m_y << ")";
}

QDebug operator<<(QDebug p_debug, Point const& p_point) {
  return p_debug << "(" << p_point.m_x << "; " << p_point.m_y << ")";
}

}
