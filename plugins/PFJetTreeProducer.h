#ifndef PFJetTreeProducer_h
#define PFJetTreeProducer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionData.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionEvaluator.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionParser.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "TTree.h"
#include "TH1F.h"

class PFJetTreeProducer : public edm::EDAnalyzer {

	public:
		typedef reco::Particle::LorentzVector LorentzVector;
		explicit PFJetTreeProducer(edm::ParameterSet const& cfg);
		virtual void beginJob();
		virtual void analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup);
		virtual void endJob();
		virtual ~PFJetTreeProducer();

	private:  
		void initialize();
		//---- configurable parameters --------   
		double htMin_,ptMin_,dEtaMax_,etaMax_,stopMass_;
		edm::InputTag srcJets_, srcJetsPruned_, srcJetsTrimmed_, srcJetsFilteredN2_, srcJetsFilteredN3_, srcJetsMassDropFiltered_, srcMET_,srcPU_,srcVrtx_, genSrc_;
		edm::Service<TFileService> fs_;
		TTree *outTree_; 
		//---- TRIGGER -------------------------
		triggerExpression::Data triggerCache_;
		std::vector<triggerExpression::Evaluator*> vtriggerSelector_;
		std::vector<std::string> vtriggerAlias_,vtriggerSelection_;
		TH1F *triggerPassHisto_,*triggerNamesHisto_,*puHisto_;
		//---- output TREE variables ------
		//---- global event variables -----
		int   run_,evt_,nVtx_,lumi_,nJets_,nJetsPruned_,nJetsTrimmed_,nJetsFilteredN2_,nJetsFilteredN3_,nJetsMassDropFiltered_;
		float rho_,met_,metSig_,ht_,mjj_,dEtajj_,dPhijj_ ;
		double chf, nhf, phf, elf, muf;
		int chm, npr;
		std::vector<bool> *triggerResult_;
		std::vector<bool> *jetIDResult_;
		//---- jet variables --------------
		std::vector<float> *pt_,*jec_,*eta_,*phi_,*mass_,*dR_,*tau1_,*tau2_,*tau3_,*energy_,*chf_,*nhf_,*phf_,*elf_,*muf_,*area_,*numJetConst_;
		std::vector<float> *ptPruned_,*jecPruned_,*etaPruned_,*phiPruned_,*massPruned_,*tau1Pruned_,*tau2Pruned_,*tau3Pruned_,*energyPruned_,*areaPruned_,*numJetConstPruned_;
		std::vector<float> *ptTrimmed_,*jecTrimmed_,*etaTrimmed_,*phiTrimmed_,*massTrimmed_,*tau1Trimmed_,*tau2Trimmed_,*tau3Trimmed_,*energyTrimmed_,*areaTrimmed_,*numJetConstTrimmed_;
		std::vector<float> *ptFilteredN2_,*jecFilteredN2_,*etaFilteredN2_,*phiFilteredN2_,*massFilteredN2_,*tau1FilteredN2_,*tau2FilteredN2_,*tau3FilteredN2_,*energyFilteredN2_,*areaFilteredN2_,*numJetConstFilteredN2_;
		std::vector<float> *ptFilteredN3_,*jecFilteredN3_,*etaFilteredN3_,*phiFilteredN3_,*massFilteredN3_,*tau1FilteredN3_,*tau2FilteredN3_,*tau3FilteredN3_,*energyFilteredN3_,*areaFilteredN3_,*numJetConstFilteredN3_;
		std::vector<float> *ptMassDropFiltered_,*jecMassDropFiltered_,*etaMassDropFiltered_,*phiMassDropFiltered_,*massMassDropFiltered_,*tau1MassDropFiltered_,*tau2MassDropFiltered_,*tau3MassDropFiltered_,*energyMassDropFiltered_,*areaMassDropFiltered_,*numJetConstMassDropFiltered_;
		//---- MC variables ---------------
		int npu_;
		int numStops, numPartonsStopA, numPartonsStopB;
		std::vector<float> *stopsPt_, *stopsEta_, *stopsMass_, *stopsPhi_, *stopAPt_, *stopAEta_, *stopAEnergy_, *stopAPhi_, *stopAPartonID_, *stopBPt_, *stopBEta_, *stopBEnergy_, *stopBPhi_, *stopBPartonID_, *partonPt_, *partonEta_, *partonEnergy_, *partonPhi_, *partonID_;
};

/*typedef struct {
	TLorentzVector TL;
	double tau1;
	double tau2;
	double tau3;
	bool Btagged;
} jetInfo;*/


#endif
