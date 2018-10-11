#ifndef OBJECT_HXX
#define OBJECT_HXX

#include <string>

class Object {

public:
  enum Type {
    eDeviation,
    eMutable,
    eObstacle
  };

  Object();
  virtual ~Object();

  virtual Type GetType() const = 0;
  virtual std::string GetName() const = 0;
};

#endif
