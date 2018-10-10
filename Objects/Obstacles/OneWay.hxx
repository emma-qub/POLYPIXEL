#ifndef ONEWAY_HXX
#define ONEWAY_HXX

#include "Obstacle.hxx"
#include "Core/Vector.hxx"
#include "Core/Point.hxx"

class OneWay: public Obstacle {

public:
  OneWay(double p_xa = -1, double p_ya = -1, double p_xb = -1, double p_yb = -1, double p_xd = -1, double p_yd = -1);
  ~OneWay() override;

  inline ppxl::Segment GetLine() const { return m_line; }
  inline void SetLine(ppxl::Segment const& p_line) { m_line = p_line; }
  inline ppxl::Vector GetDirection() const { return m_direction; }
  inline void SetDirection(const ppxl::Vector& p_direction) { m_direction = p_direction; }

  std::string GetName() const override;
  bool Crossing(ppxl::Segment const& p_line) const override;

  ppxl::Vector getOneWayDirection() const;
  void setOneWayDirection(const ppxl::Vector& oneWayDirection);

private:
  ppxl::Segment m_line;
  ppxl::Vector m_direction;
};

#endif
