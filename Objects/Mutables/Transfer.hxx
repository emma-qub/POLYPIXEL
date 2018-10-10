#ifndef TRANSFER_HXX
#define TRANSFER_HXX

#include "Objects/Mutables/Mutable.hxx"

class Transfer: public Mutable {

public:
  Transfer(double p_x1, double p_y1, double p_x2, double p_y2, Object* p_object = nullptr);
  ~Transfer() override;

  std::string GetName() const override;
  ppxl::Point GetObjectPosition() const override;

private:
  ppxl::Point m_position2;
};

#endif
