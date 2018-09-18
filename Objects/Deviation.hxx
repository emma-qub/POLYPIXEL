#ifndef DEVIATION_HXX
#define DEVIATION_HXX

#include "Object.hxx"

#include "Core/Segment.hxx"

class Deviation: Object {

public:
  Deviation();
  ~Deviation() override;

  virtual QList<ppxl::Segment> deviateLine(ppxl::Segment const& line) const = 0;
};

#endif
