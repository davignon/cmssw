#ifndef DQMOFFLINE_L1TRIGGER_L1TEFFICIENCYTAU_OFFLINE_H
#define DQMOFFLINE_L1TRIGGER_L1TEFFICIENCYTAU_OFFLINE_H

// system include files
#include <memory>
#include <unistd.h>

// user include files

#include "DataFormats/L1Trigger/interface/Muon.h"		 		
#include "DataFormats/L1Trigger/interface/BXVector.h"	
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TrackTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "TRegexp.h"
#include "TString.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

class TauL1tPair {

 public :

  TauL1tPair(const reco::PFTau *tau, const l1t::Tau *regTau) : 				
  m_tau(tau), m_regTau(regTau), m_eta(999.), m_phi_bar(999.), m_phi_end(999.) { };
    
  TauL1tPair(const TauL1tPair& tauL1tPair);

  ~TauL1tPair() { };

  double dR();
  double eta() const { return m_tau->eta(); };
  double phi() const { return m_tau->phi(); };
  double pt()  const { return m_tau->pt(); };

  double l1tPt() const { return m_regTau ? m_regTau->pt() : -1.; };					
  double l1tIso() const { return m_regTau ? m_regTau->hwIso() : -1.; };
  double l1tPhi() const { return m_regTau ? m_regTau->phi() : -5.; };					
  double l1tEta() const { return m_regTau ? m_regTau->eta() : -5.; };					

private :

  const reco::PFTau *m_tau;
  const l1t::Tau *m_regTau;					

  double m_eta;
  double m_phi_bar;
  double m_phi_end;

};

//
// DQM class declaration
//

class L1TEfficiencyTaus_Offline : public DQMEDAnalyzer {
  
public:
  
  L1TEfficiencyTaus_Offline(const edm::ParameterSet& ps);   // Constructor
  virtual ~L1TEfficiencyTaus_Offline();                     // Destructor
  
protected:
  
   // Luminosity Block
  virtual void beginLuminosityBlock(edm::LuminosityBlock const& lumiBlock, edm::EventSetup const& c);
  virtual void dqmEndLuminosityBlock  (edm::LuminosityBlock const& lumiBlock, edm::EventSetup const& c);
  virtual void dqmBeginRun(const edm::Run& run, const edm::EventSetup& iSetup);
  virtual void bookControlHistos(DQMStore::IBooker &);
  virtual void bookEfficiencyHistos(DQMStore::IBooker &ibooker, int ptCut);
  virtual void bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& run, const edm::EventSetup& iSetup) override;
  //virtual void analyze (const edm::Event& e, const edm::EventSetup& c);

private:

  void analyze (const edm::Event& e, const edm::EventSetup& c);

  // Helper Functions
  const reco::Vertex getPrimaryVertex(edm::Handle<reco::VertexCollection> & vertex,edm::Handle<reco::BeamSpot> & beamSpot);
  bool matchHlt(edm::Handle<trigger::TriggerEvent>  & triggerEvent, const reco::Muon * muon);

  // Cut and Matching

  void getTauL1tPairs(edm::Handle<l1t::TauBxCollection> & l1tCands);			

  void getTightMuons(edm::Handle<reco::MuonCollection> & muons, edm::Handle<reco::PFMETCollection> &mets, const reco::Vertex & vertex, edm::Handle<trigger::TriggerEvent> & trigEvent);
  void getProbeTaus(const edm::Event& e, edm::Handle<reco::PFTauCollection> & taus, edm::Handle<reco::MuonCollection> & muons, const reco::Vertex & vertex);
  //void getProbeTaus(edm::Handle<edm::TriggerResults> & trigResults,edm::Handle<trigger::TriggerEvent> & trigEvent);  
  
private:
  
  bool  m_verbose;

  HLTConfigProvider m_hltConfig;

  edm::ESHandle<MagneticField> m_BField;
  edm::ESHandle<Propagator> m_propagatorAlong;
  edm::ESHandle<Propagator> m_propagatorOpposite;

  // histos
  std::map<int, std::map<std::string, MonitorElement*> > m_EfficiencyHistos;
  std::map<std::string, MonitorElement*> m_ControlHistos;

  // helper variables
  std::vector<const reco::Muon*>  m_TightMuons;
  std::vector<const reco::PFTau*>  m_ProbeTaus;
  std::vector<TauL1tPair>  m_TauL1tPairs;

  std::vector<reco::PFTauCollection>  m_RecoTaus;		
  std::vector<l1t::TauBxCollection>  m_L1tTaus;		  
  std::vector<reco::PFTau>  m_RecoRecoTaus;		
  BXVector<l1t::Tau>  m_L1tL1tTaus;		  
  
  // config params
  std::vector<int> m_L1tPtCuts;

  edm::EDGetTokenT<reco::PFTauCollection> m_TauInputTag;
  edm::EDGetTokenT<reco::MuonCollection> m_MuonInputTag;
  edm::EDGetTokenT<reco::PFMETCollection> m_MetInputTag;
  edm::EDGetTokenT<l1t::TauBxCollection> m_L1tInputTag;
  edm::EDGetTokenT<reco::PFTauDiscriminator> m_AntiMuInputTag;
  edm::EDGetTokenT<reco::PFTauDiscriminator> m_AntiEleInputTag;
  edm::EDGetTokenT<reco::PFTauDiscriminator> m_Comb3TInputTag;
  edm::EDGetTokenT<reco::PFTauDiscriminator> m_DecayModeFindingInputTag;

  edm::EDGetTokenT<reco::VertexCollection> m_VtxInputTag;
  edm::EDGetTokenT<reco::BeamSpot> m_BsInputTag;

  edm::EDGetTokenT<trigger::TriggerEvent> m_trigInputTag;
  std::string m_trigProcess;
  edm::EDGetTokenT<edm::TriggerResults> m_trigProcess_token;
  std::vector<std::string> m_trigNames;
  std::vector<int> m_trigIndices;

  float m_MaxTauEta;
  float m_MaxL1tTauDR;
  float m_MaxHltTauDR;
  // CB ignored at present
  // float m_MinMuonDR;
  
};

#endif
