#ifndef DataFormats_ScoutingBeamSpot_h
#define DataFormats_ScoutingBeamSpot_h

//struct for holding beamspot information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
struct ScoutingBeamSpot
{
  explicit ScoutingBeamSpot(double x, double y, double z,
          double sigmaZ, double widthX, double widthY, double dxdz, double dydz,
          double errorX, double errorY, double errorZ,
          double errorSigmaZ, double errorDxDz, double errorDyDz,
          double errorBeamWidth):
      x_(x), y_(y), z_(z), sigmaZ_(sigmaZ), widthX_(widthX), widthY_(widthY), dxdz_(dxdz), dydz_(dydz),
      errorX_(errorX), errorY_(errorY), errorZ_(errorZ), 
      errorSigmaZ_(errorSigmaZ), errorDxDz_(errorDxDz), errorDyDz_(errorDyDz), errorBeamWidth_(errorBeamWidth)
    {}
  ScoutingBeamSpot():x_(0), y_(0), z_(0), sigmaZ_(0), widthX_(0), widthY_(0), dxdz_(0), dydz_(0),
    errorX_(0), errorY_(0), errorZ_(0), errorSigmaZ_(0), errorDxDz_(0), errorDyDz_(0), errorBeamWidth_(0)
    { }
  double x_;
  double y_;
  double z_;
  double sigmaZ_;
  double widthX_;
  double widthY_;
  double dxdz_;
  double dydz_;
  double errorX_;
  double errorY_;
  double errorZ_;
  double errorSigmaZ_;
  double errorDxDz_;
  double errorDyDz_;
  double errorBeamWidth_;
};

#endif
