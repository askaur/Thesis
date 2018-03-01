#ifndef uHTRSource_hh_included
#define uHTRSource_hh_included 1

#include "hcal/ReadoutApplication.hh"
#include "hcal/toolbox/DynamicBag.hh"
#include "hcal/uhtr/uHTRMonitoringLumi.hh"
#include "hcal/toolbox/CrateRenderer.hh"
#include "xdata/Boolean.h"
#include "hcal/trig/TriggerMessageBlock.h"
#include "hcal/toolbox/ThreadSafeQueue.hh"

typedef hcal::toolbox::ThreadSafeQueue<HcalExtendedTriggerMessageBlock> ExternalTriggerListType;

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

  class uHTRSource : public hcal::ReadoutApplication {
  public:
    XDAQ_INSTANTIATOR();
    uHTRSource(xdaq::ApplicationStub* as);
    ~uHTRSource();
    
    /** \brief SOAP callback for turning on and off channels
	See hcal::soap::ChannelActivation for the parsing method used.
    */
    xoap::MessageReference OnChannelControl(xoap::MessageReference msg);    

  protected:
    virtual void init()                throw (hcal::exception::Exception);
    virtual void enable()              throw (hcal::exception::Exception);
    virtual void reset()               throw (hcal::exception::Exception);

    virtual int readout(unsigned int expected, unsigned int* eventNumbers, std::vector< ::toolbox::mem::Reference* >& data) throw (hcal::exception::Exception);

    virtual unsigned int getAverageEventSize() const;
    virtual unsigned int getSourceId() const;

  private:
    std::string makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item);

    class CardInfo {
    public:
      CardInfo();      
      //void registerFields(xdata::Bag<uHTRSource::CardInfo>* bag);
      // these are special and needed externally
      xdata::Boolean present;
      xdata::Integer crateId;
      xdata::Integer slotId;
      std::vector<bool> readMe;
      int nread;
    };

    static const int MAX_CARDS_PER_CRATE = 12;
    static const int MAX_CRATE = 12;
    static const int MAX_FIBERS_PER_CARD=24; 
    int MAX_CHANNELS_PER_FIBER;

    int m_fedId;
    bool m_separateCapIDs;
    int m_binsToRead;
    int m_nOrbits;
    unsigned int n_event;
    int n_events_requested;
    ExternalTriggerListType* m_listOfTriggers;
    std::string m_triggerInterchangeName;
 
    ::toolbox::BSem m_accessProtect;
    uhal::ConnectionManager*            m_connection;
    int                                 m_crateId[MAX_CRATE];
    hcal::uhtr::uHTR*                   m_uHTRs[MAX_CARDS_PER_CRATE * MAX_CRATE];
    CardInfo                            m_info[MAX_CARDS_PER_CRATE * MAX_CRATE];

  };

}

#endif // uHTRSource_hh_included 
