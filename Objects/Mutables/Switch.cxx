#include "Switch.hxx"

Switch::Switch(double p_x, double p_y, Object* p_object):
  Mutable(p_x, p_y, p_object) {
}

Object::Type Switch::GetType() const {
  return eMutable;
}

Switch::~Switch() = default;

std::string Switch::GetName() const {
  return "Switch";
}

ppxl::Point Switch::GetObjectPosition() const {
  if (GetRoundNumber()%2 == 1) {
    return ppxl::Point(-1, -1);
  }

  return GetPosition();
}
