#ifndef TAPE_HXX
#define TAPE_HXX

#include "Obstacle.hxx"

class Tape: public Obstacle {

public:
  Tape(double p_x, double p_y, double p_w, double p_h);
  ~Tape() override;

  inline double getX() const { return m_x; }
  inline void setX(double p_x) { m_x = p_x; }

  inline double getY() const { return m_y; }
  inline void setY(double p_y) { m_y = p_y; }

  inline double getW() const { return m_w; }
  inline void setW(double p_w) { m_w = p_w; }

  inline double getH() const { return m_h; }
  inline void setH(double p_h) { m_h = p_h; }

  std::string GetName() const override;
  bool Crossing(ppxl::Segment const& line) const final;

protected:
  std::vector<ppxl::Point> getTapeVertices() const;
  std::vector<ppxl::Segment> getTapeEdges() const;

private:
  double m_x;
  double m_y;
  double m_w;
  double m_h;
};

#endif
