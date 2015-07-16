import FWCore.ParameterSet.Config as cms

process = cms.Process("SCOUTING")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:outputPFScouting.root'
    )
)

process.scoutingCaloProducer = cms.EDProducer('ScoutingCaloProducer',
    caloJetCollection = cms.InputTag('hltAK4CaloJetsCorrectedIDPassed'),
    vertexCollection = cms.InputTag('hltPixelVertices'),
    metCollection = cms.InputTag('hltMetCleanUsingJetID'),
    rho = cms.InputTag('hltFixedGridRhoFastjetAllCalo'),
    caloJetPtCut = cms.double(20.0),
    caloJetEtaCut = cms.double(3.0)
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('caloScoutingPacked.root')
)
  
process.p = cms.Path(process.scoutingCaloProducer)

process.e = cms.EndPath(process.out)
