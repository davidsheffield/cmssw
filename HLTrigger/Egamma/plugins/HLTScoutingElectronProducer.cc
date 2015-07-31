// -*- C++ -*-
//
// Package:    HLTrigger/Egamma
// Class:      HLTScoutingElectronProducer
//
/**\class HLTScoutingElectronProducer HLTScoutingElectronProducer.cc HLTrigger/Egamma/plugin/HLTScoutingElectronProducer.cc

Description: Producer for ScoutingElectron

*/
//
// Original Author:  David G. Sheffield (Rutgers)
//         Created:  Mon, 20 Jul 2015
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/getRef.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"

#include "DataFormats/HLTReco/interface/HLTScoutingElectron.h"

class HLTScoutingElectronProducer : public edm::stream::EDProducer<> {
    typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::RecoEcalCandidate>, float,
						unsigned int> > RecoEcalCandMap;
    public:
        explicit HLTScoutingElectronProducer(const edm::ParameterSet&);
        ~HLTScoutingElectronProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::RecoEcalCandidateCollection> EgammaCandidateCollection_;
        edm::EDGetTokenT<reco::GsfTrackCollection> EgammaGsfTrackCollection_;
        edm::EDGetTokenT<RecoEcalCandMap> SigmaIEtaIEtaMap_;
        edm::EDGetTokenT<RecoEcalCandMap> HoverEMap_;
        edm::EDGetTokenT<RecoEcalCandMap> DetaMap_;
        edm::EDGetTokenT<RecoEcalCandMap> DphiMap_;
        edm::EDGetTokenT<RecoEcalCandMap> MissingHitsMap_;
        edm::EDGetTokenT<RecoEcalCandMap> OneOEMinusOneOPMap_;
        edm::EDGetTokenT<RecoEcalCandMap> EcalPFClusterIsoMap_;
        edm::EDGetTokenT<RecoEcalCandMap> EleGsfTrackIsoMap_;
        edm::EDGetTokenT<RecoEcalCandMap> HcalPFClusterIsoMap_;

        double electronPtCut;
        double electronEtaCut;
};

//
// constructors and destructor
//
HLTScoutingElectronProducer::HLTScoutingElectronProducer(const edm::ParameterSet& iConfig):
    EgammaCandidateCollection_(consumes<reco::RecoEcalCandidateCollection>
			       (iConfig.getParameter<edm::InputTag>("EgammaCandidates"))),
    EgammaGsfTrackCollection_(consumes<reco::GsfTrackCollection>
			      (iConfig.getParameter<edm::InputTag>("EgammaGsfTracks"))),
    SigmaIEtaIEtaMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						    "SigmaIEtaIEtaMap"))),
    HoverEMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>("HoverEMap"))),
    DetaMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>("DetaMap"))),
    DphiMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>("DphiMap"))),
    MissingHitsMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						  "MissingHitsMap"))),
    OneOEMinusOneOPMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						      "OneOEMinusOneOPMap"))),
    EcalPFClusterIsoMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						       "EcalPFClusterIsoMap"))),
    EleGsfTrackIsoMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						     "EleGsfTrackIsoMap"))),
    HcalPFClusterIsoMap_(consumes<RecoEcalCandMap>(iConfig.getParameter<edm::InputTag>(
						       "HcalPFClusterIsoMap"))),
    electronPtCut(iConfig.getParameter<double>("electronPtCut")),
    electronEtaCut(iConfig.getParameter<double>("electronEtaCut"))
{
    //register products
    produces<HLTScoutingElectronCollection>("scoutingElectrons");
}

HLTScoutingElectronProducer::~HLTScoutingElectronProducer()
{ }

