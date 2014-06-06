######## JETS  CA8 ----------------------------
import FWCore.ParameterSet.Config as cms
from jetSubs.MyJetSubsAnalyzer.PAT_goodPV_cff import *

############################################## NO GROOMING
from RecoJets.JetProducers.ca4PFJets_cfi import ca4PFJets
ca8PFJetsCHS = ca4PFJets.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.8),
)
jetSource = 'ca8PFJetsCHS'

#### Adding corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsCA8CHS = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHS.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsCA8CHS.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHS.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHS.useRho = True

from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patJetsCA8CHS = patJets.clone()
patJetsCA8CHS.jetSource = jetSource
patJetsCA8CHS.addJetCharge = False
patJetsCA8CHS.embedCaloTowers = False
patJetsCA8CHS.embedPFCandidates = False
patJetsCA8CHS.addAssociatedTracks = False
patJetsCA8CHS.addBTagInfo = False
patJetsCA8CHS.addDiscriminators = False
patJetsCA8CHS.addJetID = False
patJetsCA8CHS.addGenPartonMatch = False
patJetsCA8CHS.embedGenPartonMatch = False
patJetsCA8CHS.addGenJetMatch = False
patJetsCA8CHS.getJetMCFlavour = False
patJetsCA8CHS.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHS'))

#### Adding Nsubjetiness
patJetsCA8CHSwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHS"),
  cone=cms.double(0.8)
)

####################################################### PRUNNING
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
ca8PFJetsCHSpruned = ak5PFJetsPruned.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.8),
  jetAlgorithm = cms.string("CambridgeAachen"),
)
jetSourcePruned = 'ca8PFJetsCHSpruned'

# corrections 
patJetCorrFactorsCA8CHSpruned = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHSpruned.src = jetSourcePruned
patJetCorrFactorsCA8CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHSpruned.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHSpruned.useRho = True

patJetsCA8CHSpruned = patJets.clone()
patJetsCA8CHSpruned.jetSource = jetSourcePruned
patJetsCA8CHSpruned.addJetCharge = False
patJetsCA8CHSpruned.embedCaloTowers = False
patJetsCA8CHSpruned.embedPFCandidates = False
patJetsCA8CHSpruned.addAssociatedTracks = False
patJetsCA8CHSpruned.addBTagInfo = False
patJetsCA8CHSpruned.addDiscriminators = False
patJetsCA8CHSpruned.addJetID = False
patJetsCA8CHSpruned.addGenPartonMatch = False
patJetsCA8CHSpruned.embedGenPartonMatch = False
patJetsCA8CHSpruned.addGenJetMatch = False
patJetsCA8CHSpruned.getJetMCFlavour = False
patJetsCA8CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHSpruned'))

#### Adding Nsubjetiness
patJetsCA8CHSprunedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHSpruned"),
  cone=cms.double(0.8)
)

################################################################ TRIMMING
from RecoJets.JetProducers.ak5PFJetsTrimmed_cfi import ak5PFJetsTrimmed
ca8PFJetsCHStrimmed = ak5PFJetsTrimmed.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.8),
  jetAlgorithm = cms.string("CambridgeAachen"),
)
jetSourceTrimmed = 'ca8PFJetsCHStrimmed'

# corrections 
patJetCorrFactorsCA8CHStrimmed = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHStrimmed.src = jetSourceTrimmed
patJetCorrFactorsCA8CHStrimmed.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHStrimmed.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHStrimmed.useRho = True

### pats
patJetsCA8CHStrimmed = patJets.clone()
patJetsCA8CHStrimmed.jetSource = jetSourceTrimmed
patJetsCA8CHStrimmed.addJetCharge = False
patJetsCA8CHStrimmed.embedCaloTowers = False
patJetsCA8CHStrimmed.embedPFCandidates = False
patJetsCA8CHStrimmed.addAssociatedTracks = False
patJetsCA8CHStrimmed.addBTagInfo = False
patJetsCA8CHStrimmed.addDiscriminators = False
patJetsCA8CHStrimmed.addJetID = False
patJetsCA8CHStrimmed.addGenPartonMatch = False
patJetsCA8CHStrimmed.embedGenPartonMatch = False
patJetsCA8CHStrimmed.addGenJetMatch = False
patJetsCA8CHStrimmed.getJetMCFlavour = False
patJetsCA8CHStrimmed.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHStrimmed'))

#### Adding Nsubjetiness
patJetsCA8CHStrimmedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHStrimmed"),
  cone=cms.double(0.8)
)

############################################################### FILTERING
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsFiltered
ca8PFJetsCHSfilteredN2 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(2),
  rParam = cms.double(0.8),
  jetAlgorithm = cms.string("CambridgeAachen"),
)

jetSourceFilteredN2 = 'ca8PFJetsCHSfilteredN2'

# corrections 
patJetCorrFactorsCA8CHSfilteredN2 = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHSfilteredN2.src = jetSourceFilteredN2
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsCA8CHSfilteredN2.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHSfilteredN2.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHSfilteredN2.useRho = True

patJetsCA8CHSfilteredN2 = patJets.clone()
patJetsCA8CHSfilteredN2.jetSource = jetSourceFilteredN2
patJetsCA8CHSfilteredN2.addJetCharge = False
patJetsCA8CHSfilteredN2.embedCaloTowers = False
patJetsCA8CHSfilteredN2.embedPFCandidates = False
patJetsCA8CHSfilteredN2.addAssociatedTracks = False
patJetsCA8CHSfilteredN2.addBTagInfo = False
patJetsCA8CHSfilteredN2.addDiscriminators = False
patJetsCA8CHSfilteredN2.addJetID = False
patJetsCA8CHSfilteredN2.addGenPartonMatch = False
patJetsCA8CHSfilteredN2.embedGenPartonMatch = False
patJetsCA8CHSfilteredN2.addGenJetMatch = False
patJetsCA8CHSfilteredN2.getJetMCFlavour = False
patJetsCA8CHSfilteredN2.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHSfilteredN2'))

