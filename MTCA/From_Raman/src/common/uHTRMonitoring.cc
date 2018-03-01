#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRMonitoring.hh"
#include "hcal/uhtr/uHTRsensors.hh"
#include <string>
#include <iostream>
#include "toolbox/string.h"
#include "xcept/tools.h"

static const std::string label_Id("ID");
static const std::string label_Crate("CRATEID");
static const std::string label_Slot("SLOT");  
static const std::string label_FRONTBACK("FRONTBACK");  
static const std::string label_Fiber("Fiber");  
static const std::string label_BadData("BAD_DATA");
static const std::string label_BadDataRate("BAD_DATA_RATE");
static const std::string label_BadAlign("BAD_ALIGN");
static const std::string label_AlignStatus("ALIGN_STATUS");
static const std::string label_AlignOccupancy("ALIGN_OCCUPANCY");
static const std::string label_OrbitRate("ORBIT_RATE");

//DAQ
static const std::string label_EventNumber("DAQ_EVENT_NUMBER");
static const std::string label_DAQHeaderOccupancy("DAQ_HEADER_OCCUPANCY");
static const std::string label_DAQHeaderPeakOccupancy("DAQ_HEADER_PEAK_OCCUPANCY");

//DTC/TTC
static const std::string label_DTC_Rate_40mhz("DTC_RATE_40MHZ");
static const std::string label_DTC_Rate_Orbit("DTC_RATE_ORBIT");
static const std::string label_DTC_Bunch_Count("DTC_BUNCH_COUNT");
static const std::string label_DTC_Event_Number("DTC_EVENT_NUMBER");
static const std::string label_DTC_Bc0_Error("DTC_BC0_ERROR");
static const std::string label_DTC_Single_Error("DTC_SINGLE_ERROR");
static const std::string label_DTC_Double_Error("DTC_DOUBLE_ERROR");


static const std::string label_Item("ITEM");
static const std::string label_ItemNumber("ITEMNUM");
static const std::string label_TEMP("TEMP");
static const std::string label_VOLT("VOLT");

static const char* TempVoltItems[]={"FRONT_FPGA","BACK_FPGA",
				  "FRONT_FPGA_AUX","BACK_FPGA_AUX",
				  "PM1VA","PM1VB","PM3_3V","APM2_5V","APM1_8V",0};
static const int n_TempVoltItems=9;

namespace hcal {
  namespace uhtr {

    uHTRMonitoring::uHTRMonitoring(log4cplus::Logger logger) : 
      m_crateId("CRATEID"),
      m_perFiber("PerFiber"),
      m_peruHTR("PerUHTR"),
      m_TempVolt("TempVolt"),
      m_DAQStatus("DAQStatus"),
       m_DTCStatus("DTCStatus"),
      m_logger(logger),
      m_startLock(::toolbox::BSem::FULL)
    {
      m_perFiber.addUpdateListener(this);
      m_peruHTR.addUpdateListener(this);
      m_TempVolt.addUpdateListener(this);
      m_DAQStatus.addUpdateListener(this);
       m_DTCStatus.addUpdateListener(this);
    }

    void uHTRMonitoring::add(uHTR* card) {
      m_uHTRcards.insert(std::pair<int,uHTR*>(card->slot(),card));
    }

    void uHTRMonitoring::takeStartLock() {
      m_startLock.take();
    }
    void uHTRMonitoring::releaseStartLock() {
      m_startLock.give();
    }

    void uHTRMonitoring::setup(int icrate) {
      m_crate=icrate;
      m_crateId.value()=icrate;
      createTables();
    }

    void uHTRMonitoring::update(hcal::monitor::Monitorable* item) {
      takeStartLock();
      try {
	if (item==&m_peruHTR) updatePeruHTR();
	if (item==&m_TempVolt) updateTempVolt();
	if (item==&m_DAQStatus) updateDAQStatus();
	if (item==&m_DTCStatus) updateDTCStatus();
	if (item==&m_perFiber) updatePerFiber();
      } catch (xcept::Exception& e) {
	LOG4CPLUS_ERROR(m_logger,xcept::stdformat_exception_history(e));    
      }
      releaseStartLock();
      LOG4CPLUS_DEBUG(m_logger,"Updating uHTRMonitoring tables");
    }

    std::vector<hcal::monitor::Monitorable*> uHTRMonitoring::getMonitorables() {
      std::vector<hcal::monitor::Monitorable*> stuff;
      stuff.push_back(&m_crateId);
      stuff.push_back(&m_perFiber);
      stuff.push_back(&m_peruHTR);
      stuff.push_back(&m_TempVolt);
      stuff.push_back(&m_DAQStatus);
      stuff.push_back(&m_DTCStatus);
      return stuff;
    }



