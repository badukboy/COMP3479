#include "Triangle.h"
#include <iostream>
#include <string>
#include "ShapeVisitor.h"

#define triangle "triangle"
using namespace std;

Triangle::Triangle(istream& is) {
  try {
    is >> get<0>(vertices_) >> get<1>(vertices_) >> get<2>(vertices_);
  } catch (string s) {
    is.setstate(ios_base::failbit);
    cout << s << endl;
  }
}

void Triangle::draw() const {
  cout << "[T: " << get<0>(vertices_) << ", ";
  cout << get<1>(vertices_) << ", ";
  cout << get<2>(vertices_) << "]";
}

void Triangle::save(ostream& os) const {
  os << triangle << "\n";
  os << get<0>(vertices_) << ' ' << get<1>(vertices_);
  os << ' ' << get<2>(vertices_) << endl;
}

void Triangle::accept(ShapeVisitor& v) {
  v.visitTriangle(this);
}
