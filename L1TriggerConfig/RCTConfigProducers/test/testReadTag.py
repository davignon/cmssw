import FWCore.ParameterSet.Config as cms

process = cms.Process('L1TEST')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring('L1-O2O')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

process.source = cms.Source('EmptySource')

# Load conditions
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG')
process.GlobalTag.globaltag = cms.string('POSTLS162_V2::All')

from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
process.newRCTConfig = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    connect = cms.string('sqlite_file:l1config.db'),
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('L1RCTParametersRcd'),
            tag = cms.string('rctTag')
        )
    )
)

process.prefer("newRCTConfig")

process.l1RCTParametersTest = cms.EDAnalyzer("L1RCTParametersTester")

process.p = cms.Path(
    process.l1RCTParametersTest
)

