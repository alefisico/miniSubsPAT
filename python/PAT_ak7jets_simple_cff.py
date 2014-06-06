######## JETS  AK7 ----------------------------
import FWCore.ParameterSet.Config as cms
from jetSubs.MyJetSubsAnalyzer.PAT_goodPV_cff import *

############################################## NO GROOMING
from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
ak7PFJetsCHS = ak5PFJets.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.7),
)
jetSource = 'ak7PFJetsCHS'

#### Adding corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsAK7CHS = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHS.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK7CHS.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHS.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHS.useRho = True

from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patJetsAK7CHS = patJets.clone()
patJetsAK7CHS.jetSource = jetSource
patJetsAK7CHS.addJetCharge = False
patJetsAK7CHS.embedCaloTowers = False
patJetsAK7CHS.embedPFCandidates = False
patJetsAK7CHS.addAssociatedTracks = False
patJetsAK7CHS.addBTagInfo = False
patJetsAK7CHS.addDiscriminators = False
patJetsAK7CHS.addJetID = False
patJetsAK7CHS.addGenPartonMatch = False
patJetsAK7CHS.embedGenPartonMatch = False
patJetsAK7CHS.addGenJetMatch = False
patJetsAK7CHS.getJetMCFlavour = False
patJetsAK7CHS.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHS'))

#### Adding Nsubjetiness
patJetsAK7CHSwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHS"),
  cone=cms.double(0.7)
)

####################################################### PRUNNING
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
ak7PFJetsCHSpruned = ak5PFJetsPruned.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.7),
  jetAlgorithm = cms.string("AntiKt"),
)
jetSourcePruned = 'ak7PFJetsCHSpruned'

# corrections 
patJetCorrFactorsAK7CHSpruned = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHSpruned.src = jetSourcePruned
patJetCorrFactorsAK7CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHSpruned.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHSpruned.useRho = True

patJetsAK7CHSpruned = patJets.clone()
patJetsAK7CHSpruned.jetSource = jetSourcePruned
patJetsAK7CHSpruned.addJetCharge = False
patJetsAK7CHSpruned.embedCaloTowers = False
patJetsAK7CHSpruned.embedPFCandidates = False
patJetsAK7CHSpruned.addAssociatedTracks = False
patJetsAK7CHSpruned.addBTagInfo = False
patJetsAK7CHSpruned.addDiscriminators = False
patJetsAK7CHSpruned.addJetID = False
patJetsAK7CHSpruned.addGenPartonMatch = False
patJetsAK7CHSpruned.embedGenPartonMatch = False
patJetsAK7CHSpruned.addGenJetMatch = False
patJetsAK7CHSpruned.getJetMCFlavour = False
patJetsAK7CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHSpruned'))

#### Adding Nsubjetiness
patJetsAK7CHSprunedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHSpruned"),
  cone=cms.double(0.7)
)

################################################################ TRIMMING
from RecoJets.JetProducers.ak5PFJetsTrimmed_cfi import ak5PFJetsTrimmed
ak7PFJetsCHStrimmed = ak5PFJetsTrimmed.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.7),
  jetAlgorithm = cms.string("AntiKt"),
)
jetSourceTrimmed = 'ak7PFJetsCHStrimmed'

# corrections 
patJetCorrFactorsAK7CHStrimmed = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHStrimmed.src = jetSourceTrimmed
patJetCorrFactorsAK7CHStrimmed.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHStrimmed.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHStrimmed.useRho = True

### pats
patJetsAK7CHStrimmed = patJets.clone()
patJetsAK7CHStrimmed.jetSource = jetSourceTrimmed
patJetsAK7CHStrimmed.addJetCharge = False
patJetsAK7CHStrimmed.embedCaloTowers = False
patJetsAK7CHStrimmed.embedPFCandidates = False
patJetsAK7CHStrimmed.addAssociatedTracks = False
patJetsAK7CHStrimmed.addBTagInfo = False
patJetsAK7CHStrimmed.addDiscriminators = False
patJetsAK7CHStrimmed.addJetID = False
patJetsAK7CHStrimmed.addGenPartonMatch = False
patJetsAK7CHStrimmed.embedGenPartonMatch = False
patJetsAK7CHStrimmed.addGenJetMatch = False
patJetsAK7CHStrimmed.getJetMCFlavour = False
patJetsAK7CHStrimmed.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHStrimmed'))

#### Adding Nsubjetiness
patJetsAK7CHStrimmedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHStrimmed"),
  cone=cms.double(0.7)
)

############################################################### FILTERING
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsFiltered
ak7PFJetsCHSfilteredN2 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(2),
  rParam = cms.double(0.7),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceFilteredN2 = 'ak7PFJetsCHSfilteredN2'

# corrections 
patJetCorrFactorsAK7CHSfilteredN2 = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHSfilteredN2.src = jetSourceFilteredN2
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK7CHSfilteredN2.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHSfilteredN2.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHSfilteredN2.useRho = True

