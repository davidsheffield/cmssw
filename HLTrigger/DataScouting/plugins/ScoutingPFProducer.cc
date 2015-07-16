// -*- C++ -*-
//
// Package:    HLTrigger/DataScouting
// Class:      ScoutingPFProducer
// 
/**\class ScoutingPFProducer ScoutingPFProducer.cc HLTrigger/DataScouting/plugins/ScoutingPFProducer.cc

Description: Producer for ScoutingPFJets from reco::PFJet objects, ScoutingVertexs from reco::Vertexs and ScoutingParticles from reco::PFCandidates

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
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingParticle.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"

#include "DataFormats/Math/interface/deltaR.h"

class ScoutingPFProducer : public edm::EDProducer {
    public:
        explicit ScoutingPFProducer(const edm::ParameterSet&);
        ~ScoutingPFProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::PFJetCollection> pfJetCollection_;
        edm::EDGetTokenT<reco::JetTagCollection> pfJetTagCollection_;
        edm::EDGetTokenT<reco::PFCandidateCollection> pfCandidateCollection_;
        edm::EDGetTokenT<reco::VertexCollection> vertexCollection_;

        double pfJetPtCut;
        double pfJetEtaCut;
        double pfCandidatePtCut;

        bool doJetTags;
        bool doCandidates;
        bool doVertices;
};

//
// constructors and destructor
//
ScoutingPFProducer::ScoutingPFProducer(const edm::ParameterSet& iConfig):
    pfJetCollection_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("pfJetCollection"))),
    pfJetTagCollection_(consumes<reco::JetTagCollection>(iConfig.getParameter<edm::InputTag>("pfJetTagCollection"))),
    pfCandidateCollection_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCandidateCollection"))),
    vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexCollection"))),
    pfJetPtCut(iConfig.getParameter<double>("pfJetPtCut")),
    pfJetEtaCut(iConfig.getParameter<double>("pfJetEtaCut")),
    pfCandidatePtCut(iConfig.getParameter<double>("pfCandidatePtCut")),
    doJetTags(iConfig.getParameter<bool>("doJetTags")),
    doCandidates(iConfig.getParameter<bool>("doCandidates")),
    doVertices(iConfig.getParameter<bool>("doVertices"))
{
    //register products
    produces<ScoutingPFJetCollection>("scoutingPFJets");
    produces<ScoutingParticleCollection>("scoutingPFCandidates");
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
    //get PF jet tags
    Handle<reco::JetTagCollection> pfJetTagCollection;
    if(doJetTags && !iEvent.getByToken(pfJetTagCollection_, pfJetTagCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: pfJetTagCollection" << "\n";
        return;
    }
    //get PF candidates
    Handle<reco::PFCandidateCollection> pfCandidateCollection;
    if(doCandidates && !iEvent.getByToken(pfCandidateCollection_, pfCandidateCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: pfCandidateCollection" << "\n";
        return;
    }
    //get vertices
    Handle<reco::VertexCollection> vertexCollection;
    if(doVertices && !iEvent.getByToken(vertexCollection_, vertexCollection)){
        edm::LogError ("ScoutingPFProducer") << "invalid collection: vertexCollection" << "\n";
        return;
    }

    //produce vertices
    std::auto_ptr<ScoutingVertexCollection> outVertices(new ScoutingVertexCollection());
    if(doVertices){
        for(auto &vtx : *vertexCollection){
            outVertices->emplace_back(
                    vtx.x(), vtx.y(), vtx.z(), vtx.zError()
                    );
        }
    }

    //produce PF candidates
    std::auto_ptr<ScoutingParticleCollection> outPFCandidates(new ScoutingParticleCollection());
    if(doCandidates){
        for(auto &cand : *pfCandidateCollection){
            if(cand.pt() > pfCandidatePtCut){
                outPFCandidates->emplace_back(
                        cand.pt(), cand.eta(), cand.phi(), cand.mass(), cand.pdgId()
                        );
            }
        }
    }
    
    //produce PF jets
    std::auto_ptr<ScoutingPFJetCollection> outPFJets(new ScoutingPFJetCollection());
    for(auto &jet : *pfJetCollection){
        if(jet.pt() < pfJetPtCut || fabs(jet.eta()) > pfJetEtaCut) continue;
        //find the jet tag corresponding to the jet
        float tagValue = -20;
        float minDR = 0.1;
        if(doJetTags){
            for(auto &tag : *pfJetTagCollection){
                float dR = reco::deltaR(jet, *(tag.first));
                if(dR < minDR){
                    minDR = dR;
                    tagValue = tag.second;
                }
            }
        }
        //get the PF constituents of the jet
        std::vector<int> candIndices;
        if(doCandidates){
            for(auto &cand : jet.getPFConstituents()){
                if(cand->pt() > pfCandidatePtCut){
                    //search for the candidate in the collection
                    float minDR = 0.01;
                    int matchIndex = -1;
                    int outIndex = 0;
                    for(auto &outCand : *outPFCandidates){
                        float dR = sqrt(pow(cand->eta() - outCand.eta(), 2) + pow(cand->phi() - outCand.phi(), 2));
                        if(dR < minDR){
                            minDR = dR;
                            matchIndex = outIndex;
                        }
                        if(minDR == 0){
                            break;
                        }
                        outIndex++;
                    }
                    candIndices.push_back(matchIndex);
                }
            }
        }
        outPFJets->emplace_back(
                    jet.pt(), jet.eta(), jet.phi(), jet.mass(), jet.jetArea(),
                    jet.chargedHadronEnergy(), jet.neutralHadronEnergy(), jet.photonEnergy(),
                    jet.electronEnergy(), jet.muonEnergy(), jet.HFHadronEnergy(), jet.HFEMEnergy(),
                    jet.chargedHadronMultiplicity(), jet.neutralHadronMultiplicity(), jet.photonMultiplicity(),
                    jet.electronMultiplicity(), jet.muonMultiplicity(), 
                    jet.HFHadronMultiplicity(), jet.HFEMMultiplicity(), 
                    jet.hoEnergy(), tagValue, 0.0, candIndices
                    );
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
    desc.add<edm::InputTag>("pfJetTagCollection",edm::InputTag("hltCombinedSecondaryVertexBJetTagsPF"));
    desc.add<edm::InputTag>("pfCandidateCollection", edm::InputTag("hltParticleFlow"));
    desc.add<edm::InputTag>("vertexCollection", edm::InputTag("hltPixelVertices"));
    desc.add<double>("pfJetPtCut", 20.0);
    desc.add<double>("pfJetEtaCut", 3.0);
    desc.add<double>("pfCandidatePtCut", 0.6);
    desc.add<bool>("doJetTags", true);
    desc.add<bool>("doCandidates", true);
    desc.add<bool>("doVertices", true);
    descriptions.add("scoutingPFJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingPFProducer);
