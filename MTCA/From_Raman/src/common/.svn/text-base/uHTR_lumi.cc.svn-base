#include "hcal/uhtr/uHTR.hh"
#include "uhal/HwInterface.hpp"
#include <unistd.h>
#include "uhal/ConnectionManager.hpp"
#include "uhal/ValMem.hpp"
#include "uhal/uhal.hpp"
#include "toolbox/string.h"

namespace hcal {
  namespace uhtr {
  
    using namespace toolbox ;

    
    void uHTR::lumi_link_reset() {
      u_front.getNode("LINKS.XTRA0_DIFFCTRL").write( 0x7);
      u_front.getNode("LINKS.XTRA0_RESET").write(1);
      u_front.getNode("LINKS.XTRA0_RESET").write(0);

      if (isBHM()) {
	u_back.getNode("BHM.LINK_RESET").write(1);
	u_back.getNode("BHM.DECODER_RESET").write(1);
      } else {
	u_back.getNode("LUMI.LINK_RESET").write(1);
	u_back.getNode("LUMI.DECODER_RESET").write(1);
      }

      dispatch();
    }

    void uHTR::lumi_setup( int n_lhc_orb , int n_cms_orb, int lhc_threshold, int cms1_threshold, int cms2_threshold, const std::vector<bool>& mask_lhc, const std::vector<bool>& mask_cms ) {

      if (mask_lhc.size()!=(unsigned int) NFIBERS*n_channels_per_fiber_) XCEPT_RAISE(hcal::exception::Exception,"mask_lhc incorrect length");
      if (mask_cms.size()!=(unsigned int) NFIBERS*n_channels_per_fiber_) XCEPT_RAISE(hcal::exception::Exception,"mask_cmt incorrect length");
     
      u_back.getNode( "LUMI").getNode("LHC_N_ORBITS").write( n_lhc_orb );
      u_back.getNode( "LUMI").getNode("CMS_N_ORBITS").write( n_cms_orb );
      u_front.getNode("LUMI_LHC_THRESHOLD").write(lhc_threshold);
      u_front.getNode("LUMI_CMS1_THRESHOLD").write(cms1_threshold);
      u_front.getNode("LUMI_CMS2_THRESHOLD").write(cms2_threshold);

      // convert array of bits into three 32-bit words
      std::vector<uint32_t> buffer_lhc(24,0),buffer_cms(24,0);
      for (int ifiber=0; ifiber<NFIBERS; ifiber++)
	for (int ic=0; ic<n_channels_per_fiber_; ic++) {
	  size_t lin_index=(ifiber*n_channels_per_fiber_)+ic;
	  if (lin_index<mask_lhc.size() && mask_lhc[lin_index]) 
	    buffer_lhc[ifiber]|=1<<(ic);
	  if (lin_index<mask_cms.size() && mask_cms[lin_index]) 
	    buffer_cms[ifiber]|=1<<(ic);
	}
      u_front.getNode("LUMI_LHC_CHANNEL_MASK").writeBlock(buffer_lhc);
      u_front.getNode("LUMI_CMS_CHANNEL_MASK").writeBlock(buffer_cms);

      n_lhc_orb = n_LHC_Orbit ; 
      n_cms_orb = n_CMS_Orbit ; 
      dispatch();
    }

