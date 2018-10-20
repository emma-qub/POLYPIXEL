#ifndef DEVIATION_HXX
#define DEVIATION_HXX

#include "Core/Objects/Object.hxx"

#include "Core/Geometry/Segment.hxx"

class Deviation: public Object {

public:
  Deviation();
  ~Deviation() override;

  virtual std::vector<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const = 0;
};

#endif
