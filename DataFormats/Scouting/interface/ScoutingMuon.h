#ifndef DataFormats_ScoutingMuon_h
#define DataFormats_ScoutingMuon_h

#include <vector>

// Class for holding muon information, for use in data scouting
// IMPORTANT: the content of this class should be changed only in backwards compatible ways!
class ScoutingMuon
{
    public:
        //constructor with values for all data fields
        ScoutingMuon(float pt, float eta, float phi, float m, float ecalIso, float hcalIso,
		     float trackIso, float chi2, float ndof, int charge, float dxy, float dz,
		     int nValidMuonHits, int nValidPixelHits, int nMatchedStations,
		     int nTrackerLayersWithMeasurement):
            pt_(pt), eta_(eta), phi_(phi), m_(m),
            ecalIso_(ecalIso), hcalIso_(hcalIso), trackIso_(trackIso),
            chi2_(chi2), ndof_(ndof), charge_(charge), dxy_(dxy), dz_(dz),
            nValidMuonHits_(nValidMuonHits), nValidPixelHits_(nValidPixelHits),
            nMatchedStations_(nMatchedStations),
		nTrackerLayersWithMeasurement_(nTrackerLayersWithMeasurement) {}
        //default constructor
        ScoutingMuon():pt_(0), eta_(0), phi_(0), m_(0), ecalIso_(0), hcalIso_(0), trackIso_(0),
		chi2_(0), ndof_(0), charge_(0), dxy_(0), dz_(0), nValidMuonHits_(0),
		nValidPixelHits_(0), nMatchedStations_(0), nTrackerLayersWithMeasurement_(0) {}

        //accessor functions
        float pt() { return pt_; }
        float eta() { return eta_; }
        float phi() { return phi_; }
        float m() { return m_; }
        float ecalIso() { return ecalIso_; }
        float hcalIso() { return hcalIso_; }
        float trackIso() { return trackIso_; }
        float chi2() { return chi2_; }
        float ndof() { return ndof_; }
        int charge() { return charge_; }
        float dxy() { return dxy_; }
        float dz() { return dz_; }
        int nValidMuonHits() { return nValidMuonHits_; }
        int nValidPixelHits() { return nValidPixelHits_; }
        int nMatchedStations() { return nMatchedStations_; }
        int nTrackerLayersWithMeasurement() { return nTrackerLayersWithMeasurement_; }

    private:
        float pt_;
        float eta_;
        float phi_;
        float m_;
        float ecalIso_;
        float hcalIso_;
        float trackIso_;
        float chi2_;
        float ndof_;
        int charge_;
        float dxy_;
        float dz_;
        int nValidMuonHits_;
        int nValidPixelHits_;
        int nMatchedStations_;
        int nTrackerLayersWithMeasurement_;
};

typedef std::vector<ScoutingMuon> ScoutingMuonCollection;

#endif
