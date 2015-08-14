#include "YReflectionVisitor.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Point.h"
using namespace std;

void YReflectionVisitor::visitCircle(Circle *s) {
  Point tmp = s->getCentre();
  tmp.setX(tmp.getX() * -1);
  s->setCentre(tmp);
}

void YReflectionVisitor::visitRectangle(Rectangle *s) {
  vector<Point> points(2);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setX(points[i].getX() * -1);
    s->setVertex(i, points[i]);
  }
}

void YReflectionVisitor::visitTriangle(Triangle *s){
  vector<Point> points(3);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setX(points[i].getX() * -1);
    s->setVertex(i, points[i]);
  }
}

