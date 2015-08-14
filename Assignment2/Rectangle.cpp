#include "Rectangle.h"
#include "ShapeVisitor.h"
#include <iostream>
#define rectangle "rectangle"
using namespace std;

Rectangle::Rectangle(istream& is) {
  try{
    is >> vertices_.first >> vertices_.second;
  } catch(string s) {
    is.setstate(ios_base::failbit);
    cout << s << endl;
  }
}


void Rectangle::draw() const {
  cout << "[R: " << vertices_.first << ", " << vertices_.second;
  cout << ']';
}

void Rectangle::save(ostream& os) const {
  os << rectangle << '\n' << vertices_.first << ' ';
  os << vertices_.second << endl;
}

void Rectangle::accept(ShapeVisitor& v) {
  v.visitRectangle(this);
}
