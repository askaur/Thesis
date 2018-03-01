#ifndef uHTRManager_hh_included
#define uHTRManager_hh_included 1

#include "hcal/Application.hh"
#include "hcal/toolbox/DynamicBag.hh"
#include "hcal/uhtr/uHTRMonitoring.hh"
#include "hcal/toolbox/CrateRenderer.hh"
#include "hcal/toolbox/HTMLHistoRender.hh"
#include "xdata/Boolean.h"

namespace hcal {
  namespace uhtr {
    class uHTR;
  }
}

namespace uhal {
  class ConnectionManager;
}

namespace hcal {

  class uHTRManager : hcal::Application {
  public:
    XDAQ_INSTANTIATOR();
    uHTRManager(xdaq::ApplicationStub* as);
    ~uHTRManager();
  protected:
    virtual void init()                throw (hcal::exception::Exception);
    virtual void enable()              throw (hcal::exception::Exception);
    virtual void disable()             throw (hcal::exception::Exception);
    virtual void webPeek(xgi::Input*,
			 xgi::Output*) throw (hcal::exception::Exception);    
    virtual void reset()               throw (hcal::exception::Exception);
    
  private:
    void webSingleCard(xgi::Output*, hcal::uhtr::uHTR* card, int myslot,
		       const std::string& item,
		       const std::map<std::string, std::string>& params);

    void webSingleCard_Link( xgi::Output*, hcal::uhtr::uHTR* card, int myslot,
		       const std::string& item,
		       const std::map<std::string, std::string>& params);

    void web_uHTR_DAQ(xgi::Output*, hcal::uhtr::uHTR* card, int myslot,
		       const std::string& item,
		       const std::map<std::string, std::string>& params);

    void web_uHTR_Trigger(xgi::Output*, hcal::uhtr::uHTR* card, int myslot,
		       const std::string& item,
		       const std::map<std::string, std::string>& params);

    void setupCrateView() ; 

    std::string makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item);

    static const int FM_GROUNDSTATE = 0x0001; // function mask for destructive ground-state 
    static const int FM_MON         = 0x0004; // function maks for monitoring
    static const int FM_DAQ         = 0x0008; // function mask for DAQ activities
    static const int FM_TRIG        = 0x0010; // function mask for TRIGGER activities
    static const int FM_LUMI        = 0x0020; // function mask for LUMI activities

    xdata::Integer m_crateId;
    xdata::Boolean m_present;
    int m_fe_dataRate;

    class CardInfo {
    public:
      CardInfo();      
      void registerFields(xdata::Bag<uHTRManager::CardInfo>* bag);
      // these are special and needed externally
      xdata::Boolean present;
      xdata::Integer crateId;
      xdata::Integer slotId;

      // these are local
      int gtx_speed ;
      // links 
      int orbitDelay;
      bool autoAlign;
      int autoOrbitPresamples;
      // DAQ parameters
      int bcOffsetDAQ;
      bool noZS, zsMarkAndPass, zsSumByTwo;
      int nsamples, npresamples, pipeline, zsMask, zsSimpleThreshold;
      int tp_samples, tp_presamples;
      bool zs_tp_enable, zs_tp_soionly;
      int functionMask;
      // Trigger Parameters
      //int n_addr_bits, n_data_bits, n_luts ;
      //uint32_t base_address,  stride ; 
      //bool last_group ;

    };

    static const int MAX_CARDS_PER_CRATE = 12;
    bool m_firstInitDone;

    hcal::uhtr::uHTR* m_uHTRs[MAX_CARDS_PER_CRATE];
    hcal::toolbox::DynamicBag<CardInfo> m_info[MAX_CARDS_PER_CRATE];
    hcal::uhtr::uHTRMonitoring m_monitoringHelper;

    uhal::ConnectionManager * m_connection;

    // crate display
    hcal::toolbox::CrateRenderer*       m_crateView ;
    //    hcal::toolbox::HTMLHistoRender    m_HTMLHisto;
  };

}

#endif // uHTRManager_hh_included 
