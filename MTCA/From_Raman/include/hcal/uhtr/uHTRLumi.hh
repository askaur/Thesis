#ifndef uHTRLumi_hh_included
#define uHTRLumi_hh_included 1

#include "hcal/Application.hh"
#include "hcal/toolbox/DynamicBag.hh"
#include "hcal/uhtr/uHTRMonitoringLumi.hh"
#include "hcal/toolbox/CrateRenderer.hh"
#include "xdata/Boolean.h"

class TFile;

namespace toolbox {
  class Task;
}

namespace hcal {
  namespace uhtr {
    class uHTR;
  }
}

namespace uhal {
  class ConnectionManager;
}


namespace hcal {

  class CommandQueue;

  class uHTRLumi : hcal::Application {
  public:
    XDAQ_INSTANTIATOR();
    uHTRLumi(xdaq::ApplicationStub* as);
    ~uHTRLumi();

    int runTask();
    static const char* lumi_hName[] ;

  protected:
    virtual void init()                throw (hcal::exception::Exception);
    virtual void enable()              throw (hcal::exception::Exception);
    virtual void disable()             throw (hcal::exception::Exception);
    virtual void read()                throw (hcal::exception::Exception);
    virtual void webPeek(xgi::Input*,
			 xgi::Output*) throw (hcal::exception::Exception);    
    virtual void reset()               throw (hcal::exception::Exception);
    
  private:
    void webSingleCard(xgi::Output*, hcal::uhtr::uHTR* card, int myslot,
		       const std::string& item ) ;
    //		       const std::map<std::string, std::string>& params);
    void setupCrateView( int cid );

    //void webSingleCardTest(hcal::uhtr::uHTR* card, int myslot,
    //	 	       const std::string& item,
    //		       const std::map<std::string, std::string>& params);

    std::string makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item);
    void programLUT( hcal::uhtr::uHTR* card ); 

    class CardInfo {
    public:
      CardInfo();      
      //void registerFields(xdata::Bag<uHTRLumi::CardInfo>* bag);
      // these are special and needed externally
      xdata::Boolean present;
      xdata::Integer crateId;
      xdata::Integer slotId;

      // these are local
      int n_lhc_orb;
      int n_cms_orb;
      int lhc_threshold;
      int cms1_threshold;
      int cms2_threshold;

      std::vector<bool> lhcMask;
      std::vector<bool> cmsMask;
    };

    static const int MAX_CARDS_PER_CRATE = 12;
    static const int MAX_CRATE = 3;
    //bool m_firstInitDone;
    int m_readoutId;

    uhal::ConnectionManager*            m_connection;
    int                                 m_crateId[MAX_CRATE];
    hcal::uhtr::uHTR*                   m_uHTRs[MAX_CARDS_PER_CRATE * MAX_CRATE];
    hcal::uhtr::uHTRMonitoringLumi      m_monitoringHelper;
    CardInfo                            m_info[MAX_CARDS_PER_CRATE * MAX_CRATE];

    TFile* hfile ;
    std::string m_histoLoc ; 

    // for the parallel operation
    CommandQueue* m_cmdQueue;
    ::toolbox::Task* m_task;

    // crate display
    hcal::toolbox::CrateRenderer*       m_crateView[MAX_CRATE] ;
    std::vector<int> h_itemId ; 

  };

}

#endif // uHTRLumi_hh_included 
