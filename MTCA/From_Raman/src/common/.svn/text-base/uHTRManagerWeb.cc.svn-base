#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRManager.hh"
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "toolbox/string.h"


namespace hcal {

  // Displace information for each uHTR card
  void uHTRManager::webSingleCard(xgi::Output* o, hcal::uhtr::uHTR* card, 
				   int myslot,
				   const std::string& item,
				   const std::map<std::string, std::string>& params)   {
       
       if      (item=="daq"     || item=="daqspy")     web_uHTR_DAQ( o, card, myslot, item, params) ;
       else if (item=="trigger" || item=="triggerspy") web_uHTR_Trigger( o, card, myslot, item, params) ;
       else             webSingleCard_Link( o, card, myslot, item, params ) ;

  }

  void uHTRManager::webSingleCard_Link(xgi::Output* o, hcal::uhtr::uHTR* card, 
				   int myslot,
				   const std::string& item,
				   const std::map<std::string, std::string>& params)   {
    using namespace hcal::uhtr;

    
    *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
    *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
    
    *o << "<head><title> uHTR " << card->name() << "</title></head>\n";
    *o << "<body bgcolor='#6699ff'>\n";
    *o << "<h1 style='font-family: sans-serif'>uHTRManager " << card->name() << " Board Status</h1>\n";    

    // here's where we display the information...
    const char* names[]={"00","01","02","03","04","05","06","07","08","09","10","11",
                         "12","13","14","15","16","17","18","19","20","21","22","23", 0};
    const int inames=24;

    try {
      // get firmware information
      uint8_t flavorF, majorF, minorF, patchF;
      uint8_t flavorB, majorB, minorB, patchB;
      card->firmwareRevision(true,flavorF, majorF, minorF, patchF);
      card->firmwareRevision(false,flavorB, majorB, minorB, patchB);
      *o << "<p align='right'><table bgcolor='#ffffff' width=80% align='center' border='2'> " << std::endl ;
      *o << "<tr><td>Front FPGA firmware revision:<td> "  
         << uHTR::firmwareFlavorToString(flavorF).c_str()<<"    "<< std::hex<<(int)majorF << "." << (int)minorF << "." << (int)patchF << std::dec <<" </tr>"<< std::endl;
      *o << "<tr><td>Back FPGA firmware revision:<td> " 
	 << uHTR::firmwareFlavorToString(flavorB).c_str()<<"    "<< std::hex<< (int)majorB << "." << (int)minorB << "." << (int)patchB << std::dec <<" </tr>"<< std::endl;
      
      //      card->link_setup_query( align_v, isAutoOrb, clk_src ) ;
      int islot = atoi(params.find("slot")->second.c_str());
      CardInfo& info=m_info[islot-1].bag ;

      uint32_t evn,orn;
      card->info_evn_orn(evn,orn);

      *o << "<tr><td>Event Number<td>" << evn << "</tr>\n";
      *o << "<tr><td>Orbit Number <td>"<< orn <<"</tr>\n"; 
      *o << "<tr><td>Link Alignment Orbit Delay : <td>"<< std::dec << info.orbitDelay << "</tr>\n";
      *o << "<tr><td>Clock status: <td><a href=\"" << makeLinkForCardAction(card,"clock") << "\">Check</a></tr>\n";
      *o << "</table>\n";
      
    }
    catch (std::exception &e) {
      std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
      LOG4CPLUS_ERROR(getApplicationLogger(), message);
      *o << "<BR><B>" << message.c_str() << "</B>";
      return;
    }
    
    // right pane
    //*o << "<table rules=all><tr><td width=50% >";
    //*o << "</td>\n";
    //*o << "<td bgcolor=#CCCCCC width=50% valign=top>";
   
    *o << "<br> \n" ;
    // First PPOD ( fiber 0~ 11 ) 
    *o << " PPOD Rx 0\n" ;
    *o << "<table bgcolor='#ffffff' width=100% border='1' align='right'>" ;
    *o << "<tr><td>Action";
    for (int i=0; i<inames/2; i++) {
      //*o << "<td><a href=\"" << makeLinkForCardAction(card,"spy")+"&linkid=" << std::dec << i<<"\">spy</a>";
      *o << "<td><a href=\"" << makeLinkForCardAction(card,"spy")+"&linkid=" << std::dec << i<<"\">" << names[i] <<"</a>";
    }
    *o << "</tr>\n";

    // output 
    try {

      *o << "<tr><td>Bad Data Counter";
          for (int i=0; i<inames/2; i++) {
              std::string cnt_state = ( card->info_link_BadDataCounterOn(i) ==0 ) ? "X": "ON" ;
     	      *o << "<td align='right'>" << cnt_state;
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>Bad Data";
          for (int i=0; i<inames/2; i++) {
     	      *o << "<td align='right'>" << card->info_link_nBadData( i );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Rollover Count";
          for (int i=0; i<inames/2; i++) {
     	      *o << "<td align='right'>" << card->info_link_nBadDataRollover( i );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Bad Data Rate";
          for (int i=0; i<inames/2; i++) {
      	        *o << "<td align='right'>" << card->info_link_BadDataRate( i ) ; 
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>GTX Reset Count";
          std::vector<uint32_t> gtxErrV = card->link_gtx_reset_count() ;
          for (int i=0; i<inames/2; i++) {
     	      *o << "<td align='right'>" << gtxErrV[i];
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>Bad Align";
          for (int i=0; i<inames/2; i++) {
   	      *o << "<td align='right'>" << card->info_link_nBadAlign( i );
          }
      *o << "</tr>\n";
      
      *o << "<tr><td> DTC Pulse OK";
          for (int i=0; i<inames/2; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 6) & 1 ) ;
          }
      *o << "</tr>\n";

      *o << "<tr><td> Fiber Pulse OK";
          for (int i=0; i<inames/2; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 4) & 1 ) ;
          }
      *o << "</tr>\n";

      *o << "<tr><td>Alignment Done";
          for (int i=0; i<inames/2; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 1) & 1 );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Reset Done";
          for (int i=0; i<inames/2; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,0) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Pll Lick";
          for (int i=0; i<inames/2; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,1) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Byte Aligned";
          for (int i=0; i<inames/2; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,2) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Align occupancy";
          for (int i=0; i<inames/2; i++) {
  	      *o << "<td align='right'>" << card->info_link_alignOccupancy(i);
          }
      *o << "</tr>\n";

      *o << "<tr><td>Align delta";
          for (int i=0; i<inames/2; i++) {
  	      *o << "<td align='right'>" << card->info_link_alignDelta(i);
          }
      *o << "</tr>\n";

      *o << "<tr><td>Orbit rate";
          for (int i=0; i<inames/2; i++) {
	      *o << "<td align='right'>" << card->info_link_orbitRate(i);
          }
      *o << "</tr>\n";
    }
    catch (std::exception &e) {
      std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
      LOG4CPLUS_ERROR(getApplicationLogger(), message);
      *o << "<BR><B>" << message.c_str() << "</B>";
      return;
    }
    *o << "</table>\n";

    *o << "<br> \n" ;

    // Second PPOD ( fiber 12 ~ 23 ) 
    *o << " PPOD Rx 1\n" ;
    *o << "<table bgcolor='#ffffff' width=100% border='1'>";
    *o << "<tr><td>Action";
    for (int i=12; i<inames; i++) {
      //*o << "<td><a href=\"" << makeLinkForCardAction(card,"spy")+"&linkid=" << std::dec << i<<"\">spy</a>";
      *o << "<td><a href=\"" << makeLinkForCardAction(card,"spy")+"&linkid=" << std::dec << i<<"\">" << names[i] <<"</a>";
    }
    *o << "</tr>\n";

    // output 
    try {
      *o << "<tr><td>Bad Data Counter";
          for (int i=12; i<inames; i++) {
              std::string cnt_state = ( card->info_link_BadDataCounterOn(i) ==0 ) ? "X": "ON" ;
     	      *o << "<td align='right'>" << cnt_state;
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>Bad Data";
          for (int i=12; i<inames; i++) {
     	      *o << "<td align='right'>" << card->info_link_nBadData( i );
          }
      *o << "</tr>\n";
 
      *o << "<tr><td>Rollover Count";
          for (int i=12; i<inames; i++) {
     	      *o << "<td align='right'>" << card->info_link_nBadDataRollover( i );
          }
      *o << "</tr>\n";
     
      *o << "<tr><td>Bad Data Rate";
          for (int i=12; i<inames; i++) {
      	        *o << "<td align='right'>" << card->info_link_BadDataRate( i ) ; 
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>GTX Reset Count";
          std::vector<uint32_t> gtxErrV = card->link_gtx_reset_count() ;
          for (int i=12; i<inames; i++) {
     	      *o << "<td align='right'>" << gtxErrV[i];
          }
      *o << "</tr>\n";
      
      *o << "<tr><td>Bad Align";
          for (int i=12; i<inames; i++) {
   	      *o << "<td align='right'>" << card->info_link_nBadAlign( i );
          }
      *o << "</tr>\n";
      
      *o << "<tr><td> DTC Pulse OK";
          for (int i=12; i<inames; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 6) & 1 );
          }
      *o << "</tr>\n";

      *o << "<tr><td> Fiber Pulse OK";
          for (int i=12; i<inames; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 4) & 1 );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Alignment Done";
          for (int i=12; i<inames; i++) {
              *o << "<td align='right'>" << ( (card->info_link_alignStatus(i) >> 1) & 1 );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Reset Done";
          for (int i=12; i<inames; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,0) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Pll Lick";
          for (int i=12; i<inames; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,1) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Byte Aligned";
          for (int i=12; i<inames; i++) {
	    *o << "<td align='right'>" << ( card->info_link_ResetInfo(i,2) );
          }
      *o << "</tr>\n";

      *o << "<tr><td>Align occupancy";
          for (int i=12; i<inames; i++) {
  	      *o << "<td align='right'>" << card->info_link_alignOccupancy(i);
          }
      *o << "</tr>\n";

      *o << "<tr><td>Align delta";
          for (int i=12; i<inames; i++) {
  	      *o << "<td align='right'>" << card->info_link_alignDelta(i);
          }
      *o << "</tr>\n";

      *o << "<tr><td>Orbit rate";
          for (int i=12; i<inames; i++) {
	      *o << "<td align='right'>" << card->info_link_orbitRate(i);
          }
      *o << "</tr>\n";
    }
    catch (std::exception &e) {
      std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
      LOG4CPLUS_ERROR(getApplicationLogger(), message);
      *o << "<BR><B>" << message.c_str() << "</B>";
      return;
    }
    *o << "</table>\n";

    *o << "<br> \n" ;

    // Check the SPY
    if (item=="spy") {
      try {
	int linkid=atoi(params.find("linkid")->second.c_str());
	int presamples=1;
	int offset=0;
	card->link_spySetup(true,false, false, false, presamples,offset);
	card->link_spyExecute();
	std::vector<uint32_t> buffer;
	buffer=card->link_spyRead(linkid);

	*o << "<h2>Input spy for fiber "<< linkid<< "</h2>\n";
	//*o << "<h4>Pre-Sample : "<< align_v[1] << "&nbsp;&nbsp;&nbsp; Offset : "<< align_v[2] <<"</h4> \n";
	
	//make HTML histogram
	int nchannel=card->n_channels_per_fiber();
	card->link_histo_start();

	for (uint chan=0; chan<nchannel; chan++){
	  std::vector<uint32_t> histobincontent=card->link_histo_read(linkid*nchannel+chan);
	  std::stringstream ss;
	  ss << linkid*nchannel+chan;
	  std::string histname="1d amplitude for channel "+ss.str(); 
	  int sizeofhisto=histobincontent.size();
	  hcal::toolbox::HTMLHistoRender* m_HTMLHistoFiber=new hcal::toolbox::HTMLHistoRender(sizeofhisto,histname, 600,300);
	  m_HTMLHistoFiber->loadBins(histobincontent);
	  m_HTMLHistoFiber->render(*o);
	}

	*o << "<table width=80% border='1'>";
	*o << "<tr><th>i<th>Raw <th>Q0<th> Q1<th> Q2<th> C0<th> C1<th> C2";
	int jj=0;
	for (std::vector<uint32_t>::const_iterator j=buffer.begin(); j!=buffer.end(); j++) {
	  uint32_t qiea=((*j)>>25)&0x7F;
	  uint32_t qieb=((*j)>>17)&0x7F;
	  uint32_t qiec=((*j)>>9)&0x7F;
	  uint32_t capa=((*j)>>7)&0x3;
	  uint32_t capb=((*j)>>5)&0x3;
	  uint32_t capc=((*j)>>3)&0x3;
	  *o << ::toolbox::toString("<tr><td>%04d <td>%08x <td>%2x <td>%2x <td>%2x <td>%d <td>%d <td>%d</tr>\n",jj, (int)*j, (int)qiea, (int)qieb, (int)qiec, (int)capa, (int)capb, (int)capc);
	  *o << std::endl;
	  jj++;
	}
	*o << "</table>"; 
      }
      catch (std::exception &e) {
	std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
	LOG4CPLUS_ERROR(getApplicationLogger(), message);
	*o << "<BR><B>" << message.c_str() << "</B>";
	//return;
      }
    }
 
    if (item=="clock") {
      // output clock rates
      
      try {
	std::vector<std::pair<double,double> > clockRatesF = card->getClockRates(true);
	std::vector<std::pair<double,double> > clockRatesB = card->getClockRates(false);

	*o << "<table border='1'><tr><th colspan=3 align=center>Clock Rate (MHz)</th></tr>\n";
	*o << "<tr><th>Expected</th><th>Observed (Front)</th><th>Observed (Back)</th></tr>\n";
	for (size_t i=0; i<clockRatesF.size() ; i++) {
	  *o << "<tr><td>" << clockRatesF[i].first << "<td> " << clockRatesF[i].second << "<td>" << clockRatesB[i].second << "</tr>\n";
	}
	*o << "</table>\n";
      }
      catch (std::exception &e) {
	std::string message = ::toolbox::toString("Could not read clock rate from card %s: %s", card->name().c_str(), e.what());
	LOG4CPLUS_ERROR(getApplicationLogger(), message);
	*o << "<BR><B>" << message.c_str() << "</B>";
	//return;
      }
    }
 

    *o << " \n";


  }