    void uHTRMonitoring::updatePerFiber() {
      LOG4CPLUS_DEBUG(m_logger, "Updating uHTR fiber monitoring");
      
      
      for(int iSlot = 1; iSlot <= 12; iSlot++) {

 	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
 	if(uHTRIter!=m_uHTRcards.end() ) {
 	  LOG4CPLUS_DEBUG(m_logger, ::toolbox::toString("Getting monitoring information for slot=%d",iSlot));
 	  uHTR* uHTR = (*uHTRIter).second;
	  
 	  try {
 	    uHTR->link_query();
 	  }
 	  catch (std::exception &e) {
 	    LOG4CPLUS_ERROR(m_logger, ::toolbox::toString("Could not read from card %s: %s", uHTR->name().c_str(), e.what()));
 	  }
	  
 	  for(int iFiber = 0; iFiber < 24; iFiber++) {
 	    const int hash[] = {m_crateId.value(), iSlot, iFiber};
	    
 	    m_perFiber.setValueAt(hash, label_BadData, (int)uHTR->info_link_nBadData(iFiber));
 	    m_perFiber.setValueAt(hash, label_BadDataRate, (double)uHTR->info_link_BadDataRate(iFiber));
 	    m_perFiber.setValueAt(hash, label_BadAlign, (int)uHTR->info_link_nBadAlign(iFiber));
 	    m_perFiber.setValueAt(hash, label_AlignStatus, (int)uHTR->info_link_alignStatus(iFiber));
 	    m_perFiber.setValueAt(hash, label_AlignOccupancy, (int)uHTR->info_link_alignOccupancy(iFiber));
 	    m_perFiber.setValueAt(hash, label_OrbitRate, (double)uHTR->info_link_orbitRate(iFiber));

 	  }
	}
      }
    }
    
    void uHTRMonitoring::updateTempVolt() {
         LOG4CPLUS_DEBUG(m_logger, "Updating uHTR temperature/voltage monitoring");
      

         for(int iSlot = 1; iSlot <= 12; iSlot++) {

            std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
            if (uHTRIter!=m_uHTRcards.end() ) {
               LOG4CPLUS_DEBUG(m_logger, ::toolbox::toString("Getting monitoring information for slot=%d",iSlot));
               uHTR* uHTR = (*uHTRIter).second;


	       std::map<std::string, double > sensorInfo = getUHTRSensors( uHTR ) ;

	       for (int item=0; item<n_TempVoltItems; item++) {
		 const int hash[] = { m_crateId.value(), uHTR->slot(), item }; 

		 std::string key_temp=std::string("Temp_")+TempVoltItems[item];
		 std::string key_volt=std::string("Volt_")+TempVoltItems[item];

		 m_TempVolt.setValueAt( hash, label_TEMP, sensorInfo[key_temp] );
		 m_TempVolt.setValueAt( hash, label_VOLT, sensorInfo[key_volt] );
	       }
	    }
         }
    }

