#ifndef COUNTDOWN_HXX
#define COUNTDOWN_HXX

#include "Core/Objects/Mutables/Mutable.hxx"

class Countdown: public Mutable {

public:
  Countdown(int p_counter, double p_x, double p_y, Object* p_object = nullptr);
  ~Countdown() override;

  inline int GetCounter() const { return m_counter; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  ppxl::Point GetObjectPosition() const override;

private:
  int m_counter;
};

#endif
