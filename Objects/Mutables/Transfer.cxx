#include "Transfer.hxx"

Transfer::Transfer(double p_x1, double p_y1, double p_x2, double p_y2, Object* p_object):
  Mutable(p_x1, p_y1, p_object),
  m_position2(p_x2, p_y2) {
}

Transfer::~Transfer() = default;

Object::ObjectType Transfer::GetObjectType() const {
  return eTransfer;
}

Object::CategoryType Transfer::GetCategoryType() const {
  return eMutable;
}

std::string Transfer::GetName() const {
  return "Transfer";
}

ppxl::Point Transfer::GetObjectPosition() const {
  if (GetRoundNumber()%2 == 0) {
    return GetPosition();
  } else {
    return m_position2;
  }
}
