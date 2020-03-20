#ifndef PORTAL_HXX
#define PORTAL_HXX

#include "Deviation.hxx"
#include "Core/Geometry/Vector.hxx"

class Portal: public Deviation {

public:
  Portal(double p_xaIn = -1, double p_yaIn = -1, double p_xbIn = -1, double p_ybIn = -1, double p_xaOut = -1, double p_yaOut = -1, double p_xbOut = -1, double p_ybOut = -1);
  Portal(ppxl::Segment const& p_in, ppxl::Segment const& p_out);
  ~Portal() override;

  inline ppxl::Segment GetIn() const { return m_in; }
  inline void SetIn(ppxl::Segment const& p_in) { m_in = p_in; };
  inline ppxl::Segment GetOut() const { return m_out; }
  inline void SetOut(ppxl::Segment const& p_out) { m_out = p_out; };
  inline ppxl::Vector GetNormalIn() const { return m_in.GetNormal(); }
  inline ppxl::Vector GetNormalOut() const { return m_out.GetNormal(); }

  inline void SetCreating(bool p_creating) { m_creating = p_creating; }
  inline bool IsCreating() const { return m_creating; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  std::vector<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const override;
  std::vector<ppxl::Segment> DeviateLine2(ppxl::Segment const& p_line) const;

  virtual void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) override;
  ppxl::Point GetControlPoint(ControlPointType p_controlPointType) const override;

private:
  ppxl::Segment m_in;
  ppxl::Segment m_out;
  bool m_creating;
};

#endif
