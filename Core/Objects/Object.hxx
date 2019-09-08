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

  Object();
  virtual ~Object();

  virtual CategoryType GetCategoryType() const = 0;
  virtual ObjectType GetObjectType() const = 0;
  virtual std::string GetName() const = 0;
  virtual bool Intersect(ppxl::Point const& p_point, double p_tolerence = DBL_EPSILON) const = 0;
};

#endif
