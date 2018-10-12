#ifndef MUTABLE_HXX
#define MUTABLE_HXX

#include "Objects/Object.hxx"

#include "Core/Point.hxx"

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

private:
  Object* m_object;
  int m_roundNumber;
  ppxl::Point m_position;
};

#endif
