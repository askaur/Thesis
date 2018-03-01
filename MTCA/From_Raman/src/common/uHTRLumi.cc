#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRLumi.hh"
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
#include "hcal/toolbox/ThreadSafeQueue.hh"
#include "toolbox/Task.h"
//#include "hcal/toolbox/CrateRenderer.hh"
#include <TFile.h>
#include <TH1.h>
#include <TSystem.h>

XDAQ_INSTANTIATOR_IMPL(hcal::uHTRLumi);

namespace hcal {

  class LumiDAQTask : public ::toolbox::Task {
  public:
    LumiDAQTask(uHTRLumi* app) : Task("LumiDAQTask"), p_app(app) { }
    virtual int svc() { return p_app->runTask(); }
  private:
    uHTRLumi* p_app;
  };

  static const int CMD_START = 0x1;
  static const int CMD_HALT = 0x2;
  static const int CMD_QUIT = 0x3;
  
  class CommandQueue : public ::hcal::toolbox::ThreadSafeQueue<int> {
  public:
    CommandQueue() : ::hcal::toolbox::ThreadSafeQueue<int>() { }
  };
  

  uHTRLumi::CardInfo::CardInfo() {
    present = false;
    crateId = -1;
    slotId = -1;
  }

  uHTRLumi::uHTRLumi(xdaq::ApplicationStub* as) : 
    Application(as), 
    m_monitoringHelper(getApplicationLogger()) 
  {
    m_connection = 0 ;
    m_task=0;
    m_cmdQueue=new CommandQueue();

    for (int i=0; i<MAX_CRATE; i++) { 
	m_crateId[i] = -1;
        m_crateView[i] = 0 ;
    }
    for (int i=0; i<MAX_CARDS_PER_CRATE*MAX_CRATE; i++) {
        m_uHTRs[i]=0; // clear!
    } 
    h_itemId.clear()  ;  

  }
  
  uHTRLumi::~uHTRLumi() {
    if(!m_connection) delete m_connection;
  }

