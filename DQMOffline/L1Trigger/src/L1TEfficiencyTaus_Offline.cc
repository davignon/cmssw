#include "DQMOffline/L1Trigger/interface/L1TEfficiencyTaus_Offline.h"

#include "DataFormats/L1Trigger/interface/Muon.h"							
#include "DQMServices/Core/interface/DQMStore.h"
#include "DataFormats/Histograms/interface/MEtoEDMFormat.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonPFIsolation.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/TauReco/interface/CaloTau.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/BaseTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"
#include "RecoTauTag/RecoTau/interface/TauDiscriminationProducerBase.h"

#include "TMath.h"
#include "TLorentzVector.h"

using namespace reco;
using namespace trigger;
using namespace edm;
using namespace std;
using namespace l1t;

TauL1tPair::TauL1tPair(const TauL1tPair& tauL1tPair) {

  m_tau    = tauL1tPair.m_tau;			
  m_regTau  = tauL1tPair.m_regTau;		

  m_eta     = tauL1tPair.m_eta;
  m_phi_bar = tauL1tPair.m_phi_bar;
  m_phi_end = tauL1tPair.m_phi_end;

}

double TauL1tPair::dR() {
  
  float dEta = m_regTau ? (m_regTau->eta() - eta()) : 999.;					
  float dPhi = m_regTau ? (m_regTau->phi() - phi()) : 999.; 
  float dr = sqrt(dEta*dEta + dPhi*dPhi);
  return dr;
}

//__________DQM_base_class_______________________________________________
L1TEfficiencyTaus_Offline::L1TEfficiencyTaus_Offline(const ParameterSet & ps){

  m_verbose = ps.getUntrackedParameter<bool>("verbose");

  // Initializing config params
  m_L1tPtCuts = ps.getUntrackedParameter< vector<int> >("l1tPtCuts");
  m_TauInputTag =  consumes<reco::PFTauCollection>(ps.getUntrackedParameter<InputTag>("tauInputTag"));
  m_AntiMuInputTag = consumes<reco::PFTauDiscriminator>(ps.getUntrackedParameter<InputTag>("antiMuInputTag"));
  m_AntiEleInputTag = consumes<reco::PFTauDiscriminator>(ps.getUntrackedParameter<InputTag>("antiEleInputTag"));
  m_DecayModeFindingInputTag = consumes<reco::PFTauDiscriminator>(ps.getUntrackedParameter<InputTag>("decayModeFindingInputTag"));
  m_Comb3TInputTag = consumes<reco::PFTauDiscriminator>(ps.getUntrackedParameter<InputTag>("comb3TInputTag"));
  m_MuonInputTag =  consumes<reco::MuonCollection>(ps.getUntrackedParameter<InputTag>("muonInputTag"));
  m_L1tInputTag  =  consumes<l1t::TauBxCollection>(ps.getUntrackedParameter<InputTag>("l1tInputTag"));  
  m_MetInputTag =  consumes<reco::PFMETCollection>(ps.getUntrackedParameter<InputTag>("metInputTag"));

  m_VtxInputTag =  consumes<VertexCollection>(ps.getUntrackedParameter<InputTag>("vtxInputTag"));
  m_BsInputTag  =  consumes<BeamSpot>(ps.getUntrackedParameter<InputTag>("bsInputTag"));

  m_trigInputTag = consumes<trigger::TriggerEvent>(ps.getUntrackedParameter<InputTag>("trigInputTag"));
  m_trigProcess  = ps.getUntrackedParameter<string>("trigProcess");
  m_trigProcess_token  = consumes<edm::TriggerResults>(ps.getUntrackedParameter<InputTag>("trigProcess_token"));
  m_trigNames    = ps.getUntrackedParameter<vector<string> >("triggerNames");

  m_MaxTauEta   = 2.1;
  m_MaxL1tTauDR = 0.5;
  m_MaxHltTauDR = 0.5;
}

 
//_____________________________________________________________________
L1TEfficiencyTaus_Offline::~L1TEfficiencyTaus_Offline(){ }
//----------------------------------------------------------------------
void L1TEfficiencyTaus_Offline::dqmBeginRun(const edm::Run& run, const edm::EventSetup& iSetup){

  bool changed = true;
  m_hltConfig.init(run,iSetup,m_trigProcess,changed);

}  
//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& run, const edm::EventSetup& iSetup){

  bookControlHistos(ibooker);
  vector<int>::const_iterator l1tPtCutsIt  = m_L1tPtCuts.begin();
  vector<int>::const_iterator l1tPtCutsEnd = m_L1tPtCuts.end();

  for (; l1tPtCutsIt!=l1tPtCutsEnd; ++ l1tPtCutsIt) {
    bookEfficiencyHistos(ibooker, (*l1tPtCutsIt));
  } 
 
  vector<string>::const_iterator trigNamesIt  = m_trigNames.begin();
  vector<string>::const_iterator trigNamesEnd = m_trigNames.end();

  for (; trigNamesIt!=trigNamesEnd; ++trigNamesIt) { 
    
    TString tNameTmp = TString(*trigNamesIt); 
    TRegexp tNamePattern = TRegexp(tNameTmp,true);
    int tIndex = -1;
    
    for (unsigned ipath = 0; ipath < m_hltConfig.size(); ++ipath) {
      
      TString tmpName = TString(m_hltConfig.triggerName(ipath));
      if (tmpName.Contains(tNamePattern)) {
	tIndex = int(ipath);
	m_trigIndices.push_back(tIndex);
      }
    }
        
  }
   
}  

