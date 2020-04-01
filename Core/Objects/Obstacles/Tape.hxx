#ifndef TAPE_HXX
#define TAPE_HXX

#include "Obstacle.hxx"

class Tape: public Obstacle {

public:
  Tape(double p_x, double p_y, double p_w, double p_h);
  ~Tape() override;

  inline double GetX1() const { return m_x1; }
  inline double GetY1() const { return m_y1; }

  inline double GetX2() const { return m_x2; }
  inline double GetY2() const { return m_y2; }

  inline double GetXmin() const { return std::min(m_x1, m_x2); }
  inline double GetYmin() const { return std::min(m_y1, m_y2); }

  inline double GetXmax() const { return std::max(m_x1, m_x2); }
  inline double GetYmax() const { return std::max(m_y1, m_y2); }

  inline double GetW() const { return std::abs(m_x2 - m_x1); }
  inline double GetH() const { return std::abs(m_y2 - m_y1); }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const override;
  bool Crossing(ppxl::Segment const& p_line) const override;

  void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) override;
  void Translate(ppxl::Vector const& p_direction) override;
  ppxl::Point GetControlPoint(ControlPointType p_controlPointType) const override;

protected:
  std::vector<ppxl::Point> GetTapeVertices() const;
  std::vector<ppxl::Segment> GetTapeEdges() const;

private:
  double m_x1;
  double m_y1;
  double m_x2;
  double m_y2;
};

#endif
