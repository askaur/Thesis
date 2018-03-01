#include "hcal/uhtr/uHTR.hh"
#include "uhal/HwInterface.hpp"
#include <unistd.h>
#include "uhal/ValMem.hpp"
#include "uhal/uhal.hpp"
#include "toolbox/string.h"

namespace hcal {
  namespace uhtr {
  
    using namespace toolbox ;


    void uHTR::trigger_query_lut_type(int group, int& n_addr_bits, int& n_data_bits, int& n_luts, uint32_t& base_address, uint32_t& stride, bool& last_group) {
      std::string nname=::toolbox::toString("TRIGGER.LUT_GROUP_%d",group);
      uhal::Node& lut_group = u_front.getNode(nname);

      uhal::ValWord<uint32_t> na=lut_group.getNode("N_ADDR_BITS").read();
      uhal::ValWord<uint32_t> nd=lut_group.getNode("N_DATA_BITS").read();
      uhal::ValWord<uint32_t> nl=lut_group.getNode("N_LUTS").read();
      uhal::ValWord<uint32_t> llg=lut_group.getNode("LAST_LUT_GROUP").read();
      uhal::ValWord<uint32_t> ba=lut_group.getNode("BASE_ADDRESS").read();
      uhal::ValWord<uint32_t> sr=lut_group.getNode("STRIDE").read();

      dispatch();

      n_addr_bits=na.value();
      n_data_bits=nd.value();
      n_luts=nl.value();
      base_address=ba.value();
      stride=sr.value();
      last_group=(llg.value()!=0);

    }


    void uHTR::trigger_program_lut( int group, int index, const std::vector<uint32_t>& image) {
     
      int n_addr_bits, n_data_bits, n_luts;
      uint32_t base_address,  stride;
      bool last_group;

      trigger_query_lut_type(group,n_addr_bits, n_data_bits, n_luts, base_address,  stride, last_group);

      if (index>=n_luts) {
	XCEPT_RAISE(hcal::exception::Exception,::toolbox::toString("LUT %d out of range for group %d (max is %d)",index,group,n_luts-1));
      }

      uint32_t lutAddress=base_address+stride*index;

      int tries=5;

      bool mismatch=false;

      do {

	u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(1);
	u_front.getClient().writeBlock( lutAddress, image);
	u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(0);
	dispatch();

	u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(1);
	uhal::ValVector<uint32_t> data=u_front.getClient().readBlock(lutAddress, 1<<n_addr_bits);
	u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(0);
	dispatch();

	mismatch=false;
	for (size_t i=0; i<data.size()  && i<image.size(); i++)
	  if (data[i]!=image[i]) mismatch=true;
	tries--;
      } while (tries>0 && mismatch);

      if (mismatch) {
	XCEPT_RAISE(hcal::exception::Exception,::toolbox::toString("Readback failure for LUT %d in group %d",index,group));
      }

    }

    void uHTR::trigger_read_lut( int group, int index, std::vector<uint32_t>& image) {
      int n_addr_bits, n_data_bits, n_luts;
      uint32_t base_address,  stride;
      bool last_group;

      trigger_query_lut_type(group,n_addr_bits, n_data_bits, n_luts, base_address,  stride, last_group);

      if (index>=n_luts) {
	XCEPT_RAISE(hcal::exception::Exception,::toolbox::toString("LUT %d out of range for group %d (max is %d)",index,group,n_luts-1));
      }

      uint32_t lutAddress=base_address+stride*index;

      u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(1);
      uhal::ValVector<uint32_t> data=u_front.getClient().readBlock(lutAddress, 1<<n_addr_bits);
      u_front.getNode("TRIGGER.LUT_ACCESS_ENABLE").write(0);
      dispatch();

      image.clear();
      for (size_t i=0; i<data.size(); i++)
	image.push_back(data[i]);
    }


    
    void uHTR::trigger_spy_execute(uint32_t active_mask, uint32_t orbit_delay) {
      u_front.getNode("TRIGGER_SPY.ENABLE_MASK").write(0);
      u_front.getNode("TRIGGER_SPY.ORBIT_DELAY").write(orbit_delay);
      u_front.getNode("TRIGGER_SPY.ENABLE_MASK").write(active_mask);
      dispatch();
    }
    
