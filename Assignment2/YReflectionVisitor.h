#ifndef YREFLECTIONVISITOR_H
#define YREFLECTIONVISITOR_H
#include "ShapeVisitor.h"
#include <vector>

class Circle;
class Rectangle;
class Triangle;

class  YReflectionVisitor: public ShapeVisitor {
public:
  virtual ~YReflectionVisitor() {}  

  // provide implementation for the following in CPP file
  // implementation should reflect the shape about the y-axis
  virtual void visitCircle(Circle *s);
  virtual void visitRectangle(Rectangle *s);
  virtual void visitTriangle(Triangle *s);
};
#endif
