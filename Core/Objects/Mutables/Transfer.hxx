#ifndef TRANSFER_HXX
#define TRANSFER_HXX

#include "Core/Objects/Mutables/Mutable.hxx"

class Transfer: public Mutable {

public:
  Transfer(double p_x1, double p_y1, double p_x2, double p_y2, Object* p_object = nullptr);
  ~Transfer() override;

  inline ppxl::Point GetPosition2() const { return m_position2; }

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  ppxl::Point GetObjectPosition() const override;

private:
  ppxl::Point m_position2;
};

#endif