    void uHTR::trigger_spy_read(int trigger_link_id,std::vector<uint32_t>& buffer) {
      static const int N_LINKS=14;
      static const char* names[]={"PPOD_0","PPOD_1","PPOD_2","PPOD_3","PPOD_4","PPOD_5","PPOD_6","PPOD_7","PPOD_8",
				"PPOD_9","PPOD_10","PPOD_11","SFP_0","SFP_1",0};

      buffer.clear();
      if (trigger_link_id>=N_LINKS || trigger_link_id<0) return;
      uhal::ValVector<uint32_t> data;
      data=u_front.getNode("TRIGGER_SPY").getNode(names[trigger_link_id]).readBlock(2048);
      dispatch();
      buffer.reserve(data.size());
      for (size_t i=0; i<data.size(); i++)
	buffer.push_back(data[i]);
    }

    void uHTR::trigger_self_set_thresholds(const std::vector<uint32_t>& thresholds) {
      u_front.getNode("TRIGGER.SELF_TRIGGER_THRESHOLDS").writeBlock(thresholds);
      dispatch();
    }

    std::vector<uint32_t> uHTR::trigger_self_get_thresholds() {
      std::vector<uint32_t> retval;

      uhal::ValVector<uint32_t> data=u_front.getNode("TRIGGER.SELF_TRIGGER_THRESHOLDS").readBlock(28);
      dispatch();

      for (size_t i=0; i<data.size(); i++) retval.push_back(data[i]);
      return retval;
    }

    std::vector<uint32_t> uHTR::trigger_self_read_histogram(int bit) {
      std::vector<uint32_t> retval;

      std::string stub=::toolbox::toString("BIT%d",bit);
      
      uhal::Node& section=u_back.getNode("SELF_TRIGGER").getNode(stub);
      
      bool overf, avail;
      do {
	uhal::ValWord<uint32_t> ovrf_vw=section.getNode("OVERFLOW").read();
	uhal::ValWord<uint32_t> avail_vw=section.getNode("AVAILABLE").read();
	dispatch();
	overf=(ovrf_vw.value()!=0);
	avail=(avail_vw.value()!=0);
	if (overf) {
	  u_back.getNode("SELF_TRIGGER").getNode(stub+"_ADVANCE_POINTER").write(1);
	  u_back.getNode("SELF_TRIGGER").getNode(stub+"_ADVANCE_POINTER").write(1);
	  u_back.getNode("SELF_TRIGGER").getNode(stub+"_RESET_LOSS").write(1);
	  dispatch();
	}
	if (!avail) ::sleep(1);
      } while (overf || !avail);

      uhal::ValVector<uint32_t> vw=section.getNode("HIST").readBlock(3564);
      u_back.getNode("SELF_TRIGGER").getNode(stub+"_ADVANCE_POINTER").write(1);
      dispatch();
      
      retval.reserve(3564);
      for (size_t i=0; i<3564; i++)
	retval.push_back(vw[i]);
      return retval;
    }
 
    // *************************************** 
    // * SFP Spy - check what is received from SFP 
    // * Check the Trigger Primitive sending from SFP   
    // * TxFPGA ->  1 : Front , 0 : Back
    // *************************************** 
    void uHTR::sfp_setup( int link, int TxFPGA )
    {
        //u_back.getNode("SFP_LINKS.CTRL_WORD_0").write(0x1B00003);
	//u_back.getNode("SFP_LINKS.CTRL_WORD_1").write(0x1B00003);
	//dispatch();
        if ( link ==  0 || link == 2 ) {
           uhal::ValWord<uint32_t> ctrl0 = u_back.getNode("SFP_LINKS.CTRL_WORD_0").read() ;
	   dispatch();
           uint32_t newval = ctrl0.value() ;
           // Reset TX and RX
           newval |= 1 ;
           newval |= 1 << 1 ;

	   // Set up FPGA for Tx
	   if ( TxFPGA == 1 ) newval |= 1 << 24 ;
	   if ( TxFPGA == 0 ) newval &= ~(1 << 24 );
	   
	   u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( newval );
	   dispatch();
	   usleep(10) ;
	   newval &= ~(1 << 0);
	   newval &= ~(1 << 1);
	   u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( newval );
	   dispatch();       
        }
        if ( link ==  1 || link == 2 ) {
           uhal::ValWord<uint32_t> ctrl1 = u_back.getNode("SFP_LINKS.CTRL_WORD_1").read() ;
	   dispatch();
           uint32_t newval = ctrl1.value() ;
           // Reset TX and RX
           newval |= 1  ;
           newval |= 1 << 1 ;

           if ( TxFPGA == 1 ) newval |= 1 << 24 ;
           if ( TxFPGA == 0 ) newval &= ~(1 << 24 );
	   u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( newval );
	   dispatch();
	   //          printf("**** <2> Setup Reset bit and TxFPGA **** \n") ;
           usleep(10) ;
           newval &= ~(1 << 0);
           newval &= ~(1 << 1);
	   u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( newval );
	   dispatch();
        }
        
        //printf("**** <3> Reset bit cleared  **** \n") ;
    }

