#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRSource.hh"
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
#include "hcal/toolbox/InterchangePoint.hh"
#include "hcal/soap/ChannelActivation.hh"
//#include "hcal/toolbox/CrateRenderer.hh"
#include "xoap/MessageFactory.h" // for return
#include "xoap/Method.h"

struct uHTRSourceFEDPayloadHeader {
  uint32_t version_l1a; // MSB is version, this is version 1
  uint32_t orbit_period_start;
  uint32_t orbit_period_end;
  uint32_t npresent_wordsPerHisto; // top 16 bits is n_present, bottom 16 is words/histogram
};

struct uHTRSourceHistogram {
  uint32_t error_location; //
  uint32_t bin0;
};

XDAQ_INSTANTIATOR_IMPL(hcal::uHTRSource);

namespace hcal {


  uHTRSource::CardInfo::CardInfo() {
    present = false;
    crateId = -1;
    slotId = -1;
  }

  uHTRSource::uHTRSource(xdaq::ApplicationStub* as) : 
    ReadoutApplication(as), 
    m_accessProtect(::toolbox::BSem::FULL)
  {
    m_connection = 0 ;
    m_listOfTriggers = 0;

    xoap::bind(this, &hcal::uHTRSource::OnChannelControl, "ChannelControl", hcal::soap::ChannelActivation::SOAP_NS);

    for (int i=0; i<MAX_CRATE; i++) { 
      m_crateId[i] = -1;
    }
    for (int i=0; i<MAX_CARDS_PER_CRATE*MAX_CRATE; i++) {
      m_uHTRs[i]=0; // clear!
    } 

  }
  
  uHTRSource::~uHTRSource() {
    if(!m_connection) delete m_connection;  
    if(!m_listOfTriggers) delete m_listOfTriggers;
  }

  void uHTRSource::init()                throw (hcal::exception::Exception) {
    hcal::ReadoutApplication::init();

    //bool anyMonitoring=false;
    std::string friendlyname;
 
    hcal::cfg::CfgDocument* cfg=getConfigurationDoc();
    hcal::cfg::Context cxt_source("source");

    m_fedId = cfg->get( "FEDID",cxt_source, 18 );
    m_separateCapIDs = cfg->get("SeparateCAPIDs", cxt_source, false);
    m_binsToRead = cfg->get("BinsToRead",cxt_source, 32);
    m_connection = hcal::uhtr::getConnectionManager();
    m_nOrbits = cfg->get("NOrbits",cxt_source,0);

    m_triggerInterchangeName=cfg->get("TriggerListInterchangeName",cxt_source,"");

    // set up crates
    std::vector<int> crateIds;
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {
      m_crateId[iCrate]=cfg->get("CRATE_ID",iCrate,cxt_source,-1);
      if (m_crateId[iCrate]<0) continue; 
  
      // Set up each card in the crate
      hcal::cfg::Context cxt_uhtr("uHTR");
      cxt_uhtr.set("CRATE", m_crateId[iCrate]);

      for (int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++) {

	int iCard = (iCrate)*MAX_CARDS_PER_CRATE + slot - 1;

	CardInfo& info=m_info[iCard];

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
	  uhal::HwInterface hw = m_connection->getDevice(friendlyname);
	  m_uHTRs[ iCard ]=new hcal::uhtr::uHTR(friendlyname,hw);	  
	} RETHROW_UHAL_EXCEPTION;

	cfg::Context fccxt(cxt_source);
        fccxt.set("CRATE",info.crateId);
	fccxt.set("SLOT",slot);

	m_uHTRs[ iCard ]->setCrateSlot( m_crateId[ iCrate ], slot );

	if (m_uHTRs[iCard]->is4800()) MAX_CHANNELS_PER_FIBER=6;
	else MAX_CHANNELS_PER_FIBER=3;
	info.readMe=std::vector<bool>(MAX_CHANNELS_PER_FIBER*MAX_FIBERS_PER_CARD,false);

	for (int fiber=0; fiber<MAX_FIBERS_PER_CARD; fiber++){
	  for (int channel=0; channel<MAX_CHANNELS_PER_FIBER; channel++){
	    //set if channel should be read.
	    hcal::cfg::Context cxt_channel(fccxt);
	    cxt_channel.set("FIBER",fiber);
	    cxt_channel.set("CHANNEL",channel);
	    info.readMe[fiber*MAX_CHANNELS_PER_FIBER+channel]=cfg->get("README",cxt_channel,false);
	  }
	}
      } 
    }
    releaseConfigurationDoc(cfg);
    
