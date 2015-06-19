#ifndef DataFormats_ScoutingPFCandidate_h
#define DataFormats_ScoutingPFCandidate_h

#include <vector>

//struct for holding PF candidate information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
struct ScoutingPFCandidate
{
  explicit ScoutingPFCandidate(float pt, float eta, float phi, float m, 
          int pdgId):
      pt_(pt), eta_(eta), phi_(phi), m_(m), pdgId_(pdgId)
    {}
  ScoutingPFCandidate():pt_(0), eta_(0), phi_(0), m_(0), pdgId_(0)
    {}
  float pt_;
  float eta_;
  float phi_;
  float m_;
  int pdgId_;
};

typedef std::vector<ScoutingPFCandidate> ScoutingPFCandidateCollection;

#endif
