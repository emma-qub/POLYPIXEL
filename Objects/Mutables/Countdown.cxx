#include "Countdown.hxx"

Countdown::Countdown(int p_counter, double p_x, double p_y, Object* p_object):
  Mutable(p_x, p_y, p_object),
  m_counter(p_counter) {
}

Object::Type Countdown::GetType() const {
  return eMutable;
}

Countdown::~Countdown() = default;

std::string Countdown::GetName() const {
  return "Countdown";
}

ppxl::Point Countdown::GetObjectPosition() const {
  if (m_counter < GetRoundNumber()) {
    return ppxl::Point(-1, -1);
  }

  return GetPosition();
}
