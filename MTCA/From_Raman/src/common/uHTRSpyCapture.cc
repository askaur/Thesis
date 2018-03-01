#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRSpyCapture.hh"
#include "hcal/uhtr/CrateConnections.hh"
#include "hcal/uhtr/LinkFormatTools.hh"
#include "hcal/cfg/CfgDocument.hh"
#include "hcal/cfg/Context.hh"
#include "hcal/exception/Exception.hh"
#include "hcal/aux/hcalCrate.hh"
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "toolbox/string.h"
#include "toolbox/Runtime.h"
#include "hcal/exception/uHALException.hh"
#include "hcal/toolbox/ThreadSafeQueue.hh"
#include "hcal/toolbox/InterchangePoint.hh"
#include "hcal/soap/ChannelActivation.hh"
//#include "hcal/toolbox/CrateRenderer.hh"
#include "xoap/MessageFactory.h" // for return
#include "xoap/Method.h"

struct uHTRSpyCaptureHeader {
  uint16_t version_l1a; // MSB is version, this is version 1
};

struct uHTRSpyCaptureChannel {
  uint16_t channel_header; //channelID[0,7],capID[8,9],error[10,11],flavor[12,14]=0, 1[15]
  uint16_t line0;//QIE data[0,7], TDC data [8,12], error[13], 0[14,15]
};



XDAQ_INSTANTIATOR_IMPL(hcal::uHTRSpyCapture);

namespace hcal {

  uHTRSpyCapture::uHTRSpyCapture(xdaq::ApplicationStub* as) : 
    ReadoutApplication(as), 
    m_accessProtect(::toolbox::BSem::FULL)
  {

    m_connection = 0 ;
    m_listOfTriggers = 0;
  }
  
  uHTRSpyCapture::~uHTRSpyCapture() {
    if(!m_connection) delete m_connection;  
    if(!m_listOfTriggers) delete m_listOfTriggers;
  }

  void uHTRSpyCapture::init()                throw (hcal::exception::Exception) {
    hcal::ReadoutApplication::init();
    m_connection = hcal::uhtr::getConnectionManager();

    //bool anyMonitoring=false;
    std::string friendlyname;
 
    hcal::cfg::CfgDocument* cfg=getConfigurationDoc();
    hcal::cfg::Context cxt_spycapture("spycature");

    m_fedId = cfg->get( "FEDID",cxt_spycapture, 18 );
    m_crateId = cfg->get( "CRATEID",cxt_spycapture, 115 );
    m_slotId = cfg->get( "SLOTID",cxt_spycapture, 8 );
    m_nBX_before = cfg->get( "NBX_BEFORE",cxt_spycapture, 10 ); 
    m_nBX_after = cfg->get( "NBX_AFTER",cxt_spycapture, 10 ); 

    m_triggerInterchangeName=cfg->get("TriggerListInterchangeName",cxt_spycapture,"");

    friendlyname=::toolbox::toString("hcal.crate%d.slot%d",m_crateId,m_slotId);
    friendlyname=cfg->get("FriendlyName",cxt_spycapture,friendlyname);

    try {
      uhal::HwInterface hw = m_connection->getDevice(friendlyname);
      m_uHTR=new hcal::uhtr::uHTR(friendlyname,hw);	  
    } RETHROW_UHAL_EXCEPTION;

    m_uHTR->setCrateSlot( m_crateId, m_slotId );

    if (m_uHTR->is1600()) {
      LOG4CPLUS_ERROR(getApplicationLogger(),"SpyCapture readout application is on for 4800 FW for now");
      return; 
    } 

    MAX_CHANNELS_PER_FIBER=6;
    releaseConfigurationDoc(cfg);

    try {
      // read firmware version to check card availability
      uint8_t flavorF, majorF, minorF, patchF;
      uint8_t flavorB, majorB, minorB, patchB;
      m_uHTR->firmwareRevision(true,flavorF, majorF, minorF, patchF);
      m_uHTR->firmwareRevision(false,flavorB, majorB, minorB, patchB);
      // if we read the default values, we could not communicate with the card
      if (flavorB == 0 && majorB == 0 && minorB == 0 && patchB == 0) {
	std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Back FPGA of %s",friendlyname.c_str());
	XCEPT_RAISE(hcal::exception::Exception, errorMsg);
      } else if(flavorF == 0 && majorF == 0 && minorF == 0 && patchF == 0) {
	std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Front FPGA of %s",friendlyname.c_str());
	XCEPT_RAISE(hcal::exception::Exception, errorMsg);
      } 
	
      // setup
      m_uHTR->link_spyCaptureSetup(m_nBX_after);
      m_uHTR->link_spySetStart(0x5555);
    } RETHROW_UHAL_EXCEPTION;
    if (!m_triggerInterchangeName.empty()) {
      if (m_listOfTriggers!=0) delete m_listOfTriggers;
      m_listOfTriggers=new ExternalTriggerListType;
      hcal::toolbox::InterchangePoint::getInstance().addObject(m_triggerInterchangeName,m_listOfTriggers); // TODO: Understand reasonable cleanup....
    }
  }

  void uHTRSpyCapture::enable()              throw (hcal::exception::Exception) {
    xdaq::Application* tapp=getApplicationContext()->getFirstApplication("DummyTriggerAdapter");
    if (tapp!=0) {
      xdata::Integer* ttt=dynamic_cast<xdata::Integer*>(tapp->getApplicationInfoSpace()->find("TriggersToTake"));      
      n_events_requested=ttt->value_;
    } else {
      n_events_requested=-1;
    }

    if (n_events_requested>0) LOG4CPLUS_INFO(getApplicationLogger(),::toolbox::toString("Events requested: %d",n_events_requested));
    else LOG4CPLUS_INFO(getApplicationLogger(),"No event limit identified");	

    hcal::ReadoutApplication::enable();

  }

