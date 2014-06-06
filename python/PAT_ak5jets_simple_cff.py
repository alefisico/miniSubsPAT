######## JETS  AK5 ----------------------------
import FWCore.ParameterSet.Config as cms
from jetSubs.MyJetSubsAnalyzer.PAT_goodPV_cff import *

############################################## NO GROOMING
from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
ak5PFJetsCHS = ak5PFJets.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.5),
)
jetSource = 'ak5PFJetsCHS'

#### Adding corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsAK5CHS = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHS.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK5CHS.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHS.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHS.useRho = True

from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patJetsAK5CHS = patJets.clone()
patJetsAK5CHS.jetSource = jetSource
patJetsAK5CHS.addJetCharge = False
patJetsAK5CHS.embedCaloTowers = False
patJetsAK5CHS.embedPFCandidates = False
patJetsAK5CHS.addAssociatedTracks = False
patJetsAK5CHS.addBTagInfo = False
patJetsAK5CHS.addDiscriminators = False
patJetsAK5CHS.addJetID = False
patJetsAK5CHS.addGenPartonMatch = False
patJetsAK5CHS.embedGenPartonMatch = False
patJetsAK5CHS.addGenJetMatch = False
patJetsAK5CHS.getJetMCFlavour = False
patJetsAK5CHS.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHS'))

#### Adding Nsubjetiness
patJetsAK5CHSwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHS"),
  cone=cms.double(0.5)
)

####################################################### PRUNNING
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
ak5PFJetsCHSpruned = ak5PFJetsPruned.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.5),
  jetAlgorithm = cms.string("AntiKt"),
)
jetSourcePruned = 'ak5PFJetsCHSpruned'

# corrections 
patJetCorrFactorsAK5CHSpruned = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHSpruned.src = jetSourcePruned
patJetCorrFactorsAK5CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHSpruned.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHSpruned.useRho = True

patJetsAK5CHSpruned = patJets.clone()
patJetsAK5CHSpruned.jetSource = jetSourcePruned
patJetsAK5CHSpruned.addJetCharge = False
patJetsAK5CHSpruned.embedCaloTowers = False
patJetsAK5CHSpruned.embedPFCandidates = False
patJetsAK5CHSpruned.addAssociatedTracks = False
patJetsAK5CHSpruned.addBTagInfo = False
patJetsAK5CHSpruned.addDiscriminators = False
patJetsAK5CHSpruned.addJetID = False
patJetsAK5CHSpruned.addGenPartonMatch = False
patJetsAK5CHSpruned.embedGenPartonMatch = False
patJetsAK5CHSpruned.addGenJetMatch = False
patJetsAK5CHSpruned.getJetMCFlavour = False
patJetsAK5CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHSpruned'))

#### Adding Nsubjetiness
patJetsAK5CHSprunedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHSpruned"),
  cone=cms.double(0.5)
)

################################################################ TRIMMING
from RecoJets.JetProducers.ak5PFJetsTrimmed_cfi import ak5PFJetsTrimmed
ak5PFJetsCHStrimmed = ak5PFJetsTrimmed.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.5),
  jetAlgorithm = cms.string("AntiKt"),
)
jetSourceTrimmed = 'ak5PFJetsCHStrimmed'

# corrections 
patJetCorrFactorsAK5CHStrimmed = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHStrimmed.src = jetSourceTrimmed
patJetCorrFactorsAK5CHStrimmed.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHStrimmed.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHStrimmed.useRho = True

### pats
patJetsAK5CHStrimmed = patJets.clone()
patJetsAK5CHStrimmed.jetSource = jetSourceTrimmed
patJetsAK5CHStrimmed.addJetCharge = False
patJetsAK5CHStrimmed.embedCaloTowers = False
patJetsAK5CHStrimmed.embedPFCandidates = False
patJetsAK5CHStrimmed.addAssociatedTracks = False
patJetsAK5CHStrimmed.addBTagInfo = False
patJetsAK5CHStrimmed.addDiscriminators = False
patJetsAK5CHStrimmed.addJetID = False
patJetsAK5CHStrimmed.addGenPartonMatch = False
patJetsAK5CHStrimmed.embedGenPartonMatch = False
patJetsAK5CHStrimmed.addGenJetMatch = False
patJetsAK5CHStrimmed.getJetMCFlavour = False
patJetsAK5CHStrimmed.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHStrimmed'))

#### Adding Nsubjetiness
patJetsAK5CHStrimmedwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHStrimmed"),
  cone=cms.double(0.5)
)

############################################################### FILTERING
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsFiltered
ak5PFJetsCHSfilteredN2 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(2),
  rParam = cms.double(0.5),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceFilteredN2 = 'ak5PFJetsCHSfilteredN2'

# corrections 
patJetCorrFactorsAK5CHSfilteredN2 = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHSfilteredN2.src = jetSourceFilteredN2
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK5CHSfilteredN2.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHSfilteredN2.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHSfilteredN2.useRho = True

patJetsAK5CHSfilteredN2 = patJets.clone()
patJetsAK5CHSfilteredN2.jetSource = jetSourceFilteredN2
patJetsAK5CHSfilteredN2.addJetCharge = False
patJetsAK5CHSfilteredN2.embedCaloTowers = False
patJetsAK5CHSfilteredN2.embedPFCandidates = False
patJetsAK5CHSfilteredN2.addAssociatedTracks = False
patJetsAK5CHSfilteredN2.addBTagInfo = False
patJetsAK5CHSfilteredN2.addDiscriminators = False
patJetsAK5CHSfilteredN2.addJetID = False
patJetsAK5CHSfilteredN2.addGenPartonMatch = False
patJetsAK5CHSfilteredN2.embedGenPartonMatch = False
patJetsAK5CHSfilteredN2.addGenJetMatch = False
patJetsAK5CHSfilteredN2.getJetMCFlavour = False
patJetsAK5CHSfilteredN2.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHSfilteredN2'))

