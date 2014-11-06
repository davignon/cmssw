// -*- C++ -*-
//
// Package:    RCTConfigWriter
// Class:      RCTConfigWriter
// 
/**\class RCTConfigWriter RCTConfigWriter.h L1TriggerConfig/RCTConfigWriter/src/RCTConfigWriter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Nicholas Smith
//         Created:  Wed November 5 2014
//
//
// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1RCTParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondTools/L1Trigger/interface/DataWriter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>

#include <iostream>

//
// class declaration
//

class L1RCTParametersWriter : public edm::EDAnalyzer {
public:
  explicit L1RCTParametersWriter(const edm::ParameterSet&) {}
  virtual  ~L1RCTParametersWriter() {}
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;  

};



void L1RCTParametersWriter::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup)
{
    l1t::DataWriter dataWriter;
  
    std::string token = dataWriter.writePayload(evSetup, "L1RCTParametersRcd@L1RCTParameters");
    if ( dataWriter.updateIOV("L1RCTParametersRcd", token, 1, false) ) std::cout << "IOV updated!" << std::endl;
    std::cout << "Payload token = " << token << std::endl;
}

DEFINE_FWK_MODULE(L1RCTParametersWriter);



