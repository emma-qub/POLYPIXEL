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
  void SetLine(ppxl::Segment const& p_line) { m_line = p_line; }
  inline ppxl::Vector GetNormal() const { return m_line.GetNormal(); }

  inline double GetX1() const { return m_line.GetA().GetX(); }
  inline double GetY1() const { return m_line.GetA().GetY(); }
  inline double GetX2() const { return m_line.GetB().GetX(); }
  inline double GetY2() const { return m_line.GetB().GetY(); }
  inline double GetNX() const { return m_line.GetNormal().GetX(); }
  inline double GetNY() const { return m_line.GetNormal().GetY(); }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  bool Crossing(ppxl::Segment const& p_line) const override;

  virtual void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) override;
  ppxl::Point GetControlPoint(ControlPointType p_controlPointType) const override;

private:
  ppxl::Segment m_line;
};

#endif
