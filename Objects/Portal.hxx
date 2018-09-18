#ifndef PORTAL_HXX
#define PORTAL_HXX

#include "Deviation.hxx"
#include "Core/Vector.hxx"

class Portal: public Deviation {

public:
  Portal(int p_xaIn = -1, int p_yaIn = -1, int p_xbIn = -1, int p_ybIn = -1, int p_xaOut = -1, int p_yaOut = -1, int p_xbOut = -1, int p_ybOut = -1);
  Portal(ppxl::Segment const& p_in, ppxl::Segment const& p_out);
  virtual ~Portal();

  inline ppxl::Segment getIn() const { return m_in; }
  inline ppxl::Segment getOut() const { return m_out; }
  inline ppxl::Vector getNormalIn() const { return m_normalIn; }
  inline ppxl::Vector getNormalOut() const { return m_normalOut; }

  std::string getName() const;
  QList<ppxl::Segment> deviateLine(ppxl::Segment const& p_line) const final;
  virtual QList<ppxl::Segment> deviateLine2(ppxl::Segment const& p_line) const final;

private:
  ppxl::Segment m_in;
  ppxl::Segment m_out;

  ppxl::Vector m_normalIn;
  ppxl::Vector m_normalOut;
};

#endif