    void uHTR::lumi_read_setup( int& n_lhc_orb , int& n_cms_orb, int& lhc_threshold, int& cms1_threshold, int& cms2_threshold, std::vector<bool>& mask_lhc, std::vector<bool>& mask_cms ) {
      uhal::ValWord<uint32_t> vw_lhc_orb=u_back.getNode("LUMI.LHC_N_ORBITS").read();
      uhal::ValWord<uint32_t> vw_cms_orb=u_back.getNode("LUMI.CMS_N_ORBITS").read();
      uhal::ValWord<uint32_t> vw_lhc_threshold=u_front.getNode("LUMI_LHC_THRESHOLD").read();
      uhal::ValWord<uint32_t> vw_cms1_threshold=u_front.getNode("LUMI_CMS1_THRESHOLD").read();
      uhal::ValWord<uint32_t> vw_cms2_threshold=u_front.getNode("LUMI_CMS2_THRESHOLD").read();

      uhal::ValVector<uint32_t> vw_mask_lhc=u_front.getNode("LUMI_LHC_CHANNEL_MASK").readBlock(24);
      uhal::ValVector<uint32_t> vw_mask_cms=u_front.getNode("LUMI_CMS_CHANNEL_MASK").readBlock(24);
      dispatch();

      n_lhc_orb=vw_lhc_orb.value();
      n_cms_orb=vw_cms_orb.value();
      lhc_threshold=vw_lhc_threshold.value();
      cms1_threshold=vw_cms1_threshold.value();
      cms2_threshold=vw_cms2_threshold.value();

      // convert three 32-bit words into array of bits
      mask_lhc.clear(); mask_cms.clear();
      for (int ifiber=0; ifiber<NFIBERS; ifiber++)
	for (int ic=0; ic<n_channels_per_fiber_; ic++) {
	  mask_lhc.push_back(vw_mask_lhc[ifiber] & (1<<(ic)));
	  mask_cms.push_back(vw_mask_cms[ifiber] & (1<<(ic)));
	}
    }

    void uHTR::lumi_program_lut(int ifiber, int ichan, const std::vector<uint16_t>& image) {
      char buffer[100];
      snprintf(buffer,100,"FIBER_%d_CHAN_%d",ifiber,ichan);
      uhal::Node& lut=u_front.getNode("LUMI_LUTS").getNode(buffer);
      std::vector<uint32_t> data;
      for (size_t i=0; i<128 && i<image.size(); i++) 
	data.push_back(image[i]);

      try {
          lut.write( 1 ) ;
          dispatch();
          lut.writeBlock(data);
          dispatch();
      } catch (xcept::Exception& e) {
          printf( "Exception!  %s\n",e.message().c_str() );
      }

      // Verifying what has been writtien
      uhal::ValVector<uint32_t> observe =lut.readBlock(128);
      dispatch();
      for (size_t i=0; i<128 && i<data.size(); i++) {
          if ( observe[i] != image[i] ) {
             printf(" LUT Mismatch !!! \n") ;
             break ;
          }
      }

    }

    /// Read LUT for LUMI (ifiber = 0..23, ichan = 0..2)
    void uHTR::lumi_read_lut(int ifiber, int ichan, std::vector<uint16_t>& image) {
      char buffer[100];
      snprintf(buffer,100,"FIBER_%d_CHAN_%d",ifiber,ichan);
      uhal::Node& lut=u_front.getNode("LUMI_LUTS").getNode(buffer);

      uhal::ValVector<uint32_t> data=lut.readBlock(128);
      dispatch();

      image.clear();
      for (size_t i=0; i<128 && i<data.size(); i++)
	image.push_back(data[i]);
    }

    const char* uHTR::LUMI_HISTONAMES[] = { "LHC", "LHC_VALID", "CMS1", "CMS2", "CMS_VALID", "CMS_ET", 0 } ;

    void uHTR::lumi_histo_status( std::map<std::string, bool>& hAvailable, std::map<std::string, bool>& hOverflow ) {

         std::vector< uhal::ValWord<uint32_t> > aV ; 
         std::vector< uhal::ValWord<uint32_t> > oV ; 
         for (int i = 0 ; LUMI_HISTONAMES[i] !=0 ; i++ ) { 
	     uhal::Node& lumiNode  = u_back.getNode("LUMI");
	     uhal::Node& histNode = lumiNode.getNode( LUMI_HISTONAMES[i] );
	     uhal::Node& availNode  = histNode.getNode( "AVAILABLE" );
	     uhal::Node& overfNode  = histNode.getNode( "OVERFLOW" );
             aV.push_back( availNode.read() ) ;
             oV.push_back( overfNode.read() ) ;
         }
         dispatch();

         for (int i = 0 ; LUMI_HISTONAMES[i] !=0 ; i++ ) { 
             hAvailable[ LUMI_HISTONAMES[i] ] = ( aV[i].value() != 0 );        
             hOverflow[ LUMI_HISTONAMES[i] ] = ( oV[i].value() != 0 );        
         }
    }

