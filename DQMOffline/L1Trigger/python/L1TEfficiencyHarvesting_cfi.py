import FWCore.ParameterSet.Config as cms

l1tEfficiencyHarvesting = cms.EDAnalyzer("L1TEfficiency_Harvesting",

  verbose  = cms.untracked.bool(False),
  plotCfgs = cms.untracked.VPSet(
    cms.untracked.PSet( dqmBaseDir = cms.untracked.string("L1T/Efficiency/Taus"),
                        plots      = cms.untracked.vstring("EffvsPt_passIso_Etaneg_","EffvsPt_passIso_Etapos_","EffvsPt_passIso_Barrel_","EffvsPt_passIso_Endcaps_","EffvsPt_passNoIso_Etaneg_","EffvsPt_passNoIso_Etapos_","EffvsPt_passNoIso_Barrel_","EffvsPt_passNoIso_Endcaps_","EffvsPhi_passNoIso_","EffvsEta_passNoIso_","EffvsPhi_passIso_","EffvsEta_passIso_","EffvsPt_passIso_","EffvsPt_passNoIso_")
                      )
  )
                                         
)
