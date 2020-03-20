#ifndef MUTABLE_HXX
#define MUTABLE_HXX

#include "Core/Objects/Object.hxx"

#include "Core/Geometry/Point.hxx"

class Mutable: public Object {

public:
  Mutable(double p_x, double p_y, Object* p_object = nullptr);
  ~Mutable() override;

  void SetObject(Object* p_object);
  inline Object* GetObject() const { return m_object; }
  void AddRound();
  int GetRoundNumber() const;
  ppxl::Point GetPosition() const;

  virtual ppxl::Point GetObjectPosition() const = 0;

  virtual void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) override;

private:
  Object* m_object;
  int m_roundNumber;
  ppxl::Point m_position;
};

#endif
