#ifndef OBSTACLE_HXX
#define OBSTACLE_HXX

#include "Object.hxx"
#include "Core/Segment.hxx"

class Obstacle: public Object {

public:
  Obstacle();
  ~Obstacle() override;

  virtual bool crossing(ppxl::Segment const& line) const = 0;
};

#endif
