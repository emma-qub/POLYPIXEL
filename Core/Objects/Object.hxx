#ifndef OBJECT_HXX
#define OBJECT_HXX

#include <string>
#include "Core/Geometry/Point.hxx"
#include <cfloat>

class Object {

public:
  enum CategoryType {
    eDeviation,
    eMutable,
    eObstacle
  };

  enum ObjectType {
    eMirror,
    ePortal,
    eCountdown,
    eDisposable,
    eSwitch,
    eTransfer,
    eOneWay,
    eTape
  };

  enum ControlPointType {
    eLeft,
    eTopLeft,
    eTop,
    eTopRight,
    eRight,
    eBottomRight,
    eBottom,
    eBottomLeft,
    eCenter,
    eTopLeftYellow,
    eCenterYellow,
    eBottomRightYellow,
    eTopLeftBlue,
    eCenterBlue,
    eBottomRightBlue,
    eVertex,
    eNone
  };

  Object();
  virtual ~Object();

  virtual CategoryType GetCategoryType() const = 0;
  virtual ObjectType GetObjectType() const = 0;
  virtual std::string GetName() const = 0;
  virtual bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const = 0;
  virtual void MoveControlPoint(ppxl::Point const& p_point, ControlPointType p_controlPointType) = 0;
  virtual void Translate(ppxl::Vector const& p_direction) = 0;
  virtual ppxl::Point GetControlPoint(ControlPointType p_controlPointType) const = 0;

  ppxl::Point GetLeft() const { return GetControlPoint(eLeft); };
  ppxl::Point GetTopLeft() const { return GetControlPoint(eTopLeft); };
  ppxl::Point GetTop() const { return GetControlPoint(eTop); };
  ppxl::Point GetTopRight() const { return GetControlPoint(eTopRight); };
  ppxl::Point GetRight() const { return GetControlPoint(eRight); };
  ppxl::Point GetBottomRight() const { return GetControlPoint(eBottomRight); };
  ppxl::Point GetBottom() const { return GetControlPoint(eBottom); };
  ppxl::Point GetBottomLeft() const { return GetControlPoint(eBottomLeft); };
  ppxl::Point GetCenter() const { return GetControlPoint(eCenter); };
  ppxl::Point GetTopLeftYellow() const { return GetControlPoint(eTopLeftYellow); };
  ppxl::Point GetCenterYellow() const { return GetControlPoint(eCenterYellow); };
  ppxl::Point GetBottomRightYellow() const { return GetControlPoint(eBottomRightYellow); };
  ppxl::Point GetTopLeftBlue() const { return GetControlPoint(eTopLeftBlue); };
  ppxl::Point GetCenterBlue() const { return GetControlPoint(eCenterBlue); };
  ppxl::Point GetBottomRightBlue() const { return GetControlPoint(eBottomRightBlue); };
};

#endif
