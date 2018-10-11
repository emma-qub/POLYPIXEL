#include "Disposable.hxx"

Disposable::Disposable(double p_x, double p_y, Object* p_object):
  Mutable(p_x, p_y, p_object),
  m_objectUsed(false) {
}

Disposable::~Disposable() = default;

void Disposable::UseObject() {
  m_objectUsed = true;
}

Object::ObjectType Disposable::GetObjectType() const {
  return eDisposable;
}

Object::CategoryType Disposable::GetCategoryType() const {
  return eMutable;
}

std::string Disposable::GetName() const {
  return "Disposable";
}

ppxl::Point Disposable::GetObjectPosition() const {
  if (m_objectUsed) {
    return ppxl::Point(-1, -1);
  }

  return GetPosition();
}