    void uHTR::sfp_resets( bool link, bool ec )
    {
      uhal::ValWord<uint32_t> ctrl0 = u_back.getNode("SFP_LINKS.CTRL_WORD_0").read() ;
      uhal::ValWord<uint32_t> ctrl1 = u_back.getNode("SFP_LINKS.CTRL_WORD_1").read() ;
      dispatch();

      uint32_t val0 = ctrl0.value() ;
      uint32_t val1 = ctrl1.value() ;

      if (link) {
	// Reset TX and RX
	uint32_t newval0 = val0|1 ;
	newval0 |= 1 << 1 ;
	
	// Reset TX and RX
	uint32_t newval1 = val1|1 ;
	newval1 |= 1 << 1 ;
	
	u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( newval0 );
	u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( newval1 );
	dispatch();
	usleep(10) ;
	u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( val0 );
	u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( val1 );
	dispatch();	
      }

      if (ec) {
	// Reset TX and RX
	uint32_t newval0 = val0|(1<<23) ;
	
	// Reset TX and RX
	uint32_t newval1 = val1|(1<<23) ;
	
	u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( newval0 );
	u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( newval1 );
	dispatch();
	usleep(10) ;
	u_back.getNode("SFP_LINKS.CTRL_WORD_0").write( val0 );
	u_back.getNode("SFP_LINKS.CTRL_WORD_1").write( val1 );
	dispatch();
      }
        //printf("**** <3> Reset bit cleared  **** \n") ;
    }

    void uHTR::sfp_query( uint32_t& ctl0, uint32_t& st0, uint32_t& err0, float& clk0,
			  uint32_t& ctl1, uint32_t& st1, uint32_t& err1, float& clk1
			  ) 
    {
        uhal::ValWord<uint32_t>  ctrl0 = u_back.getNode("SFP_LINKS.CTRL_WORD_0").read() ;
        uhal::ValWord<uint32_t>  st0w   = u_back.getNode("SFP_LINKS.STATUS_0").read() ;
        uhal::ValWord<uint32_t>  err0w  = u_back.getNode("SFP_LINKS.ERROR_COUNTER_0").read() ;
        uhal::ValWord<uint32_t>  clk0w  = u_back.getNode("SFP_LINKS.RX_CLK_RATE0").read() ;
        uhal::ValWord<uint32_t>  ctrl1 = u_back.getNode("SFP_LINKS.CTRL_WORD_1").read() ;
        uhal::ValWord<uint32_t>  st1w   = u_back.getNode("SFP_LINKS.STATUS_1").read() ;
        uhal::ValWord<uint32_t>  err1w  = u_back.getNode("SFP_LINKS.ERROR_COUNTER_1").read() ;
        uhal::ValWord<uint32_t>  clk1w  = u_back.getNode("SFP_LINKS.RX_CLK_RATE1").read() ;
        
	dispatch();

        ctl0  = ctrl0.value() ;
        st0   = st0w.value() ;
	err0  = err0w.value();
        clk0  = clk0w.value()/1e4 ;

        ctl1  = ctrl1.value() ;
        st1   = st1w.value() ;
	err1  = err1w.value();
        clk1  = clk1w.value()/1e4 ;
    }

    void uHTR::sfp_spySetup(bool enable_capture_sw, bool enable_capture_dtc, bool orbit_no_wait, int pre, int delay, int adelay)
    {
	u_back.getNode("SFP_SPY_ENABLE_CAPTURE_SW").write(enable_capture_sw?(1):(0));
	u_back.getNode("SFP_SPY_ENABLE_CAPTURE_DTC").write(enable_capture_dtc?(1):(0));
	u_back.getNode("SFP_SPY_ORBIT_NO_WAIT").write(orbit_no_wait?(1):(0));
        u_back.getNode("SFP_SPY_PULSE_PRE").write(pre);
	u_back.getNode("SFP_SPY_PULSE_DELAY").write(delay);
	u_back.getNode("SFP_SPY_ALIGN_DELAY").write(adelay);
        u_back.getNode("SFP_SPY_RAM_LINKMASK").write( 0x3 );
	dispatch();
    }

