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

    // *****************************************
    // *     DAQ Block  
    // *****************************************

    void uHTR::daq_path_setup(uint32_t moduleId, uint32_t bcOffset, uint32_t nsamples, uint32_t npre, uint32_t n_tp_samples, uint32_t ntp_pre, uint32_t pipeline) {
      u_back.getNode("DAQ_MODULEID").write(moduleId);
      u_back.getNode("DAQ_BC_OFFSET").write(bcOffset);
      u_back.getNode("DAQ_NSAMPLES").write(nsamples);
      u_back.getNode("DAQ_NPRESAMPLES").write(npre);
      u_back.getNode("DAQ_PIPELINELENGTH").write(pipeline);

      u_front.getNode("PIPELINE0.PRESAMPLES").write(npre);
      u_front.getNode("PIPELINE0.NSAMPLES").write(nsamples);
      u_front.getNode("PIPELINE0.PIPELINEDEPTH").write(pipeline);

      u_front.getNode("PIPELINE1.PRESAMPLES").write(npre);
      u_front.getNode("PIPELINE1.NSAMPLES").write(nsamples);
      u_front.getNode("PIPELINE1.PIPELINEDEPTH").write(pipeline);

      u_back.getNode("DAQ_N_TP_SAMPLES").write(n_tp_samples);
      u_back.getNode("DAQ_N_TP_PRESAMPLES").write(ntp_pre);

      u_front.getNode("PIPELINE_TP.PRESAMPLES").write(ntp_pre);
      u_front.getNode("PIPELINE_TP.NSAMPLES").write(n_tp_samples);

      /// TODO: calculate necessary offset here...
      u_front.getNode("PIPELINE_TP.PIPELINEDEPTH").write(pipeline);

      dispatch();
    }
    void uHTR::daq_path_setup_query(uint32_t& moduleId, uint32_t& bcOffset, uint32_t& nsamples, uint32_t& npre,  uint32_t& n_tp_samples, uint32_t& ntp_pre, uint32_t& pipeline, bool& enabled) {
      daq_query();

      moduleId=iDaq.moduleID;
      bcOffset=iDaq.bcOffset;
      nsamples=iDaq.nsamples;
      npre=iDaq.npre;
      pipeline=iDaq.pipeline;
      enabled=iDaq.formatter_enable;
      n_tp_samples=iDaq.n_tp_samples;
      ntp_pre=iDaq.n_tp_presamples;
    }

    void uHTR::daq_query() {
      uhal::ValWord<uint32_t> t1=u_back.getNode("DAQ_MODULEID").read();
      uhal::ValWord<uint32_t> t2=u_back.getNode("DAQ_BC_OFFSET").read();
      uhal::ValWord<uint32_t> t3=u_back.getNode("DAQ_ORN").read();
      uhal::ValWord<uint32_t> t4=u_back.getNode("DAQ_NSAMPLES").read();
      uhal::ValWord<uint32_t> t5=u_back.getNode("DAQ_NPRESAMPLES").read();
      uhal::ValWord<uint32_t> t6=u_back.getNode("DAQ_PIPELINELENGTH").read();
      uhal::ValWord<uint32_t> t7=u_back.getNode("DAQ_FORMATTER_ENABLE").read();
      uhal::ValWord<uint32_t> t8=u_back.getNode("DAQ_LASTEVN").read();
      uhal::ValWord<uint32_t> t9=u_back.getNode("DAQ_N_TP_SAMPLES").read();
      uhal::ValWord<uint32_t> t10=u_back.getNode("DAQ_N_TP_PRESAMPLES").read();

      uhal::ValWord<uint32_t> z1=u_back.getNode("DAQ_ZS_DISABLE").read();
      uhal::ValWord<uint32_t> z2=u_back.getNode("DAQ_ZS_MARKANDPASS").read();
      uhal::ValWord<uint32_t> z3=u_back.getNode("DAQ_ZS_SUMBYTWO").read();
      uhal::ValWord<uint32_t> z4=u_back.getNode("DAQ_ZS_MASK").read();
      uhal::ValVector<uint32_t> z5 = u_back.getNode("DAQ_ZS_THRESHOLDS").readBlock(128);
      uhal::ValWord<uint32_t> z6 = u_back.getNode("DAQ_TP_ZS_ENABLE").read();
      uhal::ValWord<uint32_t> z7 = u_back.getNode("DAQ_TP_ZS_SOI_ONLY").read();
      dispatch();

      iDaq.moduleID = t1.value();
      iDaq.bcOffset = t2.value();
      iDaq.orbit_Num= t3.value();
      iDaq.nsamples = t4.value();
      iDaq.npre     = t5.value();
      iDaq.pipeline = t6.value();
      iDaq.formatter_enable = (t7.value()!=0);
      iDaq.lastEvent= t8.value();
      iDaq.n_tp_samples=t9.value();
      iDaq.n_tp_presamples=t10.value();

      iDaq.zs_disable   = (z1.value()!=0) ;
      iDaq.zs_MarkNPass = (z2.value()!=0) ;
      iDaq.zs_sumbytwo  = (z3.value()!=0) ;
      iDaq.zsMask       =  z4.value();
      for (int i=0; i<128; i++) 
	iDaq.zsThreshold[i] = z5[i] ;       
      iDaq.zs_tp_enable=(z6.value()!=0);
      iDaq.zs_tp_soi_only=(z7.value()!=0);

    }

    void uHTR::daq_zs_setup(bool nzs, bool markAndPass, uint32_t zs_mask, bool sumByTwo, const std::vector<uint32_t>& thresholds, bool tp_zs_enable, bool tp_zs_soi_only) {
      u_back.getNode("DAQ_ZS_SUMBYTWO").write(sumByTwo?(1):(0));
      u_back.getNode("DAQ_ZS_DISABLE").write(nzs?(1):(0));
      u_back.getNode("DAQ_ZS_MARKANDPASS").write(markAndPass?(1):(0));
      u_back.getNode("DAQ_ZS_MASK").write(zs_mask&0xFFFF);
      if (thresholds.size()<=128)
	u_back.getNode("DAQ_ZS_THRESHOLDS").writeBlock(thresholds);

      u_back.getNode("DAQ_TP_ZS_ENABLE").write(tp_zs_enable?(1):(0));
      u_back.getNode("DAQ_TP_ZS_SOI_ONLY").write(tp_zs_soi_only?(1):(0));
      dispatch();
    }
    void uHTR::daq_zs_setup_query(bool& nzs, bool& markAndPass, uint32_t& zs_mask, bool& sumByTwo,  std::vector<uint32_t>& thresholds, bool& tp_zs_enable, bool& tp_zs_soi_only) {
      daq_query();

      nzs=iDaq.zs_disable;
      markAndPass=iDaq.zs_MarkNPass;
      sumByTwo=iDaq.zs_sumbytwo;
      zs_mask=iDaq.zsMask;
      tp_zs_enable=iDaq.zs_tp_enable;
      tp_zs_soi_only=iDaq.zs_tp_soi_only;

      thresholds=std::vector<uint32_t>(128,0);
      for (int i=0; i<128; i++)
	thresholds[i]=iDaq.zsThreshold[i];
    }
    uint32_t uHTR::daq_path_headerOccupancy(bool peak) { 
      if (peak) {
	uhal::ValWord<uint32_t> t=u_back.getNode("DAQ_HEADER_PEAKOCCUPANCY").read();
	dispatch();
	return t.value();
      } else {
	uhal::ValWord<uint32_t> t=u_back.getNode("DAQ_HEADER_OCCUPANCY").read();
	dispatch();
	return t.value();
      }
    }
    void uHTR::info_evn_orn(uint32_t& evn, uint32_t& orn) { 
      uhal::ValWord<uint32_t> t1=u_back.getNode("DAQ_ORN").read();
      uhal::ValWord<uint32_t> t2=u_back.getNode("DAQ_LASTEVN").read();
      dispatch();
      orn=t1.value();
      evn=t2.value();
    }
    void uHTR::daq_path_enable(bool enable) {
      if (enable) {
	u_back.getNode("DAQ_FORMATTER_ENABLE").write(1);
	u_front.getNode("PIPELINE0.ENABLE_ACCEPT").write(1);
	u_front.getNode("PIPELINE1.ENABLE_ACCEPT").write(1);
	u_front.getNode("PIPELINE_TP.ENABLE_ACCEPT").write(1);
      } else {
	u_back.getNode("DAQ_FORMATTER_ENABLE").write(0);
	u_front.getNode("PIPELINE0.ENABLE_ACCEPT").write(0);
	u_front.getNode("PIPELINE1.ENABLE_ACCEPT").write(0);
	u_front.getNode("PIPELINE_TP.ENABLE_ACCEPT").write(0);
      }
      dispatch();
    }

    void uHTR::daq_path_reset() {
      u_front.getNode("PIPELINE0.RESET_SOFT").write(1);
      u_front.getNode("PIPELINE1.RESET_SOFT").write(1);
      u_front.getNode("PIPELINE_TP.RESET_SOFT").write(1);
      u_back.getNode("DAQ_ZS_RESET").write(1);
      u_back.getNode("DAQ_RESET_L1FIFO_FULL").write(1);
      u_back.getNode("DAQ_RESET_FORMATTER").write(1);
      u_back.getNode("DAQ_RESET_LINK").write(1);
      u_back.getNode("DAQ_ERAM_RESET").write(1);
      u_front.getNode("PIPELINE0.RESET_SOFT").write(0);
      u_front.getNode("PIPELINE1.RESET_SOFT").write(0);
      u_front.getNode("PIPELINE_TP.RESET_SOFT").write(0);
      u_back.getNode("DAQ_ZS_RESET").write(0);
      u_back.getNode("DAQ_ERAM_RESET").write(0);
      dispatch();
    }


    //
    uint32_t uHTR::daq_spy_eventSize() { //< returns zero if there is no event to read.
      uhal::ValWord<uint32_t> t=u_back.getNode("DAQ_SPY_WORD_COUNT").read();
      dispatch();
      return t.value();
    }

    void uHTR::daq_spy_read(std::vector<uint32_t>& buffer, bool reset_after_read) { //< read the spy buffer
      buffer.clear();
      uint32_t needSpace=daq_spy_eventSize();
      if (needSpace==0) return;
      buffer.reserve(needSpace);

      uhal::ValVector<uint32_t> readBlock = u_back.getNode("DAQ_SPY_BUFFER").readBlock(needSpace);
      dispatch();      
      for (uint32_t j=0; j<needSpace; j++) buffer.push_back(readBlock[j]);
      
      if (reset_after_read) {
	
	u_back.getNode("DAQ_SPY_ADVANCE").write(1);
	dispatch();
      }
    }


    void uHTR::daq_f2b_links_query() {
      using namespace uhal;
      ValWord<uint32_t> st0=u_back.getNode("DAQ_F2B.STATUS0").read();
      ValWord<uint32_t> ec0=u_back.getNode("DAQ_F2B.ERROR_COUNT0").read();
      ValWord<uint32_t> wc0l=u_back.getNode("DAQ_F2B.WORDS0_LOW").read();
      ValWord<uint32_t> wc0h=u_back.getNode("DAQ_F2B.WORDS0_HIGH").read();

      ValWord<uint32_t> st1=u_back.getNode("DAQ_F2B.STATUS1").read();
      ValWord<uint32_t> ec1=u_back.getNode("DAQ_F2B.ERROR_COUNT1").read();
      ValWord<uint32_t> wc1l=u_back.getNode("DAQ_F2B.WORDS1_LOW").read();
      ValWord<uint32_t> wc1h=u_back.getNode("DAQ_F2B.WORDS1_HIGH").read();

      ValWord<uint32_t> st2=u_back.getNode("DAQ_F2B.STATUS2").read();
      ValWord<uint32_t> ec2=u_back.getNode("DAQ_F2B.ERROR_COUNT2").read();
      ValWord<uint32_t> wc2l=u_back.getNode("DAQ_F2B.WORDS2_LOW").read();
      ValWord<uint32_t> wc2h=u_back.getNode("DAQ_F2B.WORDS2_HIGH").read();

      dispatch();
      bool need_dispatch=false;

      iDaq.f2b_status[0]=st0.value();
      iDaq.f2b_status[1]=st1.value();
      iDaq.f2b_status[2]=st2.value();
      
      // remove low 16-bits
      iDaq.f2b_errors[0]&=0xFFFF0000u;
      iDaq.f2b_errors[1]&=0xFFFF0000u;
      iDaq.f2b_errors[2]&=0xFFFF0000u;

      // handle rollover
      if (ec0.value()==0xFFFF) {
	iDaq.f2b_errors[0]+=0x10000;
	u_back.getNode("DAQ_F2B.RESET_ERRORS0").write(1);
	need_dispatch=true;
      } else iDaq.f2b_errors[0]+=ec0.value();

      if (ec1.value()==0xFFFF) {
	iDaq.f2b_errors[1]+=0x10000;
	u_back.getNode("DAQ_F2B.RESET_ERRORS1").write(1);
	need_dispatch=true;
      } else iDaq.f2b_errors[1]+=ec1.value();

      if (ec2.value()==0xFFFF) {
	iDaq.f2b_errors[2]+=0x10000;
	u_back.getNode("DAQ_F2B.RESET_ERRORS2").write(1);
	need_dispatch=true;
      } else iDaq.f2b_errors[2]+=ec2.value();

      iDaq.f2b_words[0]=wc0l.value()+(uint64_t(wc0h.value())<<32);
      iDaq.f2b_words[1]=wc1l.value()+(uint64_t(wc1h.value())<<32);
      iDaq.f2b_words[2]=wc2l.value()+(uint64_t(wc2h.value())<<32);
      
      for (int i=0; i<N_DAQ_F2B; i++) {
	iDaq.f2b_error_rate[i].addObservation(iDaq.f2b_errors[i]);
	iDaq.f2b_data_rate[i].addObservation(iDaq.f2b_words[i]);
      }      
      if (need_dispatch) dispatch();
    }

    void uHTR::daq_f2b_links_reset() {
      u_back.getNode("DAQ_F2B.RESET0").write(1);
      u_back.getNode("DAQ_F2B.RESET1").write(1);
      u_back.getNode("DAQ_F2B.RESET2").write(1);
      dispatch();
      
      ::usleep(100);

      u_back.getNode("DAQ_F2B.RESET_ERRORS0").write(1);
      u_back.getNode("DAQ_F2B.RESET_ERRORS1").write(1);
      u_back.getNode("DAQ_F2B.RESET_ERRORS2").write(1);
      dispatch();
      

      for (int i=0; i<N_DAQ_F2B; i++) {
	iDaq.f2b_error_rate[i].resetHistory();
	iDaq.f2b_data_rate[i].resetHistory();
      }
    }
    void uHTR::daq_f2b_links_status(std::vector<uint32_t>& linkStatus, std::vector<uint32_t>& linkErrors, std::vector<double>& linkErrorRate) {
      linkStatus.clear(); 
      linkErrors.clear();
      linkErrorRate.clear();
      for (int ilink=0; ilink<N_DAQ_F2B; ilink++) {
	linkStatus.push_back(iDaq.f2b_status[ilink]);
	linkErrors.push_back(iDaq.f2b_errors[ilink]);
	linkErrorRate.push_back(iDaq.f2b_error_rate[ilink].getRate());
      }
    }
    void uHTR::daq_f2b_links_data(std::vector<uint64_t>& linkDataVolume, std::vector<double>& linkDataRate) {
      linkDataVolume.clear();
      linkDataRate.clear();
      for (int ilink=0; ilink<N_DAQ_F2B; ilink++) {
	linkDataVolume.push_back(iDaq.f2b_words[ilink]);
	linkDataRate.push_back(iDaq.f2b_data_rate[ilink].getRate());
      }
    }
    std::vector<uint32_t> uHTR::daq_f2b_links_spy(int ilink) {
      u_back.getNode("DAQ_F2B.SPY_MODE").write(2); // halt the fifo
      std::string spyAddr=::toolbox::toString("DAQ_F2B.SPY%d",ilink);
      uhal::ValVector<uint32_t> datai=u_back.getNode(spyAddr).readBlock(64);
      u_back.getNode("DAQ_F2B.SPY_MODE").write(0); // release the fifo
      dispatch();
      std::vector<uint32_t> data;
      data.reserve(64);
      for (size_t i=0; i<datai.size(); i++)
	data.push_back(datai[i]);
      return data;
    }

    // *****************************************
    // *   Data Pipeline Block  
    // *****************************************
    std::map<std::string, uint32_t> uHTR::pipelineInfo( bool pipe_id ) {
 
        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;

	uhal::ValWord<uint32_t> nsample        = pipeNode.getNode( "DAQ_NSAMPLES" ).read() ;
	uhal::ValWord<uint32_t> npresample     = pipeNode.getNode( "DAQ_NPRESAMPLES" ).read() ;
	uhal::ValWord<uint32_t> pipelength     = pipeNode.getNode( "DAQ_PIPELINELENGTH" ).read() ;
	uhal::ValWord<uint32_t> enable_accept  = pipeNode.getNode( "ENABLE_ACCEPT" ).read() ;
        uhal::ValWord<uint32_t> num_fibers     = pipeNode.getNode( "DAQ_NUM_FIBERS" ).read();
	uhal::ValWord<uint32_t> num_slices     = pipeNode.getNode( "DAQ_NUM_SLICES" ).read();
	uhal::ValWord<uint32_t> num_channels   = pipeNode.getNode( "DAQ_NUM_CHANNELS" ).read() ;
	uhal::ValWord<uint32_t> phase_delay    = pipeNode.getNode( "DAQ_PHASE_DELAY" ).read() ;
	uhal::ValWord<uint32_t> pipe_fudge     = pipeNode.getNode( "DAQ_PIPE_FUDGE" ).read() ;
        dispatch();

        printf(" ============================================= \n") ;
        printf("  ==   This is PIPELINE %d                 ==  \n", (int)pipe_id ) ;
        printf("  NSamples        : %d, NPresample    : %d \n", nsample.value(), npresample.value() );
        printf("  Pipeline Length : %d, N_Fibers      : %d \n", pipelength.value(), num_fibers.value() ) ;
	printf("  N_Slices        : %d, N_Channel     : %d \n", num_slices.value(), num_channels.value() );
	printf("  Phase Delay     : %d, Enable Accept : %d \n", phase_delay.value(), enable_accept.value() );
	printf("  PIPE Fudge      : %d                     \n", pipe_fudge.value() );
 
        std::map<std::string, uint32_t> pipeInfo ;
        pipeInfo["DAQ_NSAMPLES"]       = nsample.value() ;  
        pipeInfo["DAQ_NPRESAMPLES"]    = npresample.value() ;
        pipeInfo["DAQ_PIPELINELENGTH"] = pipelength.value() ;
        pipeInfo["ENABLE_ACCEPT"]      = enable_accept.value() ;
        pipeInfo["DAQ_NUM_FIBERS"]     = num_fibers.value() ;
        pipeInfo["DAQ_NUM_SLICES"]     = num_slices.value() ;
        pipeInfo["DAQ_NUM_CHANNELS"]   = num_channels.value() ;
        pipeInfo["DAQ_PHASE_DELAY"]    = phase_delay.value() ;
        pipeInfo["DAQ_PIPE_FUDGE"]     = pipe_fudge.value() ;

        return pipeInfo ;
    }

    void uHTR::pipeline_setup( bool pipe_id, uint32_t nsamples, uint32_t npresamples, uint32_t pipe_length, uint32_t phase_delay ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "DAQ_NSAMPLES" ).write(  nsamples ) ;
        pipeNode.getNode( "DAQ_NPRESAMPLES" ).write( npresamples ) ;
        pipeNode.getNode( "DAQ_PIPELINELENGTH" ).write( pipe_length ) ;
        pipeNode.getNode( "DAQ_PHASE_DELAY" ).write( phase_delay ) ;
        dispatch();

    }

    void uHTR::pipeline_enable( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "ENABLE_ACCEPT" ).write( 1 ) ;
        pipeNode.getNode( "RAM_ENABLE_CHOICE" ).write( 1 ) ;
        dispatch() ;
    }

    void uHTR::pipeline_delay_setup( bool pipe_id, std::string registerName, uint32_t delay ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( registerName ).write( delay ) ;
        dispatch() ;
    }

    void uHTR::pipeline_debug( bool pipe_id, uint32_t debug_flag ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "DEBUG" ).write( debug_flag ) ;
        dispatch() ;
    }

    uint32_t uHTR::pipeline_debug_check( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        uhal::ValWord<uint32_t> debug = pipeNode.getNode( "DEBUG" ).read() ;
        dispatch() ;
        return debug.value() ; 
    }

    void uHTR::pipeline_set_fiber( bool pipe_id, uint32_t theValue ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "DAQ_WHICH_FIBER"  ).write( theValue ) ;
        dispatch() ;
    }

    void uHTR::pipeline_set_slice( bool pipe_id, uint32_t theSlice ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "DAQ_WHICH_SLICE" ).write( theSlice ) ;
        dispatch() ;
    }

    void uHTR::pipeline_set_channel( bool pipe_id, uint32_t theChannel ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        pipeNode.getNode( "DAQ_WHICH_CHANNEL" ).write( theChannel ) ;
        dispatch() ;
    }

    void uHTR::pipeline_test_set( bool pipe_id, std::string test_hold, uint32_t hold, uint32_t q_accept ) {
   
        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        //pipeNode.getNode( test_hold ).write( q_hold ) ;
        if ( strncasecmp( test_hold.c_str() , "SLICE", 4 ) == 0 )  pipeNode.getNode( "DAQ_SLICE_HOLD" ).write( hold ) ; 
        if ( strncasecmp( test_hold.c_str() , "QUEUE", 4 ) == 0 )  pipeNode.getNode( "DAQ_QUEUE_HOLD" ).write( hold ) ; 
        if ( strncasecmp( test_hold.c_str() , "FIFO", 4 ) == 0 )   pipeNode.getNode( "DAQ_FIFO_HOLD" ).write( hold ) ; 
        pipeNode.getNode( "ENABLE_ACCEPT" ).write( q_accept ) ;
        dispatch() ;
    }

    std::map<std::string, uint32_t > uHTR::pipeline_status( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        uhal::ValWord<uint32_t> q_hold           = pipeNode.getNode( "DAQ_QUEUE_HOLD" ).read() ;
        uhal::ValWord<uint32_t> q_empty          = pipeNode.getNode( "DAQ_QUEUE_EMPTY" ).read() ;
        uhal::ValWord<uint32_t> s_hold           = pipeNode.getNode( "DAQ_SLICE_HOLD" ).read() ;
        uhal::ValWord<uint32_t> s_empty          = pipeNode.getNode( "DAQ_SLICE_EMPTY" ).read() ;
        uhal::ValWord<uint32_t> f_hold           = pipeNode.getNode( "DAQ_FIFO_HOLD" ).read() ;
        uhal::ValWord<uint32_t> f_empty          = pipeNode.getNode( "DAQ_FIFO_EMPTY" ).read() ;
        uhal::ValWord<uint32_t> enable_accept    = pipeNode.getNode( "ENABLE_ACCEPT" ).read() ;
        uhal::ValWord<uint32_t> accept_counter   = pipeNode.getNode( "DAQ_ACCEPT_COUNTER" ).read() ;
        uhal::ValWord<uint32_t> state_RamControl = pipeNode.getNode( "DAQ_RAM_CTRL_STATE" ).read() ;
        dispatch() ;

        std::map<std::string, uint32_t> queueInfo ;
        queueInfo["DAQ_QUEUE_HOLD"]     = q_hold.value() ;
        queueInfo["DAQ_QUEUE_EMPTY"]    = q_empty.value() ;
        queueInfo["DAQ_SLICE_HOLD"]     = s_hold.value() ;
        queueInfo["DAQ_SLICE_EMPTY"]    = s_empty.value() ;
        queueInfo["DAQ_FIFO_HOLD"]      = f_hold.value() ;
        queueInfo["DAQ_FIFO_EMPTY"]     = f_empty.value() ;
        queueInfo["ENABLE_ACCEPT"]  = enable_accept.value() ;
        queueInfo["DAQ_ACCEPT_COUNTER"] = accept_counter.value() ;
        queueInfo["DAQ_RAM_CTRL_STATE"] = state_RamControl.value() ;
        return queueInfo ;

    }

    std::map<std::string, uint32_t > uHTR::pipeline_ctrl( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        uhal::ValWord<uint32_t> v1 = pipeNode.getNode( "DAQ_READ_ADDR" ).read() ;
        uhal::ValWord<uint32_t> v2 = pipeNode.getNode( "DAQ_WRITE_ADDR" ).read() ;
        uhal::ValWord<uint32_t> v3 = pipeNode.getNode( "DAQ_WRITE_PTR" ).read() ;
        dispatch() ;

        std::map<std::string, uint32_t> ctrl_info ;
        ctrl_info["DAQ_READ_ADDR"]   = v1.value() ;
        ctrl_info["DAQ_WRITE_ADDR"]  = v2.value() ;
        ctrl_info["DAQ_WRITE_PTR"]   = v3.value() ;
     
        return ctrl_info ;
    }


    std::map<std::string, uint32_t> uHTR::pipeline_cnt( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        uhal::ValWord<uint32_t> v2 = pipeNode.getNode( "DAQ_ERROR_COUNT" ).read() ;
        uhal::ValWord<uint32_t> v3 = pipeNode.getNode( "DAQ_SLICE_COUNTER" ).read() ;
        uhal::ValWord<uint32_t> v4 = pipeNode.getNode( "DAQ_TR" ).read() ;
        uhal::ValWord<uint32_t> v5 = pipeNode.getNode( "DAQ_WCNT" ).read() ;
        uhal::ValWord<uint32_t> v6 = pipeNode.getNode( "DAQ_RCNT" ).read() ;
        uhal::ValWord<uint32_t> v7 = pipeNode.getNode( "DAQ_FIFO_TR" ).read() ;
        uhal::ValWord<uint32_t> v8 = pipeNode.getNode( "DAQ_FIFO_WCNT" ).read() ;
        dispatch() ;

        std::map<std::string, uint32_t> cnt_info ;
        cnt_info["DAQ_ERROR_COUNT"]   = v2.value() ;
        cnt_info["DAQ_SLICE_COUNTER"] = v3.value() ;
        cnt_info["DAQ_TR"]            = v4.value() ;
        cnt_info["DAQ_WCNT"]          = v5.value() ;
        cnt_info["DAQ_WCNT"]          = v6.value() ;
        return cnt_info ;
    }

    void uHTR::pipeline_SWL1A( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
	pipeNode.getNode( "DAQ_SWL1A" ).write( 1 ) ;
	pipeNode.getNode( "DAQ_SWL1A" ).write( 0 ) ;
        dispatch() ;
    }
 
    void uHTR::pipeline_advance( bool pipe_id, uint32_t adv, std::string advance_name ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        if ( strncasecmp( advance_name.c_str() , "SLICE", 4 ) == 0 )  pipeNode.getNode( "DAQ_SLICE_ADVANCE" ).write( adv ) ; 
        if ( strncasecmp( advance_name.c_str() , "QUEUE", 4 ) == 0 )  pipeNode.getNode( "DAQ_QUEUE_ADVANCE" ).write( adv ) ; 
        if ( strncasecmp( advance_name.c_str() , "FIFO", 4 ) == 0 )  pipeNode.getNode( "DAQ_FIFO_ADVANCE" ).write( adv ) ; 
        dispatch() ;
    }
 
    uint32_t uHTR::pipeline_Data( bool pipe_id, std::string data_src ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        std::string data_src_name ; 
        if ( strncasecmp( data_src.c_str() , "SLICE", 4 ) == 0 )  data_src_name = "DAQ_SLICE_DATA" ; 
        if ( strncasecmp( data_src.c_str() , "QUEUE", 4 ) == 0 )  data_src_name = "DAQ_QUEUE_DATA" ; 
        if ( strncasecmp( data_src.c_str() , "FIFO", 4 ) == 0 )   data_src_name = "DAQ_FIFO_DATA"; 
	uhal::ValWord<uint32_t> q_data    = pipeNode.getNode( data_src_name ).read() ;
        dispatch() ;

        return q_data.value() ;    
    }

    uint32_t uHTR::pipeline_overflow( bool pipe_id ) {

        uhal::Node& pipeNode = ( pipe_id ) ? u_front.getNode("PIPELINE1") :  u_front.getNode("PIPELINE0") ;
        uhal::ValWord<uint32_t> ovfl = pipeNode.getNode( "DAQ_PIPE_OVERFLOW" ).read() ;
        dispatch() ;
        return ovfl.value() ; 
    }


  }
}