patJetsAK7CHSfilteredN2 = patJets.clone()
patJetsAK7CHSfilteredN2.jetSource = jetSourceFilteredN2
patJetsAK7CHSfilteredN2.addJetCharge = False
patJetsAK7CHSfilteredN2.embedCaloTowers = False
patJetsAK7CHSfilteredN2.embedPFCandidates = False
patJetsAK7CHSfilteredN2.addAssociatedTracks = False
patJetsAK7CHSfilteredN2.addBTagInfo = False
patJetsAK7CHSfilteredN2.addDiscriminators = False
patJetsAK7CHSfilteredN2.addJetID = False
patJetsAK7CHSfilteredN2.addGenPartonMatch = False
patJetsAK7CHSfilteredN2.embedGenPartonMatch = False
patJetsAK7CHSfilteredN2.addGenJetMatch = False
patJetsAK7CHSfilteredN2.getJetMCFlavour = False
patJetsAK7CHSfilteredN2.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHSfilteredN2'))

#### Adding Nsubjetiness
patJetsAK7CHSfilteredN2withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHSfilteredN2"),
  cone=cms.double(0.7)
)

ak7PFJetsCHSfilteredN3 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(3),
  rParam = cms.double(0.7),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceFilteredN3 = 'ak7PFJetsCHSfilteredN3'

# corrections 
patJetCorrFactorsAK7CHSfilteredN3 = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHSfilteredN3.src = jetSourceFilteredN3
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK7CHSfilteredN3.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHSfilteredN3.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHSfilteredN3.useRho = True

patJetsAK7CHSfilteredN3 = patJets.clone()
patJetsAK7CHSfilteredN3.jetSource = jetSourceFilteredN3
patJetsAK7CHSfilteredN3.addJetCharge = False
patJetsAK7CHSfilteredN3.embedCaloTowers = False
patJetsAK7CHSfilteredN3.embedPFCandidates = False
patJetsAK7CHSfilteredN3.addAssociatedTracks = False
patJetsAK7CHSfilteredN3.addBTagInfo = False
patJetsAK7CHSfilteredN3.addDiscriminators = False
patJetsAK7CHSfilteredN3.addJetID = False
patJetsAK7CHSfilteredN3.addGenPartonMatch = False
patJetsAK7CHSfilteredN3.embedGenPartonMatch = False
patJetsAK7CHSfilteredN3.addGenJetMatch = False
patJetsAK7CHSfilteredN3.getJetMCFlavour = False
patJetsAK7CHSfilteredN3.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHSfilteredN3'))

#### Adding Nsubjetiness
patJetsAK7CHSfilteredN3withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHSfilteredN3"),
  cone=cms.double(0.7)
)


############################################################### MassDropFilter
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsMassDropFiltered
ak7PFJetsCHSmassDropFiltered = ak5PFJetsMassDropFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.7),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceMassDropFiltered = 'ak7PFJetsCHSmassDropFiltered'

# corrections 
patJetCorrFactorsAK7CHSmassDropFiltered = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHSmassDropFiltered.src = jetSourceMassDropFiltered
patJetCorrFactorsAK7CHSmassDropFiltered.levels == ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHSmassDropFiltered.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHSmassDropFiltered.useRho = True

patJetsAK7CHSmassDropFiltered = patJets.clone()
patJetsAK7CHSmassDropFiltered.jetSource = jetSourceMassDropFiltered
patJetsAK7CHSmassDropFiltered.addJetCharge = False
patJetsAK7CHSmassDropFiltered.embedCaloTowers = False
patJetsAK7CHSmassDropFiltered.embedPFCandidates = False
patJetsAK7CHSmassDropFiltered.addAssociatedTracks = False
patJetsAK7CHSmassDropFiltered.addBTagInfo = False
patJetsAK7CHSmassDropFiltered.addDiscriminators = False
patJetsAK7CHSmassDropFiltered.addJetID = False
patJetsAK7CHSmassDropFiltered.addGenPartonMatch = False
patJetsAK7CHSmassDropFiltered.embedGenPartonMatch = False
patJetsAK7CHSmassDropFiltered.addGenJetMatch = False
patJetsAK7CHSmassDropFiltered.getJetMCFlavour = False
patJetsAK7CHSmassDropFiltered.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHSmassDropFiltered'))

#### Adding Nsubjetiness
patJetsAK7CHSmassDropFilteredwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK7CHSmassDropFiltered"),
  cone=cms.double(0.7)
)





# JETS AK7 ----------------------------

ak7Jets = cms.Sequence(
  goodOfflinePrimaryVertices +
  pfNoPileUpSequence +
  ak7PFJetsCHS + 
  patJetCorrFactorsAK7CHS +
  patJetsAK7CHS +  
  patJetsAK7CHSwithNsub +
  ak7PFJetsCHSpruned +
  patJetCorrFactorsAK7CHSpruned +
  patJetsAK7CHSpruned +
  patJetsAK7CHSprunedwithNsub +
  ak7PFJetsCHStrimmed +
  patJetCorrFactorsAK7CHStrimmed +
  patJetsAK7CHStrimmed +
  patJetsAK7CHStrimmedwithNsub +
  ak7PFJetsCHSfilteredN2 +
  patJetCorrFactorsAK7CHSfilteredN2 +
  patJetsAK7CHSfilteredN2 +
  patJetsAK7CHSfilteredN2withNsub + 
  ak7PFJetsCHSfilteredN3 +
  patJetCorrFactorsAK7CHSfilteredN3 +
  patJetsAK7CHSfilteredN3 +
  patJetsAK7CHSfilteredN3withNsub + 
  ak7PFJetsCHSmassDropFiltered +
  patJetCorrFactorsAK7CHSmassDropFiltered +
  patJetsAK7CHSmassDropFiltered +
  patJetsAK7CHSmassDropFilteredwithNsub
)
