#ifndef MIRROR_HXX
#define MIRROR_HXX

#include "Deviation.hxx"

class Mirror: public Deviation {

public:
  Mirror(double p_xa = -1, double p_ya = -1, double p_xb = -1, double p_yb = -1);
  ~Mirror() override;

  inline ppxl::Segment GetLine() const { return m_line; }
  inline void SetLine(ppxl::Segment const& p_line) { m_line = p_line; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point) const override;
  std::vector<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const override;

private:
  ppxl::Segment m_line;
};

#endif