    int uHTR::lumi_histo_overflow( int monitoringhist ){
      uhal::Node& lumiNode  = u_back.getNode("LUMI");
      uhal::Node& histNode = lumiNode.getNode( LUMI_HISTONAMES[monitoringhist] );
      uhal::Node& overfNode  = histNode.getNode( "OVERFLOW" );
      uhal::ValWord<uint32_t> overflowvalue=overfNode.read();
      int returnvalue=overflowvalue.value();;
      return returnvalue;
    }


    std::vector<uint32_t> uHTR::lumi_link_spy() {
      std::string prefix="LUMI";
      if (isBHM()) prefix="BHM";

      u_back.getNode(prefix+".LINK_SPY_MODE").write(2); // halt the fifo
      std::string spyAddr=prefix+".LINK_SPY";
      uhal::ValVector<uint32_t> datai=u_back.getNode(spyAddr).readBlock(64);
      u_back.getNode(prefix+".LINK_SPY_MODE").write(0); // release the fifo
      dispatch();
      std::vector<uint32_t> data;
      data.reserve(64);
      for (size_t i=0; i<datai.size(); i++)
	data.push_back(datai[datai.size()-1-i]); // invert the data order here
      return data;
    }


    void uHTR::lumi_link_status(uint32_t& status, float& lb0, float& lt0, uint32_t& errors) {
      std::string prefix="LUMI";
      if (isBHM()) prefix="BHM";

         uhal::ValWord<uint32_t> aSt = u_back.getNode( prefix+".LINK_ALIGN_STATE" ).read() ; 
         uhal::ValWord<uint32_t> lbc = u_back.getNode( prefix+".LUMI_BC0_RATE" ).read() ; 
         uhal::ValWord<uint32_t> tbc = u_back.getNode( prefix+".TRIG_BC0_RATE" ).read() ; 
         uhal::ValWord<uint32_t> le = u_back.getNode( prefix+".LINK_ERRORS" ).read() ; 
         dispatch() ;
         status=aSt.value() ;  
	 lb0=lbc.value()/1e6;
	 lt0=tbc.value()/1e6;
	 errors=le.value();
    }
    
    void uHTR::lumi_read_histogram( const std::string& hName, LumiHistogram& iLumi ) {

         int nBX = 3564 ; 
	 uhal::Node& lumiNode  = u_back.getNode("LUMI");
	 uhal::Node& histoNode = lumiNode.getNode(hName);
	 uhal::ValWord<uint32_t> h_av = histoNode.getNode( "AVAILABLE" ).read();
         dispatch();

         if ( h_av.value() != 0 ) {
            uhal::ValVector<uint32_t> lumiBlock = histoNode.getNode( "HIST").readBlock( nBX ) ;
	    uhal::ValWord<uint32_t> orb_1st     = histoNode.getNode( "FIRST_ORBIT" ).read() ;
	    uhal::ValWord<uint32_t> orb_last    = histoNode.getNode( "LAST_ORBIT" ).read() ;
	    uhal::ValWord<uint32_t> h_overflow  = histoNode.getNode( "OVERFLOW" ).read() ;

	    uhal::ValWord<uint32_t> h_nibble  = histoNode.getNode( "LUMI_NIBBLE" ).read() ;
	    uhal::ValWord<uint32_t> h_section  = histoNode.getNode( "LUMI_SECTION" ).read() ;
	    uhal::ValWord<uint32_t> h_run  = histoNode.getNode( "CMS_RUN" ).read() ;
	    uhal::ValWord<uint32_t> h_fill  = histoNode.getNode( "LHC_FILL" ).read() ;

	    dispatch() ;
	    lumiNode.getNode( hName+"_ADVANCE_POINTER"  ).write(1) ;
	    dispatch() ;

            iLumi.h.clear() ;
            iLumi.h.reserve( nBX ) ;
	    for ( int k=0; k< nBX ; k++) {
	        iLumi.h.push_back( lumiBlock[k] ) ;
	    }
	    iLumi.type = hName ;
	    iLumi.orb_init = orb_1st.value() ;
	    iLumi.orb_last = orb_last.value() ;
	    iLumi.overflow = h_overflow.value() ;
	    iLumi.lumi_nibble = h_nibble.value();
	    iLumi.lumi_section = h_section.value();
	    iLumi.cms_run = h_run.value();
	    iLumi.lhc_fill = h_fill.value();

         }

    }