#### Adding Nsubjetiness
patJetsCA8CHSfilteredN2withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHSfilteredN2"),
  cone=cms.double(0.8)
)

ca8PFJetsCHSfilteredN3 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(3),
  rParam = cms.double(0.8),
  jetAlgorithm = cms.string("CambridgeAachen"),
)

jetSourceFilteredN3 = 'ca8PFJetsCHSfilteredN3'

# corrections 
patJetCorrFactorsCA8CHSfilteredN3 = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHSfilteredN3.src = jetSourceFilteredN3
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsCA8CHSfilteredN3.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHSfilteredN3.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHSfilteredN3.useRho = True

patJetsCA8CHSfilteredN3 = patJets.clone()
patJetsCA8CHSfilteredN3.jetSource = jetSourceFilteredN3
patJetsCA8CHSfilteredN3.addJetCharge = False
patJetsCA8CHSfilteredN3.embedCaloTowers = False
patJetsCA8CHSfilteredN3.embedPFCandidates = False
patJetsCA8CHSfilteredN3.addAssociatedTracks = False
patJetsCA8CHSfilteredN3.addBTagInfo = False
patJetsCA8CHSfilteredN3.addDiscriminators = False
patJetsCA8CHSfilteredN3.addJetID = False
patJetsCA8CHSfilteredN3.addGenPartonMatch = False
patJetsCA8CHSfilteredN3.embedGenPartonMatch = False
patJetsCA8CHSfilteredN3.addGenJetMatch = False
patJetsCA8CHSfilteredN3.getJetMCFlavour = False
patJetsCA8CHSfilteredN3.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHSfilteredN3'))

#### Adding Nsubjetiness
patJetsCA8CHSfilteredN3withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHSfilteredN3"),
  cone=cms.double(0.8)
)


############################################################### MassDropFilter
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsMassDropFiltered
ca8PFJetsCHSmassDropFiltered = ak5PFJetsMassDropFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.8),
  jetAlgorithm = cms.string("CambridgeAachen"),
)

jetSourceMassDropFiltered = 'ca8PFJetsCHSmassDropFiltered'

# corrections 
patJetCorrFactorsCA8CHSmassDropFiltered = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHSmassDropFiltered.src = jetSourceMassDropFiltered
patJetCorrFactorsCA8CHSmassDropFiltered.levels == ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHSmassDropFiltered.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHSmassDropFiltered.useRho = True

patJetsCA8CHSmassDropFiltered = patJets.clone()
patJetsCA8CHSmassDropFiltered.jetSource = jetSourceMassDropFiltered
patJetsCA8CHSmassDropFiltered.addJetCharge = False
patJetsCA8CHSmassDropFiltered.embedCaloTowers = False
patJetsCA8CHSmassDropFiltered.embedPFCandidates = False
patJetsCA8CHSmassDropFiltered.addAssociatedTracks = False
patJetsCA8CHSmassDropFiltered.addBTagInfo = False
patJetsCA8CHSmassDropFiltered.addDiscriminators = False
patJetsCA8CHSmassDropFiltered.addJetID = False
patJetsCA8CHSmassDropFiltered.addGenPartonMatch = False
patJetsCA8CHSmassDropFiltered.embedGenPartonMatch = False
patJetsCA8CHSmassDropFiltered.addGenJetMatch = False
patJetsCA8CHSmassDropFiltered.getJetMCFlavour = False
patJetsCA8CHSmassDropFiltered.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHSmassDropFiltered'))

#### Adding Nsubjetiness
patJetsCA8CHSmassDropFilteredwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsCA8CHSmassDropFiltered"),
  cone=cms.double(0.8)
)





# JETS CA8 ----------------------------

ca8Jets = cms.Sequence(
  goodOfflinePrimaryVertices +
  pfNoPileUpSequence +
  ca8PFJetsCHS + 
  patJetCorrFactorsCA8CHS +
  patJetsCA8CHS +  
  patJetsCA8CHSwithNsub +
  ca8PFJetsCHSpruned +
  patJetCorrFactorsCA8CHSpruned +
  patJetsCA8CHSpruned +
  patJetsCA8CHSprunedwithNsub +
  ca8PFJetsCHStrimmed +
  patJetCorrFactorsCA8CHStrimmed +
  patJetsCA8CHStrimmed +
  patJetsCA8CHStrimmedwithNsub +
  ca8PFJetsCHSfilteredN2 +
  patJetCorrFactorsCA8CHSfilteredN2 +
  patJetsCA8CHSfilteredN2 +
  patJetsCA8CHSfilteredN2withNsub + 
  ca8PFJetsCHSfilteredN3 +
  patJetCorrFactorsCA8CHSfilteredN3 +
  patJetsCA8CHSfilteredN3 +
  patJetsCA8CHSfilteredN3withNsub + 
  ca8PFJetsCHSmassDropFiltered +
  patJetCorrFactorsCA8CHSmassDropFiltered +
  patJetsCA8CHSmassDropFiltered +
  patJetsCA8CHSmassDropFilteredwithNsub
)
