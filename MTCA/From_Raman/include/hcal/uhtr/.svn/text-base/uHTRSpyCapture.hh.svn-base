#ifndef uHTRSpyCapture_hh_included
#define uHTRSpyCapture_hh_included 1

#include "hcal/ReadoutApplication.hh"
#include "hcal/toolbox/DynamicBag.hh"
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

  class uHTRSpyCapture : public hcal::ReadoutApplication {
  public:
    XDAQ_INSTANTIATOR();
    uHTRSpyCapture(xdaq::ApplicationStub* as);
    ~uHTRSpyCapture();
    
  protected:
    virtual void init()                throw (hcal::exception::Exception);
    virtual void enable()              throw (hcal::exception::Exception);
    virtual void reset()               throw (hcal::exception::Exception);

    virtual int readout(unsigned int expected, unsigned int* eventNumbers, std::vector< ::toolbox::mem::Reference* >& data) throw (hcal::exception::Exception);

    virtual unsigned int getAverageEventSize() const;
    virtual unsigned int getSourceId() const;

  private:
    std::string makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item);

    static const int MAX_FIBERS_PER_CARD=24; 
    int MAX_CHANNELS_PER_FIBER;

    int m_fedId;
    int m_crateId;
    int m_slotId;

    int m_nBX_before;
    int m_nBX_after;

    int n_events_requested;
    ExternalTriggerListType* m_listOfTriggers;
    std::string m_triggerInterchangeName;


    ::toolbox::BSem m_accessProtect;
    uhal::ConnectionManager*            m_connection;
    hcal::uhtr::uHTR*                   m_uHTR;

  };

}

#endif // uHTRSpyCapture_hh_included 
