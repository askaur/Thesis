#include "hcal/uhtr/uHTR.hh"
#include "hcal/uhtr/uHTRLumi.hh"
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "toolbox/string.h"

namespace hcal {

  void uHTRLumi::webSingleCard(xgi::Output* o, hcal::uhtr::uHTR* card, 
				   int myslot,
				   const std::string& item ) { 
//				   const std::map<std::string, std::string>& params)   {
    using namespace hcal::uhtr;

    
    *o << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
    *o << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
    
    *o << "<head><title> uHTR Lumi Info " << card->name() << "</title></head>\n";
    *o << "<body bgcolor='#6699ff'>\n";
    *o << "<h1 style='font-family: sans-serif'>uHTRLumi " << card->name() << " Status</h1>\n";    

    // here's where we display the information...
    try {
      // This is the master overall table which has two panes
      // get firmware information
      uint8_t flavorF, majorF, minorF, patchF;
      uint8_t flavorB, majorB, minorB, patchB;
      card->firmwareRevision(true,flavorF, majorF, minorF, patchF);
      card->firmwareRevision(false,flavorB, majorB, minorB, patchB);
      *o << "<table bgcolor='#ffffff' width=50% > <p align='right'>";
      *o <<" <tr><td>Front FPGA firmware revision:</td><td> " ;
      *o << std::hex << (int)majorF << "." << (int)minorF << "." << (int)patchF << std::dec <<"</td>"<< std::endl;
      *o << "<tr><td>Back FPGA firmware revision:</td><td> " ;
      *o << std::hex << (int)majorB << "." << (int)minorB << "." << (int)patchB << std::dec <<"</td>"<< std::endl;
      
      //      card->link_setup_query( align_v, isAutoOrb, clk_src ) ;
     
      int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
      std::vector<bool> mask_lhc, mask_cms;
      card->lumi_read_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );

      *o << "<tr><td>Number LHC Orbit </td><td>" << n_lhc_orb     << "</td></tr>\n";
      *o << "<tr><td>LHC  Threshold : </td><td>"<< lhc_threshold  << "</td></tr>\n";
      *o << "<tr><td>Number CMS Orbit </td><td>"<<  n_cms_orb     << "</td></tr>\n"; 
      *o << "<tr><td>CMS1 Threshold : </td><td>"<< cms1_threshold << "</td></tr>\n";
      *o << "<tr><td>CMS2 Threshold : </td><td>"<< cms2_threshold << "</td></tr>\n";

      float lbr, tbr;
      uint32_t link_state, errors;
      card->lumi_link_status(link_state, lbr, tbr, errors) ;
      std::string status_str = ( link_state == 0x3f ) ? "Happy" : "Sad"  ;
      *o << "<tr><td>Lumi Link Status : </td><td>"<< status_str << "</td></tr>\n";
      *o << "<tr><td>Lumi Link ERROR count : </td><td>"<< errors << "</td></tr>\n";
      *o << "<tr><td>LUT </td><td><a href=\"" << makeLinkForCardAction(card,"lut") << "\"> Check</a></tr>\n";

      *o << "</table>\n";
      *o << " \n";

      std::map<std::string, bool > aM ;
      std::map<std::string, bool > oM ;
      aM.clear();
      oM.clear();
      card->lumi_histo_status( aM, oM ) ;
      *o << "<table bgcolor='#ffffff' width=100%><p align='right'>";
      *o << "<tr><td> Histogram Name : </td>" ;
      for ( int i=0; i<6; i++) {
          *o << "<td>" << uHTR::LUMI_HISTONAMES[i]  <<" </td>" ;
      }
      *o << "</tr> \n";
      *o << "<tr><td> Overflow ? (0) No, (1)Yes</td>" ;
      for ( int i=0; i<6; i++) {
          *o << "<td>" << oM[ uHTR::LUMI_HISTONAMES[i] ]  <<" </td>" ;
      }
      *o << "</tr> \n";

      *o << "</table>\n";
      *o << " \n";


      *o << "<table bgcolor='#ffffff' width=100%><p align='right'>";
 
      *o << "<tr><td>  Fiber </td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          if ( ifiber < 9 ) *o <<"<td>0"<< ifiber+1 <<"</td> "  ;
          else              *o <<"<td>"<< ifiber+1 <<"</td> "  ;
      }
      *o << "</p> </tr>\n";


