#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRMonitoringLumi.hh"
#include <string>
#include <iostream>
#include "toolbox/string.h"
#include "xcept/tools.h"

static const std::string label_ReadoutId("READOUTID");
static const std::string label_Id("ID");
static const std::string label_Crate("CRATEID");
static const std::string label_Slot("SLOT");  

//LUMI
static const std::string label_Lumi_Bc0_Rate("LUMI_BC0_RATE");
static const std::string label_Lumi_Overflow_LHC("LUMI_OVERFLOW_LHC");
static const std::string label_Lumi_Overflow_LHC_Valid("LUMI_OVERFLOW_LHC_VALID");
static const std::string label_Lumi_Overflow_CMS1("LUMI_OVERFLOW_CMS1");
static const std::string label_Lumi_Overflow_CMS2("LUMI_OVERFLOW_CMS2");
static const std::string label_Lumi_Overflow_CMS_Valid("LUMI_OVERFLOW_CMS_VALID");
static const std::string label_Lumi_Overflow_CMS_Et("LUMI_OVERFLOW_CMS_ET");
static const std::string label_Lumi_OverflowRate_LHC("LUMI_OVERFLOWRATE_LHC");
static const std::string label_Lumi_OverflowRate_LHC_Valid("LUMI_OVERFLOWRATE_LHC_VALID");
static const std::string label_Lumi_OverflowRate_CMS1("LUMI_OVERFLOWRATE_CMS1");
static const std::string label_Lumi_OverflowRate_CMS2("LUMI_OVERFLOWRATE_CMS2");
static const std::string label_Lumi_OverflowRate_CMS_Valid("LUMI_OVERFLOWRATE_CMS_VALID");
static const std::string label_Lumi_OverflowRate_CMS_Et("LUMI_OVERFLOWRATE_CMS_ET");

namespace hcal {
  namespace uhtr {

    uHTRMonitoringLumi::uHTRMonitoringLumi(log4cplus::Logger logger) : 
      m_readoutId(label_ReadoutId),
      m_LumiStatus("LumiStatus"),
      m_logger(logger),
      m_startLock(::toolbox::BSem::FULL)
    {
      m_LumiStatus.addUpdateListener(this);
    }

    static int indexMaker(const uHTR* card, int histogram=0) {
      return histogram*100000+card->crate()*100+card->slot();
    }
    static int indexMaker(int iCrate, int iSlot, int histogram=0) {
      return histogram*100000+iCrate*100+iSlot;
    }

    void uHTRMonitoringLumi::add(uHTR* card) {
      m_uHTRcards.insert(std::pair<int,uHTR*>(indexMaker(card),card));
    }

    void uHTRMonitoringLumi::takeStartLock() {
      m_startLock.take();
    }
    void uHTRMonitoringLumi::releaseStartLock() {
      m_startLock.give();
    }

    void uHTRMonitoringLumi::setup(int readoutid, const std::vector<int>& icrates) {
      m_crates=icrates;
      m_readoutId.value()=readoutid;
      createTables();
    }

    void uHTRMonitoringLumi::update(hcal::monitor::Monitorable* item) {
      takeStartLock();
      try {
	if (item==&m_LumiStatus) updateLumiStatus();
      } catch (xcept::Exception& e) {
	LOG4CPLUS_ERROR(m_logger,xcept::stdformat_exception_history(e));    
      }
      releaseStartLock();
      LOG4CPLUS_DEBUG(m_logger,"Updating uHTRMonitoringLumi tables");
    }

    std::vector<hcal::monitor::Monitorable*> uHTRMonitoringLumi::getMonitorables() {
      std::vector<hcal::monitor::Monitorable*> stuff;
      stuff.push_back(&m_readoutId);
      stuff.push_back(&m_LumiStatus);
      return stuff;
    }
    