//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::beginLuminosityBlock(LuminosityBlock const& lumiBlock, EventSetup const& c) {
  
}


//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::dqmEndLuminosityBlock(LuminosityBlock const& lumiBlock, EventSetup const& c) {
  
}


//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::analyze(const Event & iEvent, const EventSetup & eventSetup){

  Handle<reco::PFTauCollection> taus;
  iEvent.getByToken(m_TauInputTag, taus);

  Handle<reco::MuonCollection> muons;
  iEvent.getByToken(m_MuonInputTag, muons);

  Handle<BeamSpot> beamSpot;
  iEvent.getByToken(m_BsInputTag, beamSpot);

  Handle<VertexCollection> vertex;
  iEvent.getByToken(m_VtxInputTag, vertex);
  Handle<l1t::TauBxCollection> l1tCands;				

  iEvent.getByToken(m_L1tInputTag,l1tCands);
  
  Handle<edm::TriggerResults> trigResults;
  iEvent.getByToken(m_trigProcess_token,trigResults);
  
  edm::Handle<trigger::TriggerEvent> trigEvent;
  iEvent.getByToken(m_trigInputTag,trigEvent);

  edm::Handle<reco::PFMETCollection> mets;
  iEvent.getByToken(m_MetInputTag, mets);

  eventSetup.get<IdealMagneticFieldRecord>().get(m_BField);									
  const Vertex primaryVertex = getPrimaryVertex(vertex,beamSpot);

  getTightMuons(muons,mets,primaryVertex,trigEvent);
  getProbeTaus(iEvent,taus,muons,primaryVertex);
  getTauL1tPairs(l1tCands);

  PFTauCollection::const_iterator tauIt  = taus->begin();
  PFTauCollection::const_iterator tauEnd = taus->end();

  for(; tauIt!=tauEnd; ++tauIt) {

    float eta = tauIt->eta();
    float phi = tauIt->phi();
    float pt  = tauIt->pt();

    m_EfficiencyHistos[0]["RecoEtaNEW_Histo"]->Fill(eta);	
    m_EfficiencyHistos[0]["RecoPhiNEW_Histo"]->Fill(phi);	
    m_EfficiencyHistos[0]["RecoPtNEW_Histo"]->Fill(pt);		

  }
  vector<l1t::Tau> l1tContainer;
  
  for (auto tau = l1tCands->begin(0); tau != l1tCands->end(0); ++tau) {
     l1tContainer.push_back(*tau);
  }

  vector<l1t::Tau>::const_iterator l1tIt = l1tContainer.begin();;				
  vector<l1t::Tau>::const_iterator l1tEnd = l1tContainer.end();				
    
  for(; l1tIt!=l1tEnd; ++l1tIt) {

    float eta = l1tIt->eta();
    float phi = l1tIt->phi();
    float pt  = l1tIt->pt();

    m_EfficiencyHistos[0]["L1TEtaNEW_Histo"]->Fill(eta);	
    m_EfficiencyHistos[0]["L1TPhiNEW_Histo"]->Fill(phi);	
    m_EfficiencyHistos[0]["L1TPtNEW_Histo"]->Fill(pt);	

  }

  vector<TauL1tPair>::const_iterator tauL1tPairsIt  = m_TauL1tPairs.begin();
  vector<TauL1tPair>::const_iterator tauL1tPairsEnd = m_TauL1tPairs.end(); 
	
  for(; tauL1tPairsIt!=tauL1tPairsEnd; ++tauL1tPairsIt) {

    float eta = tauL1tPairsIt->eta();
    float phi = tauL1tPairsIt->phi();
    float pt  = tauL1tPairsIt->pt();

    // unmatched gmt cands have l1tPt = -1.	
    float l1tPt  = tauL1tPairsIt->l1tPt();
cout<<pt<<" "<<l1tPt<<endl;

    vector<int>::const_iterator l1tPtCutsIt  = m_L1tPtCuts.begin();
    vector<int>::const_iterator l1tPtCutsEnd = m_L1tPtCuts.end();


    for (; l1tPtCutsIt!=l1tPtCutsEnd; ++ l1tPtCutsIt) {
      
      int l1tPtCut = (*l1tPtCutsIt);
      bool l1tAboveCut = (l1tPt > l1tPtCut);

      stringstream ptCutToTag; ptCutToTag << l1tPtCut;
      string ptTag = ptCutToTag.str();

      if (fabs(eta) < m_MaxTauEta) {
        m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Den"]->Fill(pt);
        if (eta<0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Etaneg_Den"]->Fill(pt);
        if (eta>=0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Etapos_Den"]->Fill(pt);
        if (fabs(eta)<1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Barrel_Den"]->Fill(pt);
        if (fabs(eta)>=1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Endcaps_Den"]->Fill(pt);
        m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Den"]->Fill(pt);
        if (eta<0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Etaneg_Den"]->Fill(pt);
        if (eta>=0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Etapos_Den"]->Fill(pt);
        if (fabs(eta)<1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Barrel_Den"]->Fill(pt);
        if (fabs(eta)>=1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Endcaps_Den"]->Fill(pt);
        if (l1tAboveCut && tauL1tPairsIt->l1tIso()>0.5){
           m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Num"]->Fill(pt);
           if (eta<0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Etaneg_Num"]->Fill(pt);
           if (eta>=0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Etapos_Num"]->Fill(pt);
           if (fabs(eta)<1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Barrel_Num"]->Fill(pt);
           if (fabs(eta)>=1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passIso_Endcaps_Num"]->Fill(pt);
        }
        if (l1tAboveCut){
           m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Num"]->Fill(pt);
           if (eta<0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Etaneg_Num"]->Fill(pt);
           if (eta>=0) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Etapos_Num"]->Fill(pt);
           if (fabs(eta)<1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Barrel_Num"]->Fill(pt);
           if (fabs(eta)>=1.5) m_EfficiencyHistos[l1tPtCut]["EffvsPt_passNoIso_Endcaps_Num"]->Fill(pt);
        }

        if (pt > 35) {
            m_EfficiencyHistos[l1tPtCut]["EffvsPhi_passIso_Den"]->Fill(phi);
            m_EfficiencyHistos[l1tPtCut]["EffvsEta_passIso_Den"]->Fill(eta);
            m_EfficiencyHistos[l1tPtCut]["EffvsPhi_passNoIso_Den"]->Fill(phi);
            m_EfficiencyHistos[l1tPtCut]["EffvsEta_passNoIso_Den"]->Fill(eta);
            if (l1tAboveCut) {
               m_EfficiencyHistos[l1tPtCut]["EffvsPhi_passNoIso_Num"]->Fill(phi);
               m_EfficiencyHistos[l1tPtCut]["EffvsEta_passNoIso_Num"]->Fill(eta);
	    }
            if (l1tAboveCut && tauL1tPairsIt->l1tIso()){
               m_EfficiencyHistos[l1tPtCut]["EffvsPhi_passIso_Num"]->Fill(phi);
               m_EfficiencyHistos[l1tPtCut]["EffvsEta_passIso_Num"]->Fill(eta);
            }
        }

      }
   }
  }//loop over tau-L1 pairs
 
}

//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::bookControlHistos(DQMStore::IBooker& ibooker) { 
  
  ibooker.setCurrentFolder("L1T/Efficiency/Taus/Control");
  
  string name = "TauL1tDeltaR";
  m_ControlHistos[name] = ibooker.book1D(name.c_str(),name.c_str(),50.,0.,0.5);

  name = "NTightVsAll";
  m_ControlHistos[name] = ibooker.book2D(name.c_str(),name.c_str(),5,-0.5,4.5,5,-0.5,4.5);

  name = "NProbesVsTight";
  m_ControlHistos[name] = ibooker.book2D(name.c_str(),name.c_str(),5,-0.5,4.5,5,-0.5,4.5);
  
}


//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::bookEfficiencyHistos(DQMStore::IBooker &ibooker, int ptCut) { 

  stringstream ptCutToTag; ptCutToTag << ptCut;
  string ptTag = ptCutToTag.str();
  
  ibooker.setCurrentFolder("L1T/Efficiency/Taus/");

  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Num"] = ibooker.book1D("EffvsPt_passIso_Num","EffvsPt_passIso_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Etaneg_Num"] = ibooker.book1D("EffvsPt_passIso_Etaneg_Num","EffvsPt_passIso_Etaneg_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Etapos_Num"] = ibooker.book1D("EffvsPt_passIso_Etapos_Num","EffvsPt_passIso_Etapos_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Barrel_Num"] = ibooker.book1D("EffvsPt_passIso_Barrel_Num","EffvsPt_passIso_Barrel_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Endcaps_Num"] = ibooker.book1D("EffvsPt_passIso_Endcaps_Num","EffvsPt_passIso_Endcaps_Num",50,0.,100.);

  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Num"] = ibooker.book1D("EffvsPt_passNoIso_Num","EffvsPt_passNoIso_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Etaneg_Num"] = ibooker.book1D("EffvsPt_passNoIso_Etaneg_Num","EffvsPt_passNoIso_Etaneg_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Etapos_Num"] = ibooker.book1D("EffvsPt_passNoIso_Etapos_Num","EffvsPt_passNoIso_Etapos_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Barrel_Num"] = ibooker.book1D("EffvsPt_passNoIso_Barrel_Num","EffvsPt_passNoIso_Barrel_Num",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Endcaps_Num"] = ibooker.book1D("EffvsPt_passNoIso_Endcaps_Num","EffvsPt_passNoIso_Endcaps_Num",50,0.,100.);

  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Den"] = ibooker.book1D("EffvsPt_passIso_Den","EffvsPt_passIso_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Etaneg_Den"] = ibooker.book1D("EffvsPt_passIso_Etaneg_Den","EffvsPt_passIso_Etaneg_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Etapos_Den"] = ibooker.book1D("EffvsPt_passIso_Etapos_Den","EffvsPt_passIso_Etapos_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Barrel_Den"] = ibooker.book1D("EffvsPt_passIso_Barrel_Den","EffvsPt_passIso_Barrel_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passIso_Endcaps_Den"] = ibooker.book1D("EffvsPt_passIso_Endcaps_Den","EffvsPt_passIso_Endcaps_Den",50,0.,100.);

  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Den"] = ibooker.book1D("EffvsPt_passNoIso_Den","EffvsPt_passNoIso_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Etaneg_Den"] = ibooker.book1D("EffvsPt_passNoIso_Etaneg_Den","EffvsPt_passNoIso_Etaneg_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Etapos_Den"] = ibooker.book1D("EffvsPt_passNoIso_Etapos_Den","EffvsPt_passNoIso_Etapos_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Barrel_Den"] = ibooker.book1D("EffvsPt_passNoIso_Barrel_Den","EffvsPt_passNoIso_Barrel_Den",50,0.,100.);
  m_EfficiencyHistos[ptCut]["EffvsPt_passNoIso_Endcaps_Den"] = ibooker.book1D("EffvsPt_passNoIso_Endcaps_Den","EffvsPt_passNoIso_Endcaps_Den",50,0.,100.);

  m_EfficiencyHistos[ptCut]["EffvsPhi_passNoIso_Den"] = ibooker.book1D("EffvsPhi_passNoIso_Den","EffvsPhi_passNoIso_Den",23,-3.15,3.15);
  m_EfficiencyHistos[ptCut]["EffvsEta_passNoIso_Den"] = ibooker.book1D("EffvsEta_passNoIso_Den","EffvsEta_passNoIso_Den",23,-2.1,2.1);
  m_EfficiencyHistos[ptCut]["EffvsPhi_passIso_Den"] = ibooker.book1D("EffvsPhi_passIso_Den","EffvsPhi_passIso_Den",23,-3.15,3.15);
  m_EfficiencyHistos[ptCut]["EffvsEta_passIso_Den"] = ibooker.book1D("EffvsEta_passIso_Den","EffvsEta_passIso_Den",23,-2.1,2.1);

  m_EfficiencyHistos[ptCut]["EffvsPhi_passNoIso_Num"] = ibooker.book1D("EffvsPhi_passNoIso_Num","EffvsPhi_passNoIso_Num",23,-3.15,3.15);
  m_EfficiencyHistos[ptCut]["EffvsEta_passNoIso_Num"] = ibooker.book1D("EffvsEta_passNoIso_Num","EffvsEta_passNoIso_Num",23,-2.1,2.1);
  m_EfficiencyHistos[ptCut]["EffvsPhi_passIso_Num"] = ibooker.book1D("EffvsPhi_passIso_Num","EffvsPhi_passIso_Num",23,-3.15,3.15);
  m_EfficiencyHistos[ptCut]["EffvsEta_passIso_Num"] = ibooker.book1D("EffvsEta_passIso_Num","EffvsEta_passIso_Num",23,-2.1,2.1);
  
  string name1 = "RecoEtaNEW_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5); 
  string name2 = "RecoPhiNEW_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi()); 
  string name3 = "RecoPtNEW_Histo";
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.);
  name1 = "L1TEtaNEW_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5);
  name2 = "L1TPhiNEW_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi());
  name3 = "L1TPtNEW_Histo"; 
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.);
  name1 = "RecoEtaNEWtight_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5); 
  name2 = "RecoPhiNEWtight_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi());
  name3 = "RecoPtNEWtight_Histo";
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.);
  name1 = "RecoEtaNEWtight2_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5); 
  name2 = "RecoPhiNEWtight2_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi());
  name3 = "RecoPtNEWtight2_Histo";
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.);
  name1 = "TagMuonEta_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5);
  name2 = "TagMuonPhi_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi());
  name3 = "TagMuonPt_Histo";
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.); 
  name1 = "ProbeTauEta_Histo";
  m_EfficiencyHistos[0][name1] = ibooker.book1D(name1.c_str(),name1.c_str(),50,-2.5,2.5); 
  name2 = "ProbeTauPhi_Histo";
  m_EfficiencyHistos[0][name2] = ibooker.book1D(name2.c_str(),name2.c_str(),24,-TMath::Pi(),TMath::Pi());
  name3 = "ProbeTauPt_Histo";
  m_EfficiencyHistos[0][name3] = ibooker.book1D(name3.c_str(),name3.c_str(),50,0.,100.);    						
}


//_____________________________________________________________________
const reco::Vertex L1TEfficiencyTaus_Offline::getPrimaryVertex( Handle<VertexCollection> & vertex,
								 Handle<BeamSpot> & beamSpot ) {
  
  Vertex::Point posVtx;
  Vertex::Error errVtx;
  
  bool hasPrimaryVertex = false;

  if (vertex.isValid())
    {

      vector<Vertex>::const_iterator vertexIt  = vertex->begin();
      vector<Vertex>::const_iterator vertexEnd = vertex->end();

      for (;vertexIt!=vertexEnd;++vertexIt) 
	{
	  if (vertexIt->isValid() && 
	      !vertexIt->isFake()) 
	    {
	      posVtx = vertexIt->position();
	      errVtx = vertexIt->error();
	      hasPrimaryVertex = true;	      
	      break;
	    }
	}
    }

  if ( !hasPrimaryVertex ) {
    posVtx = beamSpot->position();
    errVtx(0,0) = beamSpot->BeamWidthX();
    errVtx(1,1) = beamSpot->BeamWidthY();
    errVtx(2,2) = beamSpot->sigmaZ();
  }

  const Vertex primaryVertex(posVtx,errVtx);
  
  return primaryVertex;
}

//_____________________________________________________________________
void L1TEfficiencyTaus_Offline::getTightMuons(edm::Handle<reco::MuonCollection> & muons, edm::Handle<reco::PFMETCollection> &mets,  const Vertex & vertex, edm::Handle<trigger::TriggerEvent> & trigEvent) {

  m_TightMuons.clear();
  MuonCollection::const_iterator muonIt  = muons->begin();
  MuonCollection::const_iterator muonEnd = muons->end();

  MuonCollection::const_iterator muonIt2  = muons->begin();
  MuonCollection::const_iterator muonEnd2 = muons->end();

  const PFMET *pfmet=NULL;
  pfmet=&(mets->front());

  int nb_mu=0;

  for(; muonIt2!=muonEnd2; ++muonIt2) {
    if (fabs(muonIt2->eta())< 2.4 && muonIt2->pt()>10 && muon::isLooseMuon((*muonIt))) {
      ++nb_mu;
    }
  }
  bool foundTightMu=false;
  for(; muonIt!=muonEnd; ++muonIt) {
    if (!matchHlt(trigEvent,&(*muonIt))) continue;
    float muiso=(muonIt->pfIsolationR04().sumChargedHadronPt+max(muonIt->pfIsolationR04().sumNeutralHadronEt+muonIt->pfIsolationR04().sumPhotonEt-0.5*muonIt->pfIsolationR04().sumPUPt,0.0))/muonIt->pt();

    if (muiso<0.1 && nb_mu<2 && !foundTightMu && fabs(muonIt->eta())< 2.4 && muonIt->pt()>25 && muon::isMediumMuon((*muonIt))) {
      float mt=sqrt(pow(muonIt->pt() + pfmet->pt(), 2) - pow(muonIt->px() + pfmet->px(),2) - pow(muonIt->py() + pfmet->py(), 2));
      if (mt<30){
         m_TightMuons.push_back(&(*muonIt));
         foundTightMu=true;
	 //cout<<"muon found! :)"<<endl;
         m_EfficiencyHistos[0]["TagMuonEta_Histo"]->Fill(muonIt->eta());
         m_EfficiencyHistos[0]["TagMuonPhi_Histo"]->Fill(muonIt->phi());
         m_EfficiencyHistos[0]["TagMuonPt_Histo"]->Fill(muonIt->pt());
      } 
    }
  }
  m_ControlHistos["NTightVsAll"]->Fill(muons->size(),m_TightMuons.size());
  vector<const reco::Muon*>::const_iterator tightMuIt  = m_TightMuons.begin();
  vector<const reco::Muon*>::const_iterator tightMuEnd  = m_TightMuons.end();
  
  for(; tightMuIt!=tightMuEnd; ++tightMuIt) {
    float eta = (*tightMuIt)->eta();
    float phi = (*tightMuIt)->phi();
    float pt  = (*tightMuIt)->pt();

    m_EfficiencyHistos[0]["RecoEtaNEWtight_Histo"]->Fill(eta);	
    m_EfficiencyHistos[0]["RecoPhiNEWtight_Histo"]->Fill(phi);	
    m_EfficiencyHistos[0]["RecoPtNEWtight_Histo"]->Fill(pt);	
  }
}

void L1TEfficiencyTaus_Offline::getProbeTaus(const Event & iEvent,edm::Handle<reco::PFTauCollection> & taus, edm::Handle<reco::MuonCollection> & muons, const Vertex & vertex) {

  m_ProbeTaus.clear();
  PFTauCollection::const_iterator tauIt  = taus->begin();
  PFTauCollection::const_iterator tauEnd = taus->end();

  Handle<reco::PFTauDiscriminator> antimu;
  iEvent.getByToken(m_AntiMuInputTag, antimu);
  Handle<reco::PFTauDiscriminator> dmf;
  iEvent.getByToken(m_DecayModeFindingInputTag, dmf);
  Handle<reco::PFTauDiscriminator> antiele;
  iEvent.getByToken(m_AntiEleInputTag, antiele);
  Handle<reco::PFTauDiscriminator> comb3T;
  iEvent.getByToken(m_Comb3TInputTag, comb3T);

  if (m_TightMuons.size()>0){
     TLorentzVector mymu;
     mymu.SetPtEtaPhiE(m_TightMuons[0]->pt(),m_TightMuons[0]->eta(),m_TightMuons[0]->phi(),m_TightMuons[0]->energy());
     for(unsigned iTau=0; tauIt!=tauEnd; ++tauIt,++iTau) {
        reco::PFTauRef tauCandidate(taus, iTau);
	TLorentzVector mytau;
	mytau.SetPtEtaPhiE(tauIt->pt(),tauIt->eta(),tauIt->phi(),tauIt->energy());
        if (fabs(tauIt->charge())==1 && fabs(tauIt->eta())< 2.1 && tauIt->pt()>20 && (*antimu)[tauCandidate] > 0.5 && (*antiele)[tauCandidate] > 0.5 && (*dmf)[tauCandidate] > 0.5 && (*comb3T)[tauCandidate] > 0.5) {
	    if (mymu.DeltaR(mytau)>0.5 && (mymu+mytau).M()>40 && (mymu+mytau).M()<80 && m_TightMuons[0]->charge()*tauIt->charge()<0){
               m_ProbeTaus.push_back(&(*tauIt));
	       cout<<"********** tau found ************"<<endl;
	    }
        }
     }
  }
}

//_____________________________________________________________________

void L1TEfficiencyTaus_Offline::getTauL1tPairs(edm::Handle<l1t::TauBxCollection> & l1tCands) {					

  m_TauL1tPairs.clear();
  
  vector<const reco::PFTau*>::const_iterator probeTauIt  = m_ProbeTaus.begin();	
  vector<const reco::PFTau*>::const_iterator probeTauEnd = m_ProbeTaus.end();		
  vector<l1t::Tau> l1tContainer;
  
  for (auto tau = l1tCands->begin(0); tau != l1tCands->end(0); ++tau) {
    l1tContainer.push_back(*tau);
  }

  vector<l1t::Tau>::const_iterator l1tIt;					
  vector<l1t::Tau>::const_iterator l1tEnd = l1tContainer.end();				
  
  for (; probeTauIt!=probeTauEnd; ++probeTauIt) {

    float eta = (*probeTauIt)->eta();		
    float phi = (*probeTauIt)->phi();		
    float pt  = (*probeTauIt)->pt();			

    m_EfficiencyHistos[0]["ProbeTauEta_Histo"]->Fill(eta);	
    m_EfficiencyHistos[0]["ProbeTauPhi_Histo"]->Fill(phi);	
    m_EfficiencyHistos[0]["ProbeTauPt_Histo"]->Fill(pt);	
    
    TauL1tPair pairBestCand((*probeTauIt),0);    
    l1tIt = l1tContainer.begin();
    
    for(; l1tIt!=l1tEnd; ++l1tIt) {
      
      TauL1tPair pairTmpCand((*probeTauIt),&(*l1tIt));

      if (pairTmpCand.dR() < m_MaxL1tTauDR && pairTmpCand.l1tPt() > pairBestCand.l1tPt())
	pairBestCand = pairTmpCand;

    }
    
    m_TauL1tPairs.push_back(pairBestCand);
  
    m_ControlHistos["TauL1tDeltaR"]->Fill(pairBestCand.dR());

  }

}

//_____________________________________________________________________
bool L1TEfficiencyTaus_Offline::matchHlt(edm::Handle<TriggerEvent>  & triggerEvent, const reco::Muon * muon) {


  double matchDeltaR = 9999;

  TriggerObjectCollection trigObjs = triggerEvent->getObjects();

  vector<int>::const_iterator trigIndexIt  = m_trigIndices.begin();
  vector<int>::const_iterator trigIndexEnd = m_trigIndices.end();
  
  for(; trigIndexIt!=trigIndexEnd; ++trigIndexIt) {

    const vector<string> moduleLabels(m_hltConfig.moduleLabels(*trigIndexIt));
    const unsigned moduleIndex = m_hltConfig.size((*trigIndexIt))-2;
    const unsigned hltFilterIndex = triggerEvent->filterIndex(InputTag(moduleLabels[moduleIndex],
								       "",m_trigProcess));
    
    if (hltFilterIndex < triggerEvent->sizeFilters()) {
      const Keys triggerKeys(triggerEvent->filterKeys(hltFilterIndex));
      const Vids triggerVids(triggerEvent->filterIds(hltFilterIndex));
      
      const unsigned nTriggers = triggerVids.size();
      for (size_t iTrig = 0; iTrig < nTriggers; ++iTrig) {
        const TriggerObject trigObject = trigObjs[triggerKeys[iTrig]];
	
        double dRtmp = deltaR((*muon),trigObject);
        if (dRtmp < matchDeltaR) matchDeltaR = dRtmp;
	
      }
    }
  }
  
  return (matchDeltaR < m_MaxHltTauDR);

}


//define this as a plug-in
DEFINE_FWK_MODULE(L1TEfficiencyTaus_Offline);
