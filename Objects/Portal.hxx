#ifndef PORTAL_HXX
#define PORTAL_HXX

#include "Deviation.hxx"
#include "Core/Vector.hxx"

class Portal: public Deviation {

public:
  Portal(int p_xaIn = -1, int p_yaIn = -1, int p_xbIn = -1, int p_ybIn = -1, int p_xaOut = -1, int p_yaOut = -1, int p_xbOut = -1, int p_ybOut = -1);
  Portal(ppxl::Segment const& p_in, ppxl::Segment const& p_out);
  virtual ~Portal();

  inline ppxl::Segment GetIn() const { return m_in; }
  inline ppxl::Segment GetOut() const { return m_out; }
  inline ppxl::Vector GetNormalIn() const { return m_normalIn; }
  inline ppxl::Vector GetNormalOut() const { return m_normalOut; }

  std::string GetName() const;
  QList<ppxl::Segment> DeviateLine(ppxl::Segment const& p_line) const final;
  QList<ppxl::Segment> DeviateLine2(ppxl::Segment const& p_line) const;

private:
  ppxl::Segment m_in;
  ppxl::Segment m_out;

  ppxl::Vector m_normalIn;
  ppxl::Vector m_normalOut;
};

#endif
