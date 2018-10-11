#include "Switch.hxx"

Switch::Switch(double p_x, double p_y, Object* p_object):
  Mutable(p_x, p_y, p_object) {
}

Switch::~Switch() = default;

Object::ObjectType Switch::GetObjectType() const {
  return eSwitch;
}

Object::CategoryType Switch::GetCategoryType() const {
  return eMutable;
}

std::string Switch::GetName() const {
  return "Switch";
}

ppxl::Point Switch::GetObjectPosition() const {
  if (GetRoundNumber()%2 == 1) {
    return ppxl::Point(-1, -1);
  }

  return GetPosition();
}
