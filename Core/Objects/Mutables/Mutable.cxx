#include "Mutable.hxx"

Mutable::Mutable(double p_x, double p_y, Object* p_object):
  Object(),
  m_object(p_object),
  m_roundNumber(0),
  m_position(ppxl::Point(p_x, p_y)) {
}

Mutable::~Mutable() = default;

void Mutable::SetObject(Object* p_object) {
  m_object = p_object;
}

void Mutable::AddRound() {
  ++m_roundNumber;
}

int Mutable::GetRoundNumber() const {
  return m_roundNumber;
}

ppxl::Point Mutable::GetPosition() const {
  return m_position;
}

void Mutable::MoveControlPoint(const ppxl::Point& p_point, Object::ControlPointType p_controlPointType) {

}
