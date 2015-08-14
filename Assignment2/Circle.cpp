#include "Circle.h"
#include "ShapeVisitor.h"
#include "Point.h"
#include <string>
class XReflectionVisitor;
using namespace std;

Circle::Circle(istream& is) {
  Point p;
  int radius;
  
  try {
    if(!(is >> p >> radius)) 
      throw "badCircleException";
    centre_ = p;
    radius_ = radius;
  } catch (...) {
    throw; 
  }
}

void Circle::draw() const {
  cout << "[C: " << centre_ << ", " << radius_ << ']';
}

void Circle::save(ostream& os) const {
  os << "circle\n" << centre_ << ' ' << radius_ << endl;
}


void Circle::accept(ShapeVisitor& v){ 
  v.visitCircle(this);
}
