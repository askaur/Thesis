#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRManager.hh"
#include "hcal/uhtr/CrateConnections.hh"
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

XDAQ_INSTANTIATOR_IMPL(hcal::uHTRManager);

namespace hcal {

  uHTRManager::CardInfo::CardInfo() {
    present = false;
    crateId = -1;
    slotId = -1;
    functionMask = 0;
  }

  void uHTRManager::CardInfo::registerFields(xdata::Bag<uHTRManager::CardInfo>* bag) {
    bag->addField("crateId", &crateId);
    bag->addField("slot", &slotId);
    bag->addField("present", &present);
  }
  
  uHTRManager::uHTRManager(xdaq::ApplicationStub* as) : 
    Application(as), 
    m_monitoringHelper(getApplicationLogger()){
    m_connection=0;

    m_crateId=-1;
    m_fe_dataRate=1600;
    getApplicationInfoSpace()->fireItemAvailable("crateId", &m_crateId);	

    m_crateView = 0 ;

    for (int i=0; i<MAX_CARDS_PER_CRATE; i++) 
      m_uHTRs[i]=0; // clear!

    m_firstInitDone=false;
   
  }
  
  uHTRManager::~uHTRManager() {
    if(!m_connection) delete m_connection;
  }

  void uHTRManager::init()                throw (hcal::exception::Exception) {
    hcal::Application::init();

    uhal::setLogLevelTo( uhal::Error() );

    bool anyMonitoring=false;

    // try grabbing information from the crate object
    int hc_id=hcalCrate::getCrateId(getApplicationContext());
    if (hc_id>=0 && m_crateId.value_<=0) m_crateId=hc_id;
    m_connection = hcal::uhtr::getConnectionManager();
    // done with hcalCrate


    // Get configuration for each uHTR in the crate from ConfigurationDoc in xml 
    hcal::cfg::Context cxt("uHTR");
    hcal::cfg::CfgDocument* cfg=getConfigurationDoc();

    cxt.set("CRATE", m_crateId.value_);

    std::string friendlyname;
    for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
      CardInfo& info=m_info[slot-1].bag;

      hcal::cfg::Context slotcxt(cxt);
      slotcxt.set("SLOT",slot);

      info.present = cfg->get("PRESENT",slotcxt,false);

      if (!info.present.value_) continue;
      
      info.crateId = m_crateId;
      info.slotId = slot;
      info.present = false; 
     
      const std::string crateTemp = ::toolbox::toString("crate_%d", slot-1).c_str();
      const std::string slotTemp = ::toolbox::toString("uHTR_%d", slot-1).c_str();
      const std::string presentTemp = ::toolbox::toString("present_%d", slot-1).c_str();

      friendlyname=::toolbox::toString("hcal.crate%d.slot%d",m_crateId.value_,slot);
      
      friendlyname=cfg->get("FriendlyName",slotcxt,friendlyname);

      try {
	LOG4CPLUS_INFO(getApplicationLogger(), friendlyname);
	uhal::HwInterface hw = m_connection->getDevice(friendlyname);

	m_uHTRs[slot-1]=new hcal::uhtr::uHTR(friendlyname,hw);
      } RETHROW_UHAL_EXCEPTION;

      if(!m_firstInitDone) {
	getApplicationInfoSpace()->fireItemAvailable(crateTemp, &(m_info[slot-1]));
	getApplicationInfoSpace()->fireItemAvailable(slotTemp, &(m_info[slot-1]));
	getApplicationInfoSpace()->fireItemAvailable(presentTemp, &(m_info[slot-1]));	
	m_firstInitDone = true;
      }

      std::string functions = cfg->get("functions",slotcxt,"DAQ,TRIG,LUMI,MON");
      if (strcasestr(functions.c_str(),"DAQ")) info.functionMask |= FM_DAQ;
      if (strcasestr(functions.c_str(),"TRIG")) info.functionMask |= FM_TRIG;
      if (strcasestr(functions.c_str(),"LUMI")) info.functionMask |= FM_LUMI;
      if (strcasestr(functions.c_str(),"MON")) info.functionMask |= FM_MON;
     
      info.orbitDelay = cfg->get("AlignOrbitDelay",slotcxt,10);
      info.autoAlign = cfg->get("AutoAlign",slotcxt,true);
      // setup for GTX ; 1: 1.6 Gbps , 2: 4.8 Gbps
      info.gtx_speed = cfg->get("GTX_Speed", slotcxt,1) ;

      if (info.functionMask & FM_DAQ) {
	info.bcOffsetDAQ = cfg->get("BCOffset",slotcxt,0);
	info.nsamples = cfg->get("nsamples",slotcxt,10);
	info.npresamples = cfg->get("npresamples",slotcxt,4);
	info.pipeline = cfg->get("pipeline",slotcxt,20);
	info.noZS = cfg->get("noZS",slotcxt,false);
	info.zsMarkAndPass = cfg->get("zsMarkAndPass",slotcxt,false);
	info.zsSumByTwo = cfg->get("zsSumByTwo",slotcxt,false);
	info.zsMask = cfg->get("zsMask",slotcxt,0);
	info.zsSimpleThreshold = cfg->get("zsSimpleThreshold",slotcxt,-1);
	info.zs_tp_enable = cfg->get("zsTPenable",slotcxt,true);
	info.zs_tp_soionly = cfg->get("zsTPonlySOI",slotcxt,false);
	info.tp_samples = cfg->get("tp_samples",slotcxt,4);
	info.tp_presamples = cfg->get("tp_presamples",slotcxt,2);
      }


      m_uHTRs[slot-1]->setCrateSlot(m_crateId.value_,slot);

      if (info.functionMask & FM_MON) {
	m_monitoringHelper.add(m_uHTRs[slot-1]);
	anyMonitoring=true;
      }

      //      m_uHTRs[slot-1]->gatherInfo();
    }
    
