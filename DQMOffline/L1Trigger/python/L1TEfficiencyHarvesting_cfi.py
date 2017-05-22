import FWCore.ParameterSet.Config as cms

l1tEfficiencyMuons_Harvesting = cms.EDAnalyzer("L1TEfficiency_Harvesting",
                                               verbose  = cms.untracked.bool(False),
                                               plotCfgs = cms.untracked.VPSet(
        cms.untracked.PSet( dqmBaseDir = cms.untracked.string("L1T/Efficiency/Muons"),
                            plots      = cms.untracked.vstring("EffvsPt16","EffvsEta16","EffvsPhi16",
                                                               "EffvsPt_OPEN_16","EffvsEta_OPEN_16","EffvsPhi_OPEN_16",
                                                               "EffvsPt_DOUBLE_16","EffvsEta_DOUBLE_16","EffvsPhi_DOUBLE_16",
                                                               "EffvsPt_SINGLE_16","EffvsEta_SINGLE_16","EffvsPhi_SINGLE_16",
                                                               "EffvsPt20","EffvsEta20","EffvsPhi20",
                                                               "EffvsPt_OPEN_20","EffvsEta_OPEN_20","EffvsPhi_OPEN_20",
                                                               "EffvsPt_DOUBLE_20","EffvsEta_DOUBLE_20","EffvsPhi_DOUBLE_20",
                                                               "EffvsPt_SINGLE_20","EffvsEta_SINGLE_20","EffvsPhi_SINGLE_20",
                                                               "EffvsPt25","EffvsEta25","EffvsPhi25",
                                                               "EffvsPt_OPEN_25","EffvsEta_OPEN_25","EffvsPhi_OPEN_25",
                                                               "EffvsPt_DOUBLE_25","EffvsEta_DOUBLE_25","EffvsPhi_DOUBLE_25",
                                                               "EffvsPt_SINGLE_25","EffvsEta_SINGLE_25","EffvsPhi_SINGLE_25"
                            )
        )
    )


l1tEfficiencyTaus_Harvesting = cms.EDAnalyzer("L1TEfficiency_Harvesting",
  verbose  = cms.untracked.bool(False),
  plotCfgs = cms.untracked.VPSet(
    cms.untracked.PSet( dqmBaseDir = cms.untracked.string("L1T/Efficiency/Taus"),
                        plots      = cms.untracked.vstring("EffvsPt_passIso_Etaneg_","EffvsPt_passIso_Etapos_","EffvsPt_passIso_Barrel_","EffvsPt_passIso_Endcaps_","EffvsPt_passNoIso_Etaneg_","EffvsPt_passNoIso_Etapos_","EffvsPt_passNoIso_Barrel_","EffvsPt_passNoIso_Endcaps_","EffvsPhi_passNoIso_","EffvsEta_passNoIso_","EffvsPhi_passIso_","EffvsEta_passIso_","EffvsPt_passIso_","EffvsPt_passNoIso_")
                      )
  )
                                         
)