    void uHTR::bhm_setup(int n_orb, const std::vector<uint8_t>& thresholds, const std::vector<uint8_t>& tdcmap) {
      if (!isBHM()) return;

      u_back.getNode("BHM.N_ORBITS").write(n_orb);
      std::vector<uint32_t> tool1, tool2;

      for (int i=0; i<int(thresholds.size()) && i<BHM_N_THRESHOLDS; i++) {
	if (i<12) tool1.push_back(thresholds[i]);
	else tool2.push_back(thresholds[i]);
      }
      u_front.getNode("BHM.THRESHOLDS_11_0").writeBlock(tool1);
      u_front.getNode("BHM.THRESHOLDS_23_12").writeBlock(tool2);

      tool1.clear();
      for (int i=0; i<int(tdcmap.size()) && i<BHM_N_TDCMAP; i++) 
	tool1.push_back(tdcmap[i]);

      u_front.getNode("BHM.TDC_MAP").writeBlock(tool1);
      
      dispatch();
    }

    void uHTR::bhm_read_setup(int& n_orb, std::vector<uint8_t>& thresholds, std::vector<uint8_t>& tdcmap) {
      if (!isBHM()) return;
      thresholds.clear();
      tdcmap.clear();
      uhal::ValWord<uint32_t> o=u_back.getNode("BHM.N_ORBITS").read();
      uhal::ValVector<uint32_t> t1=u_front.getNode("BHM.THRESHOLDS_11_0").readBlock(12);
      uhal::ValVector<uint32_t> t2=u_front.getNode("BHM.THRESHOLDS_23_12").readBlock(12);
      uhal::ValVector<uint32_t> tm=u_front.getNode("BHM.TDC_MAP").readBlock(64);
      dispatch();
      n_orb=int(o.value());
      for (int i=0; i<12; i++) thresholds.push_back(uint8_t(t1[i]));
      for (int i=0; i<12; i++) thresholds.push_back(uint8_t(t2[i]));
      for (int i=0; i<BHM_N_TDCMAP; i++) tdcmap.push_back(uint8_t(tm[i]));
    }

    void uHTR::bhm_histo_status( std::vector<bool>& hAvailable, std::vector<bool>& hOverflow) {
      if (!isBHM()) return;
      uhal::ValVector<uint32_t> a=u_back.getNode("BHM.DATA_AVAILABLE").readBlock(3);
      uhal::ValVector<uint32_t> l=u_back.getNode("BHM.DATA_LOST").readBlock(3);

      dispatch();
      hAvailable.clear();
      hOverflow.clear();

      // we hide the bunch-splitting here, so we show only 24 answers
      for (int i=0; i<BHM_N_THRESHOLDS; i++) {
	int ibin=(i*4)/32;
	uint32_t mask=(1<<(((i*4)%32)));
	hAvailable.push_back((a[ibin]&mask)!=0);
	hOverflow.push_back((l[ibin]&mask)!=0);
      }

    }

