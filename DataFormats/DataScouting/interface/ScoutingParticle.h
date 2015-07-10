#ifndef DataFormats_ScoutingParticle_h
#define DataFormats_ScoutingParticle_h

#include <vector>

//class for holding PF candidate information, for use in data scouting 
//IMPORTANT: the content of this class should be changed only in backwards compatible ways!
class ScoutingParticle
{
    public:
        //constructor with values for all data fields
        ScoutingParticle(float pt, float eta, float phi, float m, 
                int pdgId):
            pt_(pt), eta_(eta), phi_(phi), m_(m), pdgId_(pdgId) {}
        //default constructor
        ScoutingParticle():pt_(0), eta_(0), phi_(0), m_(0), pdgId_(0) {}

        //accessor functions
        float pt() { return pt_; }
        float eta() { return eta_; }
        float phi() { return phi_; }
        float m() { return m_; }
        int pdgId() { return pdgId_; }

    private:
        float pt_;
        float eta_;
        float phi_;
        float m_;
        int pdgId_;
};

typedef std::vector<ScoutingParticle> ScoutingParticleCollection;

#endif
