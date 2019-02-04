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
  void SetIn(ppxl::Segment const& p_in);
  inline ppxl::Segment GetOut() const { return m_out; }
  void SetOut(ppxl::Segment const& p_out);
  inline ppxl::Vector GetNormalIn() const { return m_normalIn; }
  inline ppxl::Vector GetNormalOut() const { return m_normalOut; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  std::vector<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const override;
  std::vector<ppxl::Segment> DeviateLine2(ppxl::Segment const& p_line) const;

private:
  ppxl::Segment m_in;
  ppxl::Segment m_out;

  ppxl::Vector m_normalIn;
  ppxl::Vector m_normalOut;
};

#endif