// ------------ method called to produce the data  ------------
void HLTScoutingElectronProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    // Get RecoEcalCandidate
    Handle<reco::RecoEcalCandidateCollection> EgammaCandidateCollection;
    if(!iEvent.getByToken(EgammaCandidateCollection_,
			  EgammaCandidateCollection)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: EgammaCandidateCollection" << "\n";
        return;
    }

    // Get GsfTrack
    Handle<reco::GsfTrackCollection> EgammaGsfTrackCollection;
    if(!iEvent.getByToken(EgammaGsfTrackCollection_,
			  EgammaGsfTrackCollection)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: EgammaGsfTrackCollection" << "\n";
        return;
    }

    // Get SigmaIEtaIEtaMap
    Handle<RecoEcalCandMap> SigmaIEtaIEtaMap;
    if(!iEvent.getByToken(SigmaIEtaIEtaMap_, SigmaIEtaIEtaMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaClusterShape:sigmaIEtaIEta5x5" << "\n";
        return;
    }

    // Get HoverEMap
    Handle<RecoEcalCandMap> HoverEMap;
    if(!iEvent.getByToken(HoverEMap_, HoverEMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaHoverE" << "\n";
        return;
    }

    // Get DetaMap
    Handle<RecoEcalCandMap> DetaMap;
    if(!iEvent.getByToken(DetaMap_, DetaMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaGsfTrackVars:Deta" << "\n";
        return;
    }

    // Get Map
    Handle<RecoEcalCandMap> DphiMap;
    if(!iEvent.getByToken(DphiMap_, DphiMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaGsfTrackVars:Dphi" << "\n";
        return;
    }

    // Get MissingHitsMap
    Handle<RecoEcalCandMap> MissingHitsMap;
    if(!iEvent.getByToken(MissingHitsMap_, MissingHitsMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaGsfTrackVars:MissingHits" << "\n";
        return;
    }

    // Get 1/E - 1/p Map
    Handle<RecoEcalCandMap> OneOEMinusOneOPMap;
    if(!iEvent.getByToken(OneOEMinusOneOPMap_, OneOEMinusOneOPMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaGsfTrackVars:OneOESuperMinusOneOP" << "\n";
        return;
    }

    // Get EcalPFClusterIsoMap
    Handle<RecoEcalCandMap> EcalPFClusterIsoMap;
    if(!iEvent.getByToken(EcalPFClusterIsoMap_, EcalPFClusterIsoMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaEcalPFClusterIso" << "\n";
        return;
    }

    // Get EleGsfTrackIsoMap
    Handle<RecoEcalCandMap> EleGsfTrackIsoMap;
    if(!iEvent.getByToken(EleGsfTrackIsoMap_, EleGsfTrackIsoMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: hltEgammaEleGsfTrackIso" << "\n";
        return;
    }

    // Get HcalPFClusterIsoMap
    Handle<RecoEcalCandMap> HcalPFClusterIsoMap;
    if(!iEvent.getByToken(HcalPFClusterIsoMap_, HcalPFClusterIsoMap)){
        edm::LogError ("HLTScoutingElectronProducer")
	    << "invalid collection: HcalPFClusterIso" << "\n";
        return;
    }

    // Produce electrons
    std::auto_ptr<HLTScoutingElectronCollection> outElectrons(new HLTScoutingElectronCollection());
    int index = 0;
    for (auto &electron : *EgammaCandidateCollection) {
	reco::RecoEcalCandidateRef electronRef = getRef(EgammaCandidateCollection, index);
	++index;
	if (electronRef.isNull() && !electronRef.isAvailable())
	    continue;

	if (electron.pt() < electronPtCut)
	    continue;
	if (fabs(electron.eta()) > electronEtaCut)
	    continue;
	// Tight electron ID cut
	if (fabs(electron.eta()) < 1.479) {
	    if (fabs((*DetaMap)[electronRef]) > 0.006046
		|| fabs((*DphiMap)[electronRef]) > 0.028092
		|| (*SigmaIEtaIEtaMap)[electronRef] > 0.009947
		|| (*HoverEMap)[electronRef] > 0.045772
		|| (*OneOEMinusOneOPMap)[electronRef] > 0.020118
		|| (*MissingHitsMap)[electronRef] > 1)
		continue;
	} else if (fabs(electron.eta()) < 2.5) {
	    if (fabs((*DetaMap)[electronRef]) > 0.007057
		|| fabs((*DphiMap)[electronRef]) > 0.030159
		|| (*SigmaIEtaIEtaMap)[electronRef] > 0.028237
		|| (*HoverEMap)[electronRef] > 0.067778
		|| (*OneOEMinusOneOPMap)[electronRef] > 0.098919
		|| (*MissingHitsMap)[electronRef] > 1)
		continue;
	}

	outElectrons->emplace_back(electron.pt(), electron.eta(), electron.phi(), electron.mass(),
				   0.0, // d0
				   0.0, // dZ
				   (*DetaMap)[electronRef], (*DphiMap)[electronRef],
				   (*SigmaIEtaIEtaMap)[electronRef], (*HoverEMap)[electronRef],
				   (*OneOEMinusOneOPMap)[electronRef],
				   (*MissingHitsMap)[electronRef],
				   0, // charge
				   (*EcalPFClusterIsoMap)[electronRef],
				   (*HcalPFClusterIsoMap)[electronRef],
				   (*EleGsfTrackIsoMap)[electronRef]);
	// float pt, float eta, float phi, float m,
	// float d0, float dZ, float dEtaIn, float dPhiIn,
	// float sigmaIetaIeta, float hOverE, float ooEMOop,
	// int missingHits, int charge,
	// float ecalIso, float hcalIso, float trackIso
    }

    // Put output
    iEvent.put(outElectrons, "scoutingElectrons");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void HLTScoutingElectronProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("EgammaCandidates", edm::InputTag("hltEgammaCandidates"));
    desc.add<edm::InputTag>("EgammaGsfTracks", edm::InputTag("hltEgammaGsfTracks"));
    desc.add<edm::InputTag>("SigmaIEtaIEtaMap", edm::InputTag(
				"hltEgammaClusterShape:sigmaIEtaIEta5x5"));
    desc.add<edm::InputTag>("HoverEMap", edm::InputTag("hltEgammaHoverE"));
    desc.add<edm::InputTag>("DetaMap", edm::InputTag("hltEgammaGsfTrackVars:Deta"));
    desc.add<edm::InputTag>("DphiMap", edm::InputTag("hltEgammaGsfTrackVars:Dphi"));
    desc.add<edm::InputTag>("MissingHitsMap", edm::InputTag("hltEgammaGsfTrackVars:MissingHits"));
    desc.add<edm::InputTag>("OneOEMinusOneOPMap", edm::InputTag(
				"hltEgammaGsfTrackVars:OneOESuperMinusOneOP"));
    desc.add<edm::InputTag>("EcalPFClusterIsoMap", edm::InputTag("hltEgammaEcalPFClusterIso"));
    desc.add<edm::InputTag>("EleGsfTrackIsoMap", edm::InputTag("hltEgammaEleGsfTrackIso"));
    desc.add<edm::InputTag>("HcalPFClusterIsoMap", edm::InputTag("hltEgammaHcalPFClusterIso"));
    desc.add<double>("electronPtCut", 10.0);
    desc.add<double>("electronEtaCut", 3.0);
    descriptions.add("scoutingElectronProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTScoutingElectronProducer);