#### Adding Nsubjetiness
patJetsAK5CHSfilteredN2withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHSfilteredN2"),
  cone=cms.double(0.5)
)

ak5PFJetsCHSfilteredN3 = ak5PFJetsFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  nFilt = cms.int32(3),
  rParam = cms.double(0.5),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceFilteredN3 = 'ak5PFJetsCHSfilteredN3'

# corrections 
patJetCorrFactorsAK5CHSfilteredN3 = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHSfilteredN3.src = jetSourceFilteredN3
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK5CHSfilteredN3.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHSfilteredN3.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHSfilteredN3.useRho = True

patJetsAK5CHSfilteredN3 = patJets.clone()
patJetsAK5CHSfilteredN3.jetSource = jetSourceFilteredN3
patJetsAK5CHSfilteredN3.addJetCharge = False
patJetsAK5CHSfilteredN3.embedCaloTowers = False
patJetsAK5CHSfilteredN3.embedPFCandidates = False
patJetsAK5CHSfilteredN3.addAssociatedTracks = False
patJetsAK5CHSfilteredN3.addBTagInfo = False
patJetsAK5CHSfilteredN3.addDiscriminators = False
patJetsAK5CHSfilteredN3.addJetID = False
patJetsAK5CHSfilteredN3.addGenPartonMatch = False
patJetsAK5CHSfilteredN3.embedGenPartonMatch = False
patJetsAK5CHSfilteredN3.addGenJetMatch = False
patJetsAK5CHSfilteredN3.getJetMCFlavour = False
patJetsAK5CHSfilteredN3.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHSfilteredN3'))

#### Adding Nsubjetiness
patJetsAK5CHSfilteredN3withNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHSfilteredN3"),
  cone=cms.double(0.5)
)


############################################################### MassDropFilter
from RecoJets.JetProducers.ak5PFJetsFiltered_cfi import ak5PFJetsMassDropFiltered
ak5PFJetsCHSmassDropFiltered = ak5PFJetsMassDropFiltered.clone(
  src = 'pfNoPileUp',
  jetPtMin = cms.double(20.0),
  doAreaFastjet = cms.bool(True),
  rParam = cms.double(0.5),
  jetAlgorithm = cms.string("AntiKt"),
)

jetSourceMassDropFiltered = 'ak5PFJetsCHSmassDropFiltered'

# corrections 
patJetCorrFactorsAK5CHSmassDropFiltered = patJetCorrFactors.clone()
patJetCorrFactorsAK5CHSmassDropFiltered.src = jetSourceMassDropFiltered
patJetCorrFactorsAK5CHSmassDropFiltered.levels == ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK5CHSmassDropFiltered.payload = 'AK5PFchs'
patJetCorrFactorsAK5CHSmassDropFiltered.useRho = True

patJetsAK5CHSmassDropFiltered = patJets.clone()
patJetsAK5CHSmassDropFiltered.jetSource = jetSourceMassDropFiltered
patJetsAK5CHSmassDropFiltered.addJetCharge = False
patJetsAK5CHSmassDropFiltered.embedCaloTowers = False
patJetsAK5CHSmassDropFiltered.embedPFCandidates = False
patJetsAK5CHSmassDropFiltered.addAssociatedTracks = False
patJetsAK5CHSmassDropFiltered.addBTagInfo = False
patJetsAK5CHSmassDropFiltered.addDiscriminators = False
patJetsAK5CHSmassDropFiltered.addJetID = False
patJetsAK5CHSmassDropFiltered.addGenPartonMatch = False
patJetsAK5CHSmassDropFiltered.embedGenPartonMatch = False
patJetsAK5CHSmassDropFiltered.addGenJetMatch = False
patJetsAK5CHSmassDropFiltered.getJetMCFlavour = False
patJetsAK5CHSmassDropFiltered.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK5CHSmassDropFiltered'))

#### Adding Nsubjetiness
patJetsAK5CHSmassDropFilteredwithNsub = cms.EDProducer("NjettinessAdder",
  src=cms.InputTag("patJetsAK5CHSmassDropFiltered"),
  cone=cms.double(0.5)
)





# JETS AK5 ----------------------------

ak5Jets = cms.Sequence(
  goodOfflinePrimaryVertices +
  pfNoPileUpSequence +
  ak5PFJetsCHS + 
  patJetCorrFactorsAK5CHS +
  patJetsAK5CHS +  
  patJetsAK5CHSwithNsub +
  ak5PFJetsCHSpruned +
  patJetCorrFactorsAK5CHSpruned +
  patJetsAK5CHSpruned +
  patJetsAK5CHSprunedwithNsub +
  ak5PFJetsCHStrimmed +
  patJetCorrFactorsAK5CHStrimmed +
  patJetsAK5CHStrimmed +
  patJetsAK5CHStrimmedwithNsub +
  ak5PFJetsCHSfilteredN2 +
  patJetCorrFactorsAK5CHSfilteredN2 +
  patJetsAK5CHSfilteredN2 +
  patJetsAK5CHSfilteredN2withNsub + 
  ak5PFJetsCHSfilteredN3 +
  patJetCorrFactorsAK5CHSfilteredN3 +
  patJetsAK5CHSfilteredN3 +
  patJetsAK5CHSfilteredN3withNsub + 
  ak5PFJetsCHSmassDropFiltered +
  patJetCorrFactorsAK5CHSmassDropFiltered +
  patJetsAK5CHSmassDropFiltered +
  patJetsAK5CHSmassDropFilteredwithNsub
)
