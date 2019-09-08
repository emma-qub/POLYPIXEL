#ifndef ONEWAY_HXX
#define ONEWAY_HXX

#include "Obstacle.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Segment.hxx"

class OneWay: public Obstacle {

public:
  OneWay(double p_xa = -1, double p_ya = -1, double p_xb = -1, double p_yb = -1);
  ~OneWay() override;

  inline ppxl::Segment GetLine() const { return m_line; }
  void SetLine(ppxl::Segment const& p_line) { m_line = p_line; m_direction = m_line.GetNormal(); }
  inline ppxl::Vector GetDirection() const { return m_direction; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  bool Crossing(ppxl::Segment const& p_line) const override;

private:
  ppxl::Segment m_line;
  ppxl::Vector m_direction;
};

#endif
