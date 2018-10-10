#ifndef DIODE_HXX
#define DIODE_HXX

#include "Obstacle.hxx"
#include "Core/Vector.hxx"
#include "Core/Point.hxx"

class OneWay: public Obstacle {

public:
  OneWay(ppxl::Point const& A = ppxl::Point(), ppxl::Point const& B = ppxl::Point(),
    ppxl::Point const& O = ppxl::Point(), ppxl::Point const& D = ppxl::Point());
  ~OneWay() override;

  bool Crossing(ppxl::Segment const& line) const final;

private:
  ppxl::Segment m_diodeLine;
  ppxl::Vector m_diodeDirection;
};

#endif
