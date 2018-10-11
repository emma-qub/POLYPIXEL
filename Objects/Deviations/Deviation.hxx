#ifndef DEVIATION_HXX
#define DEVIATION_HXX

#include "Objects/Object.hxx"

#include "Core/Segment.hxx"

class Deviation: public Object {

public:
  Deviation();
  ~Deviation() override;

  virtual QList<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const = 0;
};

#endif
