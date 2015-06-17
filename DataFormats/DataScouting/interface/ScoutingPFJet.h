#ifndef DataFormats_ScoutingPFJet_h
#define DataFormats_ScoutingPFJet_h

#include <vector>

//struct for holding PF jet information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
struct ScoutingPFJet
{
  explicit ScoutingPFJet(float pt, float eta, float phi, float m, float jetArea, 
          float chargedHadronEnergy, float neutralHadronEnergy, float photonEnergy,
          float electronEnergy, float muonEnergy, float HFHadronEnergy, float HFEMEnergy,
          int chargedHadronMultiplicity, int neutralHadronMultiplicity, int photonMultiplicity,
          int electronMultiplicity, int muonMultiplicity,
          int HFHadronMultiplicity, int HFEMMultiplicity,
          float HOEnergy, float csv, float userFloat, std::vector<int> constituents):
      pt_(pt), eta_(eta), phi_(phi), m_(m), jetArea_(jetArea), 
      chargedHadronEnergy_(chargedHadronEnergy), neutralHadronEnergy_(neutralHadronEnergy),
      photonEnergy_(photonEnergy), electronEnergy_(electronEnergy), muonEnergy_(muonEnergy),
      HFHadronEnergy_(HFHadronEnergy), HFEMEnergy_(HFEMEnergy), 
      chargedHadronMultiplicity_(chargedHadronMultiplicity), neutralHadronMultiplicity_(neutralHadronMultiplicity),
      photonMultiplicity_(photonMultiplicity), electronMultiplicity_(electronMultiplicity),
      muonMultiplicity_(muonMultiplicity), HFHadronMultiplicity_(HFHadronMultiplicity),
      HFEMMultiplicity_(HFEMMultiplicity), HOEnergy_(HOEnergy), csv_(csv), userFloat_(userFloat),
      constituents_(constituents)
    {}
  ScoutingPFJet():pt_(0), eta_(0), phi_(0), m_(0), 
    jetArea_(0), chargedHadronEnergy_(0), neutralHadronEnergy_(0),
    photonEnergy_(0), electronEnergy_(0), muonEnergy_(0), HFHadronEnergy_(0), HFEMEnergy_(0),
    chargedHadronMultiplicity_(0), neutralHadronMultiplicity_(0), photonMultiplicity_(0),
    electronMultiplicity_(0), muonMultiplicity_(0), HFHadronMultiplicity_(0), 
    HFEMMultiplicity_(0), HOEnergy_(0), csv_(0), userFloat_(0), constituents_(std::vector<int>())
    {}
  float pt_;
  float eta_;
  float phi_;
  float m_;
  float jetArea_;
  float chargedHadronEnergy_;
  float neutralHadronEnergy_;
  float photonEnergy_;
  float electronEnergy_;
  float muonEnergy_;
  float HFHadronEnergy_;
  float HFEMEnergy_;
  int chargedHadronMultiplicity_;
  int neutralHadronMultiplicity_;
  int photonMultiplicity_;
  int electronMultiplicity_;
  int muonMultiplicity_;
  int HFHadronMultiplicity_;
  int HFEMMultiplicity_;
  float HOEnergy_;
  float csv_;
  float userFloat_;
  std::vector<int> constituents_;
};

typedef std::vector<ScoutingPFJet> ScoutingPFJetCollection;

#endif
