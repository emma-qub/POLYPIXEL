#ifndef SWITCH_HXX
#define SWITCH_HXX

#include "Core/Objects/Mutables/Mutable.hxx"

class Switch: public Mutable {

public:
  Switch(double p_x, double p_y, Object* p_object = nullptr);
  ~Switch() override;

  CategoryType GetCategoryType() const override;
  ObjectType GetObjectType() const override;
  std::string GetName() const override;
  ppxl::Point GetObjectPosition() const override;

};

#endif
