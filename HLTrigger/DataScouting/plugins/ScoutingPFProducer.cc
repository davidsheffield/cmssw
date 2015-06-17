// -*- C++ -*-
//
// Package:    HLTrigger/DataScouting
// Class:      ScoutingPFProducer
// 
/**\class ScoutingPFProducer ScoutingPFProducer.cc HLTrigger/DataScouting/plugins/ScoutingPFProducer.cc

Description: Producer for ScoutingPFJets from reco::PFJet objects, ScoutingVertexs from reco::Vertexs and ScoutingPFCandidates from reco::PFCandidates

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

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFCandidate.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"

class ScoutingPFProducer : public edm::EDProducer {
    public:
        explicit ScoutingPFProducer(const edm::ParameterSet&);
        ~ScoutingPFProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::PFJetCollection> pfJetCollection_;
        edm::EDGetTokenT<reco::PFCandidateCollection> pfCandidateCollection_;
        edm::EDGetTokenT<reco::VertexCollection> vertexCollection_;
};

//
// constructors and destructor
//
ScoutingPFProducer::ScoutingPFProducer(const edm::ParameterSet& iConfig):
    pfJetCollection_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("pfJetCollection"))),
    pfCandidateCollection_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCandidateCollection"))),
    vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexCollection")))
{
    //register products
    produces<ScoutingPFJetCollection>("scoutingPFJets");
    produces<ScoutingPFCandidateCollection>("scoutingPFCandidates");
    produces<ScoutingVertexCollection>("scoutingVertices");
}

ScoutingPFProducer::~ScoutingPFProducer()
{ }

// ------------ method called to produce the data  ------------
void ScoutingPFProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    //get PF jets
    Handle<reco::PFJetCollection> pfJetCollection;
    if(!iEvent.getByToken(pfJetCollection_, pfJetCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: pfJetCollection" << "\n";
        return;
    }
    //get PF candidates
    Handle<reco::PFCandidateCollection> pfCandidateCollection;
    if(!iEvent.getByToken(pfCandidateCollection_, pfCandidateCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: pfCandidateCollection" << "\n";
        return;
    }
    //get vertices
    Handle<reco::VertexCollection> vertexCollection;
    if(!iEvent.getByToken(vertexCollection_, vertexCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: vertexCollection" << "\n";
        return;
    }

    //produce vertices
    std::auto_ptr<ScoutingVertexCollection> outVertices(new ScoutingVertexCollection());
    for(auto &vtx : *vertexCollection){
        outVertices->push_back(ScoutingVertex());
    }

    //produce PF candidates
    std::auto_ptr<ScoutingPFCandidateCollection> outPFCandidates(new ScoutingPFCandidateCollection());
    for(auto &cand : *pfCandidateCollection){
        outPFCandidates->push_back(ScoutingPFCandidate());
    }
    
    //produce PF jets
    std::auto_ptr<ScoutingPFJetCollection> outPFJets(new ScoutingPFJetCollection());
    for(auto &jet : *pfJetCollection){
        outPFJets->push_back(ScoutingPFJet());
    }

    //put output
    iEvent.put(outVertices, "scoutingVertices");
    iEvent.put(outPFCandidates, "scoutingPFCandidates");
    iEvent.put(outPFJets, "scoutingPFJets");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ScoutingPFProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("pfJetCollection",edm::InputTag("hltAK4PFJets"));
    desc.add<edm::InputTag>("pfCandidateCollection", edm::InputTag("hltParticleFlow"));
    desc.add<edm::InputTag>("vertexCollection", edm::InputTag("hltPixelVertices"));
    descriptions.add("scoutingPFJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingPFProducer);
