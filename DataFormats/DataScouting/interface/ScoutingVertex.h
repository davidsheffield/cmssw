#ifndef DataFormats_ScoutingVertex_h
#define DataFormats_ScoutingVertex_h

#include <vector>

//struct for holding vertex information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
struct ScoutingVertex
{
  explicit ScoutingVertex(double x, double y, double z, double zError):
      x_(x), y_(y), z_(z), zError_(zError)
    {}
  ScoutingVertex(): x_(0), y_(0), z_(0), zError_(0)
    {}
  double x_;
  double y_;
  double z_;
  double zError_;
};

typedef std::vector<ScoutingVertex> ScoutingVertexCollection;

#endif
