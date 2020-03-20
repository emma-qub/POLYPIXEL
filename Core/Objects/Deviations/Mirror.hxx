#ifndef MIRROR_HXX
#define MIRROR_HXX

#include "Deviation.hxx"

class Mirror: public Deviation {

public:
  Mirror(double p_xa = -1, double p_ya = -1, double p_xb = -1, double p_yb = -1);
  ~Mirror() override;

  inline ppxl::Segment GetLine() const { return m_line; }
  inline void SetLine(ppxl::Segment const& p_line) { m_line = p_line; }

  inline double GetX1() const { return m_line.GetA().GetX(); }
  inline double GetY1() const { return m_line.GetA().GetY(); }
  inline double GetX2() const { return m_line.GetB().GetX(); }
  inline double GetY2() const { return m_line.GetB().GetY(); }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  std::vector<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const override;

  virtual void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) override;
  ppxl::Point GetControlPoint(ControlPointType p_controlPointType) const override;

private:
  ppxl::Segment m_line;
};

#endif
