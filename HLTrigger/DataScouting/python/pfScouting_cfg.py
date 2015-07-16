import FWCore.ParameterSet.Config as cms

process = cms.Process("SCOUTING")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:outputPFScouting.root'
    )
)

process.scoutingPFProducer = cms.EDProducer('ScoutingPFProducer',
    pfJetCollection = cms.InputTag('hltAK4PFJetsCorrected'),
    pfJetTagCollection = cms.InputTag('hltCombinedSecondaryVertexBJetTagsPF'),
    pfCandidateCollection = cms.InputTag('hltParticleFlow'),
    vertexCollection = cms.InputTag('hltPixelVertices'),
    metCollection = cms.InputTag('hltPFMETProducer'),
    rho = cms.InputTag('hltFixedGridRhoFastjetAll'),

    pfJetPtCut = cms.double(20),
    pfJetEtaCut = cms.double(3.0),
    pfCandidatePtCut = cms.double(0.6)
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('pfScoutingPacked.root')
)
  
process.p = cms.Path(process.scoutingPFProducer)

process.e = cms.EndPath(process.out)
