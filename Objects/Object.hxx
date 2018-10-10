#ifndef OBJECT_HXX
#define OBJECT_HXX

#include <string>

class Object {

public:
  Object();
  virtual ~Object();

  virtual std::string GetName() const = 0;
};

#endif