  void uHTRLumi::init()                throw (hcal::exception::Exception) {
    hcal::Application::init();

    uhal::setLogLevelTo( uhal::Error() );

    //bool anyMonitoring=false;
    std::string friendlyname;

    hcal::cfg::CfgDocument* cfg=getConfigurationDoc();
    hcal::cfg::Context cxt_lumi("lumi");
    m_histoLoc = cfg->get( "HISTOLOC", cxt_lumi, "/tmp/lumi_%08d.root" ) ;
    m_readoutId = cfg->get( "READOUTID",cxt_lumi, 0 );

    m_connection = hcal::uhtr::getConnectionManager();
    // set up crates
    CardInfo info ;
    bool use1to1lut[36] = { false };
    std::vector<int> crateIds;
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {

      m_crateId[iCrate]=cfg->get("CRATE_ID",iCrate,cxt_lumi,-1);
      if (m_crateId[iCrate]<0) continue; 
      crateIds.push_back(m_crateId[iCrate]);

      // Set up each card in the crate
      hcal::cfg::Context cxt_uhtr("uHTR");
      cxt_uhtr.set("CRATE", m_crateId[iCrate]);

      for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {

	int iCard = (iCrate)*MAX_CARDS_PER_CRATE + slot - 1;
	//CardInfo& info=m_info[ iCard ].bag;

	hcal::cfg::Context cxt_slot(cxt_uhtr);
	cxt_slot.set("SLOT",slot);
	
	info.present = cfg->get("PRESENT",cxt_slot,false);

	if (!info.present.value_) continue;
	
	info.crateId = m_crateId[ iCrate ] ;
	info.slotId  = slot;
	info.present = false; 

	const std::string crateTemp   = ::toolbox::toString("crate_%d", m_crateId[iCrate]).c_str();
	const std::string slotTemp    = ::toolbox::toString("uHTR_%d", slot-1).c_str();
	const std::string presentTemp = ::toolbox::toString("present_%d", slot-1).c_str();

	friendlyname=::toolbox::toString("hcal.crate%d.slot%d",m_crateId[iCrate],slot);

	friendlyname=cfg->get("FriendlyName",cxt_slot,friendlyname);

        // connect to the lived uHTR 
	try {
	  LOG4CPLUS_INFO(getApplicationLogger(), friendlyname);
	  uhal::HwInterface hw = m_connection->getDevice(friendlyname);
	  m_uHTRs[ iCard ]=new hcal::uhtr::uHTR(friendlyname,hw);	  
	} RETHROW_UHAL_EXCEPTION;

        // read setup for lumi histograms 
	info.n_lhc_orb = cfg->get("N_LHC_Orbit",cxt_lumi,0x8000 );
	info.n_cms_orb = cfg->get("N_CMS_Orbit",cxt_lumi,0x8000 );
	info.lhc_threshold  = cfg->get("LHC_Threshold",cxt_lumi, 1);
	info.cms1_threshold = cfg->get("CMS1_Threshold",cxt_lumi, 1);
	info.cms2_threshold = cfg->get("CMS2_Threshold",cxt_lumi, 1);

	cfg::Context fccxt(cxt_lumi);
        fccxt.set("CRATE",info.crateId);
	fccxt.set("SLOT",slot);
        info.lhcMask=std::vector<bool>(24*3,false);
        info.cmsMask=std::vector<bool>(24*3,false);

	for (int fc=0; fc<3; fc++) {
	    fccxt.set("FIBERCHAN",fc);
	    uint32_t lhc_mask=cfg->get("LHC_Mask",fccxt,0);
	    uint32_t cms_mask=cfg->get("CMS_Mask",fccxt,0);
	    for (int ifiber=0; ifiber<24; ifiber++) {
	        if ( lhc_mask&(1<<(ifiber)) ) info.lhcMask[ifiber*3+fc]=true;
		if ( cms_mask&(1<<(ifiber)) ) info.cmsMask[ifiber*3+fc]=true;
          }
	}
        m_info[iCard] =  info ;

        // get the setup for LUT
	use1to1lut[iCard] = cfg->get("Use1to1LUT", fccxt, false );

	m_uHTRs[ iCard ]->setCrateSlot( m_crateId[ iCrate ], slot );
      } 
    }
    releaseConfigurationDoc(cfg);
    
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

	  int iCard = (iCrate)*MAX_CARDS_PER_CRATE + slot - 1;
	  hcal::uhtr::uHTR* uhtr=m_uHTRs[ iCard ];
	  
	  if ( uhtr==0 ) continue;
	  //const CardInfo& info=m_info[slot-1].bag;

	  try {
	    // read firmware version to check card availability
	    uint8_t flavorF, majorF, minorF, patchF;
	    uint8_t flavorB, majorB, minorB, patchB;
	    uhtr->firmwareRevision(true,flavorF, majorF, minorF, patchF);
	    uhtr->firmwareRevision(false,flavorB, majorB, minorB, patchB);
	    // if we read the default values, we could not communicate with the card
	    if (flavorB == 0 && majorB == 0 && minorB == 0 && patchB == 0) {
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
	    
	    // this is needed until we have the clock mezzanine (dangerous for multiple apps to do this!!!!)
            std::vector<std::pair<double,double> > clocksF, clocksB;
	    clocksB=uhtr->getClockRates(false);
	    clocksF=uhtr->getClockRates(true);

            // set clock for uHTR 
            double dClk_b = fabs( 320 - clocksB[ clocksB.size() -1 ].second ) ;
            double dClk_f = fabs( 240 - clocksF[ clocksB.size() -1 ].second ) ;
            if ( dClk_b > 3 || dClk_f > 3 ) { 
  	       uhtr->setupClock(1,240);
	       uhtr->setupClock(2,320);
               usleep(500000);
	       clocksB=uhtr->getClockRates(false);
	       clocksF=uhtr->getClockRates(true);
               for (size_t i=0; i<clocksB.size(); i++) {
                   printf("  Clock expected at %10.4f MHz : %10.4f MHz (front)  %10.4f MHz (back) \n",clocksB[i].first,clocksF[i].second,clocksB[i].second);
               }
            }
	    
	    // here we proceed with initialization
	    LOG4CPLUS_DEBUG(getApplicationLogger(), ::toolbox::toString("Finished setting clock for card %s", friendlyname.c_str()));
	    uhtr->lumi_link_reset() ;

	    LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString("Masks are hard-coded in zero -- need to be fixed by using database %s", friendlyname.c_str()));
            //std::vector<bool> mask_lhc(uhtr::uHTR::NCHANNELS_PER_FIBER * uhtr::uHTR::NFIBERS, false ) ; 
            //std::vector<bool> mask_cms(uhtr::uHTR::NCHANNELS_PER_FIBER * uhtr::uHTR::NFIBERS, false ) ;
            uhtr->lumi_setup( m_info[iCard].n_lhc_orb,     m_info[iCard].n_cms_orb, 
                              m_info[iCard].lhc_threshold, m_info[iCard].cms1_threshold, m_info[iCard].cms2_threshold, 
                              m_info[iCard].lhcMask,       m_info[iCard].cmsMask );
            // program LUT , current use is 1to1 LUT
            if ( use1to1lut[iCard] ) programLUT( uhtr ) ; 
	       
	    m_monitoringHelper.add(uhtr);

	  } RETHROW_UHAL_EXCEPTION;
        }
	
    }

    // setup the crate view
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {

        if (m_crateId[iCrate]<0) continue; 
        LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString("==== Initialize Crate Views ====" ));
        setupCrateView( iCrate ) ;
    }

    // setup the monitoring
    if (!crateIds.empty()) {
      m_monitoringHelper.setup(m_readoutId, crateIds);
      std::vector<hcal::monitor::Monitorable*> mons=m_monitoringHelper.getMonitorables();
      for (std::vector<hcal::monitor::Monitorable*>::iterator qq=mons.begin(); qq!=mons.end(); qq++)
	exportMonitorable(*qq);    
    }
    // launch the DAQ task
    if (m_task==0) {
      m_task=new LumiDAQTask(this);
      m_task->activate();
    }
    
  }

  void uHTRLumi::enable()              throw (hcal::exception::Exception) {
    hcal::Application::enable();
    m_monitoringHelper.takeStartLock();

    std::string hfName;
    if (m_histoLoc.find('%')!=std::string::npos) {
       hfName =::toolbox::toString(m_histoLoc.c_str(),getRunNumber());
    } else {
       hfName =::toolbox::toString("%s/h_%09d.root",m_histoLoc.c_str(),getRunNumber());
    }

    hfile=new TFile( hfName.c_str(), "RECREATE");
    if ( hfile == 0  ) XCEPT_RAISE(hcal::exception::Exception, ::toolbox::toString("Unable to open file '%s' for writing", hfName.c_str() ) ) ;

    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

            int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
            hcal::uhtr::uHTR* uhtr=m_uHTRs[ iCard ];
	    //const CardInfo& info=m_info[ iCard ].bag;
	    if (uhtr==0) continue;
        }
   }    
    m_cmdQueue->push(CMD_START);

    m_monitoringHelper.releaseStartLock();
  }

  void uHTRLumi::disable()             throw (hcal::exception::Exception) {
    hcal::Application::disable();
    m_monitoringHelper.takeStartLock();

    m_cmdQueue->push(CMD_HALT);

    while (m_cmdQueue->size()>0) { // wait for the message to get through!
      ::usleep(500000);
    }

    if ( hfile != 0 ) {
       hfile->Close() ;
       delete hfile ;
    }

    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

            int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
            hcal::uhtr::uHTR* uhtr=m_uHTRs[ iCard ];
	    //const CardInfo& info=m_info[ iCard ].bag;
	    if (uhtr==0) continue;
        }
    }    

    m_monitoringHelper.releaseStartLock();
  }

  void uHTRLumi::reset()              throw (hcal::exception::Exception) {
    hcal::Application::reset();

    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

            int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
            if(m_uHTRs[iCard] !=0 ) {
               m_uHTRs[iCard]->lumi_link_reset() ; 

   	      delete m_uHTRs[iCard];
  	      m_uHTRs[iCard]=0;
            }
        }
    }
  }

  void uHTRLumi::read() throw (hcal::exception::Exception) {

    int nRead = 0 ;
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

	  int iCard = (iCrate)*MAX_CARDS_PER_CRATE + slot - 1;
	  hcal::uhtr::uHTR* uHTR=m_uHTRs[ iCard ];

          if ( uHTR ==0 ) continue ;

	  std::map<std::string, bool > aM ;
	  std::map<std::string, bool > oM ;
	  uhtr::uHTR::LumiHistogram hV ; 
	  uHTR->lumi_histo_status( aM, oM ) ;

          for ( int j=0; uhtr::uHTR::LUMI_HISTONAMES[j] != 0 ; j++   ) {
              
             if ( oM[ uhtr::uHTR::LUMI_HISTONAMES[j] ] ) {
  	        LOG4CPLUS_ERROR(getApplicationLogger(), 
                                ::toolbox::toString(" Overflow for %s at slot %d of crate %d  ",  
                                                     uhtr::uHTR::LUMI_HISTONAMES[j], slot, iCrate  ));
		m_monitoringHelper.countOverflow(iCrate,slot,j);
             }
             if ( aM[ uhtr::uHTR::LUMI_HISTONAMES[j] ] ) {

                 uHTR->lumi_read_histogram( uhtr::uHTR::LUMI_HISTONAMES[j] , hV );
                 nRead++ ; 
                 if ( hfile != 0 ) { 
                    
                    char hNameStr[128] ;
                    sprintf( hNameStr,  "h%d-%d_%s_%d", iCrate, slot, uhtr::uHTR::LUMI_HISTONAMES[j] , hV.orb_init ) ;

                    TH1D*  h1 = new TH1D( hNameStr, hNameStr , 3564, 0, 3564 ) ;

                    for ( size_t k=1; k<= hV.h.size(); k++ ) {
                        h1->Fill( k, hV.h[k-1] ) ;      
                    }

                    h1->Write();
                 }
             }  
           }     
         }
     }
     if ( nRead == 0 ) usleep( 500000 ) ;  

  }

  const char* uHTRLumi::lumi_hName[] = { "LHC", "LHC_V", "CMS1", "CMS2", "CMS_V", "CMSET", 0 } ;

  void uHTRLumi::webPeek(xgi::Input* is,
			   xgi::Output* o) throw (hcal::exception::Exception) {

    cgicc::Cgicc unpacker(is);
    std::string item, link;
    int slot  = -1;
    int crate = -1;
    int linkid = -1;
    cgicc::form_iterator tool;
    //std::map<std::string,std::string> params;

    tool=unpacker.getElement("item");
    if (tool!=(*unpacker).end()) item=*(*tool);
    tool=unpacker.getElement("crate");
    if (tool!=(*unpacker).end()) crate=atoi((*(*tool)).c_str());
    tool=unpacker.getElement("slot");
    if (tool!=(*unpacker).end()) slot=atoi((*(*tool)).c_str());
    tool=unpacker.getElement("linkid");
    if (tool!=(*unpacker).end()) {
      link=*(*tool);
      linkid=atoi( link.c_str() ); 
    } else if (crate>=0 && slot>=0) {
      linkid=crate*MAX_CARDS_PER_CRATE + slot-1;
    }

    //if( !(link.empty()) ) {
    //  params.insert(std::pair<std::string,std::string>("linkid",link));
    //}

    if ( crate >=0 && slot >=0 ) {
      //webSingleCard(o,m_uHTRs[ linkid ], linkid , item , params );
      webSingleCard(o,m_uHTRs[ linkid ], linkid , item );
    } else {

      *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      
      *o << "<head><title> uHTRLumi Status</title></head>\n";
      *o << "<body bgcolor='#6699ff'>\n";
      *o << "<h1 style='font-family: sans-serif'>uHTRLumi Status</h1>\n";

      for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {
          if (m_crateId[iCrate]<0) continue; 
          *o <<"<font face='ARIAL BLACK' size ='4'> &nbsp;&nbsp;&nbsp; Crate ID: "<< m_crateId[iCrate] <<"</font> \n"  ; 
	  //LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString(" ==== Render the crate view !!!! Crate %d ", iCrate ) );

          for( int slot = 1 ; slot <= MAX_CARDS_PER_CRATE ; slot++ ) {
	     int iCard = ( iCrate*MAX_CARDS_PER_CRATE) + slot - 1;
             if ( m_uHTRs[ iCard ] == 0 ) continue ; 
             std::map<std::string, bool > aM ;
             std::map<std::string, bool > oM ;
             try {
  	         m_uHTRs[ iCard ]->lumi_histo_status( aM, oM ) ;
             } RETHROW_UHAL_EXCEPTION;

             for ( size_t j=0; j < h_itemId.size() ; j++ ) {
                 if ( oM[ uhtr::uHTR::LUMI_HISTONAMES[j] ] ) { 
                    m_crateView[iCrate]->updateItemImage( slot, h_itemId[j] , getLinkForCGIData( toolbox::CrateRenderer::img_RedCircle ) ) ; 
                 } else {
                    m_crateView[iCrate]->updateItemImage( slot, h_itemId[j] , getLinkForCGIData( toolbox::CrateRenderer::img_GrayCircle ) ) ; 

                 }
             }  
          }
     
          m_crateView[iCrate]->render(o) ;
      }

    } 
    *o << "</body>";
    
  }

  void uHTRLumi::setupCrateView( int cid ) {
       if ( m_crateView[cid] == 0 ) {
          // testing of crateRender 
          // uTCA_centerMCH needs 14 slots , otherwise the CrateRenderer crash 
          m_crateView[cid] = new toolbox::CrateRenderer(14, toolbox::CrateRenderer::uTCA_centerMCH ) ;
          //m_crateView[cid] = new toolbox::CrateRenderer(13) ;
	  m_crateView[cid]->exportStandard(this);
	  m_crateView[cid]->addBoard("uHTR","#10FF0D");

          m_crateView[cid]->addBoardLink( "uHTR", 
                            getLinkForPeek(::toolbox::toString( "status&crate=%d", cid)+"&slot=%d"  ), 
                            "uHTR Lumi View", getLinkForCGIData( toolbox::CrateRenderer::img_YellowSquare) );

          h_itemId.clear() ;
	  for ( int ih =0 ; lumi_hName[ih]!=0 ; ih++ ) {
	      int h_itemId_ = m_crateView[cid]->addBoardItem( "uHTR", lumi_hName[ih], getLinkForCGIData( toolbox::CrateRenderer::img_GrayCircle )  ) ;
	      h_itemId.push_back ( h_itemId_ ) ;
	  }

          for ( int slot=1 ; slot <= MAX_CARDS_PER_CRATE ; slot++) {
	      int iCard = (cid*MAX_CARDS_PER_CRATE) + slot - 1;
              if ( m_info[iCard].slotId > -1 ) {
                 // need to addBoardLink first and then put boardInSlot   
	         //m_crateView[cid]->addBoardLink( "uHTR", 
                 //                     getLinkForPeek(::toolbox::toString( "status&crate=%d&slot=%d", cid, slot ) ), 
	         //         	     "uHTR Test View", getLinkForCGIData( toolbox::CrateRenderer::img_YellowSquare) );
    

                 m_crateView[cid]->boardInSlot( m_info[iCard].slotId,"uHTR");         
              } else {
                 m_crateView[cid]->noBoardInSlot( slot );
              }
          }

       } 
       
  }

  std::string uHTRLumi::makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item) {

    uint32_t linkid;
    
    for (linkid=0; linkid<uint32_t(MAX_CARDS_PER_CRATE*MAX_CRATE); linkid++) {
        if (m_uHTRs[linkid]==card) break;
    }
    int iCrate = linkid / 12 ;
    int iSlot  = (linkid % 12) + 1;
    return getLinkForPeek(::toolbox::toString( "%s&crate=%d&slot=%d",item.c_str(), iCrate, iSlot  ) );

  }

  void uHTRLumi::programLUT( hcal::uhtr::uHTR* card ) {

      std::vector<uint16_t> lut;
      // this is a 1 to 1 lut  
      for (uint16_t i=0; i<128; i++) lut.push_back(i);

      for ( int ifiber=0; ifiber<24; ifiber++) {
          for ( int ichan=0; ichan<3; ichan++) {
            card->lumi_program_lut(ifiber,ichan,lut);
          }
      }

  }

  int uHTRLumi::runTask() {    
    bool active=false;
    bool doQuit=false;
    
    do {
      if (!active || m_cmdQueue->size()>0) {
	int message=m_cmdQueue->pop();
	if (message==CMD_START) active=true;
	else if (message==CMD_HALT) active=false;
	else if (message==CMD_QUIT) doQuit=true;
      }
      if (active && !doQuit) {
         try {
  	     read();
         }  catch (...) {
	     LOG4CPLUS_INFO(getApplicationLogger(), ::toolbox::toString(" Lumi Read Error !!!! " ) );
         }
      }
    } while (!doQuit);      
      return 0;
  }
}
