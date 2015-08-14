#include "ShapeFactory.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"

using namespace std;
Shape* ShapeFactory::create () {
  string type;

  if(!(*pis_ >> type))
    throw "ShapeFactory::create::NO_TYPE_EXCEPTION";
  try {
    cerr << "type::" << type << endl;
    if(type == "circle")
      return new Circle(*pis_);
    else if(type == "rectangle")
      return new Rectangle(*pis_);
    else if(type == "triangle")
      return new Triangle(*pis_);
    else
      throw "ShapeFactory::create()::NO_TYPE_EXCEPTION";
  } catch (...) {
    throw;
  }

  return 0;
}
