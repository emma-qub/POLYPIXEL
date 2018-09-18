#ifndef POLARISATOR_HXX
#define POLARISATOR_HXX

#include "Deviation.hxx"

class Polarisator: public Deviation {

public:
  Polarisator();
  ~Polarisator() override;

  QList<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const final;

private:
  double m_cx;
  double m_cy;
  double m_radius;
};

#endif