    void uHTRMonitoring::createTables() {
      // multipliers for hash computation
      const int multipliers[] = {1, 100, 10000}; // 1*crate + 100*slot *10000*fiber  

      // per uHTR monitoring

      // Volt Temp monitoring
      m_TempVolt.setupHash(3, multipliers);
      m_TempVolt.addIdColumn(label_Id);
      m_TempVolt.addColumn(label_Slot,"int");
      m_TempVolt.addColumn(label_Item,"string");
      m_TempVolt.addColumn(label_ItemNumber,"int");
      m_TempVolt.addColumn(label_TEMP,"double");
      m_TempVolt.addColumn(label_VOLT,"double");

      for(int iSlot = 1; iSlot <= 12; iSlot++) {
	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
 	if(uHTRIter==m_uHTRcards.end() ) continue;	
	uHTR* uHTR = (*uHTRIter).second;
	  
	for (int item=0; item<n_TempVoltItems; item++) {
	  const int hash[] = { m_crateId.value(), uHTR->slot(), item }; 
	  m_TempVolt.addRow(hash);
	  m_TempVolt.setValueAt( hash, label_Slot, uHTR->slot() );
	  m_TempVolt.setValueAt( hash, label_Item, std::string(TempVoltItems[item]) );
	  m_TempVolt.setValueAt( hash, label_ItemNumber, item );
	}
      }

     // DAQ status monitoring
      m_DAQStatus.setupHash(2, multipliers);
      m_DAQStatus.addIdColumn(label_Id);
      m_DAQStatus.addColumn(label_Slot, "int");
      m_DAQStatus.addColumn(label_EventNumber,"unsigned int"); 
      m_DAQStatus.addColumn(label_DAQHeaderOccupancy,"int"); 
      m_DAQStatus.addColumn(label_DAQHeaderPeakOccupancy,"int"); 

      for(int iSlot = 1; iSlot <= 12; iSlot++) {
	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
 	if(uHTRIter!=m_uHTRcards.end() ) {	
	  uHTR* uHTR = (*uHTRIter).second;
	  const int hash[] = { m_crateId.value(), uHTR->slot() }; 
	  m_DAQStatus.addRow(hash);
	  m_DAQStatus.setValueAt( hash, label_Slot, uHTR->slot() );
	}
      }
     
     // DTC status monitoring
      m_DTCStatus.setupHash(3, multipliers);
      m_DTCStatus.addIdColumn(label_Id);
      m_DTCStatus.addColumn(label_Slot, "int");
      m_DTCStatus.addColumn(label_FRONTBACK, "int");
      m_DTCStatus.addColumn(label_DTC_Rate_40mhz,"double"); 
      m_DTCStatus.addColumn(label_DTC_Rate_Orbit,"double"); 
      m_DTCStatus.addColumn(label_DTC_Bunch_Count,"unsigned int"); 
      m_DTCStatus.addColumn(label_DTC_Event_Number,"unsigned int"); 
      m_DTCStatus.addColumn(label_DTC_Bc0_Error,"unsigned int"); 
      m_DTCStatus.addColumn(label_DTC_Single_Error,"unsigned int"); 
      m_DTCStatus.addColumn(label_DTC_Double_Error,"unsigned int"); 

      for(int iSlot = 1; iSlot <= 12; iSlot++) {
	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
 	if(uHTRIter!=m_uHTRcards.end() ) {	
	  uHTR* uHTR = (*uHTRIter).second;
	  for (int fb=0; fb<2; fb++) {
	    const int hash[] = { m_crateId.value(), uHTR->slot(), fb }; 
	    m_DTCStatus.addRow(hash);
	    m_DTCStatus.setValueAt( hash, label_Slot, uHTR->slot() );
	    m_DTCStatus.setValueAt( hash, label_FRONTBACK, fb);
	  }
	}
      }
     
      // per fiber monitoring
      m_perFiber.setupHash(3, multipliers);
      m_perFiber.addIdColumn(label_Id);
      m_perFiber.addColumn(label_Slot, "int");
      m_perFiber.addColumn(label_Fiber, "int");
      m_perFiber.addColumn(label_BadData, "int");
      m_perFiber.addColumn(label_BadDataRate, "double");
      m_perFiber.addColumn(label_BadAlign, "int");
      m_perFiber.addColumn(label_AlignStatus, "int");
      m_perFiber.addColumn(label_AlignOccupancy, "int");
      m_perFiber.addColumn(label_OrbitRate, "double");

      for(int iSlot = 1; iSlot <= 24; iSlot++) {

	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
	if(uHTRIter==m_uHTRcards.end() ) continue; 

	for(int iFiber = 0; iFiber < 24; iFiber++) {
	  const int hash[] = {m_crateId.value(), iSlot, iFiber};
	  m_perFiber.addRow(hash);
	  m_perFiber.setValueAt( hash, label_Slot, iSlot );
	  m_perFiber.setValueAt( hash, label_Fiber, iFiber);
	}
      }

      updatePerFiber();
      updatePeruHTR();
      updateTempVolt() ;
      updateDAQStatus();
      updateDTCStatus();

      LOG4CPLUS_DEBUG(m_logger,"Finished creating uHTRMonitoring tables");
    }
    
