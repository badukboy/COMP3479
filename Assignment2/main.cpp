#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "ShapeFactory.h"
#include "XReflectionVisitor.h"
#include "YReflectionVisitor.h"
#include "TranslationVisitor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

vector<Shape*>  readShapes(istream& is);
void            drawShapes(const vector<Shape*>& shapes);
void            operate(vector<Shape*>& shapes); 


int main(int argc, char* argv[]) {
  vector<Shape*> shapes;
  fstream fs;
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <source-file>" << endl;
    return 1;
  }

  fs.open(argv[1], fstream::in);

  // if the file exists, draw the shapes
  if(fs.good()) {
    shapes = readShapes(fs);
    drawShapes(shapes);
  }
  fs.close();   // close the file

  // open the file for writing
  fs.open(argv[1], fstream::out | fstream::trunc);

  operate(shapes); 

  for(auto it = shapes.begin(); it != shapes.end(); ++it) {
    (*it)->save(fs);
  }

  fs.close();
}

void operate(vector<Shape*>& shapes) {
  string        cmd;
  string        buf;

  while(getline(cin, buf)) {
    istringstream    iss(buf);
    if( (iss >> cmd) == 0) {
      continue;
    } try {
    if(cmd == "c") {
      Shape *s;
      s = ShapeFactory(iss).create();
      shapes.push_back(s);
      drawShapes(shapes);
    }
    if(cmd == "x") {
      XReflectionVisitor xrv;
      for(auto it = shapes.begin(); it != shapes.end(); ++it)
        (*it)->accept(xrv); 
      drawShapes(shapes);
    }
    if(cmd == "y") {
      YReflectionVisitor yrv;
      for(auto it = shapes.begin(); it != shapes.end(); ++it)
        (*it)->accept(yrv);
      drawShapes(shapes);
    }
    if(cmd == "t") {
      char lb, c, rb;
      int x, y;
      if(iss >> lb >> x >> c >> y >> rb 
          && lb == '(' && c == ',' && rb == ')') {
        TranslationVisitor tv(Point(x,y));
        for(auto it = shapes.begin(); it != shapes.end(); ++it)
          (*it)->accept(tv);
        drawShapes(shapes);
      } else {
        continue;
      }
    }
    } catch (...) {
      continue; 
    }
    if(cmd == "d") {
      drawShapes(shapes);
    }
  }

}

vector<Shape*> readShapes( istream& is) {
  vector<Shape*> shapes;
  ShapeFactory sf(is);
  Shape *s;

  while(1){  
    try {
      s = sf.create();
      shapes.push_back(s);
    } catch (...) {
      break;
    }
  }

  return shapes;
}

void drawShapes(const vector<Shape*>& shapes) {
  for(auto it = shapes.begin(); it != shapes.end(); ++it) {
    (*it)->draw();
    cout << endl;
  }
}
