#ifndef DataFormats_ScoutingVertex_h
#define DataFormats_ScoutingVertex_h

#include <vector>

//class for holding vertex information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
class ScoutingVertex
{
    public:
        //constructor with values for all data fields
        ScoutingVertex(double x, double y, double z, double zError):
            x_(x), y_(y), z_(z), zError_(zError) {}
        //default constructor
        ScoutingVertex(): x_(0), y_(0), z_(0), zError_(0) {}

        //accessor functions
        double x() { return x_; }
        double y() { return y_; }
        double z() { return z_; }
        double zError() { return zError_; }

    private:
        double x_;
        double y_;
        double z_;
        double zError_;
};

typedef std::vector<ScoutingVertex> ScoutingVertexCollection;

#endif
