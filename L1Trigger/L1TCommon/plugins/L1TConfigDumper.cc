
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1RCTParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"

class L1TConfigDumper : public edm::one::EDAnalyzer<>  {
   public:
      explicit L1TConfigDumper(const edm::ParameterSet&);
      ~L1TConfigDumper();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
};


L1TConfigDumper::L1TConfigDumper(const edm::ParameterSet& iConfig)

{

}


L1TConfigDumper::~L1TConfigDumper()
{
}

void
L1TConfigDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::ESHandle< L1RCTParameters > rctParam;
   iSetup.get< L1RCTParametersRcd >().get( rctParam) ;

   // config driven printout of payloads:
   //rctParam->print(std::cout);

}

void 
L1TConfigDumper::beginJob()
{
}

void 
L1TConfigDumper::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TConfigDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TConfigDumper);
