# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TDQMOffline")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")			
process.load('DQMOffline.Configuration.DQMOffline_cff')		
process.load('Configuration.EventContent.EventContent_cff')		

import FWCore.ParameterSet.Config as cms

# DQM file saver module
dqmSaver = cms.EDAnalyzer("DQMFileSaver",
    # Possible conventions are "Online", "Offline" and "RelVal".
    convention = cms.untracked.string('Offline'),
    # Save files in plain ROOT or encode ROOT objects in ProtocolBuffer
    fileFormat = cms.untracked.string('ROOT'),
    # Name of the producer.
    producer = cms.untracked.string('DQM'),
    # Name of the processing workflow.
    workflow = cms.untracked.string(''),
    # Directory in which to save the files.
    dirName = cms.untracked.string('.'),
    # Only save this directory
    filterName = cms.untracked.string(''),
    # Version name to be used in file name.
    version = cms.untracked.int32(1),
    # runIsComplete
    runIsComplete = cms.untracked.bool(False),

    # Save file every N lumi sections (-1: disabled)
    saveByLumiSection = cms.untracked.int32(-1),
    # Save file every N runs (-1: disabled)
    saveByRun = cms.untracked.int32(-1),
    # Save file at the end of the job
    saveAtJobEnd = cms.untracked.bool(True),

    # Ignore run number for MC data (-1: disabled)
    forceRunNumber = cms.untracked.int32(-1),			######
 

    # Control reference saving (default / skip / qtests / all)
    referenceHandling = cms.untracked.string('all'),
    # Control which references are saved for qtests (default: STATUS_OK)
    referenceRequireStatus = cms.untracked.int32(100)		########

)


process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.source = cms.Source('PoolSource',
        fileNames = cms.untracked.vstring(
'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/BAC68418-1193-E611-B748-02163E01268A.root'
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/3625FA62-2193-E611-ACB2-02163E0122A4.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/3835CBF2-2193-E611-933C-02163E01436A.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/3C99085D-2193-E611-A7E0-02163E013675.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/467A7F46-2193-E611-A594-FA163E65E7CC.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/4C8A2FEE-2193-E611-8C34-02163E013596.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/50B9F5CC-2193-E611-BA00-FA163E0E0B28.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/54965F6E-2193-E611-9845-02163E014769.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/56F2F754-2193-E611-97BF-FA163E6D6D3A.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/5C64E585-2193-E611-A2F5-02163E012472.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/5C991D46-2193-E611-8CA9-FA163E04EE04.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/66EEF158-2193-E611-B751-02163E0141FE.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/700942D9-2193-E611-A52D-FA163EEEC605.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/74919D65-2193-E611-9F18-02163E011D62.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/78714953-2193-E611-9FE8-FA163E99D766.root'
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/7E3F4EF7-2193-E611-B151-02163E0134BD.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/847867EF-2193-E611-B8A5-02163E0136B8.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/84C70062-2193-E611-845E-02163E012502.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/8A0A0358-2193-E611-B575-02163E012A7B.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/A0075BF1-2193-E611-9324-02163E0142FA.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/B01AAFB6-2193-E611-9A15-FA163ED130A7.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/B29DA0D6-2193-E611-8E59-FA163EF661A0.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/B8C722E8-2193-E611-87AA-FA163E1E16D7.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/BA23E464-2193-E611-9802-02163E0135D0.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/CC49FF4D-2193-E611-866F-FA163E3C23AA.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/CCE4B169-2193-E611-9065-FA163E308C86.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/CE7BC46D-2193-E611-8B96-02163E011DAF.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/D63DA554-2193-E611-954A-02163E014369.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/D65ADB56-2193-E611-A294-02163E01433C.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/D8324E6E-2193-E611-A0B6-02163E014283.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/DC4EF4DE-2193-E611-B433-02163E01430C.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/F2A183E1-2193-E611-9995-FA163EA5F2A3.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/BEE3EBF6-0E93-E611-A5ED-02163E011EE8.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/C21D587D-1093-E611-96D0-02163E0137BA.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/C884CFE2-1193-E611-835F-02163E0134B4.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/CE41F515-0F93-E611-9F57-02163E01452F.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/DA09DB78-1093-E611-AA5B-02163E014336.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/DA0C60D6-1193-E611-BB30-02163E0144FA.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/DC314FE3-1193-E611-B68B-02163E01368E.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/DCDAE409-1193-E611-8DD3-FA163E45F2F2.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/E4E65110-1193-E611-A9F5-FA163EA15AFB.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/E842B817-1193-E611-9323-02163E014301.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/E8498C0F-1193-E611-9EB0-02163E011D4E.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/E8C44CF4-0E93-E611-9AF5-FA163E24CACC.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/E8E3F81C-1193-E611-AF71-02163E011EDE.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/EAB1CFCC-0F93-E611-B42F-02163E01376F.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/EC124318-1193-E611-8C04-02163E01461D.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F0E2D8DD-0E93-E611-977F-FA163E394D18.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F25BC926-0F93-E611-AC57-02163E011EB3.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F41CD3DE-1193-E611-8315-02163E0126BB.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F48A7119-1193-E611-83C5-02163E01386D.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F4F6A50C-0F93-E611-9A03-02163E011C07.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/059/00000/F655581F-1193-E611-A325-02163E01452D.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/04A6D74F-2193-E611-8E85-FA163EBE57A9.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/08459B75-2193-E611-AAEB-02163E011DC2.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/105E39A2-2193-E611-805D-02163E014149.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/1C105C94-2193-E611-9493-02163E011AAE.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/2A6611F8-2193-E611-A0B3-02163E011FFD.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/2A9978EA-2193-E611-8E8C-02163E0134A0.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/2CE4205F-2193-E611-8AF6-02163E0118BE.root',
#'/store/data/Run2016H/SingleMuon/AOD/PromptReco-v2/000/283/067/00000/3625FA62-2193-E611-ACB2-02163E0122A4.root'
        )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))	

process.load('DQMOffline.L1Trigger.L1TEfficiencyHarvesting_cfi')

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')   

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')		

process.load("TrackingTools.Configuration.TrackingTools_cff")			


############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_ICHEP16_repro_v0', '')		

process.load('DQMOffline.L1Trigger.L1TEfficiencyTausOffline_cff')

#############################
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")		

process.l1tdumpeventsetup = cms.Path(process.dumpES)			

process.l1tEfficiencyTaus_offline.verbose   = cms.untracked.bool(False)
process.l1tEfficiencyTaus_offline.l1tInputTag  = cms.untracked.InputTag("caloStage2Digis:Tau")		
process.L1TTauSeq = cms.Sequence(
        process.l1tEfficiencyTaus_offline          
)

process.L1TTauPath = cms.Path(
	process.L1TTauSeq
)

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.dqmSaver.convention = 'Offline'
process.dqmSaver.workflow = '/RelVal/HLTriggerOffline/Tau'
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)

process.ppost = cms.EndPath(process.l1tEfficiencyHarvesting + process.dqmSaver)

