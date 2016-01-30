#
#  reEmul.py  configurable test of L1T re-emulation
#
#  Example Usage:
#
#     cmsRun reEmul.py                 : stage-2 re-emulation on 2015 zero-bias data
#
#     cmsRun reEmul.py era=stage1      : stage-1 re-emulation on 2015 zero-bias data
#
#     cmsRun reEmul.py era=stage2 output=mu_stage2.root input=/store/data/Run2015D/DoubleMuon/RAW/v1/000/260/627/00000/004EF961-6082-E511-BFB0-02163E011BC4.root max=10
#                                      : stage-2 re-emulation on 2015 double muon data
#
#  Limitations:
#
#     - stage-1 re-emulation will complain about DT digi unpacking... harmless.  Will go away when we use GT for data + overrides for stage-1 emulation.
#
#     - stage-1 re-emulation does not put muons into the ntuple... will be fixed when legacy->upgrade converter is provided for muons.
#

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
import os
import sys
import commands

options = VarParsing.VarParsing ('analysis')

options.register ('era',    'stage2',  VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The data taking Era: stage1 or stage2")
options.register ('output', 'DEFAULT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The output file name")
options.register ('input',  '',        VarParsing.VarParsing.multiplicity.list,      VarParsing.VarParsing.varType.string,  "The input files")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")
options.register ('skip',   '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The number of events to skip initially")

## options.input = '/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/00A76FFA-0C82-E511-B441-02163E01450F.root'
options.input = '/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root'
options.max  = 100 
options.skip = 0 

options.parseArguments()

if (options.era == 'stage1'):
    print "INFO: runnings L1T Stage-1 (2015) Re-Emulation"
    process = cms.Process("L1TReEmulation", eras.Run2_25ns)
elif (options.era == 'stage2'):
    print "INFO: runnings L1T Stage-2 (2016) Re-Emulation"    
    process = cms.Process("L1TReEmulation", eras.Run2_2016)
else:
    print "ERROR: unknown era:  ", options.era, "\n"
    exit(0)

if (options.output == 'DEFAULT'):
    if (eras.stage1L1Trigger.isChosen()):
        options.output ='l1t_stage1.root'
    if (eras.stage2L1Trigger.isChosen()):
        options.output ='l1t_stage2.root'
print "INFO: output:  ", options.output

print "INFO: input:  ", options.input
print "INFO: max:  ", options.max

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(options.input)
)
if options.skip > 0:
    process.source.skipEvents = cms.untracked.uint32(options.skip)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.max))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

#   For stage-1, we are re-emulating L1T based on the conditions in the GT, so
#   best for now to use MC GT, even when running over a data file, and just
#   ignore the main DT TP emulator warnings...  (In future we'll override only
#   L1T emulator related conditions, so you can use a data GT)
if (eras.stage1L1Trigger.isChosen()):
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
# Note:  For stage-2, all conditions are overriden by local config file.  Use data tag: 
if (eras.stage2L1Trigger.isChosen()):
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

#### Sim L1 Emulator Sequence:
process.load('Configuration.StandardSequences.RawToDigi_cff')
#process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('L1Trigger.Configuration.L1TReEmulateFromRAW_cff')
process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")


process.MessageLogger.suppressInfo = cms.untracked.vstring('Geometry', 'AfterSource')

process.L1TSeq = cms.Sequence(   process.RawToDigi        
                                   + process.L1TReEmulateFromRAW
#                                   + process.dumpED
#                                   + process.dumpES
)

process.L1TPath = cms.Path(process.L1TSeq)

process.hltL1T = cms.EDFilter( "HLTL1TSeed",
#    L1SeedsLogicalExpression = cms.string( "L1_SingleJet176 OR L1_SingleJet200" ),
    saveTags = cms.bool( True ),
#    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" ),
#    L1UseL1TriggerObjectMaps = cms.bool( True ),
#    L1UseAliasesForSeeding = cms.bool( True ),
#    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
#    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
#    L1NrBxInEvent = cms.int32( 3 ),
#    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
#    L1TechTriggerSeeding = cms.bool( False )
)

process.HLTPath  = cms.Path( process.hltL1T )

process.schedule = cms.Schedule(process.L1TPath)
process.schedule.extend([process.HLTPath])

# Re-emulating, so don't unpack L1T output, might not even exist...
# Also, remove uneeded unpackers for speed.
if (eras.stage2L1Trigger.isChosen()):
    process.L1TSeq.remove(process.gmtStage2Digis)
    process.L1TSeq.remove(process.caloStage2Digis)
    process.L1TSeq.remove(process.gtStage2Digis)
    process.L1TSeq.remove(process.siPixelDigis)
    process.L1TSeq.remove(process.siStripDigis)
    process.L1TSeq.remove(process.castorDigis)
    process.L1TSeq.remove(process.scalersRawToDigi)
    process.L1TSeq.remove(process.tcdsDigis)
if (eras.stage1L1Trigger.isChosen()):
    process.L1TSeq.remove(process.caloStage1Digis)
    process.L1TSeq.remove(process.caloStage1FinalDigis)
    process.L1TSeq.remove(process.gtDigis)
    process.L1TSeq.remove(process.siPixelDigis)
    process.L1TSeq.remove(process.siStripDigis)
    process.L1TSeq.remove(process.castorDigis)
    process.L1TSeq.remove(process.scalersRawToDigi)
    process.L1TSeq.remove(process.tcdsDigis)

print process.L1TSeq
print process.L1TReEmulateFromRAW
print process.HLTPath
#processDumpFile = open('config.dump', 'w')
#print >> processDumpFile, process.dumpPython()


#process.out = cms.OutputModule("PoolOutputModule", 
#   fileName = cms.untracked.string("l1tbmtf.root")
#)
#process.output_step = cms.EndPath(process.out)
#process.schedule.extend([process.output_step])
