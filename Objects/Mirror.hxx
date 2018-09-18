#ifndef MIRROR_HXX
#define MIRROR_HXX

#include "Deviation.hxx"

class Mirror: public Deviation {

public:
  Mirror(int p_xa = -1, int p_ya = -1, int p_xb = -1, int p_yb = -1);
  ~Mirror() override;

  inline ppxl::Segment getMirrorLine() const { return m_mirrorLine; }

  std::string getName() const;
  QList<ppxl::Segment> deviateLine(ppxl::Segment const& p_line) const final;

private:
  ppxl::Segment m_mirrorLine;
};

#endif