  void uHTRManager::web_uHTR_DAQ(xgi::Output* o, hcal::uhtr::uHTR* card, 
				   int myslot,
				   const std::string& item,
				   const std::map<std::string, std::string>& params)   {
    using namespace hcal::uhtr;

    
    *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
    *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
    
    *o << "<head><title> uHTR DAQ " << card->name() << "</title></head>\n";
    *o << "<body bgcolor='#6699ff'>\n";
    *o << "<h1 style='font-family: sans-serif'>uHTRManager " << card->name() << " DAQ Status</h1>\n";    

     try {
      //      card->link_setup_query( align_v, isAutoOrb, clk_src ) ;
      uint32_t evn,orn;
      card->info_evn_orn(evn,orn);

      // DAQ infomation
      uint32_t moduleid, bcOffset, occupancy, daq_nps, daq_ns, pline, tp_nps, tp_ns;
      bool nzs,enabled, zs_tp_en, zs_tp_soionly, markAndPass, zs_sumByTwo;
      uint32_t zs_mask;
      std::vector<uint32_t> zs_thresholds;
      card->daq_path_setup_query(moduleid,bcOffset, daq_ns, daq_nps, tp_ns, tp_nps, pline, enabled);
      card->daq_zs_setup_query(nzs, markAndPass, zs_mask, zs_sumByTwo, zs_thresholds, 
			       zs_tp_en, zs_tp_soionly);
      occupancy=card->daq_path_headerOccupancy();
      std::string str_daq_enable = enabled?("ENABLED"):("DISABLED") ;
      std::string str_zs, str_tp_zs;
      if (nzs) str_zs="Disabled";
      else if (markAndPass) str_zs="Mark-And-Pass";
      else str_zs="Enabled";

      if (!nzs) {
	if (zs_sumByTwo) str_zs+=" Sum-of-Two";
	else str_zs+=" Sum-of-One";
      }

      if (!zs_tp_en || nzs) str_tp_zs="Disabled";
      else if (zs_tp_soionly) str_tp_zs="SOI-only";
      else str_tp_zs="Any-Non-Zero";

      *o << "<table rules=all bgcolor='#ffffff' width=80% border='1'><tr> " << std::endl; 
      *o << "<tr><td align='center'> Event Number <td align='right'>" << evn <<"</tr> \n" ;
      *o << "<tr><td align='center'> Orbit Number <td align='right'>" << orn <<"</tr> \n" ;
      
      *o << "<tr><td align='center'> Header Occupancy  <td align='right'>" <<occupancy <<"</tr> \n";
      *o << "<tr><td align='center'> DAQ PATH          <td align='right'>" << str_daq_enable <<"</tr> \n";

      *o << "<tr><td align='center'> Zero Suppression  <td align='right'>"<< str_zs << "</tr> \n";
      *o << "<tr><td align='center'> TP ZS             <td align='right'>"<< str_tp_zs << "</tr>\n";
      *o << "<tr><td align='center'> DAQ Pre-Samples   <td align='right'>"<< daq_nps << "</tr>\n"; 
      *o << "<tr><td align='center'> N of DAQ Samples  <td align='right'>"<< daq_ns <<"</tr>\n";
      *o << "<tr><td align='center'> TP Pre-Samples    <td align='right'>"<< tp_nps << "</tr>\n"; 
      *o << "<tr><td align='center'> N of TP Samples   <td align='right'>"<< tp_ns <<"</tr>\n";
      *o << "<tr><td align='center'> Module Id         <td align='right'>"<< moduleid    << "</tr>\n"; 
      *o << "<tr><td align='center'> BC Offset         <td align='right'>"<<bcOffset <<"</tr>\n";
      *o << "<tr><td align='center'> DAQ SPY:          <td align='right'><a href=\"" << makeLinkForCardAction(card,"daqspy") << "\">Check</a></tr>\n";
      *o << "</table>" << std::endl ;
    }
    catch (std::exception &e) {
      std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
      LOG4CPLUS_ERROR(getApplicationLogger(), message);
      *o << "<BR><B>" << message.c_str() << "</B>";
      return;
    }

    if ( item == "daqspy" ) { 
      try {

          std::vector<uint32_t> buffer;
          card->daq_spy_read(buffer,true); // one-time-spy

          if (buffer.empty()) {
             *o << ::toolbox::toString(" No DAQ words found\n");
             //return;
          } else {

            *o << "<table width=100%>";

            for (size_t i=0; i<buffer.size(); i++) {
                uint16_t thisword=buffer[i]&0xFFFF;
		// interpretation!
                switch (i) {
                      case (1) :
                      *o << ::toolbox::toString("<tr><td> Event Number <td>%04d  <td>%x </tr>\n", (buffer[0]&0xFF)+(thisword<<8),(buffer[0]&0xFF)+(thisword<<8) );
                      *o << std::endl;
                      break;
                      case (3) :
                      *o << ::toolbox::toString("<tr><td> Orbit Number <td>%d  <td>submodule: %d, (0x%x) </tr>\n", (thisword>>11)&0x1F,thisword&0x7FF,thisword&0x7FF );
                      *o << std::endl;
                      break;
                      case (4):
                      *o << ::toolbox::toString("<tr><td> Format <td>%d, <td>BCN %d (0x%x) </tr>",(thisword>>12)&0xF,thisword&0xFFF,thisword&0xFFF);
                      *o << std::endl;
                      break;
                      case (5):
                      *o << ::toolbox::toString("<tr><td>Presamples <td>%d, <td>TP words %d </tr>",(thisword>>3)&0x1F,(thisword>>8)&0xFF);
                      *o << std::endl;
                      break;
                      case (6):
                      *o << ::toolbox::toString("<tr><td>Unsuppressed <td>%d, <td>compact mode %d </tr>",(thisword>>15)&0x1,(thisword>>14)&0x1);
                      *o << std::endl;
                      break;
                      case (7):
                      *o << ::toolbox::toString("<tr><td>Flavor <td>%d, <td>Pipeline length %d </tr>",(thisword>>8)&0x7F,thisword&0xFF);
                      *o << std::endl;
                      break;
                }
            }
          *o << "</table>";
          *o << "<table width=100%>";
          *o << ::toolbox::toString("<tr><td> Header Flavor  <td> Error bits  <td>Capid <td>Fiber  <td>Channel </tr>" );
          *o << std::endl;
          for (size_t i=0; i<buffer.size(); i++) {
              uint16_t thisword=buffer[i]&0xFFFF;
              if (i>=buffer.size()-4) {
                 if (i==buffer.size()-4) {
                    *o << ::toolbox::toString("<tr><td> NS : %d <td> WC : %d </tr>\n", thisword>>11, thisword&0x7ff );
                    *o << std::endl;
                 }
              } else if (thisword&0x8000) {
                 *o << ::toolbox::toString("<tr><td> %d <td> %d   %d <td> %d <td> %d <td> %d </tr>\n", 
            (thisword>>12)&0x7,(thisword>>11)&0x1,(thisword>>10)&0x1,(thisword>>8)&0x3,(thisword>>2)&0xF,(thisword>>0)&0x3);
                 *o << std::endl;
              } else {
                 *o << std::endl;
              }

          }
          *o << ::toolbox::toString("<tr><td>   <td>   <td> <td>QIE   </tr>\n" ) ;
          for (size_t i=0; i<buffer.size(); i++) {
              uint16_t thisword=buffer[i]&0xFFFF;
              *o << ::toolbox::toString("<tr><td>%04d <td>%x <td>%04x <td>%4d  %4d </tr>\n", 
                                uint32_t(i), (buffer[i]>>16)&0xF, thisword,thisword&0x7F,(thisword>>8)&0x7f );
          }
	  *o << std::endl;
          *o << "</table>";
          } 
      }
      catch (std::exception &e) {
        *o << "<table width=100%>";
	std::string message = ::toolbox::toString("Could not read clock rate from card %s: %s", card->name().c_str(), e.what());
	LOG4CPLUS_ERROR(getApplicationLogger(), message);
	*o << "<BR><B>" << message.c_str() << "</B>";
        *o << "</tr></table>";
	//return;
      }
    }
  }