      *o << "<tr><td> Channel 0 LHC mask: </td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch0 =  (mask_lhc[ifiber*3])?(1):(0) ;
          *o <<"<td>"<< ch0 <<"</td> " ;
      }
      *o << " </tr>\n";

      *o << "<tr><td> Channel 1 LHC mask: </td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch1 =  (mask_lhc[ifiber*3 + 1])?(1):(0) ;
          *o <<"<td>"<< ch1 <<"</td> " ;
      }
      *o << " </tr>\n";

      *o << "<tr><td> Channel 2 LHC mask :</td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch2 =  (mask_lhc[ifiber*3 + 2])?(1):(0) ;
          *o <<"<td>"<< ch2 <<"</td> " ;
      }
      *o << " </tr>\n";
      *o << "<tr><td> Channel 0 CMS mask: </td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch0 =  (mask_cms[ifiber*3])?(1):(0) ;
          *o <<"<td>"<< ch0 <<"</td> " ;
      }
      *o << " </tr>\n";

      *o << "<tr><td> Channel 1 CMS mask: </td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch1 =  (mask_cms[ifiber*3 + 1])?(1):(0) ;
          *o <<"<td>"<< ch1 <<"</td> " ;
      }
      *o << " </tr>\n";

      *o << "<tr><td> Channel 2 CMS mask :</td> " ;
      for (int ifiber=0; ifiber<24; ifiber++) { 
          int ch2 =  (mask_cms[ifiber*3 + 2])?(1):(0) ;
          *o <<"<td>"<< ch2 <<"</td> " ;
      }
      *o << " </tr>\n";

      *o << "</table>\n";

      //*o << "<tr><td>Clock status: <td><a href=\"" << makeLinkForCardAction(card,"clock") << "\">Check</a></tr>\n";
      
    }
    catch (std::exception &e) {
      std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
      LOG4CPLUS_ERROR(getApplicationLogger(), message);
      *o << "<BR><B>" << message.c_str() << "</B>";
      return;
    }
    
    if ( item == "lut" ) {
       //*o << "<head><title> LUT Info " << card->name() << "</title></head>\n";
       //*o << "<body bgcolor='#6699ff'>\n";
       try {
           //card->lumi_read_lut(ifiber,ichan,lut);
           std::vector<uint16_t> lut;
	   card->lumi_read_lut( 1, 1,lut);
           *o <<" <br>\n " ; 
           *o <<" A LUT sample from fiber 1 channel 1 : \n " ;
           *o <<" <table width=100%><p align='right'> " ;
  	   for (size_t i=0; i< (lut.size()/8) ; i++) {
               int j = i*8 ;
               *o <<::toolbox::toString("<tr><td>%03d <td>%03d <td>%03d <td>%03d <td>%03d <td>%03d <td>%03d <td>%03d </tr>\n" 
                                            ,lut[j], lut[j+1], lut[j+2], lut[j+3], lut[j+4], lut[j+5], lut[j+6], lut[j+7]) ;
              
           }  
           *o <<"</p></table> " ;
       }    
       catch (std::exception &e) {
             std::string message = ::toolbox::toString("Could not read from card %s: %s", card->name().c_str(), e.what());
	     LOG4CPLUS_ERROR(getApplicationLogger(), message);
	     *o << "<BR><B>" << message.c_str() << "</B>";
	     return;
       }
       //*o <<" </body> \n " ;
    }
       
  } 
  
}

