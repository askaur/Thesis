#ifndef uHTRMonitoringLumi_hh_included
#define uHTRMonitoringLumi_hh_included 1

#include "hcal/monitor/Monitorable.hh"
#include "hcal/monitor/Integer.hh"
#include "hcal/monitor/Table.hh"
#include "toolbox/BSem.h"
#include "log4cplus/logger.h"
#include <map>

namespace hcal {
  namespace uhtr { 

    class uHTR;

    class uHTRMonitoringLumi : public hcal::monitor::UpdateListener {
    public:
      uHTRMonitoringLumi(log4cplus::Logger logger);
      virtual ~uHTRMonitoringLumi() { }
      void add(uHTR* card);
      void setup(int readoutid, const std::vector<int>& crateids);

      std::vector<hcal::monitor::Monitorable*> getMonitorables();
      virtual void update(hcal::monitor::Monitorable* item);
      void takeStartLock();
      void releaseStartLock();
      void reset();
      void countOverflow(int crate, int slot, int monitoringhist);
    private:
      void updateLumiStatus();
      void createTables();
      std::vector<int> m_crates;
      hcal::monitor::Integer m_readoutId;
      std::map<int,uHTR*> m_uHTRcards;
      hcal::monitor::Table m_LumiStatus;
      log4cplus::Logger m_logger;
      ::toolbox::BSem m_startLock;
      std::map<int,int> m_overflows;
      std::map<int,hcal::toolbox::RateTool> m_overflowRate;

    };

  }
}

#endif // uHTRMonitoringLumi_hh_included
