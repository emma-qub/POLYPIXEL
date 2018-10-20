#ifndef OBSTACLE_HXX
#define OBSTACLE_HXX

#include "Core/Objects/Object.hxx"
#include "Core/Geometry/Segment.hxx"

class Obstacle: public Object {

public:
  Obstacle();
  ~Obstacle() override;

  virtual bool Crossing(ppxl::Segment const& p_line) const = 0;
};

#endif
