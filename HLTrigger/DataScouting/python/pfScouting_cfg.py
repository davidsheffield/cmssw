import FWCore.ParameterSet.Config as cms

process = cms.Process("SCOUTING")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:outputScoutingParticleFlowDST.root'
    )
)

process.scoutingPFProducer = cms.EDProducer('ScoutingPFProducer',
    pfJetCollection = cms.InputTag('hltAK4PFJetsCorrectedSelected'),
    pfCandidateCollection = cms.InputTag('hltPFCandidatesSelected'),
    vertexCollection = cms.InputTag('hltPixelVertices')
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('pfScoutingPacked.root')
)
  
process.p = cms.Path(process.scoutingPFProducer)

process.e = cms.EndPath(process.out)
