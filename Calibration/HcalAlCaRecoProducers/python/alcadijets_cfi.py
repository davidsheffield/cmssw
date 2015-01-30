import FWCore.ParameterSet.Config as cms

# producer for alcadijets (HCAL dijets)
DiJetsProd = cms.EDProducer("AlCaDiJetsProducer",
                              PFjetInput = cms.InputTag("ak4PFJetsCHS"),
                              HBHEInput = cms.InputTag("hbhereco"),
                              HFInput = cms.InputTag("hfreco"),
                              HOInput = cms.InputTag("horeco"),
                              #TriggerResults = cms.InputTag("TriggerResults::HLT"),
                              particleFlowInput = cms.InputTag("particleFlow"),
                              VertexInput = cms.InputTag("offlinePrimaryVertices"),
                              MinPtJet = cms.double(10.0)
                              )


