import FWCore.ParameterSet.Config as cms

from L1Trigger.L1Menu.customL1Ntuple_cfg import *

process.p.remove(process.l1RecoTreeProducer)
process.p.remove(process.l1MuonRecoTreeProducer)
process.p.remove(process.l1MenuTreeProducer)
process.p.remove(process.csctfDigis)

# edit here

OUTFILE="L1Tree.root"
NEVTS=200

process.TFileService.fileName=OUTFILE
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEVTS) )
process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## readFiles.extend( ['file:///data2/battilan/L1Trigger/62X_RAW_RECO.root'] )
## readFiles.extend( ['file:/uscmst1b_scratch/lpc1/lpctrig/apana/L1Upgrade/262AA156-744A-E311-9829-002618943945.root'] )
## readFiles.extend( ['root://eoscms.cern.ch//eos/cms/store/group/comm_trigger/L1Trigger/apana/DYJetsToLL_M-50_13TeV-pythia6_Fall13dr-tsg_PU40bx25__skim_150_1_6UN.root'] )
## readFiles.extend( ['root://xrootd.unl.edu//store/data/Commissioning2014/Cosmics/RAW/v3/000/228/929/00000/509C6C80-3164-E411-92E0-02163E00FFE1.root'] )

#readFiles.extend( ['root://lxcms02//data2/p/pellicci/L1DPG/root/RelValTTbar_730_GENSIMRECO.root'] )
#readFiles.extend(['root://cmsxrootd.fnal.gov///store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0074D53B-A577-E311-8655-0026189438BD.root'])
readFiles.extend( ['file:/uscms_data/d3/cbrainer/0074D53B-A577-E311-8655-0026189438BD.root'] )

## processDumpFile = open('config.dump', 'w')
## print >> processDumpFile, process.dumpPython()
