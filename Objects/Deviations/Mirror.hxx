#ifndef MIRROR_HXX
#define MIRROR_HXX

#include "Deviation.hxx"

class Mirror: public Deviation {

public:
  Mirror(double p_xa = -1, double p_ya = -1, double p_xb = -1, double p_yb = -1);
  ~Mirror() override;

  inline ppxl::Segment GetMirrorLine() const { return m_mirrorLine; }

  std::string GetName() const override;
  QList<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const final;

private:
  ppxl::Segment m_mirrorLine;
};

#endif
