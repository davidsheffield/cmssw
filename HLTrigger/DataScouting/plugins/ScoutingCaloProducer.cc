// -*- C++ -*-
//
// Package:    HLTrigger/DataScouting
// Class:      ScoutingCaloProducer
// 
/**\class ScoutingCaloProducer ScoutingCaloProducer.cc HLTrigger/DataScouting/plugins/ScoutingCaloProducer.cc

Description: Producer for ScoutingCaloJets from reco::CaloJet objects and ScoutingBeamSpot from reco::BeamSpot

*/
//
// Original Author:  Dustin James Anderson
//         Created:  Fri, 12 Jun 2015 15:49:20 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/DataScouting/interface/ScoutingCaloJet.h"
#include "DataFormats/DataScouting/interface/ScoutingBeamSpot.h"

class ScoutingCaloProducer : public edm::EDProducer {
    public:
        explicit ScoutingCaloProducer(const edm::ParameterSet&);
        ~ScoutingCaloProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::CaloJetCollection> caloJetCollection_;
        edm::EDGetTokenT<reco::BeamSpot> beamSpot_;
};

//
// constructors and destructor
//
ScoutingCaloProducer::ScoutingCaloProducer(const edm::ParameterSet& iConfig):
    caloJetCollection_(consumes<reco::CaloJetCollection>(iConfig.getParameter<edm::InputTag>("caloJetCollection"))),
    beamSpot_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot")))
{
    //register products
    produces<ScoutingCaloJetCollection>("scoutingCaloJets");
    produces<ScoutingBeamSpot>("scoutingBeamSpot");
}

ScoutingCaloProducer::~ScoutingCaloProducer()
{ }

// ------------ method called to produce the data  ------------
    void
ScoutingCaloProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    //get calo jets
    Handle<reco::CaloJetCollection> caloJetCollection;
    if(!iEvent.getByToken(caloJetCollection_, caloJetCollection)){
        edm::LogError ("ScoutingCaloProducer") << "invalid collection: caloJetCollection" << "\n";
        return;
    }

    //get beamspot object
    Handle<reco::BeamSpot> beamSpot;
    if(!iEvent.getByToken(beamSpot_, beamSpot)){
        edm::LogError ("ScoutingCaloProducer") << "invalid collection: beamSpot" << "\n";
        return;
    }

    //produce beamspot 
    std::auto_ptr<ScoutingBeamSpot> outBeamSpot(new ScoutingBeamSpot(
                beamSpot->x0(), beamSpot->y0(), beamSpot->z0(),
                beamSpot->sigmaZ(), beamSpot->BeamWidthX(), beamSpot->BeamWidthY(),
                beamSpot->dxdz(), beamSpot->dydz(), 
                beamSpot->x0Error(), beamSpot->y0Error(), beamSpot->z0Error(), 
                beamSpot->sigmaZ0Error(), beamSpot->dxdzError(), beamSpot->dydzError(), beamSpot->BeamWidthXError()
                ));

    //produce calo jets
    std::auto_ptr<ScoutingCaloJetCollection> outCaloJets(new ScoutingCaloJetCollection());
    for(auto &jet : *caloJetCollection){
        outCaloJets->push_back(ScoutingCaloJet(
                    jet.pt(), jet.eta(), jet.phi(), jet.mass(),
                    jet.jetArea(), jet.maxEInEmTowers(), jet.maxEInHadTowers(),
                    jet.hadEnergyInHB(), jet.hadEnergyInHE(), jet.hadEnergyInHF(),
                    jet.emEnergyInEB(), jet.emEnergyInEE(), jet.emEnergyInHF(),
                    jet.towersArea(), 0.0
                    ));
    }

    //put output
    iEvent.put(outCaloJets, "scoutingCaloJets");
    iEvent.put(outBeamSpot, "scoutingBeamSpot");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ScoutingCaloProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("caloJetCollection",edm::InputTag("hltAK4CaloJets"));
    desc.add<edm::InputTag>("beamSpot",edm::InputTag("hltOnlineBeamSpot"));
    descriptions.add("scoutingCaloJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingCaloProducer);
