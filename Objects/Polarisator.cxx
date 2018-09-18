#include "Polarisator.hxx"

Polarisator::Polarisator():
  Deviation() {
}

Polarisator::~Polarisator() = default;

QList<ppxl::Segment> Polarisator::deviateLine(const ppxl::Segment& p_line) const
{
  QList<ppxl::Segment> lines;
  lines << p_line;
  /// Compute lines

  return lines;
}
