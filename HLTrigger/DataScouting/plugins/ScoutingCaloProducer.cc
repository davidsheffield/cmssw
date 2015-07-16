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
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/MET.h"

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
        edm::EDGetTokenT<reco::METCollection> metCollection_;
        edm::EDGetTokenT<double> rho_;

        double caloJetPtCut;
        double caloJetEtaCut;

        bool doMet;
};

//
// constructors and destructor
//
ScoutingCaloProducer::ScoutingCaloProducer(const edm::ParameterSet& iConfig):
    caloJetCollection_(consumes<reco::CaloJetCollection>(iConfig.getParameter<edm::InputTag>("caloJetCollection"))),
    vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexCollection"))),
    metCollection_(consumes<reco::METCollection>(iConfig.getParameter<edm::InputTag>("metCollection"))),
    rho_(consumes<double>(iConfig.getParameter<edm::InputTag>("rho"))),
    caloJetPtCut(iConfig.getParameter<double>("caloJetPtCut")),
    caloJetEtaCut(iConfig.getParameter<double>("caloJetEtaCut")),
    doMet(iConfig.getParameter<bool>("doMet"))
{
    //register products
    produces<ScoutingCaloJetCollection>("scoutingCaloJets");
    produces<ScoutingVertexCollection>("scoutingVertices");
    produces<double>("rho");
    produces<double>("caloMetPt");
    produces<double>("caloMetPhi");
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
            outVertices->emplace_back(
                        vtx.x(), vtx.y(), vtx.z(), vtx.zError()
                        );
        }
    }

    //get rho
    Handle<double>rho;
    if(!iEvent.getByToken(rho_, rho)){
        edm::LogError ("ScoutingCaloProducer") << "invalid collection: rho" << "\n";
        return;
    }
    std::auto_ptr<double> outRho(new double(*rho));

    //get MET 
    Handle<reco::METCollection> metCollection;
    if(doMet && !iEvent.getByToken(metCollection_, metCollection)){
        edm::LogError ("ScoutingCaloProducer") << "invalid collection: metCollection" << "\n";
        return;
    }

    //produce calo jets
    std::auto_ptr<ScoutingCaloJetCollection> outCaloJets(new ScoutingCaloJetCollection());
    for(auto &jet : *caloJetCollection){
        if(jet.pt() > caloJetPtCut && fabs(jet.eta()) < caloJetEtaCut){
            outCaloJets->emplace_back(
                    jet.pt(), jet.eta(), jet.phi(), jet.mass(),
                    jet.jetArea(), jet.maxEInEmTowers(), jet.maxEInHadTowers(),
                    jet.hadEnergyInHB(), jet.hadEnergyInHE(), jet.hadEnergyInHF(),
                    jet.emEnergyInEB(), jet.emEnergyInEE(), jet.emEnergyInHF(),
                    jet.towersArea(), 0.0
                    );
        }
    }

    //produce MET
    double metPt = -999;
    double metPhi = -999;
    if(doMet){
        metPt = metCollection->front().pt();
        metPhi = metCollection->front().phi();
    }
    std::auto_ptr<double> outMetPt(new double(metPt));
    std::auto_ptr<double> outMetPhi(new double(metPhi));

    //put output
    iEvent.put(outCaloJets, "scoutingCaloJets");
    iEvent.put(outVertices, "scoutingVertices");
    iEvent.put(outRho, "rho");
    iEvent.put(outMetPt, "caloMetPt");
    iEvent.put(outMetPhi, "caloMetPhi");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ScoutingCaloProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("caloJetCollection",edm::InputTag("hltAK4CaloJets"));
    desc.add<edm::InputTag>("vertexCollection", edm::InputTag("hltPixelVertices"));
    desc.add<edm::InputTag>("metCollection", edm::InputTag("hltMetCleanUsingJetID"));
    desc.add<edm::InputTag>("rho", edm::InputTag("hltFixedGridRhoFastjetAllCalo"));
    desc.add<double>("caloJetPtCut", 20.0);
    desc.add<double>("caloJetEtaCut", 3.0);
    desc.add<bool>("doMet", true);
    descriptions.add("scoutingCaloJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingCaloProducer);
