#ifndef uHTRMonitoring_hh_included
#define uHTRMonitoring_hh_included 1

#include "hcal/monitor/Monitorable.hh"
#include "hcal/monitor/Integer.hh"
#include "hcal/monitor/Table.hh"
#include "toolbox/BSem.h"
#include "log4cplus/logger.h"
#include <map>

namespace hcal {
  namespace uhtr { 

    class uHTR;

    class uHTRMonitoring : public hcal::monitor::UpdateListener {
    public:
      uHTRMonitoring(log4cplus::Logger logger);
      virtual ~uHTRMonitoring() { }
      void add(uHTR* card);
      void setup(int icrate);
      std::vector<hcal::monitor::Monitorable*> getMonitorables();
      virtual void update(hcal::monitor::Monitorable* item);
      void takeStartLock();
      void releaseStartLock();
      void resetHistory();
      void reset();
      std::map<std::string, double> getUHTRSensors( uHTR* uHTR ) ;
    private:
      void updatePerFiber();
      void updatePeruHTR() {} ;
      void updateTempVolt();
      void updateDAQStatus();
      void updateDTCStatus();
      void createTables();
      int m_crate;
      hcal::monitor::Integer m_crateId;
      std::map<int,uHTR*> m_uHTRcards;
      hcal::monitor::Table m_perFiber;
      hcal::monitor::Table m_peruHTR;
      hcal::monitor::Table m_TempVolt; 
      hcal::monitor::Table m_DAQStatus;
      hcal::monitor::Table m_DTCStatus;
      log4cplus::Logger m_logger;
      ::toolbox::BSem m_startLock;
    };

  }
}
#endif // uHTRMonitoring_hh_included