    releaseConfigurationDoc(cfg);

    // Initialize uHTRs - setup clocks and DAQ path
    for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
      hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];
      if (uhtr==0) continue;
      const CardInfo& info=m_info[slot-1].bag;

      try {
	// read firmware version to check card availability
	uint8_t flavorF, majorF, minorF, patchF;
	uint8_t flavorB, majorB, minorB, patchB;
	uhtr->firmwareRevision(true,flavorF, majorF, minorF, patchF);
	uhtr->firmwareRevision(false,flavorB, majorB, minorB, patchB);
	// if we read the default values, we could not communicate with the card
	if(flavorB == 0 && majorB == 0 && minorB == 0 && patchB == 0) {
	  std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Back FPGA of %s",friendlyname.c_str());
	  LOG4CPLUS_ERROR(getApplicationLogger(),errorMsg);
	  XCEPT_RAISE(hcal::exception::Exception, errorMsg);
	} else if(flavorF == 0 && majorF == 0 && minorF == 0 && patchF == 0) {
	  std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Front FPGA of %s",friendlyname.c_str());
	  LOG4CPLUS_ERROR(getApplicationLogger(),errorMsg);
	  XCEPT_RAISE(hcal::exception::Exception, errorMsg);
	} 
	else {
	  LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString("Using uHTR firmware revisions FRONT=%x.%x.%x, BACK=%x.%x.%x", majorF, minorF, patchF, majorB, minorB, patchB));
	}

        // Set up clocks  
	// This is needed until we have the clock mezzanine
	std::vector<std::pair<double,double> > clocksF, clocksB;
	clocksB=uhtr->getClockRates(false);
	clocksF=uhtr->getClockRates(true);

	if (m_fe_dataRate==1600) {
	   double dClk_b = fabs( 320 - clocksB[ clocksB.size() -1 ].second ) ;
	   double dClk_f = fabs( 240 - clocksF[ clocksB.size() -1 ].second ) ;

	   if ( dClk_b > 3 || dClk_f > 3  ) {
              if ( info.gtx_speed == 2 ) {
  	         uhtr->setupClock(1,320);
		 uhtr->setupClock(2,240);
              } else {
  	         uhtr->setupClock(1,240);
		 uhtr->setupClock(2,320);
              }
	      usleep(500000);
	      uhtr->link_all_reset();

	      clocksB=uhtr->getClockRates(false);
	      clocksF=uhtr->getClockRates(true);
	      for (size_t i=0; i<clocksB.size(); i++) {
	          printf("  Clock expected at %10.4f MHz : %10.4f MHz (front)  %10.4f MHz (back) \n",clocksB[i].first,clocksF[i].second,clocksB[i].second);
              }
           }
        }
	// here we proceed with initialization
	LOG4CPLUS_DEBUG(getApplicationLogger(), ::toolbox::toString("Finished setting clock for card %s", friendlyname.c_str()));

	// first, set up the link initialization parameters...
	uhtr->link_init_setup(info.orbitDelay, info.autoAlign); 

      } RETHROW_UHAL_EXCEPTION;
    }

    /// This loop initializes all the links in a crate at maximum speed
    void* init_ptrs[MAX_CARDS_PER_CRATE]={0};
    int n_non_zero;
    do {
      n_non_zero=0;
      for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
	hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];
	if (uhtr==0) continue;

	try {
	  init_ptrs[slot-1]=uhtr->link_init_multistep(init_ptrs[slot-1]);
	} RETHROW_UHAL_EXCEPTION;

	if (init_ptrs[slot-1]!=0) {
	  n_non_zero++; // still work to do on this slot
	  ::usleep(10000); // sleep for 10ms here, once per crate rather than once per slot!
	}
      }
    } while (n_non_zero>0);
    
    for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
      hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];
      if (uhtr==0) continue;
      const CardInfo& info=m_info[slot-1].bag;
   
      try {
	// set up the daq-path
	if (info.functionMask & FM_DAQ) {
	  uhtr->daq_path_enable(false); // disable the DAQ path in case it's on
	  uhtr->daq_path_reset(); // reset
	  
	  int moduleId=m_crateId*16+slot;
	  uhtr->daq_path_setup(moduleId, info.bcOffsetDAQ, info.nsamples, info.npresamples, info.tp_samples, info.tp_presamples, info.pipeline);
	  
	  std::vector<uint32_t> zsThresholds(64,0); /// TODO: fix for various firmware versions!!!!!
	  if (info.zsSimpleThreshold>0) zsThresholds=std::vector<uint32_t>(64,info.zsSimpleThreshold);
	  uhtr->daq_zs_setup(info.noZS, info.zsMarkAndPass, info.zsMask, info.zsSumByTwo, zsThresholds, info.zs_tp_enable, info.zs_tp_soionly);
	}
	
      } RETHROW_UHAL_EXCEPTION;
    
    }

    // Set up the visualiation of the crate
    LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString("==== Initialize Crate Views ====" ));
    setupCrateView() ;

    if (anyMonitoring) {
      m_monitoringHelper.setup(m_crateId);
      std::vector<hcal::monitor::Monitorable*> mons=m_monitoringHelper.getMonitorables();
      for (std::vector<hcal::monitor::Monitorable*>::iterator qq=mons.begin(); qq!=mons.end(); qq++)
	exportMonitorable(*qq);
    }

  }

  void uHTRManager::enable()              throw (hcal::exception::Exception) {
    hcal::Application::enable();
    m_monitoringHelper.takeStartLock();

    for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
      hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];
      const CardInfo& info=m_info[slot-1].bag;
      if (uhtr==0) continue;

      if (info.functionMask & FM_DAQ)
	uhtr->daq_path_enable(true);
   }    

    m_monitoringHelper.releaseStartLock();
  }

  void uHTRManager::disable()             throw (hcal::exception::Exception) {
    hcal::Application::disable();
    m_monitoringHelper.takeStartLock();

    for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {
      hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];
      const CardInfo& info=m_info[slot-1].bag;
      if (uhtr==0) continue;
      

      if (info.functionMask & FM_DAQ)
	uhtr->daq_path_enable(false);
    }    

    m_monitoringHelper.releaseStartLock();
  }

  void uHTRManager::reset()              throw (hcal::exception::Exception) {
    hcal::Application::reset();

    m_monitoringHelper.reset();

    for (int i=0; i<MAX_CARDS_PER_CRATE; i++) {
      if(m_uHTRs[i]!=0) {
	delete m_uHTRs[i];
	m_uHTRs[i]=0;
      }
    }
  }

  void uHTRManager::webPeek(xgi::Input* is,
			   xgi::Output* o) throw (hcal::exception::Exception) {
    cgicc::Cgicc unpacker(is);
    std::string item, link;
    int slot=-1;
    cgicc::form_iterator tool;
    std::map<std::string,std::string> params;

    tool=unpacker.getElement("item");
    if (tool!=(*unpacker).end()) item=*(*tool);

    tool=unpacker.getElement("slot");
    if (tool!=(*unpacker).end()) slot=atoi((*(*tool)).c_str());
    if ( slot  > 0 )  params.insert(std::pair<std::string,std::string>("slot",  *(*tool) ) );
    
    tool=unpacker.getElement("linkid");
    if (tool!=(*unpacker).end()) link=*(*tool);
    if ( !(link.empty()) )  params.insert(std::pair<std::string,std::string>("linkid",link));


    if (slot>0 && slot<=MAX_CARDS_PER_CRATE && m_uHTRs[slot-1]!=0) {
      webSingleCard(o,m_uHTRs[slot-1],slot,item,params);
    } else {

      *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      
      *o << "<head><title> uHTRManager Status</title></head>\n";
      *o << "<body bgcolor='#6699ff'>\n";
      *o << "<h1 style='font-family: sans-serif'>uHTRManager Status</h1>\n";
      *o <<"<font face='ARIAL BLACK' size ='4'> &nbsp;&nbsp;&nbsp; Crate ID: "<< m_crateId.value_ <<"</font> \n"  ;

      *o << std::endl;
      
      LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString(" ==== Render the crate view !!!! slot %d ", slot ) );
      m_crateView->render(o) ;

    }  
    *o << "</body>";
    
  }

  std::string uHTRManager::makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item) {
    uint32_t linear;
    
    for (linear=0; linear<uint32_t(MAX_CARDS_PER_CRATE); linear++) {
      if (m_uHTRs[linear]==card) break;
    }
    return getLinkForPeek( ::toolbox::toString("%s&slot=%d",item.c_str(),linear+1) );
  }

  // Set up the crate view
  void uHTRManager::setupCrateView() {
       if ( m_crateView == 0 ) {
          // testing of crateRender 
          // uTCA_centerMCH needs 14 slots , otherwise the CrateRenderer crash 
          m_crateView = new toolbox::CrateRenderer(14, toolbox::CrateRenderer::uTCA_centerMCH ) ;
          //m_crateView = new toolbox::CrateRenderer(13) ;
          m_crateView->exportStandard(this);

          // need to addBoardLink first and then put boardInSlot   
	  const char* boardName = "uHTR"; 

	  m_crateView->addBoard(boardName ,"#BF00FF");
	  m_crateView->addBoardLink( boardName,
				     //getLinkForPeek(::toolbox::toString( "status&crate=%d&slot=%%d", cid  )), 
				     getLinkForPeek("status")+"&slot=%d",
                                     "Link", getLinkForCGIData( toolbox::CrateRenderer::img_GreenCircle) );
	  m_crateView->addBoardLink( boardName,
				     getLinkForPeek("trigger")+"&slot=%d",
                                     "Trig", getLinkForCGIData( toolbox::CrateRenderer::img_GreenSquare) );
	  m_crateView->addBoardLink( boardName,
				     getLinkForPeek("daq")+"&slot=%d", 
                                     "DAQ", getLinkForCGIData( toolbox::CrateRenderer::img_GreenSquare) );

          for ( int i=0; i<3; i++) 
              m_crateView->setBoardLinkTextOn( "uHTR", i, true ) ;

          for ( int slot=1 ; slot <= MAX_CARDS_PER_CRATE ; slot++) {
              hcal::uhtr::uHTR* uhtr=m_uHTRs[slot-1];

	      if (uhtr !=0 ) { 
                 m_crateView->boardInSlot( slot, "uHTR" );
              } else {
                 m_crateView->noBoardInSlot( slot );
              }
          }

       }

  }

  

}
