#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <utility>
#include <iostream>
#include "Shape.h"
#include "Point.h"

class ShapeVisitor;
class Rectangle: public Shape {
public:
  // read data for Rectangle from 'is'; used by factory
  // should read in data members saved by 'save' (except for the type name)
  // should perform some error-checking (see README)
  explicit Rectangle(std::istream& is);  

  // a Rectangle is specified by 2 (opposing) vertices
  // 'i' should be 0 or 1; if not, throw an exception
  Point getVertex(int i) const;

  // again, 'i' should be 0 or 1; if not, throw an exception
  void  setVertex(int i, const Point& p); 

  // write to standard output
  // example output:  [R: (2,-3), (4,-5)]  
  virtual void draw() const; 

  /* example output:
       rectangle
       (2,-3) (4,-5)
  */
  virtual void save(std::ostream& os = std::cout) const; 
  
  // accept a visitor
  virtual void accept(ShapeVisitor& visitor); 

private:
  std::pair<Point,Point> vertices_;
};

inline Point Rectangle::getVertex(int i) const {
  if(i != 0 && i != 1)
    throw "Rectangle::getVertex::badVertexException"; 
  
  return i == 0 ? vertices_.first : vertices_.second;
}

inline void Rectangle::setVertex(int i, const Point& p) {
  if(i == 0)
    vertices_.first = p;
  else if(i == 1)
    vertices_.second = p;
  else
    throw "Rectangle::setVertex::badVertexException";
}
#endif
