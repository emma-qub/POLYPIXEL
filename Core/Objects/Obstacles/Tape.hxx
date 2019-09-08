#ifndef TAPE_HXX
#define TAPE_HXX

#include "Obstacle.hxx"

class Tape: public Obstacle {

public:
  Tape(double p_x, double p_y, double p_w, double p_h);
  ~Tape() override;

  inline double GetX() const { return m_x; }
  inline void SetX(double p_x) { m_x = p_x; }

  inline double GetY() const { return m_y; }
  inline void SetY(double p_y) { m_y = p_y; }

  inline double GetW() const { return m_w; }
  inline void SetW(double p_w) { m_w = p_w; }

  inline double GetH() const { return m_h; }
  inline void SetH(double p_h) { m_h = p_h; }

  inline double GetLeft() const { return m_x; }
  inline double GetTop() const { return m_y; }
  inline double GetRight() const { return m_x + m_w; }
  inline double GetBottom() const { return m_y + m_h; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  bool Crossing(ppxl::Segment const& p_line) const override;

protected:
  std::vector<ppxl::Point> GetTapeVertices() const;
  std::vector<ppxl::Segment> GetTapeEdges() const;

private:
  double m_x;
  double m_y;
  double m_w;
  double m_h;
};

#endif