    void uHTR::sfp_spySetupQuery()
    {
	uhal::ValWord<uint32_t> val0 = u_back.getNode("SFP_SPY_ENABLE_CAPTURE_SW").read();
	uhal::ValWord<uint32_t> val1 = u_back.getNode("SFP_SPY_ENABLE_CAPTURE_DTC").read();
	uhal::ValWord<uint32_t> val2 = u_back.getNode("SFP_SPY_ORBIT_NO_WAIT").read();
        uhal::ValWord<uint32_t> val3 = u_back.getNode("SFP_SPY_PULSE_PRE").read();
	uhal::ValWord<uint32_t> val4 = u_back.getNode("SFP_SPY_PULSE_DELAY").read();
	uhal::ValWord<uint32_t> val5 = u_back.getNode("SFP_SPY_ALIGN_DELAY").read();
        uhal::ValWord<uint32_t> val6 = u_back.getNode("SFP_SPY_RAM_LINKMASK").read();
	dispatch();
	/*
	  ABSOLUTELY NO PRINTFS ALLOWED in uHTR class!!!!!!!
        printf("\n") ;
        printf(" EN_CAPTURE_SW ? %2x EN_CAPTURE_DTC ? %2x \n", val0.value() , val1.value() ) ;
        printf(" Orbit No Wait ? %2x Pulse PreSample? %2x \n", val2.value() , val3.value() ) ;
        printf(" Pulse Delay   ? %2x Align Delay    ? %2x \n", val4.value() , val5.value() ) ;
        printf(" RAM LINK Mask ? %4x                      \n", val6.value()  ) ;
        printf(" =========================================\n" ) ;
	*/
    }

    void uHTR::sfp_spyExecute() 
    {
        u_back.getNode("SFP_SPY_START").write(0);
        dispatch();
        usleep(100) ;
        u_back.getNode("SFP_SPY_START").write(1);
        dispatch();
    }

    //
    std::vector<uint32_t> uHTR::sfp_spyRead(int id ) {

      std::vector<uint32_t> buffer;
      // first, we have to get the pointer to the relevant spy buffer
      uhal::ValVector<uint32_t> getPtr = u_back.getNode("SFP_SPY_RAM_PTRS").readBlock(2);
      uhal::ValWord<uint32_t> n_rams   = u_back.getNode("SFP_SPY_RAM_COUNT").read();
      uhal::ValWord<uint32_t> n_words  = u_back.getNode("SFP_SPY_RAM_WORDS").read();
      dispatch();
      uint32_t ramPtr=getPtr[ id ];

      /*
	ABSOLUTELY NO PRINTFS ALLOWED IN THIS CODE
      printf(" N SpyRam : %d , N Words/ SpyRam : %d \n", n_rams.value() , n_words.value() ) ;
      printf(" SpyRam0[%08X] ,  SpyRam1[%08X] \n", getPtr[0], getPtr[1] ) ;
      */

      // *** Block Red ***
      //int block_size = n_words.value() / n_rams.value() ; 
      //uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock(ramPtr, block_size );
      //dispatch();
      
      for (int j=0; j< 1024; j++) {
          uint32_t ramPtr_ = ramPtr + (uint32_t)j ;
          uhal::ValWord<uint32_t> aword = u_hw->getClient().read( ramPtr_ );
          dispatch();
          buffer.push_back( aword.value() ) ;
          //buffer.push_back( readBlock[j] );
      }
      return buffer ;
    }

    void uHTR::get_orm_info(uint32_t& orm_cmd_queued,  uint32_t& orm_start_count, uint32_t& orm_stop_count, uint32_t& orm_cmd_count){

      uhal::ValWord<uint32_t> t1=u_front.getNode("DTC.ORM_CMD_QUEUED").read();
      uhal::ValWord<uint32_t> t2=u_front.getNode("DTC.ORM_START_COUNT").read();
      uhal::ValWord<uint32_t> t3=u_front.getNode("DTC.ORM_STOP_COUNT").read();
      uhal::ValWord<uint32_t> t4=u_front.getNode("DTC.ORM_CMD_COUNT").read();
      dispatch();
      orm_cmd_queued=t1.value();
      orm_start_count=t2.value();
      orm_stop_count=t3.value();
      orm_cmd_count=t4.value();
      
    }
    void uHTR::orm_control(bool orm_start, bool orm_stop, bool orm_send, uint32_t orm_cmd) {

      if (orm_start) {
	u_front.getNode("DTC.ORM_START").write(1);
	u_front.getNode("DTC.ORM_START").write(0);
      }
      if (orm_stop) {
	u_front.getNode("DTC.ORM_STOP").write(1);
	u_front.getNode("DTC.ORM_STOP").write(0);
      }
      if (orm_send) {
	u_front.getNode("DTC.ORM_CMD").write(orm_cmd);
	u_front.getNode("DTC.ORM_SEND").write(1);
	u_front.getNode("DTC.ORM_SEND").write(0);
      }
      dispatch();
    }



  }
}
