import FWCore.ParameterSet.Config as cms

l1tEfficiencyTaus_offline = cms.EDAnalyzer("L1TEfficiencyTaus_Offline",

  verbose   = cms.untracked.bool(False),
  l1tPtCuts = cms.untracked.vint32(28),
  
  muonInputTag = cms.untracked.InputTag("muons"),
  tauInputTag = cms.untracked.InputTag("hpsPFTauProducer"),
  metInputTag = cms.untracked.InputTag("pfMet"),
  antiMuInputTag = cms.untracked.InputTag("hpsPFTauDiscriminationByTightMuonRejection3"),
  antiEleInputTag = cms.untracked.InputTag("hpsPFTauDiscriminationByMVA6LooseElectronRejection"),
  decayModeFindingInputTag = cms.untracked.InputTag("hpsPFTauDiscriminationByDecayModeFindingOldDMs"),
  comb3TInputTag = cms.untracked.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits"),

  l1tInputTag  = cms.untracked.InputTag("caloStage2Digis:Tau"),

  vtxInputTag = cms.untracked.InputTag("offlinePrimaryVertices"),
  bsInputTag  = cms.untracked.InputTag("offlineBeamSpot"),

  triggerNames = cms.untracked.vstring(
        "HLT_IsoMu22_v*"
      ),

  trigInputTag       = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "HLT"),
  trigProcess        = cms.untracked.string("HLT"),
  trigProcess_token  = cms.untracked.InputTag("TriggerResults","","HLT")
  
)