  void uHTRSpyCapture::reset()              throw (hcal::exception::Exception) {
    hcal::ReadoutApplication::reset();
    m_uHTR->link_spySetStart(0x5555);
      //      if (m_uHTR->link_spyState()!=4){} //should I put a delay here or later?
  }


  int uHTRSpyCapture::readout(unsigned int expected, unsigned int* eventNumbers, std::vector< ::toolbox::mem::Reference* >& data) throw (hcal::exception::Exception) {

    m_accessProtect.take();

    uint32_t writeptr_L1A=m_uHTR->link_spy_writeptr_L1A_read();
    if (writeptr_L1A==0xDEADBEEF) {
      LOG4CPLUS_DEBUG(getApplicationLogger(),"Waiting for trigger...");
      ::usleep(1000);
      m_accessProtect.give();
      return 0;
    }
    
    //write header!!!!!!!!!!
    int size_in_bytes=MAX_FIBERS_PER_CARD*MAX_CHANNELS_PER_FIBER*(m_nBX_before+m_nBX_after+1)*sizeof(uint16_t)+sizeof(uHTRSpyCaptureHeader);
    ::toolbox::mem::Reference* xref=createFrame(size_in_bytes,eventNumbers[0]);
    uHTRSpyCaptureHeader* header=(uHTRSpyCaptureHeader*)getBodyCurrent();
    header->version_l1a  =(1<<8)|(eventNumbers[0]); 
    //rest of header

    uint16_t* spySpaceBase=((uint16_t*)getBodyCurrent())+(sizeof(uHTRSpyCaptureHeader)/sizeof(uint16_t));
    uint16_t* spyPtr=spySpaceBase;

    for (int fib = 0; fib < MAX_FIBERS_PER_CARD; fib++){
      uint triggerword=0;
      std::vector<uint32_t> buffer=m_uHTR->link_spyCaptureRead(fib, 0x80200030+writeptr_L1A, m_nBX_before, 10000, triggerword);
    
      size_t startPt=0;

      uint8_t v_adcs[m_nBX_before+m_nBX_after][MAX_CHANNELS_PER_FIBER];
      uint8_t v_tdcs[m_nBX_before+m_nBX_after][MAX_CHANNELS_PER_FIBER];
      bool errors[m_nBX_before+m_nBX_after];
      bool trigger[m_nBX_before+m_nBX_after];
      bool error_capID=0;
      bool error_link=0;
      int capID=0;
      int BX=0;
      for (startPt=0; startPt<6; startPt++)
	if ((buffer[startPt]&0xFF)==0xBC && (buffer[startPt+6]&0xFF)==0xBC) break;
      
      for (size_t i=startPt; i+5<buffer.size(); i+=6) {
	if (triggerword>=i && triggerword<i+6){trigger[i/6]=1;}
	else{trigger[i/6]=0;}
	int capid;
	bool bc0, ce;
	uint8_t adcs[6], tdcs[6];
	uint16_t samples[6];
	for (int j=0; j<6; j++) samples[j]=uint16_t(buffer[i+j]);
	
	bool fmtissue=!hcal::uhtr::unpack_link_6chan(samples,capid,bc0,ce,adcs,tdcs);
	
	if (BX==0){capID=capid;}
	if (fmtissue){error_link=1;errors[i/6]=1;}
	if (ce){error_capID=1;errors[i/6]=1;}
	for (int chan=0; chan<MAX_CHANNELS_PER_FIBER; chan++){
	  v_adcs[i/6][chan]=adcs[chan];
	  v_tdcs[i/6][chan]=tdcs[chan];
	}
	BX++;
      }	
      int errortot=0+error_capID;
      if(error_link) errortot+=2;
      for (int chan=0; chan<MAX_CHANNELS_PER_FIBER; chan++){
	uHTRSpyCaptureChannel* ThisChannel=(uHTRSpyCaptureChannel*)spyPtr;	

	ThisChannel->channel_header= (1 << 15) + (0 << 12) + (errortot<< 10) + (capID<<8)+(fib*MAX_CHANNELS_PER_FIBER+chan);//channelID[0,7],capID[8,9],error[10,11],flavor[12,14]=0, 1[15];
	uint16_t* lines=&(ThisChannel->line0);
	for (int nbx=0; nbx<m_nBX_before+m_nBX_after; nbx++){
	  lines[nbx]=(0 << 14) + (errors[nbx] << 13) + (v_tdcs[nbx][chan] << 8) +  v_adcs[nbx][chan];//QIE data[0,7], TDC data [8,12], error[13], 0[14,15]
	}
	spyPtr+=(m_nBX_before+m_nBX_after+1);
      }      
    }
    data.push_back(xref);

    m_uHTR->link_spySetStart(0x5555);

    // now, we add this to the trigger list if appropriate
    if (m_listOfTriggers!=0) {
      HcalExtendedTriggerMessageBlock tmb;
      memset(&tmb,0,sizeof(tmb));
      
      tmb.StdBlock.eventNumber=1&0xFFFFFF;
      tmb.StdBlock.flags_daq_ttype=0x1a000009u;
      tmb.runNumber=getRunNumber();
      
      m_listOfTriggers->push(tmb);
    }
    m_accessProtect.give();
    return 1;
 
  }

  unsigned int uHTRSpyCapture::getAverageEventSize() const { return sizeof(uint16_t) * (24*6*51);}//50 BXs

  unsigned int uHTRSpyCapture::getSourceId() const { return m_fedId; }

}