    std::map<std::string, double> uHTRMonitoring::getUHTRSensors( uHTR* uHTR ) {

         hcal::uhtr::uHTRsensors* sensors=uHTR->sensors();
  
         std::map<std::string, double> sensorInfo ;
         double temp,v;
         for (int i=1; i<=5; i++) {
             hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
             sensors->setMuxes(sb);
             sensors->readSensorsPM(temp,v);
             if ( i == 1 ) sensorInfo["Temp_PM1VA"] = temp ;
             if ( i == 2 ) sensorInfo["Temp_PM1VB"] = temp ;
             if ( i == 3 ) sensorInfo["Temp_PM3_3V"] = temp ;
             if ( i == 4 ) sensorInfo["Temp_APM1_8V"] = temp ;
             if ( i == 5 ) sensorInfo["Temp_APM2_5V"] = temp ;

             if ( i == 1 ) sensorInfo["Volt_PM1VA"] = v ;
             if ( i == 2 ) sensorInfo["Volt_PM1VB"] = v ;
             if ( i == 3 ) sensorInfo["Volt_PM3_3V"] = v ;
             if ( i == 4 ) sensorInfo["Volt_APM1_8V"] = v ;
             if ( i == 5 ) sensorInfo["Volt_APM2_5V"] = v ;
         }

         //  for (int i=7; i<=8; i++) {
         /* 
	 for (int i=6; i<=8; i++) {
	     double v1,v2;
	     if (i != 7) { // only 1 rx at this time  - can be removed later
		 hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
		 sensors->setMuxes(sb);
		 sensors->readSensorsPPOD(temp,v1,v2);
                 if ( i == 6 ) sensorInfo["Temp_PPOD_rx0"] = temp ;
                 if ( i == 7 ) sensorInfo["Temp_PPOD_rx1"] = temp ;
                 if ( i == 8 ) sensorInfo["Temp_PPOD_tx"] = temp ;
             }
         }
         */
  
         // FPGA sensor info
         for (int i=10; i<=11; i++) {
	     double v1,v2;
             hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
             sensors->setMuxes(sb);
             sensors->readSensorsFPGA(sb,temp,v1,v2);

             if ( i == 10 ) {
	       sensorInfo["Temp_BACK_FPGA"] = temp ;
	       sensorInfo["Temp_BACK_FPGA_AUX"] = temp ;
	     }
             if ( i == 11 ) {
	       sensorInfo["Temp_FRONT_FPGA"] = temp ;
	       sensorInfo["Temp_FRONT_FPGA_AUX"] = temp ;
	     }

             if ( i == 10 ) sensorInfo["Volt_BACK_FPGA"] = v1 ;
             if ( i == 11 ) sensorInfo["Volt_FRONT_FPGA"] = v1 ;

             if ( i == 10 ) sensorInfo["Volt_BACK_FPGA_AUX"] = v2 ;
             if ( i == 11 ) sensorInfo["Volt_FRONT_FPGA_AUX"] = v2 ;

         }

         return sensorInfo ;  
    }


    void uHTRMonitoring::updateDAQStatus() {
      LOG4CPLUS_DEBUG(m_logger, "Updating uHTR DAQ status");
      

      for(int iSlot = 1; iSlot <= 12; iSlot++) {

	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
	if (uHTRIter!=m_uHTRcards.end() ) {
	  LOG4CPLUS_DEBUG(m_logger, ::toolbox::toString("Getting monitoring information for slot=%d",iSlot));
	  uHTR* uHTR = (*uHTRIter).second;

	  const int hash[] = { m_crateId.value(), uHTR->slot() }; 
	  uint32_t evn, orn;

	  uHTR->info_evn_orn(evn,orn);

	  m_DAQStatus.setValueAt( hash, label_EventNumber, evn);
	  m_DAQStatus.setValueAt( hash, label_DAQHeaderOccupancy, int(uHTR->daq_path_headerOccupancy(false)));
	  m_DAQStatus.setValueAt( hash, label_DAQHeaderPeakOccupancy, int(uHTR->daq_path_headerOccupancy(true)));
	  
	}
      }
    }
    void uHTRMonitoring::updateDTCStatus() {
      LOG4CPLUS_DEBUG(m_logger, "Updating uHTR DTC/TTC status");
      

      for(int iSlot = 1; iSlot <= 12; iSlot++) {

	std::map<int,uHTR*>::iterator uHTRIter = m_uHTRcards.find(iSlot);
	if (uHTRIter!=m_uHTRcards.end() ) {
	  LOG4CPLUS_DEBUG(m_logger, ::toolbox::toString("Getting monitoring information for slot=%d",iSlot));
	  uHTR* uHTR = (*uHTRIter).second;

	  for (int fb=0; fb<2; fb++) {
	    const int hash[] = { m_crateId.value(), uHTR->slot(), fb }; 
	    uint32_t bunchcount, eventnumber, bc0error, singleerror, doubleerror, ttcphase, ttcphaselock;
	    float rate40mhz, rateorbit;
	    uHTR->get_dtc_info((fb==1), rate40mhz, rateorbit, bunchcount, eventnumber, bc0error, singleerror, doubleerror, ttcphase, ttcphaselock);//fb=1 is front

	    m_DTCStatus.setValueAt( hash, label_DTC_Rate_40mhz, double(rate40mhz));
	    m_DTCStatus.setValueAt( hash, label_DTC_Rate_Orbit, double(rateorbit));
	    m_DTCStatus.setValueAt( hash, label_DTC_Bunch_Count, bunchcount);
	    m_DTCStatus.setValueAt( hash, label_DTC_Event_Number, eventnumber);
	    m_DTCStatus.setValueAt( hash, label_DTC_Bc0_Error, bc0error);
	    m_DTCStatus.setValueAt( hash, label_DTC_Single_Error, singleerror);
	    m_DTCStatus.setValueAt( hash, label_DTC_Double_Error, doubleerror);
	  }	  
	}
      }
    }

    void uHTRMonitoring::reset() {
      
      m_uHTRcards.clear();

      m_peruHTR.clear();
      m_TempVolt.clear();
      m_DAQStatus.clear();
      m_DTCStatus.clear();
      m_perFiber.clear();
    }

  }
}