    for (int iCrate=0; iCrate<MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

	  int iCard = (iCrate)*MAX_CARDS_PER_CRATE + slot - 1;
	  hcal::uhtr::uHTR* uhtr=m_uHTRs[ iCard ];
	  
	  if ( uhtr==0 ) continue;

	  try {
	    // read firmware version to check card availability
	    uint8_t flavorF, majorF, minorF, patchF;
	    uint8_t flavorB, majorB, minorB, patchB;
	    uhtr->firmwareRevision(true,flavorF, majorF, minorF, patchF);
	    uhtr->firmwareRevision(false,flavorB, majorB, minorB, patchB);
	    // if we read the default values, we could not communicate with the card
	    if (flavorB == 0 && majorB == 0 && minorB == 0 && patchB == 0) {
	      std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Back FPGA of %s",friendlyname.c_str());
	      XCEPT_RAISE(hcal::exception::Exception, errorMsg);
	    } else if(flavorF == 0 && majorF == 0 && minorF == 0 && patchF == 0) {
	      std::string errorMsg =  ::toolbox::toString("Could not read uHTR firmware revision from Front FPGA of %s",friendlyname.c_str());
	      XCEPT_RAISE(hcal::exception::Exception, errorMsg);
	    } 
	
	    // setup
	    m_uHTRs[iCard]->link_histo_setSeparateCapIDs(m_separateCapIDs);
	    m_uHTRs[iCard]->link_histo_orbit_integrate(m_nOrbits);

	  } RETHROW_UHAL_EXCEPTION;
        }
	
    }
    if (!m_triggerInterchangeName.empty()) {
      if (m_listOfTriggers!=0) delete m_listOfTriggers;
      m_listOfTriggers=new ExternalTriggerListType;
      hcal::toolbox::InterchangePoint::getInstance().addObject(m_triggerInterchangeName,m_listOfTriggers); // TODO: Understand reasonable cleanup....
    }
  }

  void uHTRSource::enable()              throw (hcal::exception::Exception) {

    /** See if we can find the number of triggers to take */
    xdaq::Application* tapp=getApplicationContext()->getFirstApplication("DummyTriggerAdapter");
    if (tapp!=0) {
      xdata::Integer* ttt=dynamic_cast<xdata::Integer*>(tapp->getApplicationInfoSpace()->find("TriggersToTake"));      
      n_events_requested=ttt->value_;
    } else {
      n_events_requested=-1;
    }

    if (n_events_requested>0) LOG4CPLUS_INFO(getApplicationLogger(),::toolbox::toString("Events requested: %d",n_events_requested));
    else LOG4CPLUS_INFO(getApplicationLogger(),"No event limit identified");	

    n_event=0;

    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
      for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {
	
	int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
	if(m_uHTRs[iCard] ==0 ) continue;

	m_uHTRs[iCard]->link_histo_clear();
      }
    }

    hcal::ReadoutApplication::enable();
  }

  void uHTRSource::reset()              throw (hcal::exception::Exception) {
    hcal::ReadoutApplication::reset();

    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {

            int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
            if(m_uHTRs[iCard] !=0 ) {

   	      delete m_uHTRs[iCard];
  	      m_uHTRs[iCard]=0;
            }
        }
    }
  }

  std::string uHTRSource::makeLinkForCardAction(const hcal::uhtr::uHTR* card,const std::string& item) {

    uint32_t linkid;
    
    for (linkid=0; linkid<uint32_t(MAX_CARDS_PER_CRATE*MAX_CRATE); linkid++) {
        if (m_uHTRs[linkid]==card) break;
    }
    int iCrate = linkid / 12 ;
    int iSlot  = (linkid % 12) + 1;
    return getLinkForPeek(::toolbox::toString( "%s&crate=%d&slot=%d",item.c_str(), iCrate, iSlot  ) );

  }


  int uHTRSource::readout(unsigned int expected, unsigned int* eventNumbers, std::vector< ::toolbox::mem::Reference* >& data) throw (hcal::exception::Exception) {

    m_accessProtect.take();


    if (expected==0 && n_events_requested>0 && n_events_requested==int(n_event)) {
      LOG4CPLUS_DEBUG(getApplicationLogger(),"Done with run and waiting for stop...");
      printf("%d %d\n",n_events_requested, n_event);
      ::usleep(1000);
      m_accessProtect.give();
      return 0;
    }

    int n_channels=0;
    unsigned int planningFor=expected, actual=0;

    // first, we count the number of histograms that we will read out.
    for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
      for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {
	int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
	m_info[iCard].nread=0;
	for (size_t fib_chan=0; fib_chan<m_info[iCard].readMe.size(); fib_chan++){
	  if(m_info[iCard].readMe[fib_chan]) {
	    n_channels++;
	    m_info[iCard].nread++;
	  }
	}
      }
    }

    //    printf("CHAN:%d expected:%d\n",n_channels, expected);
    
    if (expected==0) { // we are looking to see...
      bool gotAnEvent=(n_channels!=0);
      for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {
	  int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
      	  if (m_info[iCard].nread==0) continue;
	  if (!m_uHTRs[iCard]->link_histo_ready()) gotAnEvent=false;
	  if (!gotAnEvent) break;
	}
	if (!gotAnEvent) break;
      }
      if (gotAnEvent) {
	planningFor=1;
	n_event++;
	eventNumbers=&n_event;
      } else {
	::usleep(1);
	m_accessProtect.give();
	return 0;
      }
    }

    if (planningFor!=1) LOG4CPLUS_ERROR(getApplicationLogger(),"uHTRSource doesn't deal well with multiple events required at once!");

    for (unsigned int ievt=0; ievt<planningFor; ievt++) {

      int n_histos=n_channels*(m_separateCapIDs?(4):(1));
      int size_in_bytes=n_histos*(m_binsToRead+2)*sizeof(uint32_t)+sizeof(uHTRSourceFEDPayloadHeader);
      ::toolbox::mem::Reference* xref=createFrame(size_in_bytes,eventNumbers[ievt]);
      uHTRSourceFEDPayloadHeader* header=(uHTRSourceFEDPayloadHeader*)getBodyCurrent();
      header->version_l1a            =(1<<24)|(eventNumbers[ievt]); 
      header->orbit_period_start     =0; //FIXME
      header->orbit_period_end       =0; //FIXME
      header->npresent_wordsPerHisto =(n_histos << 16) + (m_binsToRead << 1) + m_separateCapIDs;

      uint32_t* histoSpaceBase=((uint32_t*)getBodyCurrent())+(sizeof(uHTRSourceFEDPayloadHeader)/sizeof(uint32_t));
      uint32_t* histoPtr=histoSpaceBase;

      for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {
	  int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
	  if (m_info[iCard].nread==0) continue;
	  while (!m_uHTRs[iCard]->link_histo_ready()) {
	    ::usleep(1000*10); // 10 ms
	  }
	  for (size_t fib_chan=0; fib_chan<m_info[iCard].readMe.size(); fib_chan++){
	    if(!m_info[iCard].readMe[fib_chan])continue;
	    if (header->orbit_period_start==0){
	      m_uHTRs[iCard]->link_histo_read_FirstAndLastOrbits(header->orbit_period_start,header->orbit_period_end);
	      printf("Orbits: %d %d \n",header->orbit_period_start,header->orbit_period_end);
	    }

	    std::vector<uint32_t> histo=m_uHTRs[iCard]->link_histo_read(fib_chan);
	    int sizetotal=histo.size();
	    if (m_separateCapIDs){sizetotal=sizetotal/4;}
	    if (m_binsToRead>sizetotal){
	      std::string errorMsg =  ::toolbox::toString("Number of bin requested is larger than histogram size. m_binsToRead set to %d",sizetotal);
	      XCEPT_RAISE(hcal::exception::Exception, errorMsg);
	      m_binsToRead=sizetotal;
	    }
	    uHTRSourceHistogram* ThisHistogram=(uHTRSourceHistogram*)histoPtr;
	   
	    ThisHistogram->error_location= (m_crateId[iCrate-1] << 16) + (slot << 12) + ((fib_chan/MAX_FIBERS_PER_CARD)<< 7) + ((fib_chan % MAX_FIBERS_PER_CARD)<<2);//add possible error message [31:24]. slot starts at 1 while fiber and channels start at 0.
	    uint32_t* bins=&(ThisHistogram->bin0);
	    int ncap=1;
	    if (m_separateCapIDs){ncap=4;}
	    for (int i_capIDs=0; i_capIDs<ncap; i_capIDs++){
	      ThisHistogram->error_location++;
	      for (int bin=i_capIDs*sizetotal; bin<i_capIDs*sizetotal+m_binsToRead; bin++){
		bins[bin]=histo[bin];
	      }
	      bins[m_binsToRead]=0;
	      for (int restbins=i_capIDs*sizetotal+m_binsToRead; restbins<(i_capIDs+1)*sizetotal;restbins++){
		if (bins[m_binsToRead]+histo[restbins]<bins[m_binsToRead]){
		  bins[m_binsToRead]=std::numeric_limits<uint32_t>::max(); 
		  break;
		}
		bins[m_binsToRead]+=histo[restbins];
	      }	    
	      //move the pointer...
	      histoPtr+=(m_binsToRead+2);
	    }
	  }
	}
      }
      data.push_back(xref);
      actual++;
      for (int iCrate=1; iCrate<=MAX_CRATE; iCrate++) {
        for ( int slot=1; slot<=MAX_CARDS_PER_CRATE; slot++ ) {
	  int iCard = (iCrate-1)*MAX_CARDS_PER_CRATE + slot - 1;
	  if (m_info[iCard].nread==0) continue;
	  m_uHTRs[iCard]->link_histo_clear();
	}
      }
      // now, we add this to the trigger list if appropriate
      if (m_listOfTriggers!=0) {
	HcalExtendedTriggerMessageBlock tmb;
	memset(&tmb,0,sizeof(tmb));

	tmb.StdBlock.orbitNumber=header->orbit_period_end ;
	tmb.StdBlock.eventNumber=n_event&0xFFFFFF;
	tmb.StdBlock.flags_daq_ttype=0x1a000009u;
	tmb.runNumber=getRunNumber();
	
	m_listOfTriggers->push(tmb);
      }
    }
    m_accessProtect.give();
    return actual;
  }

  unsigned int uHTRSource::getAverageEventSize() const { return sizeof(uint32_t) * (4 + 4 * 72 * 34);}//32 bins per histogram, seperate cap ids, 72 histograms? how much safety margin?

  unsigned int uHTRSource::getSourceId() const { return m_fedId; }

  xoap::MessageReference hcal::uHTRSource::OnChannelControl(xoap::MessageReference msg) {

    using namespace hcal::soap;
    m_accessProtect.take();

    std::vector<ChannelActivation::Id> waxOn, waxOff;

    ChannelActivation::parseMessage(msg,waxOn,waxOff);

    std::vector<ChannelActivation::Id>::const_iterator i;

    for (i=waxOn.begin(); i!=waxOn.end(); i++) 
      printf("Wax on: %d %d %d\n",i->crate, i->slot, i->channel);
    for (i=waxOff.begin(); i!=waxOff.end(); i++) 
      printf("Wax off: %d %d %d\n",i->crate, i->slot, i->channel);

    m_accessProtect.give();

    xoap::MessageReference reply=xoap::createMessage();
    return reply;
  }

}