    void uHTRMonitoringLumi::createTables() {
      // multipliers for hash computation
      const int multipliers[] = {1, 10, 1000}; // 1*readoutid + 10*crate + 1000*slot *10000*fiber  

      // Lumi status monitoring"LHC", "LHC_VALID", "CMS1", "CMS2", "CMS_VALID", "CMS_ET"

      m_LumiStatus.setupHash(3, multipliers);
      m_LumiStatus.addIdColumn(label_Id);
      m_LumiStatus.addColumn(label_Crate,"int");
      m_LumiStatus.addColumn(label_Slot,"int");
      m_LumiStatus.addColumn(label_Lumi_Bc0_Rate,"double"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_LHC,"int"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_LHC_Valid,"int"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_CMS1,"int"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_CMS2,"int"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_CMS_Valid,"int"); 
      m_LumiStatus.addColumn(label_Lumi_Overflow_CMS_Et,"int"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_LHC,"double"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_LHC_Valid,"double"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_CMS1,"double"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_CMS2,"double"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_CMS_Valid,"double"); 
      m_LumiStatus.addColumn(label_Lumi_OverflowRate_CMS_Et,"double"); 

      for (std::vector<int>::const_iterator icrate=m_crates.begin(); icrate!=m_crates.end(); icrate++) {
	for(int iSlot = 1; iSlot <= 12; iSlot++) {
	  std::map<int,uHTR*>::iterator uHTRi = m_uHTRcards.find(indexMaker(*icrate,iSlot));
	  if (uHTRi==m_uHTRcards.end() ) continue;
	  const int hash[] = { m_readoutId.value(), *icrate, iSlot }; 
	  m_LumiStatus.addRow(hash);
	  m_LumiStatus.setValueAt( hash, label_Crate, *icrate );
	  m_LumiStatus.setValueAt( hash, label_Slot, iSlot );
	}
      }
      updateLumiStatus();
    }
    
    void uHTRMonitoringLumi::updateLumiStatus() {
      LOG4CPLUS_DEBUG(m_logger, "Updating uHTR Lumi status");
      
      for (std::vector<int>::const_iterator icrate=m_crates.begin(); icrate!=m_crates.end(); icrate++) {
	for(int iSlot = 1; iSlot <= 12; iSlot++) {
	  std::map<int,uHTR*>::iterator uHTRi = m_uHTRcards.find(indexMaker(*icrate,iSlot));
	  if (uHTRi==m_uHTRcards.end() ) continue;
	  const int hash[] = { m_readoutId.value(), *icrate, iSlot }; 
	  LOG4CPLUS_DEBUG(m_logger, ::toolbox::toString("Getting monitoring information for slot=%d",iSlot));
	  uHTR* uHTR = (*uHTRi).second;

	  uint32_t status, errors;
	  float lumibc0,triggerbc0;
	  uHTR->lumi_link_status(status, lumibc0, triggerbc0,errors);

	  m_LumiStatus.setValueAt( hash, label_Lumi_Bc0_Rate, lumibc0);
	  m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_LHC, m_overflows[indexMaker(*icrate, iSlot,1)]);
	  m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_LHC_Valid, m_overflows[indexMaker(*icrate, iSlot,2)]);
	  m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_CMS1, m_overflows[indexMaker(*icrate, iSlot,3)]);
	  m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_CMS2, m_overflows[indexMaker(*icrate, iSlot,4)]);	 
          m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_CMS_Valid, m_overflows[indexMaker(*icrate, iSlot,5)]);
	  m_LumiStatus.setValueAt( hash, label_Lumi_Overflow_CMS_Et, m_overflows[indexMaker(*icrate, iSlot,6)]);
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_LHC, m_overflowRate[indexMaker(*icrate, iSlot,1)].getRate());
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_LHC_Valid, m_overflowRate[indexMaker(*icrate, iSlot,2)].getRate());
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_CMS1, m_overflowRate[indexMaker(*icrate, iSlot,3)].getRate());
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_CMS2, m_overflowRate[indexMaker(*icrate, iSlot,4)].getRate());
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_CMS_Valid, m_overflowRate[indexMaker(*icrate, iSlot,5)].getRate());
	  m_LumiStatus.setValueAt( hash, label_Lumi_OverflowRate_CMS_Et, m_overflowRate[indexMaker(*icrate, iSlot,6)].getRate());
	}
      }
    }

    void uHTRMonitoringLumi::countOverflow(int crate, int slot, int monitoringhist){
      int idx=indexMaker(crate, slot, monitoringhist+1);
      std::map<int,uHTR*>::iterator uHTRi = m_uHTRcards.find(indexMaker(crate,slot));
      if (uHTRi==m_uHTRcards.end() ) return;
      uHTR* uhtr = (*uHTRi).second;
      int histoverflow = uhtr->lumi_histo_overflow(monitoringhist);	  ;
      m_overflows[idx]= histoverflow;
      m_overflowRate[idx].addObservation(histoverflow);
    }
    
    void uHTRMonitoringLumi::reset() {
      
      m_uHTRcards.clear();

      m_LumiStatus.clear();

      m_overflows.clear();

      m_overflowRate.clear();
    }

  }
}
