#ifndef DataFormats_HLTScoutingElectron_h
#define DataFormats_HLTScoutingElectron_h

#include <vector>

//class for holding electron information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
class HLTScoutingElectron
{
    public:
        //constructor with values for all data fields
        HLTScoutingElectron(float pt, float eta, float phi, float m,
                float d0, float dZ, float dEtaIn, float dPhiIn,
                float sigmaIetaIeta, float hOverE, float ooEMOop, 
                int missingHits, int charge,
                float ecalIso, float hcalIso, float trackIso):
    pt_(pt), eta_(eta), phi_(phi), m_(m),
    d0_(d0), dZ_(dZ), dEtaIn_(dEtaIn), dPhiIn_(dPhiIn), 
    sigmaIetaIeta_(sigmaIetaIeta), hOverE_(hOverE), ooEMOop_(ooEMOop),
    missingHits_(missingHits), charge_(charge),
    ecalIso_(ecalIso), hcalIso_(hcalIso), trackIso_(trackIso) {}
        //default constructor
        HLTScoutingElectron():pt_(0), eta_(0), phi_(0), m_(0), 
        d0_(0), dZ_(0), dEtaIn_(0), dPhiIn_(0),
        sigmaIetaIeta_(0), hOverE_(0), ooEMOop_(0),
        missingHits_(0), charge_(0), ecalIso_(0), hcalIso_(0), trackIso_(0) {}

        //accessor functions
        float pt() { return pt_; }
        float eta() { return eta_; }
        float phi() { return phi_; }
        float m() { return m_; }
        float d0() { return d0_; }
        float dZ() { return dZ_; }
        float dEtaIn() { return dEtaIn_; }
        float dPhiIn() { return dPhiIn_; }
        float sigmaIetaIeta() { return sigmaIetaIeta_; }
        float hOverE() { return hOverE_; }
        float ooEMOop() { return ooEMOop_; }
        int missingHits() { return missingHits_; }
        int charge() { return charge_; }
        float ecalIso() { return ecalIso_; }
        float hcalIso() { return hcalIso_; }
        float trackIso() { return trackIso_; }

    private:
        float pt_;
        float eta_;
        float phi_;
        float m_;
        float d0_;
        float dZ_;
        float dEtaIn_;
        float dPhiIn_;
        float sigmaIetaIeta_;
        float hOverE_;
        float ooEMOop_;
        int missingHits_;
        int charge_;
        float ecalIso_;
        float hcalIso_;
        float trackIso_;
};

typedef std::vector<HLTScoutingElectron> HLTScoutingElectronCollection;

#endif