  void uHTRManager::web_uHTR_Trigger(xgi::Output* o, hcal::uhtr::uHTR* card, 
				   int myslot,
				   const std::string& item,
				   const std::map<std::string, std::string>& params)   {
    using namespace hcal::uhtr;

    
    *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
    *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
    
    *o << "<head><title> uHTR Trigger " << card->name() << "</title></head>\n";
    *o << "<body bgcolor='#6699ff'>\n";
    *o << "<h1 style='font-family: sans-serif'>uHTRManager " << card->name() << " Trigger Status</h1>\n";    

    const char* names[]={"01","02","03","04" } ;
    try {
      /*
      std::string cgi_url = getLinkForPeek( ::toolbox::toString("%s&slot=%d",item.c_str(), myslot) ) ;
      *o << "<form method=\"get\" action=\""<<cgi_url<<"\"> \n" ;
      *o << "<table rules=all> " ;
      *o << "<tr><td>Trigger lut group : <td> <INPUT TYPE='text' NAME=\"TRIG_LUT_GROUP\"/>  </tr>\n " ; 
      *o << "</table> \n" ; 
      *o << "</form> \n" ;
      *o << "<br> \n" ;
      // Trigger information
      cgicc::Cgicc cgi;
      cgicc::form_iterator group_ = cgi.getElement("TRIG_LUT_GROUP");
      long group = 0 ;
      if( !group_->isEmpty() && group_ != cgi.getElements().end() ) {
        std::cout << " Group: " << group_->getIntegerValue() << std::endl;
        group = group_->getIntegerValue() ;
      }
      std::cout<<" *** trigger group : "<< group << std::endl ;
      */
      // Trigger Parameters
      int n_addr_bits, n_data_bits, n_luts ;
      uint32_t base_address,  stride ; 
      bool last_group ;

      *o << "<table rules=all><tr>" ;
      // Control table
      *o << "<td> \n" ;
      *o << "<table bgcolor='#ffffff' width=100% border='1' align='center'>" ;
      *o << "<tr> \n" ; 
      *o << "<th> LUT <th> N of LUTs <th> N Data Bits <th> Base Address \n";
      *o << "</tr>\n";
      for (int i=0; i< 4 ; i++) {
          *o << "<tr> \n" ; 
          *o << "<td><a href=\"" << makeLinkForCardAction(card,"trigger")+"&lut_id=" << std::dec << i<<"\">" << names[i] <<"</a>";
          card->trigger_query_lut_type( i, n_addr_bits, n_data_bits, n_luts, base_address, stride, last_group );
          *o << "<td> " << n_luts <<" <td> "<< n_data_bits <<" <td> " << std::hex << base_address  ;
          *o << "</tr>\n";
      }
      *o <<"</table> \n" ; 
      // LUT displace table
      *o <<"<td> \n" ;
      *o <<"<table bgcolor='#ffffff' width=100% border='1' align='center'>" ;
      if ( item == "trigger" )  {

      }
      *o <<"</table> \n" ;
      *o <<"</tr></table>" ; // end of Lut display
      *o <<" <br>\n" ;

      *o << " Trigger Link Spy \n" ;
      *o <<"<table bgcolor='#ffffff' width=100% border='1' align='center'><tr>" ;
      const char* names[]={"PPOD_0","PPOD_1","PPOD_2","PPOD_3","PPOD_4","PPOD_5","PPOD_6","PPOD_7","PPOD_8",
                           "PPOD_9","PPOD_10","PPOD_11","SFP_0","SFP_1",0};
      for ( int i =0 ; i < 14 ; i++) {
          *o <<"<td><a href=\"" << makeLinkForCardAction(card,"triggerspy")+"&link_id=" << std::dec << i<<"\">"  << names[i] <<"</a>";
      }
      *o <<"</tr></table> \n" ;
     
      *o <<"<table bgcolor='#ffffff' width=100% border='1' align='center'>" ;
      if ( item == "triggerspy" )  {
         // ilink and orbit_delay  
	 int ilink = atoi(params.find("link_id")->second.c_str());
	 int islot = atoi(params.find("slot")->second.c_str());
         CardInfo& info=m_info[islot-1].bag;

         card->link_query() ;
         usleep(10000);
         card->trigger_spy_execute(0x3FFF, info.orbitDelay);
         std::vector<uint32_t> buffer;
         card->trigger_spy_read(ilink,buffer);
         for ( size_t i=0 ; i < buffer.size(); i++)  { 
             *o <<" <tr><td> "<< std::dec << int(i)  <<" <td> "<< std::hex << buffer[i] <<"</tr> \n" ; 
         }

      }
      *o <<"</table> \n" ;

    }
    catch (std::exception &e) {
        *o << "<table width=100%>";
	std::string message = ::toolbox::toString("Could not read trigger info from card %s: %s", card->name().c_str(), e.what());
	LOG4CPLUS_ERROR(getApplicationLogger(), message);
	*o << "<BR><B>" << message.c_str() << "</B>";
        *o << "</tr></table>";
    }
  }

}


