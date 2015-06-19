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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/DataScouting/interface/ScoutingCaloJet.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"

class ScoutingCaloProducer : public edm::EDProducer {
    public:
        explicit ScoutingCaloProducer(const edm::ParameterSet&);
        ~ScoutingCaloProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::CaloJetCollection> caloJetCollection_;
        edm::EDGetTokenT<reco::VertexCollection> vertexCollection_;

        double caloJetPtCut;
        double caloJetEtaCut;
};

//
// constructors and destructor
//
ScoutingCaloProducer::ScoutingCaloProducer(const edm::ParameterSet& iConfig):
    caloJetCollection_(consumes<reco::CaloJetCollection>(iConfig.getParameter<edm::InputTag>("caloJetCollection"))),
    vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexCollection"))),
    caloJetPtCut(iConfig.getParameter<double>("caloJetPtCut")),
    caloJetEtaCut(iConfig.getParameter<double>("caloJetEtaCut"))
{
    //register products
    produces<ScoutingCaloJetCollection>("scoutingCaloJets");
    produces<ScoutingVertexCollection>("scoutingVertices");
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

    //get vertices
    Handle<reco::VertexCollection> vertexCollection;
    std::auto_ptr<ScoutingVertexCollection> outVertices(new ScoutingVertexCollection());
    if(iEvent.getByToken(vertexCollection_, vertexCollection)){
        //produce vertices (only if present; otherwise return an empty collection)
        for(auto &vtx : *vertexCollection){
            outVertices->push_back(ScoutingVertex(
                        vtx.x(), vtx.y(), vtx.z(), vtx.zError()
                        ));
        }
    }

    //produce calo jets
    std::auto_ptr<ScoutingCaloJetCollection> outCaloJets(new ScoutingCaloJetCollection());
    for(auto &jet : *caloJetCollection){
        if(jet.pt() > caloJetPtCut && fabs(jet.eta()) < caloJetEtaCut){
            outCaloJets->push_back(ScoutingCaloJet(
                    jet.pt(), jet.eta(), jet.phi(), jet.mass(),
                    jet.jetArea(), jet.maxEInEmTowers(), jet.maxEInHadTowers(),
                    jet.hadEnergyInHB(), jet.hadEnergyInHE(), jet.hadEnergyInHF(),
                    jet.emEnergyInEB(), jet.emEnergyInEE(), jet.emEnergyInHF(),
                    jet.towersArea(), 0.0
                    ));
        }
    }

    //put output
    iEvent.put(outCaloJets, "scoutingCaloJets");
    iEvent.put(outVertices, "scoutingVertices");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ScoutingCaloProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("caloJetCollection",edm::InputTag("hltAK4CaloJets"));
    desc.add<edm::InputTag>("vertexCollection", edm::InputTag("hltPixelVertices"));
    desc.add<double>("caloJetPtCut", 20.0);
    desc.add<double>("caloJetEtaCut", 3.0);
    descriptions.add("scoutingCaloJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingCaloProducer);
