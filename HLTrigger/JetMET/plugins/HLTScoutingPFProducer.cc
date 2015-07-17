// -*- C++ -*-
//
// Package:    HLTrigger/JetMET
// Class:      HLTScoutingPFProducer
// 
/**\class HLTScoutingPFProducer HLTScoutingPFProducer.cc HLTrigger/JetMET/plugins/HLTScoutingPFProducer.cc

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
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingParticle.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"

#include "DataFormats/Math/interface/deltaR.h"

class HLTScoutingPFProducer : public edm::stream::EDProducer<> {
    public:
        explicit HLTScoutingPFProducer(const edm::ParameterSet&);
        ~HLTScoutingPFProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<reco::PFJetCollection> pfJetCollection_;
        edm::EDGetTokenT<reco::JetTagCollection> pfJetTagCollection_;
        edm::EDGetTokenT<reco::PFCandidateCollection> pfCandidateCollection_;
        edm::EDGetTokenT<reco::VertexCollection> vertexCollection_;
        edm::EDGetTokenT<reco::METCollection> metCollection_;
        edm::EDGetTokenT<double> rho_;

        double pfJetPtCut;
        double pfJetEtaCut;
        double pfCandidatePtCut;

        bool doJetTags;
        bool doCandidates;
        bool doMet;
};

//
// constructors and destructor
//
HLTScoutingPFProducer::HLTScoutingPFProducer(const edm::ParameterSet& iConfig):
    pfJetCollection_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("pfJetCollection"))),
    pfJetTagCollection_(consumes<reco::JetTagCollection>(iConfig.getParameter<edm::InputTag>("pfJetTagCollection"))),
    pfCandidateCollection_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCandidateCollection"))),
    vertexCollection_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexCollection"))),
    metCollection_(consumes<reco::METCollection>(iConfig.getParameter<edm::InputTag>("metCollection"))),
    rho_(consumes<double>(iConfig.getParameter<edm::InputTag>("rho"))),
    pfJetPtCut(iConfig.getParameter<double>("pfJetPtCut")),
    pfJetEtaCut(iConfig.getParameter<double>("pfJetEtaCut")),
    pfCandidatePtCut(iConfig.getParameter<double>("pfCandidatePtCut")),
    doJetTags(iConfig.getParameter<bool>("doJetTags")),
    doCandidates(iConfig.getParameter<bool>("doCandidates")),
    doMet(iConfig.getParameter<bool>("doMet"))
{
    //register products
    produces<ScoutingPFJetCollection>("scoutingPFJets");
    produces<ScoutingParticleCollection>("scoutingPFCandidates");
    produces<ScoutingVertexCollection>("scoutingVertices");
    produces<double>("rho");
    produces<double>("pfMetPt");
    produces<double>("pfMetPhi");
}

HLTScoutingPFProducer::~HLTScoutingPFProducer()
{ }

// ------------ method called to produce the data  ------------
void HLTScoutingPFProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    //get PF jets
    Handle<reco::PFJetCollection> pfJetCollection;
    if(!iEvent.getByToken(pfJetCollection_, pfJetCollection)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: pfJetCollection" << "\n";
        return;
    }
    //get PF jet tags
    Handle<reco::JetTagCollection> pfJetTagCollection;
    if(doJetTags && !iEvent.getByToken(pfJetTagCollection_, pfJetTagCollection)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: pfJetTagCollection" << "\n";
        return;
    }
    //get PF candidates
    Handle<reco::PFCandidateCollection> pfCandidateCollection;
    if(doCandidates && !iEvent.getByToken(pfCandidateCollection_, pfCandidateCollection)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: pfCandidateCollection" << "\n";
        return;
    }
    //get vertices
    Handle<reco::VertexCollection> vertexCollection;
    if(!iEvent.getByToken(vertexCollection_, vertexCollection)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: vertexCollection" << "\n";
        return;
    }
    //get rho
    Handle<double> rho;
    if(!iEvent.getByToken(rho_, rho)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: rho" << "\n";
        return;
    }
    std::auto_ptr<double> outRho(new double(*rho));
    //get MET
    Handle<reco::METCollection> metCollection;
    if(doMet && !iEvent.getByToken(metCollection_, metCollection)){
        edm::LogError ("HLTScoutingPFProducer") << "invalid collection: metCollection" << "\n";
        return;
    }

    //produce vertices
    std::auto_ptr<ScoutingVertexCollection> outVertices(new ScoutingVertexCollection());
    for(auto &vtx : *vertexCollection){
        outVertices->emplace_back(
                vtx.x(), vtx.y(), vtx.z(), vtx.zError()
                );
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

    double metPt = -999;
    double metPhi = -999;
    if(doMet){
        metPt = metCollection->front().pt();
        metPhi = metCollection->front().phi();
    }
    std::auto_ptr<double> outMetPt(new double(metPt));
    std::auto_ptr<double> outMetPhi(new double(metPhi));


    //put output
    iEvent.put(outVertices, "scoutingVertices");
    iEvent.put(outPFCandidates, "scoutingPFCandidates");
    iEvent.put(outPFJets, "scoutingPFJets");
    iEvent.put(outRho, "rho");
    iEvent.put(outMetPt, "pfMetPt");
    iEvent.put(outMetPhi, "pfMetPhi");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void HLTScoutingPFProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("pfJetCollection",edm::InputTag("hltAK4PFJets"));
    desc.add<edm::InputTag>("pfJetTagCollection",edm::InputTag("hltCombinedSecondaryVertexBJetTagsPF"));
    desc.add<edm::InputTag>("pfCandidateCollection", edm::InputTag("hltParticleFlow"));
    desc.add<edm::InputTag>("vertexCollection", edm::InputTag("hltPixelVertices"));
    desc.add<edm::InputTag>("metCollection", edm::InputTag("hltPFMETProducer"));
    desc.add<edm::InputTag>("rho", edm::InputTag("hltFixedGridRhoFastjetAll"));
    desc.add<double>("pfJetPtCut", 20.0);
    desc.add<double>("pfJetEtaCut", 3.0);
    desc.add<double>("pfCandidatePtCut", 0.6);
    desc.add<bool>("doJetTags", true);
    desc.add<bool>("doCandidates", true);
    desc.add<bool>("doMet", true);
    descriptions.add("scoutingPFJetsProducer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTScoutingPFProducer);
