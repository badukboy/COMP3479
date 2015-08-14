#include "TranslationVisitor.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"


using namespace std;
void TranslationVisitor::visitCircle(Circle *s) {
  Point p = s->getCentre();

  p.setX(p.getX() + d_.getX());
  p.setY(p.getY() + d_.getY());

  s->setCentre(p);
}

void TranslationVisitor::visitRectangle(Rectangle *s) { 
 vector<Point> points(2);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setX(points[i].getX() + d_.getX());
    points[i].setY(points[i].getY() + d_.getY());
    s->setVertex(i, points[i]);
  }
}

void TranslationVisitor::visitTriangle(Triangle *s) {
  vector<Point> points(3);

  for(size_t i = 0; i < points.size(); ++i) {
    points[i] = s->getVertex(i);
    points[i].setX(points[i].getX() + d_.getX());
    points[i].setY(points[i].getY() + d_.getY());
    s->setVertex(i, points[i]);
  }
}
