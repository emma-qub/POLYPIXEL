#include "Polarisator.hxx"

Polarisator::Polarisator():
  Deviation() {
}

Polarisator::~Polarisator() = default;

QList<ppxl::Segment> Polarisator::DeviateLine(ppxl::Segment const& p_line) const {
  QList<ppxl::Segment> lines;
  lines << p_line;
  /// Compute lines

  return lines;
}
