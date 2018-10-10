#ifndef COUNTDOWN_HXX
#define COUNTDOWN_HXX

#include "Objects/Mutables/Mutable.hxx"

class Countdown: public Mutable {

public:
  Countdown(int p_counter, double p_x, double p_y, Object* p_object = nullptr);
  ~Countdown() override;

  std::string GetName() const override;
  ppxl::Point GetObjectPosition() const override;

private:
  int m_counter;
};

#endif
