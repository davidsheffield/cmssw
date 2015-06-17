#ifndef DataFormats_ScoutingCaloJet_h
#define DataFormats_ScoutingCaloJet_h

#include <vector>

//struct for holding calo jet information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
struct ScoutingCaloJet
{
  explicit ScoutingCaloJet(float pt, float eta, float phi, float m,
          float jetArea, float maxEInEmTowers, float maxEInHadTowers,
          float hadEnergyInHB, float hadEnergyInHE, float hadEnergyInHF,
          float emEnergyInEB, float emEnergyInEE, float emEnergyInHF,
          float towersArea, float userFloat):
      pt_(pt), eta_(eta), phi_(phi), m_(m),
      jetArea_(jetArea), maxEInEmTowers_(maxEInEmTowers), maxEInHadTowers_(maxEInHadTowers), 
      hadEnergyInHB_(hadEnergyInHB), hadEnergyInHE_(hadEnergyInHE), hadEnergyInHF_(hadEnergyInHF),
      emEnergyInEB_(emEnergyInEB), emEnergyInEE_(emEnergyInEE), emEnergyInHF_(emEnergyInHF),
      towersArea_(towersArea), userFloat_(userFloat){ }
  ScoutingCaloJet():pt_(0), eta_(0), phi_(0), m_(0), 
    jetArea_(0), maxEInEmTowers_(0), maxEInHadTowers_(0), 
    hadEnergyInHB_(0), hadEnergyInHE_(0), hadEnergyInHF_(0),
    emEnergyInEB_(0), emEnergyInEE_(0), emEnergyInHF_(0),
    towersArea_(0), userFloat_(0) { }
  float pt_;
  float eta_;
  float phi_;
  float m_;
  float jetArea_;
  float maxEInEmTowers_;
  float maxEInHadTowers_;
  float hadEnergyInHB_;
  float hadEnergyInHE_;
  float hadEnergyInHF_;
  float emEnergyInEB_;
  float emEnergyInEE_;
  float emEnergyInHF_;
  float towersArea_;
  float userFloat_;
};

typedef std::vector<ScoutingCaloJet> ScoutingCaloJetCollection;

#endif
