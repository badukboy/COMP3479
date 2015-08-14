#include "XReflectionVisitor.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Point.h"
using namespace std;

void XReflectionVisitor::visitCircle(Circle *s){
  Point tmp = s->getCentre();
  tmp.setY(tmp.getY() * -1);
  s->setCentre(tmp);
}

void XReflectionVisitor::visitRectangle(Rectangle *s){
  vector<Point> points(2);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setY(points[i].getY() * -1);
    s->setVertex(i, points[i]);
  }
}

void XReflectionVisitor::visitTriangle(Triangle *s){
  vector<Point> points(3);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setY(points[i].getY() * -1);
    s->setVertex(i, points[i]);
  }
}

