#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include <cassert>
#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include "miniPAT/miniSubsPAT/plugins/PFJetTreeProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace reco;

PFJetTreeProducer::PFJetTreeProducer(edm::ParameterSet const& cfg) {
	srcJets_                 = cfg.getParameter<edm::InputTag>             ("jets");
	srcJetsPruned_           = cfg.getParameter<edm::InputTag> 	       ("jetsPruned");
	srcJetsTrimmed_          = cfg.getParameter<edm::InputTag> 	       ("jetsTrimmed");
	srcJetsFilteredN2_         = cfg.getParameter<edm::InputTag> 	       ("jetsFilteredN2");
	srcJetsFilteredN3_         = cfg.getParameter<edm::InputTag> 	       ("jetsFilteredN3");
	srcJetsMassDropFiltered_ = cfg.getParameter<edm::InputTag> 	       ("jetsMassDropFiltered");
	srcMET_                  = cfg.getParameter<edm::InputTag>             ("met");
	srcVrtx_                 = cfg.getParameter<edm::InputTag>             ("vtx");
	srcPU_                   = cfg.getUntrackedParameter<edm::InputTag>    ("pu",edm::InputTag(""));
	//htMin_                   = cfg.getParameter<double>                    ("htMin");
	ptMin_                   = cfg.getParameter<double>                    ("ptMin");
	etaMax_                  = cfg.getParameter<double>                    ("etaMax");
	triggerCache_ 	         = triggerExpression::Data(cfg.getParameterSet("triggerConfiguration"));
	vtriggerAlias_     	 = cfg.getParameter<std::vector<std::string> > ("triggerAlias");
	vtriggerSelection_  	 = cfg.getParameter<std::vector<std::string> > ("triggerSelection");
	genSrc_          	 = cfg.getParameter<edm::InputTag>             ("genSrc");
	stopMass_       	 = cfg.getParameter<double>                    ("stopMass");

	if (vtriggerAlias_.size() != vtriggerSelection_.size()) {
		cout<<"ERROR: the number of trigger aliases does not match the number of trigger names !!!"<<endl;
		return;
	}

	for(unsigned i=0;i<vtriggerSelection_.size();i++) {
		vtriggerSelector_.push_back(triggerExpression::parse(vtriggerSelection_[i]));
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
void PFJetTreeProducer::beginJob() {

	//--- book the trigger histograms ---------
	triggerNamesHisto_ = fs_->make<TH1F>("TriggerNames","TriggerNames",1,0,1);
	triggerNamesHisto_->SetBit(TH1::kCanRebin);
	for(unsigned i=0;i<vtriggerSelection_.size();i++) {
		triggerNamesHisto_->Fill(vtriggerSelection_[i].c_str(),1);
	}
	triggerPassHisto_ = fs_->make<TH1F>("TriggerPass","TriggerPass",1,0,1);
	triggerPassHisto_->SetBit(TH1::kCanRebin);
  
	//--- book the tree -----------------------
	outTree_ = fs_->make<TTree>("events","events");
	outTree_->Branch("runNo"                ,&run_               ,"run_/I");
	outTree_->Branch("evtNo"                ,&evt_               ,"evt_/I");
	outTree_->Branch("lumi"                 ,&lumi_              ,"lumi_/I");
	outTree_->Branch("nvtx"                 ,&nVtx_              ,"nVtx_/I");
	outTree_->Branch("nJets"                ,&nJets_             ,"nJets_/I");
	outTree_->Branch("nJetsPruned"                ,&nJetsPruned_             ,"nJetsPruned_/I");
	outTree_->Branch("nJetsTrimmed"                ,&nJetsTrimmed_             ,"nJetsTrimmed_/I");
	outTree_->Branch("nJetsFilteredN2"                ,&nJetsFilteredN2_             ,"nJetsFilteredN2_/I");
	outTree_->Branch("nJetsFilteredN3"                ,&nJetsFilteredN3_             ,"nJetsFilteredN3_/I");
	outTree_->Branch("nJetsMassDropFiltered"                ,&nJetsMassDropFiltered_             ,"nJetsMassDropFiltered_/I");
	outTree_->Branch("ht"                   ,&ht_                ,"ht_/F");
	outTree_->Branch("met"                  ,&met_               ,"met_/F");
	outTree_->Branch("metSig"               ,&metSig_            ,"metSig_/F");
	outTree_->Branch("numStops",		&numStops,		"numStops/I" );
	outTree_->Branch("numPartonsStopA",		&numPartonsStopA,		"numPartonsStopA/I" );
	outTree_->Branch("numPartonsStopB",		&numPartonsStopB,		"numPartonsStopB/I" );
	//------------------------------------------------------------------
	chf_            = new std::vector<float>;
	nhf_            = new std::vector<float>;
	phf_            = new std::vector<float>;
	muf_            = new std::vector<float>;
	elf_            = new std::vector<float>;
	pt_             = new std::vector<float>;
	jec_            = new std::vector<float>;
	eta_            = new std::vector<float>;
	phi_            = new std::vector<float>;
	mass_           = new std::vector<float>;
	energy_         = new std::vector<float>;
	area_           = new std::vector<float>;
	numJetConst_    = new std::vector<float>;
	tau1_           = new std::vector<float>;
	tau2_           = new std::vector<float>;
	tau3_           = new std::vector<float>;
	ptPruned_             = new std::vector<float>;
	jecPruned_            = new std::vector<float>;
	etaPruned_            = new std::vector<float>;
	phiPruned_            = new std::vector<float>;
	massPruned_           = new std::vector<float>;
	energyPruned_         = new std::vector<float>;
	areaPruned_           = new std::vector<float>;
	numJetConstPruned_    = new std::vector<float>;
	tau1Pruned_           = new std::vector<float>;
	tau2Pruned_           = new std::vector<float>;
	tau3Pruned_           = new std::vector<float>;
	ptTrimmed_             = new std::vector<float>;
	jecTrimmed_            = new std::vector<float>;
	etaTrimmed_            = new std::vector<float>;
	phiTrimmed_            = new std::vector<float>;
	massTrimmed_           = new std::vector<float>;
	energyTrimmed_         = new std::vector<float>;
	areaTrimmed_           = new std::vector<float>;
	numJetConstTrimmed_    = new std::vector<float>;
	tau1Trimmed_           = new std::vector<float>;
	tau2Trimmed_           = new std::vector<float>;
	tau3Trimmed_           = new std::vector<float>;
	ptFilteredN2_             = new std::vector<float>;
	jecFilteredN2_            = new std::vector<float>;
	etaFilteredN2_            = new std::vector<float>;
	phiFilteredN2_            = new std::vector<float>;
	massFilteredN2_           = new std::vector<float>;
	energyFilteredN2_         = new std::vector<float>;
	areaFilteredN2_           = new std::vector<float>;
	numJetConstFilteredN2_    = new std::vector<float>;
	tau1FilteredN2_           = new std::vector<float>;
	tau2FilteredN2_           = new std::vector<float>;
	tau3FilteredN2_           = new std::vector<float>;
	ptFilteredN3_             = new std::vector<float>;
	jecFilteredN3_            = new std::vector<float>;
	etaFilteredN3_            = new std::vector<float>;
	phiFilteredN3_            = new std::vector<float>;
	massFilteredN3_           = new std::vector<float>;
	energyFilteredN3_         = new std::vector<float>;
	areaFilteredN3_           = new std::vector<float>;
	numJetConstFilteredN3_    = new std::vector<float>;
	tau1FilteredN3_           = new std::vector<float>;
	tau2FilteredN3_           = new std::vector<float>;
	tau3FilteredN3_           = new std::vector<float>;
	ptMassDropFiltered_             = new std::vector<float>;
	jecMassDropFiltered_            = new std::vector<float>;
	etaMassDropFiltered_            = new std::vector<float>;
	phiMassDropFiltered_            = new std::vector<float>;
	massMassDropFiltered_           = new std::vector<float>;
	energyMassDropFiltered_         = new std::vector<float>;
	areaMassDropFiltered_           = new std::vector<float>;
	numJetConstMassDropFiltered_    = new std::vector<float>;
	tau1MassDropFiltered_           = new std::vector<float>;
	tau2MassDropFiltered_           = new std::vector<float>;
	tau3MassDropFiltered_           = new std::vector<float>;
	dR_             = new std::vector<float>;
	stopsPt_	= new std::vector<float>;
	stopsEta_	= new std::vector<float>;
	stopsMass_	= new std::vector<float>;
	stopsPhi_	= new std::vector<float>;
	stopAPt_	= new std::vector<float>;
	stopAEta_	= new std::vector<float>;
	stopAEnergy_	= new std::vector<float>;
	stopAPhi_	= new std::vector<float>;
	stopAPartonID_	= new std::vector<float>;
	stopBPt_	= new std::vector<float>;
	stopBEta_	= new std::vector<float>;
	stopBEnergy_	= new std::vector<float>;
	stopBPhi_	= new std::vector<float>;
	stopBPartonID_	= new std::vector<float>;
	partonPt_	= new std::vector<float>;
	partonEta_	= new std::vector<float>;
	partonEnergy_	= new std::vector<float>;
	partonPhi_	= new std::vector<float>;
	partonID_	= new std::vector<float>;

	outTree_->Branch("jetChf"               ,"vector<float>"     ,&chf_);
	outTree_->Branch("jetNhf"               ,"vector<float>"     ,&nhf_);
	outTree_->Branch("jetPhf"               ,"vector<float>"     ,&phf_);
	outTree_->Branch("jetMuf"               ,"vector<float>"     ,&muf_);
	outTree_->Branch("jetElf"               ,"vector<float>"     ,&elf_);   
	outTree_->Branch("jetPt"                ,"vector<float>"     ,&pt_);
	outTree_->Branch("jetJec"               ,"vector<float>"     ,&jec_);
	outTree_->Branch("jetEta"               ,"vector<float>"     ,&eta_);
	outTree_->Branch("jetPhi"               ,"vector<float>"     ,&phi_);
	outTree_->Branch("jetEnergy"            ,"vector<float>"     ,&energy_);
	outTree_->Branch("jetMass"              ,"vector<float>"     ,&mass_);
	outTree_->Branch("jetTau1"              ,"vector<float>"     ,&tau1_);
	outTree_->Branch("jetTau2"              ,"vector<float>"     ,&tau2_);
	outTree_->Branch("jetTau3"              ,"vector<float>"     ,&tau3_);
	outTree_->Branch("jetArea"              ,"vector<float>"     ,&area_);   
	outTree_->Branch("numJetConstituent"    ,"vector<float>"     ,&numJetConst_);   
	outTree_->Branch("jetPrunedPt"                ,"vector<float>"     ,&ptPruned_);
	outTree_->Branch("jetPrunedJec"               ,"vector<float>"     ,&jecPruned_);
	outTree_->Branch("jetPrunedEta"               ,"vector<float>"     ,&etaPruned_);
	outTree_->Branch("jetPrunedPhi"               ,"vector<float>"     ,&phiPruned_);
	outTree_->Branch("jetPrunedEnergy"            ,"vector<float>"     ,&energyPruned_);
	outTree_->Branch("jetPrunedMass"              ,"vector<float>"     ,&massPruned_);
	outTree_->Branch("jetPrunedTau1"              ,"vector<float>"     ,&tau1Pruned_);
	outTree_->Branch("jetPrunedTau2"              ,"vector<float>"     ,&tau2Pruned_);
	outTree_->Branch("jetPrunedTau3"              ,"vector<float>"     ,&tau3Pruned_);
	outTree_->Branch("jetPrunedArea"              ,"vector<float>"     ,&areaPruned_);   
	outTree_->Branch("numJetPrunedConstituent"    ,"vector<float>"     ,&numJetConstPruned_);   
	outTree_->Branch("jetTrimmedPt"                ,"vector<float>"     ,&ptTrimmed_);
	outTree_->Branch("jetTrimmedJec"               ,"vector<float>"     ,&jecTrimmed_);
	outTree_->Branch("jetTrimmedEta"               ,"vector<float>"     ,&etaTrimmed_);
	outTree_->Branch("jetTrimmedPhi"               ,"vector<float>"     ,&phiTrimmed_);
	outTree_->Branch("jetTrimmedEnergy"            ,"vector<float>"     ,&energyTrimmed_);
	outTree_->Branch("jetTrimmedMass"              ,"vector<float>"     ,&massTrimmed_);
	outTree_->Branch("jetTrimmedTau1"              ,"vector<float>"     ,&tau1Trimmed_);
	outTree_->Branch("jetTrimmedTau2"              ,"vector<float>"     ,&tau2Trimmed_);
	outTree_->Branch("jetTrimmedTau3"              ,"vector<float>"     ,&tau3Trimmed_);
	outTree_->Branch("jetTrimmedArea"              ,"vector<float>"     ,&areaTrimmed_);   
	outTree_->Branch("numJetTrimmedConstituent"    ,"vector<float>"     ,&numJetConstTrimmed_);   
	outTree_->Branch("jetFilteredN2Pt"                ,"vector<float>"     ,&ptFilteredN2_);
	outTree_->Branch("jetFilteredN2Jec"               ,"vector<float>"     ,&jecFilteredN2_);
	outTree_->Branch("jetFilteredN2Eta"               ,"vector<float>"     ,&etaFilteredN2_);
	outTree_->Branch("jetFilteredN2Phi"               ,"vector<float>"     ,&phiFilteredN2_);
	outTree_->Branch("jetFilteredN2Energy"            ,"vector<float>"     ,&energyFilteredN2_);
	outTree_->Branch("jetFilteredN2Mass"              ,"vector<float>"     ,&massFilteredN2_);
	outTree_->Branch("jetFilteredN2Tau1"              ,"vector<float>"     ,&tau1FilteredN2_);
	outTree_->Branch("jetFilteredN2Tau2"              ,"vector<float>"     ,&tau2FilteredN2_);
	outTree_->Branch("jetFilteredN2Tau3"              ,"vector<float>"     ,&tau3FilteredN2_);
	outTree_->Branch("jetFilteredN2Area"              ,"vector<float>"     ,&areaFilteredN2_);   
	outTree_->Branch("numJetFilteredN2Constituent"    ,"vector<float>"     ,&numJetConstFilteredN2_);   
	outTree_->Branch("jetFilteredN3Pt"                ,"vector<float>"     ,&ptFilteredN3_);
	outTree_->Branch("jetFilteredN3Jec"               ,"vector<float>"     ,&jecFilteredN3_);
	outTree_->Branch("jetFilteredN3Eta"               ,"vector<float>"     ,&etaFilteredN3_);
	outTree_->Branch("jetFilteredN3Phi"               ,"vector<float>"     ,&phiFilteredN3_);
	outTree_->Branch("jetFilteredN3Energy"            ,"vector<float>"     ,&energyFilteredN3_);
	outTree_->Branch("jetFilteredN3Mass"              ,"vector<float>"     ,&massFilteredN3_);
	outTree_->Branch("jetFilteredN3Tau1"              ,"vector<float>"     ,&tau1FilteredN3_);
	outTree_->Branch("jetFilteredN3Tau2"              ,"vector<float>"     ,&tau2FilteredN3_);
	outTree_->Branch("jetFilteredN3Tau3"              ,"vector<float>"     ,&tau3FilteredN3_);
	outTree_->Branch("jetFilteredN3Area"              ,"vector<float>"     ,&areaFilteredN3_);   
	outTree_->Branch("numJetFilteredN3Constituent"    ,"vector<float>"     ,&numJetConstFilteredN3_);   
	outTree_->Branch("jetMassDropFilteredPt"                ,"vector<float>"     ,&ptMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredJec"               ,"vector<float>"     ,&jecMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredEta"               ,"vector<float>"     ,&etaMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredPhi"               ,"vector<float>"     ,&phiMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredEnergy"            ,"vector<float>"     ,&energyMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredMass"              ,"vector<float>"     ,&massMassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredTau1"              ,"vector<float>"     ,&tau1MassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredTau2"              ,"vector<float>"     ,&tau2MassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredTau3"              ,"vector<float>"     ,&tau3MassDropFiltered_);
	outTree_->Branch("jetMassDropFilteredArea"              ,"vector<float>"     ,&areaMassDropFiltered_);   
	outTree_->Branch("numJetMassDropFilteredConstituent"    ,"vector<float>"     ,&numJetConstMassDropFiltered_);   
	outTree_->Branch("jetDR"                ,"vector<float>"     ,&dR_); 

	//------------------------------------------------------------------
	triggerResult_ = new std::vector<bool>;
	outTree_->Branch("triggerResult","vector<bool>",&triggerResult_);
	//------------------- MC ---------------------------------
	outTree_->Branch("npu"                  ,&npu_               ,"npu_/I");
	outTree_->Branch("stopsPt",	"vector<float>", 	&stopsPt_ );
	outTree_->Branch("stopsEta",	"vector<float>", 	&stopsEta_ );
	outTree_->Branch("stopsMass",	"vector<float>", 	&stopsMass_ );
	outTree_->Branch("stopsPhi",	"vector<float>", 	&stopsPhi_ );
	outTree_->Branch("stopAPt",	"vector<float>", 	&stopAPt_ );
	outTree_->Branch("stopAEta",	"vector<float>", 	&stopAEta_ );
	outTree_->Branch("stopAEnergy",	"vector<float>", 	&stopAEnergy_ );
	outTree_->Branch("stopAPhi",	"vector<float>", 	&stopAPhi_ );
	outTree_->Branch("stopAPartonID",	"vector<float>", 	&stopAPartonID_ );
	outTree_->Branch("stopBPt",	"vector<float>", 	&stopBPt_ );
	outTree_->Branch("stopBEta",	"vector<float>", 	&stopBEta_ );
	outTree_->Branch("stopBEnergy",	"vector<float>", 	&stopBEnergy_ );
	outTree_->Branch("stopBPhi",	"vector<float>", 	&stopBPhi_ );
	outTree_->Branch("stopBPartonID",	"vector<float>", 	&stopBPartonID_ );
	outTree_->Branch("partonPt",	"vector<float>", 	&partonPt_ );
	outTree_->Branch("partonEta",	"vector<float>", 	&partonEta_ );
	outTree_->Branch("partonEnergy",	"vector<float>",&partonEnergy_ );
	outTree_->Branch("partonPhi",	"vector<float>", 	&partonPhi_ );
	outTree_->Branch("partonID",	"vector<float>",&partonID_ );
}


//////////////////////////////////////////////////////////////////////////////////////////
void PFJetTreeProducer::endJob() {  
	delete triggerResult_;
	delete chf_;
	delete nhf_;
	delete phf_;
	delete muf_;
	delete elf_;
	delete pt_;
	delete jec_;
	delete eta_;
	delete phi_;
	delete mass_;
	delete tau1_;
	delete tau2_;
	delete tau3_;
	delete energy_;
	delete area_;
	delete numJetConst_;
	delete ptPruned_;
	delete jecPruned_;
	delete etaPruned_;
	delete phiPruned_;
	delete massPruned_;
	delete tau1Pruned_;
	delete tau2Pruned_;
	delete tau3Pruned_;
	delete energyPruned_;
	delete areaPruned_;
	delete numJetConstPruned_;
	delete ptTrimmed_;
	delete jecTrimmed_;
	delete etaTrimmed_;
	delete phiTrimmed_;
	delete massTrimmed_;
	delete tau1Trimmed_;
	delete tau2Trimmed_;
	delete tau3Trimmed_;
	delete energyTrimmed_;
	delete areaTrimmed_;
	delete numJetConstTrimmed_;
	delete ptFilteredN2_;
	delete jecFilteredN2_;
	delete etaFilteredN2_;
	delete phiFilteredN2_;
	delete massFilteredN2_;
	delete tau1FilteredN2_;
	delete tau2FilteredN2_;
	delete tau3FilteredN2_;
	delete energyFilteredN2_;
	delete areaFilteredN2_;
	delete numJetConstFilteredN2_;
	delete ptFilteredN3_;
	delete jecFilteredN3_;
	delete etaFilteredN3_;
	delete phiFilteredN3_;
	delete massFilteredN3_;
	delete tau1FilteredN3_;
	delete tau2FilteredN3_;
	delete tau3FilteredN3_;
	delete energyFilteredN3_;
	delete areaFilteredN3_;
	delete numJetConstFilteredN3_;
	delete ptMassDropFiltered_;
	delete jecMassDropFiltered_;
	delete etaMassDropFiltered_;
	delete phiMassDropFiltered_;
	delete massMassDropFiltered_;
	delete tau1MassDropFiltered_;
	delete tau2MassDropFiltered_;
	delete tau3MassDropFiltered_;
	delete energyMassDropFiltered_;
	delete areaMassDropFiltered_;
	delete numJetConstMassDropFiltered_;
	delete dR_;
	delete stopsPt_;
	delete stopsEta_;
	delete stopsMass_;
	delete stopsPhi_;
	delete stopAPt_;
	delete stopAEta_;
	delete stopAEnergy_;
	delete stopAPhi_;
	delete stopAPartonID_;
	delete stopBPt_;
	delete stopBEta_;
	delete stopBEnergy_;
	delete stopBPhi_;
	delete stopBPartonID_;
	delete partonPt_;
	delete partonEta_;
	delete partonEnergy_;
	delete partonPhi_;
	delete partonID_;

	for(unsigned i=0;i<vtriggerSelector_.size();i++) {
		delete vtriggerSelector_[i];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void PFJetTreeProducer::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {

	initialize();

	edm::Handle<edm::View<pat::Jet> > jets;
	iEvent.getByLabel(srcJets_,jets);
	edm::View<pat::Jet> pat_jets = *jets;

	edm::Handle<edm::View<pat::Jet> > jetsPruned;
	iEvent.getByLabel(srcJetsPruned_,jetsPruned);
	edm::View<pat::Jet> pat_jets_pruned = *jetsPruned;

	edm::Handle<edm::View<pat::Jet> > jetsTrimmed;
	iEvent.getByLabel(srcJetsTrimmed_,jetsTrimmed);
	edm::View<pat::Jet> pat_jets_trimmed = *jetsTrimmed;

	edm::Handle<edm::View<pat::Jet> > jetsFilteredN2;
	iEvent.getByLabel(srcJetsFilteredN2_,jetsFilteredN2);
	edm::View<pat::Jet> pat_jets_filteredN2 = *jetsFilteredN2;

	edm::Handle<edm::View<pat::Jet> > jetsFilteredN3;
	iEvent.getByLabel(srcJetsFilteredN3_,jetsFilteredN3);
	edm::View<pat::Jet> pat_jets_filteredN3 = *jetsFilteredN3;

	edm::Handle<edm::View<pat::Jet> > jetsMassDropFiltered;
	iEvent.getByLabel(srcJetsMassDropFiltered_,jetsMassDropFiltered);
	edm::View<pat::Jet> pat_jets_massDropFiltered = *jetsMassDropFiltered;

	edm::Handle<edm::View<MET> >  met;
	iEvent.getByLabel(srcMET_,met);

	edm::Handle<reco::VertexCollection> recVtxs;
	iEvent.getByLabel(srcVrtx_,recVtxs);

	//---------- GenParticles -------------
	if (!iEvent.isRealData()) {
		// Way to call GenParticle from root
		edm::Handle<std::vector<reco::GenParticle>> particles;
		iEvent.getByLabel( genSrc_ , particles );
		const std::vector<reco::GenParticle> & p = *particles; 

		//cout<< iEvent.id().event() << endl;
		//check pdgId of stop
		int stopId = 1000002;
		//int stopId;
		//if ( stopMass_ == 50 ){ stopId = 1000002; }
		//else{ stopId = 1000006; }

		// Begin Loop for GenParticles
		numStops = 0;
		numPartonsStopA = 0;
		numPartonsStopB = 0;

		for(unsigned int i = 0; i < particles->size(); ++ i) {

			if( p[i].status() != 3 ) continue;				// Make sure only "final" particles are present
			
			const Candidate * mom = p[i].mother();				// call mother particle

			if( stopMass_ != 0 ){
				//////////// MCTruth Stops
				if( abs( p[i].pdgId() ) == stopId ){
					//cout<< p[i].pdgId() << " " << p[i].mass() << endl;
					stopsPt_	->push_back( p[i].pt() );
					stopsEta_	->push_back( p[i].eta() );
					stopsMass_	->push_back( p[i].mass() );
					stopsPhi_	->push_back( p[i].phi() );
					numStops++;			
				}
				
				//bool partonJet = ( ( abs ( p[i].pdgId() ) == 1 ) || ( abs( p[i].pdgId() == 2 ) ) || ( abs( p[i].pdgId() == 3 ) ) || ( abs( p[i].pdgId() == 4 ) ) || ( abs( p[i].pdgId() == 5 ) ) || ( abs( p[i].pdgId() == 21 ) ) );

				if( mom ){

					if( mom->pdgId() == stopId ){
						stopAPt_	->push_back( p[i].pt() );
						stopAEta_	->push_back( p[i].eta() );
						stopAEnergy_	->push_back( p[i].energy() );
						stopAPhi_	->push_back( p[i].phi() );
						stopAPartonID_	->push_back( p[i].pdgId() );
						numPartonsStopA++;			
					}

					if( mom->pdgId() == -stopId ){
						stopBPt_	->push_back( p[i].pt() );
						stopBEta_	->push_back( p[i].eta() );
						stopBEnergy_	->push_back( p[i].energy() );
						stopBPhi_	->push_back( p[i].phi() );
						stopBPartonID_	->push_back( p[i].pdgId() );
						numPartonsStopB++;			
					}
					//std::sort(p4Stop1B.begin(), p4Stop1B.end(), ComparePt);
					//std::sort(p4Stop1jet.begin(), p4Stop1jet.end(), ComparePt);
					//std::sort(p4Stop1jetsB.begin(), p4Stop1jetsB.end(), ComparePt);
				}
			//// end Signal GenParticles
			} else {		
				partonPt_	->push_back( p[i].pt() );
				partonEta_	->push_back( p[i].eta() );
				partonEnergy_	->push_back( p[i].energy() );
				partonPhi_	->push_back( p[i].phi() );
				partonID_	->push_back( p[i].pdgId() );
			}
		}  // end GenParticles loop 
	} // If MC
  
	//---------- pu -----------------------
	edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
	if (!iEvent.isRealData()) {
		iEvent.getByLabel(srcPU_,PupInfo);
		std::vector<PileupSummaryInfo>::const_iterator PUI;
		for(PUI = PupInfo->begin(); PUI != PupInfo->end(); ++PUI) {
			if (PUI->getBunchCrossing() == 0) {
				npu_ = PUI->getTrueNumInteractions();
			}
		}
	}// if MC 


	//-------------- Trigger Info -----------------------------------
	triggerPassHisto_->Fill("totalEvents",1);
	if (triggerCache_.setEvent(iEvent,iSetup)) {
		for(unsigned itrig=0;itrig<vtriggerSelector_.size();itrig++) {
			bool result(false);
			if (vtriggerSelector_[itrig]) {
				if (triggerCache_.configurationUpdated()) {
					vtriggerSelector_[itrig]->init(triggerCache_);
				}
				result = (*(vtriggerSelector_[itrig]))(triggerCache_);
			}
			if (result) {
				triggerPassHisto_->Fill(vtriggerAlias_[itrig].c_str(),1);
			}
			triggerResult_->push_back(result);
			//cout<< vtriggerAlias_[itrig].c_str() << " " << result << endl;
		}
	}
     

	//----- at least one good vertex -----------
	bool cut_vtx = (recVtxs->size() > 0);

	if (cut_vtx) {
		nJets_ = 0;
		nJetsPruned_ = 0;
		nJetsTrimmed_ = 0;
		nJetsFilteredN2_ = 0;
		nJetsFilteredN3_ = 0;
		nJetsMassDropFiltered_ = 0;
		float ht(0.0);
		//vector< jetInfo > jets;

		for(edm::View<pat::Jet>::const_iterator ijet = pat_jets.begin();ijet != pat_jets.end(); ++ijet) { 
			double chf = ijet->chargedHadronEnergyFraction();
			double nhf = ijet->neutralHadronEnergyFraction() + ijet->HFHadronEnergyFraction();
			double phf = ijet->photonEnergy()/(ijet->jecFactor(0) * ijet->energy());
			double elf = ijet->electronEnergy()/(ijet->jecFactor(0) * ijet->energy());
			int chm    = ijet->chargedHadronMultiplicity();
			int npr    = ijet->chargedMultiplicity() + ijet->neutralMultiplicity(); 
			double muf = ijet->muonEnergy()/(ijet->jecFactor(0) * ijet->energy());
			float eta  = fabs(ijet->eta());
			float pt   = ijet->pt();
			double area = ijet->jetArea();
			double numJetConst = ijet->getPFConstituents().size();
			bool idL   = (npr>1 && phf<0.99 && nhf<0.99);
			bool idT   = (idL && ((eta<=2.5 && nhf<0.9 && phf<0.9 && elf<0.99 && muf<0.99 && chf>0 && chm>0) || eta>2.5));
			///vector<reco::TrackRefVector> tracksAsocJet = ijet->associatedTracks(); // this is wrong, I have to add it
			//jetInfo kjet;

			//if (idT && pt > ptMin_ && eta < etaMax_ && pt > ptMin_) {
			ht += pt;
			chf_           ->push_back(chf);
			nhf_           ->push_back(nhf);
			phf_           ->push_back(phf);
			elf_           ->push_back(elf);
			muf_           ->push_back(muf);
			numJetConst_   ->push_back(numJetConst);
			area_          ->push_back(area);
			jec_           ->push_back(1./ijet->jecFactor(0));
			pt_            ->push_back(pt);
			phi_           ->push_back(ijet->phi());
			eta_           ->push_back(ijet->eta());
			mass_          ->push_back(ijet->mass());
			energy_        ->push_back(ijet->energy());
			tau1_          ->push_back(ijet->userFloat("tau1"));
			tau2_          ->push_back(ijet->userFloat("tau2"));
			tau3_          ->push_back(ijet->userFloat("tau3"));
			nJets_++;

				/*kjet.TL = TLorentzVector(ijet->px(),ijet->py(),ijet->pz(),ijet->energy());
				kjet.tau1 = ijet->userFloat("tau1");
				kjet.tau2 = ijet->userFloat("tau2");
				kjet.tau3 = ijet->userFloat("tau3");
				jets.push_back( kjet );*/
				  
				//---- match with the pruned jet collection -----
				/*double dRmin(1000);
				double auxm(0.0);
				for(edm::View<pat::Jet>::const_iterator ijetpr = pat_jets_pruned.begin();ijetpr != pat_jets_pruned.end(); ++ijetpr) { 
					float dR = deltaR(ijet->eta(),ijet->phi(),ijetpr->eta(),ijetpr->phi());
					if (dR < dRmin) {
						auxm = ijetpr->mass();
						dRmin = dR;
					} 
				} 
				dR_->push_back(dRmin);*/
			//} 
		}// jet loop  

		for(edm::View<pat::Jet>::const_iterator ijetpr = pat_jets_pruned.begin();ijetpr != pat_jets_pruned.end(); ++ijetpr) { 
			double areaPruned = ijetpr->jetArea();

			areaPruned_          ->push_back(areaPruned);
			jecPruned_           ->push_back(1./ijetpr->jecFactor(0));
			ptPruned_            ->push_back(ijetpr->pt());
			phiPruned_           ->push_back(ijetpr->phi());
			etaPruned_           ->push_back(ijetpr->eta());
			massPruned_          ->push_back(ijetpr->mass());
			energyPruned_        ->push_back(ijetpr->energy());
			tau1Pruned_          ->push_back(ijetpr->userFloat("tau1"));
			tau2Pruned_          ->push_back(ijetpr->userFloat("tau2"));
			tau3Pruned_          ->push_back(ijetpr->userFloat("tau3"));
			nJetsPruned_++;
		}

		for(edm::View<pat::Jet>::const_iterator ijettr = pat_jets_trimmed.begin();ijettr != pat_jets_trimmed.end(); ++ijettr) { 
			double areaTrimmed = ijettr->jetArea();

			areaTrimmed_          ->push_back(areaTrimmed);
			jecTrimmed_           ->push_back(1./ijettr->jecFactor(0));
			ptTrimmed_            ->push_back(ijettr->pt());
			phiTrimmed_           ->push_back(ijettr->phi());
			etaTrimmed_           ->push_back(ijettr->eta());
			massTrimmed_          ->push_back(ijettr->mass());
			energyTrimmed_        ->push_back(ijettr->energy());
			tau1Trimmed_          ->push_back(ijettr->userFloat("tau1"));
			tau2Trimmed_          ->push_back(ijettr->userFloat("tau2"));
			tau3Trimmed_          ->push_back(ijettr->userFloat("tau3"));
			nJetsTrimmed_++;
		}

		for(edm::View<pat::Jet>::const_iterator ijetfilN2 = pat_jets_filteredN2.begin();ijetfilN2 != pat_jets_filteredN2.end(); ++ijetfilN2) { 
			double areaFilteredN2 = ijetfilN2->jetArea();

			areaFilteredN2_          ->push_back(areaFilteredN2);
			jecFilteredN2_           ->push_back(1./ijetfilN2->jecFactor(0));
			ptFilteredN2_            ->push_back(ijetfilN2->pt());
			phiFilteredN2_           ->push_back(ijetfilN2->phi());
			etaFilteredN2_           ->push_back(ijetfilN2->eta());
			massFilteredN2_          ->push_back(ijetfilN2->mass());
			energyFilteredN2_        ->push_back(ijetfilN2->energy());
			tau1FilteredN2_          ->push_back(ijetfilN2->userFloat("tau1"));
			tau2FilteredN2_          ->push_back(ijetfilN2->userFloat("tau2"));
			tau3FilteredN2_          ->push_back(ijetfilN2->userFloat("tau3"));
			nJetsFilteredN2_++;
		}

		for(edm::View<pat::Jet>::const_iterator ijetfilN3 = pat_jets_filteredN3.begin();ijetfilN3 != pat_jets_filteredN3.end(); ++ijetfilN3) { 
			double areaFilteredN3 = ijetfilN3->jetArea();

			areaFilteredN3_          ->push_back(areaFilteredN3);
			jecFilteredN3_           ->push_back(1./ijetfilN3->jecFactor(0));
			ptFilteredN3_            ->push_back(ijetfilN3->pt());
			phiFilteredN3_           ->push_back(ijetfilN3->phi());
			etaFilteredN3_           ->push_back(ijetfilN3->eta());
			massFilteredN3_          ->push_back(ijetfilN3->mass());
			energyFilteredN3_        ->push_back(ijetfilN3->energy());
			tau1FilteredN3_          ->push_back(ijetfilN3->userFloat("tau1"));
			tau2FilteredN3_          ->push_back(ijetfilN3->userFloat("tau2"));
			tau3FilteredN3_          ->push_back(ijetfilN3->userFloat("tau3"));
			nJetsFilteredN3_++;
		}

		for(edm::View<pat::Jet>::const_iterator ijetmdfil = pat_jets_massDropFiltered.begin();ijetmdfil != pat_jets_massDropFiltered.end(); ++ijetmdfil) { 
			double areaMassDropFiltered = ijetmdfil->jetArea();

			areaMassDropFiltered_          ->push_back(areaMassDropFiltered);
			jecMassDropFiltered_           ->push_back(1./ijetmdfil->jecFactor(0));
			ptMassDropFiltered_            ->push_back(ijetmdfil->pt());
			phiMassDropFiltered_           ->push_back(ijetmdfil->phi());
			etaMassDropFiltered_           ->push_back(ijetmdfil->eta());
			massMassDropFiltered_          ->push_back(ijetmdfil->mass());
			energyMassDropFiltered_        ->push_back(ijetmdfil->energy());
			tau1MassDropFiltered_          ->push_back(ijetmdfil->userFloat("tau1"));
			tau2MassDropFiltered_          ->push_back(ijetmdfil->userFloat("tau2"));
			tau3MassDropFiltered_          ->push_back(ijetmdfil->userFloat("tau3"));
			nJetsMassDropFiltered_++;
		}


		ht_     = ht;
		met_    = (*met)[0].et();
		if ((*met)[0].sumEt() > 0) {
			metSig_ = (*met)[0].et()/(*met)[0].sumEt();
		}
		nVtx_   = recVtxs->size();
		run_    = iEvent.id().run();
		evt_    = iEvent.id().event();
		lumi_   = iEvent.id().luminosityBlock();
		/*if (nJets_ > 1) { 
			mjj_    = (jets[0].TL+jets[1].TL).M();
			dEtajj_ = fabs((*eta_)[0]-(*eta_)[1]); 
			dPhijj_ = fabs(deltaPhi((*phi_)[0],(*phi_)[1]));
			if (mjj_ > mjjMin_ && dEtajj_ < dEtaMax_) {
				outTree_->Fill();     
			}
		}// if nJets > 1 */

		outTree_->Fill();     
	}// if vtx
}



//////////////////////////////////////////////////////////////////////////////////////////
void PFJetTreeProducer::initialize() {
	run_            = -999;
	evt_            = -999;
	lumi_           = -999;
	nVtx_           = -999;
	nJets_          = -999;
	met_            = -999;
	metSig_         = -999;
	ht_             = -999;
	chf_            ->clear();
	nhf_            ->clear();
	phf_            ->clear();
	elf_            ->clear();
	muf_            ->clear();
	pt_             ->clear();
	eta_            ->clear();
	phi_            ->clear();
	mass_           ->clear();
	tau1_           ->clear();
	tau2_           ->clear();
	tau3_           ->clear();
	energy_         ->clear();
	area_           ->clear();
	numJetConst_    ->clear();
	jec_            ->clear();
	ptPruned_             ->clear();
	etaPruned_            ->clear();
	phiPruned_            ->clear();
	massPruned_           ->clear();
	tau1Pruned_           ->clear();
	tau2Pruned_           ->clear();
	tau3Pruned_           ->clear();
	energyPruned_         ->clear();
	areaPruned_           ->clear();
	numJetConstPruned_    ->clear();
	jecPruned_            ->clear();
	ptTrimmed_             ->clear();
	etaTrimmed_            ->clear();
	phiTrimmed_            ->clear();
	massTrimmed_           ->clear();
	tau1Trimmed_           ->clear();
	tau2Trimmed_           ->clear();
	tau3Trimmed_           ->clear();
	energyTrimmed_         ->clear();
	areaTrimmed_           ->clear();
	numJetConstTrimmed_    ->clear();
	jecTrimmed_            ->clear();
	ptFilteredN2_             ->clear();
	etaFilteredN2_            ->clear();
	phiFilteredN2_            ->clear();
	massFilteredN2_           ->clear();
	tau1FilteredN2_           ->clear();
	tau2FilteredN2_           ->clear();
	tau3FilteredN2_           ->clear();
	energyFilteredN2_         ->clear();
	areaFilteredN2_           ->clear();
	numJetConstFilteredN2_    ->clear();
	jecFilteredN2_            ->clear();
	ptFilteredN3_             ->clear();
	etaFilteredN3_            ->clear();
	phiFilteredN3_            ->clear();
	massFilteredN3_           ->clear();
	tau1FilteredN3_           ->clear();
	tau2FilteredN3_           ->clear();
	tau3FilteredN3_           ->clear();
	energyFilteredN3_         ->clear();
	areaFilteredN3_           ->clear();
	numJetConstFilteredN3_    ->clear();
	jecFilteredN3_            ->clear();
	ptMassDropFiltered_             ->clear();
	etaMassDropFiltered_            ->clear();
	phiMassDropFiltered_            ->clear();
	massMassDropFiltered_           ->clear();
	tau1MassDropFiltered_           ->clear();
	tau2MassDropFiltered_           ->clear();
	tau3MassDropFiltered_           ->clear();
	energyMassDropFiltered_         ->clear();
	areaMassDropFiltered_           ->clear();
	numJetConstMassDropFiltered_    ->clear();
	jecMassDropFiltered_            ->clear();
	dR_             ->clear();
	triggerResult_  ->clear();
	//----- MC -------
	npu_ = -999;
	numStops	= -999;
	numPartonsStopA	= -999;
	numPartonsStopB	= -999;
	stopsPt_ 	-> clear();
	stopsEta_ 	-> clear();
	stopsMass_ 	-> clear();
	stopsPhi_ 	-> clear();
	stopAPt_ 	-> clear();
	stopAEta_ 	-> clear();
	stopAEnergy_ 	-> clear();
	stopAPhi_ 	-> clear();
	stopAPartonID_ 	-> clear();
	stopBPt_ 	-> clear();
	stopBEta_ 	-> clear();
	stopBEnergy_ 	-> clear();
	stopBPhi_ 	-> clear();
	stopBPartonID_ 	-> clear();
	partonPt_ 	-> clear();
	partonEta_ 	-> clear();
	partonEnergy_ 	-> clear();
	partonPhi_ 	-> clear();
	partonID_ -> clear();
}
//////////////////////////////////////////////////////////////////////////////////////////
PFJetTreeProducer::~PFJetTreeProducer() 
{
}

DEFINE_FWK_MODULE(PFJetTreeProducer);
