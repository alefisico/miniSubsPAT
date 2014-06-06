import FWCore.ParameterSet.Config as cms

##--------- good primary vertices ---------------
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
goodOfflinePrimaryVertices = cms.EDFilter("PrimaryVertexObjectFilter",
    src          = cms.InputTag('offlinePrimaryVertices'),
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) )
)

from CommonTools.ParticleFlow.pfNoPileUp_cff import * 
from CommonTools.ParticleFlow.pfParticleSelection_cff import *

pfPileUp.checkClosestZVertex = False
pfPileUp.Vertices = 'goodOfflinePrimaryVertices'
pfPileUp.PFCandidates = 'particleFlow'
pfNoPileUp.bottomCollection = 'particleFlow'

from CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi import goodOfflinePrimaryVertices
pfNoPileUpSequence.insert(0, goodOfflinePrimaryVertices)

pileUpSubtractionSequence = cms.Sequence(
    pfNoPileUpSequence +
    pfParticleSelectionSequence
    )