    void uHTR::bhm_read_histograms(std::vector<LumiHistogram>& histos) {
      if (!isBHM()) return;

      static const int N_BUNCH_ORBIT=3564;

      histos=std::vector<LumiHistogram>(BHM_N_THRESHOLDS);
      for (int i=0; i<BHM_N_THRESHOLDS; i++) {
	histos[i].index=i;
	histos[i].h=std::vector<uint32_t>(N_BUNCH_ORBIT*4,0);
      }

      static const int N_9BIT_HIST = BHM_N_THRESHOLDS * 4;
      static const int N_9_IN_32 = N_9BIT_HIST / 3;

      uint32_t baseAddress=u_back.getNode("BHM.HISTS").getAddress();
      for (int i32=0; i32<N_9_IN_32; i32++) {

	uhal::ValVector<uint32_t> payload=u_back.getClient().readBlock(baseAddress+0x1000*i32,N_BUNCH_ORBIT);
	uhal::ValVector<uint32_t> trailer=u_back.getClient().readBlock(baseAddress+0x1000*i32+0xFE0,32);
	dispatch(); // long...

	int ih_a, ih_b, ih_c, offset;
	ih_a=(i32/4*3);
	ih_b=(i32/4*3+1);
	ih_c=(i32/4*3+2);
	offset=(i32%4);
	
	// extract/unpack data
	for (int ibin=0; ibin<N_BUNCH_ORBIT; ibin++) {
	  histos[ih_a].h[ibin*4+offset]=payload[ibin]&0x1FF;             
	  histos[ih_b].h[ibin*4+offset]=(payload[ibin]>>10)&0x1FF;
	  histos[ih_c].h[ibin*4+offset]=(payload[ibin]>>20)&0x1FF;
	}

	// extract/unpack limits
	uint32_t os=trailer[3+16]&0xFF;
	os|=(trailer[4+16]&0xFF)<<8;
	os|=(trailer[5+16]&0xFF)<<16;
	os|=(trailer[6+16]&0xFF)<<24;
	uint32_t of=trailer[7+16]&0xFF;
	of|=(trailer[8+16]&0xFF)<<8;
	of|=(trailer[9+16]&0xFF)<<16;
	of|=(trailer[10+16]&0xFF)<<24;
	uint32_t ln=(trailer[0]&0xFF)|((trailer[1]&0xFF)<<8)|((trailer[2]&0xFF)<<16)|((trailer[3]&0xFF)<<24);
	uint32_t ls=(trailer[4]&0xFF)|((trailer[5]&0xFF)<<8)|((trailer[6]&0xFF)<<16)|((trailer[7]&0xFF)<<24);
	uint32_t cr=(trailer[8]&0xFF)|((trailer[9]&0xFF)<<8)|((trailer[10]&0xFF)<<16)|((trailer[11]&0xFF)<<24);
	uint32_t lf=(trailer[12]&0xFF)|((trailer[13]&0xFF)<<8)|((trailer[14]&0xFF)<<16)|((trailer[15]&0xFF)<<24);

	for (int ii=0; ii<3; ii++) {
	  histos[ih_a+ii].orb_init=os;
	  histos[ih_a+ii].orb_last=of;
	  histos[ih_a+ii].lumi_nibble=ln;
	  histos[ih_a+ii].lumi_section=ls;
	  histos[ih_a+ii].cms_run=cr;
	  histos[ih_a+ii].lhc_fill=lf;

	  histos[ih_a+ii].overflow=trailer[1+16];
	}

	
      }
      u_back.getNode("BHM.ADVANCE_POINTER").write(0xFFFFFFFFu);
    }

    void uHTR::bhm_ampl_histo_control(bool run, bool clear) {
      u_front.getNode("BHM.CLEAR").write((clear)?(1):(0));
      u_front.getNode("BHM.RUN").write((run)?(1):(0));
      dispatch();
    }

    std::vector<uint32_t> uHTR::bhm_ampl_histo_read(int ichannel,int tbin) {
      std::string item=(ichannel<12)?("HISTOGRAMS_11_0"):("HISTOGRAMS_23_12");
      uint32_t baseAddress=u_back.getNode(item).getAddress();

      std::vector<uint32_t> hd;

      uhal::ValVector<uint32_t> hr=u_back.getClient().readBlock(baseAddress+0x100*((ichannel%12)*4+tbin),256); 
      dispatch();
      for (int i=0; i<256; i++)
	hd.push_back(hr[i]);

      return hd;      
    }


  }
}
