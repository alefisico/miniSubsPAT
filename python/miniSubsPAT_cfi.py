import FWCore.ParameterSet.Config as cms

myfilelist = cms.untracked.vstring()
myfilelist.extend(          [
                             'file:/eos/uscms/store/user/algomez/RPVSt100tojj_8TeV_HT500/RPVSt100tojj_8TeV_HT500_8/aodsim/RPVSt100tojj_8TeV_HT500_8_PU_532_204_aodsim.root',
			     ]
                                                             )
source = cms.Source ("PoolSource", fileNames=myfilelist)

