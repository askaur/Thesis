#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <algorithm>
#include "uhal/ConnectionManager.hpp"
#include "hcal/uhtr/uHTRtool.hh"

using namespace hcal::uhtr;

//============================================================================
// Debug the DataPipeline.v firmware in uHTR.
//============================================================================
// Contact: frahm@physics.umn.edu
//============================================================================

static bool pipe_id = false;  

void uHTRtool::ejf_test( uHTR* uHTR ) { 
  
  // initialize the link
  int orbDelay = 92 ; 
  bool autoAlign = false ; 
  bool autoReset = false ; 

  uHTR->link_init( orbDelay, autoAlign, autoReset ) ;
  //link_status( uHTR ) ;

  FILE *logfile = fopen( "debug.log", "a" );

  // Set pipeline   
  int which_pipe = 0;
  which_pipe = tool_readline_int("which pipeline ? ( 0 or 1 )  ", (int)which_pipe );
  pipe_id = ( which_pipe > 0  ) ? true : false ;
 
  // Check pipeline information
  std::map<std::string, uint32_t > pipeInfo = uHTR->pipelineInfo( pipe_id ) ;

  printf(" ===================================================== \n") ;
  for ( std::map<std::string, uint32_t>::iterator it = pipeInfo.begin(); it != pipeInfo.end() ; it++ ) {
      fprintf(stdout, "  PipeInfo[%s] = %d \n", (it->first).c_str() , it->second );
      fprintf(logfile,"  PipeInfo[%s] = %d \n", (it->first).c_str() , it->second );
  }

  // Set up pipeline 
  uint32_t nsamples = 12;
  nsamples = tool_readline_int("nsamples ?  ", nsamples );
  uint32_t npresamples = 4;
  npresamples = tool_readline_int("npresamples ?  ", npresamples );
  uint32_t pipe_length = 68;
  pipe_length = tool_readline_int("pipe length ?  ", pipe_length );
  uint32_t phase_delay = 6;
  phase_delay = tool_readline_int("phase delay ?  ", phase_delay );

  uHTR->pipeline_setup( pipe_id, nsamples, npresamples, pipe_length, phase_delay ) ;

  // Setup debug flags
  int pipe_debug_flags = 0 ;
  pipe_debug_flags = tool_readline_int("pipe_debug_flags ?  ", pipe_debug_flags );
  uHTR->pipeline_debug( pipe_id, pipe_debug_flags ) ;
  fprintf( stdout,  " pipe_debug_flags = %08x\n",(int) pipe_debug_flags);
  fprintf( logfile, " pipe_debug_flags = %08x\n",(int) pipe_debug_flags);
 
  if ((pipe_debug_flags & 2) > 0) {
     fprintf(stdout, "In this mode, the number of accept signals is set to 10*NUM_SLICES by the firmware.(%d)\n",
            (int)(10*pipeInfo["DAQ_NUM_SLICES"]) );
     fprintf(logfile,"In this mode, the number of accept signals is set to 10*NUM_SLICES by the firmware.(%d)\n",
             (int)(10*pipeInfo["DAQ_NUM_SLICES"]) );

     // Insert a counter on one channel of one fiber.
     uint32_t magic_fiber = 0;    
     magic_fiber = tool_readline_int("magic fiber ?  ", magic_fiber );
     uint32_t magic_channel = 0;
     magic_channel =  tool_readline_int("magic channel ?  ", magic_channel );

     uHTR->pipeline_set_fiber( pipe_id, magic_fiber ) ;
     uHTR->pipeline_set_channel( pipe_id, magic_channel ) ;

     fprintf(stdout, " magic fiber = %d\n",(int)magic_fiber);
     fprintf(logfile," magic_fiber = %d\n",(int)magic_fiber);

     fprintf(stdout, " magic channel = %d\n",(int)magic_channel);
     fprintf(logfile," magic channel = %d\n",(int)magic_channel);
  }

  //===============================================================
  // Setup the delay registers.
  // SET TO ZERO TO TEST DEFAULT VALUES IN FIRMWARE.
  //===============================================================
  uint32_t setup_delays = 0;
  setup_delays = tool_readline_int(" Setup delays ? ( 0 : No delay ) : ", setup_delays );
  //===============================================================

  if (setup_delays) {
    
    fprintf(stdout, "\n ---- Setup the delay registers: \n");
    fprintf(logfile,"\n ---- Setup the delay registers: \n");

    std::string regName[] = { "DAQ_DELAY_READ_ADDRESS", "DAQ_DELAY_ENABLE_SLICE", "DAQ_DELAY_FIBER_SLICE",
                              "DAQ_DELAY_WRITE_ENABLE", "DAQ_DELAY_READ_SLICE",   "DAQ_DELAY_CHANNEL",  
                              "DAQ_DELAY_FIBER",        "DAQ_DELAY_SLICE",        "DAQ_DELAY_FIFO_WRITE",
                              "DAQ_CHOOSE_CHANNEL_FIFO","DAQ_CHOOSE_FIBER_FIFO",  "DAQ_CHOOSE_SLICEL_FIFO",
                              "DAQ_WRITE_ENABLE",        0  } ;

    uint32_t delay[] = { 1,  3,  0, 
                         3,  0,  0, 
                         0,  0,  0, 
                         0,  0,  0, 
                         0,  0      } ;

    
    for ( int i=0; regName[i] !=0 ; i++) {
        uHTR->pipeline_delay_setup( pipe_id, regName[i], delay[i] ) ;
        fprintf(stdout,  "  %s = %d\n", regName[i].c_str() , delay[i] );
        fprintf(logfile, "  %s = %d\n", regName[i].c_str() , delay[i] );
    }

  }
  //==========================================================================

  fprintf(stdout, "INTERNAL ERROR\n");
  fprintf(logfile,"INTERNAL ERROR\n");
  fclose( logfile ) ; 
}


void uHTRtool::pipeline_queue_test( uHTR* uHTR ) { 

    FILE *logfile = fopen( "debug.log", "a" );

    fprintf(stdout, " === Debugging QUEUE ===\n");
    fprintf(logfile," === Debugging QUEUE ===\n");
    
    // Check debug flag first   
    uint32_t pipe_debug_flags = uHTR->pipeline_debug_check( pipe_id ) ;

    std::map<std::string, uint32_t> pipeInfo = uHTR->pipelineInfo( pipe_id ) ;
    uint32_t num_slices = pipeInfo["DAQ_NUM_SLICES"] ;
    // Get num_accept 
    uint32_t num_accept = 1;
    if ((pipe_debug_flags & 2) > 0) num_accept = 10*num_slices;
    else num_accept = tool_readline_int("num_accept ?  ", num_accept );

    fprintf(stdout, "num_accept = %d\n",(int)num_accept);
    fprintf(logfile,"num_accept = %d\n",(int)num_accept);

    // Get current pipeline info
    uint32_t num_slice_accept[100];
    for (uint32_t j=0; j<num_slices; j++) {
        num_slice_accept[j] = num_accept / num_slices;
        if ((num_accept % num_slices) > j) num_slice_accept[j]++;
    }

    // Set pipeline queue
    uint32_t queue_hold = 1;
    uint32_t enable_accept = 1;
    uHTR->pipeline_test_set( pipe_id, "QUEUE" ,queue_hold, enable_accept ) ;

    // Check the setup of pipeline queue
    std::map<std::string, uint32_t> queueInfo = uHTR->pipeline_status( pipe_id ) ;
    fprintf(stdout, "queue_hold = %d\n", queueInfo["DAQ_QUEUE_HOLD"] );
    fprintf(logfile,"queue_hold = %d\n", queueInfo["DAQ_QUEUE_HOLD"] );
    
    fprintf(stdout, "enable_accept = %d\n", queueInfo["ENABLE_ACCEPT"] );
    fprintf(logfile,"enable_accept = %d\n", queueInfo["ENABLE_ACCEPT"] );

    fprintf(stdout, "queue_empty = %d\n", queueInfo["DAQ_QUEUE_EMPTY"] );
    fprintf(logfile,"queue_empty = %d\n", queueInfo["DAQ_QUEUE_EMPTY"] );
    
    // Issue the swL1A signals.
    sleep(1);
    for (uint32_t j=0; j<num_accept; j++) { 
      uHTR->pipeline_SWL1A( pipe_id  ) ;
      usleep(10);
      fprintf(stdout, "swL1A[%d] sent.\n",(int)j+1);
      fprintf(logfile,"swL1A[%d] sent.\n",(int)j+1);
    } 
    sleep(1) ;
    
    // check queueInfo again , 
    queueInfo.clear() ;
    queueInfo = uHTR->pipeline_status( pipe_id ) ;
    fprintf(stdout, " accept_counter = %d\n", queueInfo["DAQ_ACCEPT_COUNTER"] );
    fprintf(logfile," accept_counter = %d\n", queueInfo["DAQ_ACCEPT_COUNTER"] );
    
    fprintf(stdout, " state_RamControl = %d\n", queueInfo["DAQ_RAM_CTRL_STATE"] );
    fprintf(logfile," state_RamControl = %d\n", queueInfo["DAQ_RAM_CTRL_STATE"] );
    
    // Empty the queue.
    uint32_t queue_data;
    uint32_t num_expected;
    uint32_t k;

    num_expected = num_accept;
    fprintf(stdout, "\nEmpty the queue: expecting %d words\n",
	    (int)num_expected);
    fprintf(logfile,"\nEmpty the queue: expecting %d words\n",
	    (int)num_expected);
    
    uint32_t queue_empty = queueInfo["DAQ_QUEUE_EMPTY"] ;
    fprintf(stdout, "queue_empty = %d\n", queueInfo["DAQ_QUEUE_EMPTY"] );
    fprintf(logfile,"queue_empty = %d\n", queueInfo["DAQ_QUEUE_EMPTY"] );
    
    k = 0;
    while ( queue_empty == 0 ) {
        k++;
        if ( k > num_expected+10 ) {
   	   fprintf(stdout, "ERROR: TOO MANY.\n");
	   fprintf(logfile,"ERROR: TOO MANY.\n");
	   break ;
        }

        queue_data = uHTR->pipeline_Data( pipe_id, "DAQ_QUEUE_DATA" );
	fprintf(stdout, "queue_data[%d] = %08x\n",(int)k, (int)queue_data );
	fprintf(logfile,"queue_data[%d] = %08x\n",(int)k, (int)queue_data );

	uHTR->pipeline_advance( pipe_id, 0, "QUEUE" ) ; //strobe
	usleep(10);
    }
    
    queueInfo.clear() ;
    queueInfo = uHTR->pipeline_status( pipe_id ) ;
    queue_empty = queueInfo["DAQ_QUEUE_EMPTY"] ;
    fprintf(stdout, "queue_empty = %d\n",(int)queue_empty);
    fprintf(logfile,"queue_empty = %d\n",(int)queue_empty);

    if (k != num_expected) {
      fprintf(stdout, "ERROR in QUEUE.\n");
      fprintf(logfile,"ERROR in QUEUE.\n");
    }
    else {
      fprintf(stdout, "QUEUE: OK\n");
      fprintf(logfile,"QUEUE: OK\n");
    }

    fprintf(logfile, "\n------------------\n\n" ) ;
    fclose( logfile ) ; 

    //return;
    exit(0);

}

void uHTRtool::pipeline_capture_test( uHTR* uHTR ) { 
    
    FILE *logfile = fopen( "debug.log", "a" );

    fprintf(stdout, "SPYING ON CAPTURE REGS\n");
    fprintf(logfile,"SPYING ON CAPTURE REGS\n");

    // Check debug flag first and get pipeline setup info  
    uint32_t pipe_debug_flags = uHTR->pipeline_debug_check( pipe_id ) ;
    std::map<std::string, uint32_t> pipeInfo = uHTR->pipelineInfo( pipe_id ) ;
    uint32_t num_slices = pipeInfo["DAQ_NUM_SLICES"] ;

    uint32_t num_accept = 1;
    if ((pipe_debug_flags & 2) > 0) num_accept = 10*num_slices;
    else num_accept = tool_readline_int("num_accept ?  ", num_accept );

    while (num_accept > num_slices) {
      num_accept = tool_readline_int("num_accept ?  (must be <= num_slices)", num_accept );
    }
    fprintf(stdout, "num_accept = %d\n",(int)num_accept);
    fprintf(logfile,"num_accept = %d\n",(int)num_accept);
    
    uint32_t num_slice_accept[100];
    for (uint32_t j=0; j<num_slices; j++) {
      num_slice_accept[j] = num_accept / num_slices;
      if ((num_accept % num_slices) > j) num_slice_accept[j]++;
    }

    // set up test 
    uint32_t slice_hold = ((1 << num_slices) - 1) * 16;
    fprintf(stdout, "reg_hold = %d\n",(int)slice_hold/16);
    fprintf(logfile,"reg_hold = %d\n",(int)slice_hold/16);
    
    uint32_t enable_accept = 1;
    fprintf(stdout, "enable_accept = %d\n",(int)enable_accept);
    fprintf(logfile,"enable_accept = %d\n",(int)enable_accept);

    uHTR->pipeline_test_set( pipe_id, "SLICE", slice_hold, enable_accept ) ;

    // Issue the swL1A signals.
    sleep(1);
    for (uint32_t j=0; j<num_accept; j++) { 
      uHTR->pipeline_SWL1A( pipe_id ) ;
      usleep(10);
      fprintf(stdout, "swL1A[%d] sent.\n",(int)j+1);
      fprintf(logfile,"swL1A[%d] sent.\n",(int)j+1);
    } 
    sleep(1) ;
 
    // Check the status   
    std::map<std::string, uint32_t> queueInfo = uHTR->pipeline_status( pipe_id ) ;
    fprintf(stdout, "accept_counter = %d\n", queueInfo["DAQ_ACCEPT_COUNTER"] ) ;
    fprintf(logfile,"accept_counter = %d\n", queueInfo["DAQ_ACCEPT_COUNTER"] ) ;
    
    fprintf(stdout, "state_RamControl = %d\n",queueInfo["DAQ_RAM_CTRL_STATE"] );
    fprintf(logfile,"state_RamControl = %d\n",queueInfo["DAQ_RAM_CTRL_STATE"] );
    
    fprintf(stdout, "\n");
    fprintf(logfile,"\n");

    // Empty the slices, one at a time.
    uint32_t slice_id;
    uint32_t which_fiber;
    uint32_t k;
    uint32_t capture_data;

    uint32_t bit0, bit16;
    uint32_t cap_id0, cap_id1, cap_id2;
    uint32_t data0, data1, data2;

    // Loop over the slices.
    for (slice_id=0; slice_id<num_slices; slice_id++) {
      if (num_slice_accept[slice_id] > 0) {
	for (which_fiber=0; which_fiber< pipeInfo["DAQ_NUM_FIBERS"]; which_fiber++) {
	  
	  uHTR->pipeline_set_slice( pipe_id, slice_id ) ;
	  uHTR->pipeline_set_fiber( pipe_id, which_fiber ) ;
	  fprintf(stdout, "slice_id = %d,   ",(int)slice_id);
	  fprintf(logfile,"slice_id = %d,   ",(int)slice_id);
	  
	  fprintf(stdout, "fiber = %d\n",(int)which_fiber);
	  fprintf(logfile,"fiber = %d\n",(int)which_fiber);
	  
	  for (k=0; k< pipeInfo["DAQ_NSAMPLES"]+2; k++) {
            capture_data = uHTR->pipeline_Data( pipe_id, "DAQ_CAPTURE_DATA" );
	    fprintf(stdout, "capture_data[%d][%2d] = %08x     ",
		    (int)slice_id, (int)which_fiber, (int)capture_data);
	    fprintf(logfile,"capture_data[%d][%2d] = %08x     ",
		    (int)slice_id, (int)which_fiber, (int)capture_data);
	    
	    bit0  = (capture_data >>  0) & 0x1;
	    bit16 = (capture_data >> 16) & 0x1;
	    
	    cap_id0 = (capture_data >> 7) & 0x3;
	    cap_id1 = (capture_data >> 5) & 0x3;
	    cap_id2 = (capture_data >> 3) & 0x3;
	    
	    data0 = (capture_data >> 25) & 0x7F;
	    data1 = (capture_data >> 17) & 0x7F;
	    data2 = (capture_data >>  9) & 0x7F;
	    
	    fprintf(stdout, " %x  %02x    ",
		    (int)cap_id0, (int)data0);
	    fprintf(logfile," %x  %02x    ",
		    (int)cap_id0, (int)data0);
	    
	    fprintf(stdout, " %x  %02x    ",
		    (int)cap_id1, (int)data1);
	    fprintf(logfile," %x  %02x    ",
		    (int)cap_id1, (int)data1);
	    
	    fprintf(stdout, " %x  %02x    ",
		    (int)cap_id2, (int)data2);
	    fprintf(logfile," %x  %02x    ",
		    (int)cap_id2, (int)data2);
	    
	    fprintf(stdout, "%x %x \n",bit16,bit0);
	    fprintf(logfile,"%x %x \n",bit16,bit0);
	    
	    uHTR->pipeline_advance( pipe_id, (1 << slice_id), "SLICE" ) ; //strobe
	    usleep(10);
	  }
	  fprintf(stdout, "\n");
	  fprintf(logfile,"\n");
	}
      }
    }

    // disable enable_accept
    uHTR->pipeline_test_set( pipe_id, "SLICE", slice_hold, 0 ) ;
    fprintf(stdout, "enable_accept = %d\n",(int)enable_accept);
    fprintf(logfile,"enable_accept = %d\n",(int)enable_accept);
    
    fprintf(logfile, "\n------------------\n\n" ) ;
    fclose( logfile ) ; 
    //return;
    exit(0);

}

void uHTRtool::pipeline_slice_test( uHTR* uHTR ) { 
    
    FILE *logfile = fopen( "debug.log", "a" );

    fprintf(stdout, "Debugging SLICE.\n");
    fprintf(logfile,"Debugging SLICE.\n");

    // Check debug flag first and get pipeline setup info  
    uint32_t pipe_debug_flags = uHTR->pipeline_debug_check( pipe_id ) ;
    std::map<std::string, uint32_t> pipeInfo = uHTR->pipelineInfo( pipe_id ) ;
    uint32_t num_slices   = pipeInfo["DAQ_NUM_SLICES"] ;
    uint32_t num_fibers   = pipeInfo["DAQ_NUM_FIBERS"] ;
    uint32_t num_channels = pipeInfo["DAQ_NUM_CHANNELS"] ;
    uint32_t nsamples     = pipeInfo["DAQ_NSAMPLES"] ;     
    uint32_t npresamples  = pipeInfo["DAQ_NPRESAMPLES"] ;
    uint32_t pipe_length  = pipeInfo["DAQ_PIPELINELENGTH"] ; 
    uint32_t fudge        = pipeInfo["DAQ_PIPE_FUDGE"] ; 

    uint32_t num_accept = 1;
    if ((pipe_debug_flags & 2) > 0) num_accept = 10*num_slices;
    else num_accept = tool_readline_int("num_accept ?  ", num_accept );
    fprintf(stdout, "num_accept = %d\n",(int)num_accept);
    fprintf(logfile,"num_accept = %d\n",(int)num_accept);
    
    uint32_t num_slice_accept[100];
    for (uint32_t j=0; j<num_slices; j++) {
      num_slice_accept[j] = num_accept / num_slices;
      if ((num_accept % num_slices) > j) num_slice_accept[j]++;
    }

    uint32_t slice_hold = (1 << num_slices) - 1;
    fprintf(stdout, "slice_hold = %d\n",(int)slice_hold);
    fprintf(logfile,"slice_hold = %d\n",(int)slice_hold);
    
    uint32_t enable_accept = 1;
    fprintf(stdout, "enable_accept = %d\n",(int)enable_accept);
    fprintf(logfile,"enable_accept = %d\n",(int)enable_accept);

    uHTR->pipeline_test_set( pipe_id, "SLICE", slice_hold, enable_accept ) ;
    
    std::map<std::string, uint32_t> sliceInfo = uHTR->pipeline_status( pipe_id ) ;
    uint32_t slice_empty = sliceInfo["DAQ_SLICE_EMPTY"];
    fprintf(stdout, "slice_empty = %d\n",(int)slice_empty);
    fprintf(logfile,"slice_empty = %d\n",(int)slice_empty);
    
    // Issue the swL1A signals.
    sleep(1);
    for (uint32_t j=0; j<num_accept; j++) { 
      uHTR->pipeline_SWL1A( pipe_id ) ;
      usleep(10);
      fprintf(stdout, "swL1A[%d] sent.\n",(int)j+1);
      fprintf(logfile,"swL1A[%d] sent.\n",(int)j+1);
    } 
    sleep(1) ;
    
    sliceInfo.clear() ;
    sliceInfo = uHTR->pipeline_status( pipe_id ) ;

    fprintf(stdout, "accept_counter = %d\n", sliceInfo["DAQ_ACCEPT_COUNTER"] );
    fprintf(logfile,"accept_counter = %d\n", sliceInfo["DAQ_ACCEPT_COUNTER"] );
    
    fprintf(stdout, "state_RamControl = %d\n", sliceInfo["DAQ_RAM_CTRL_STATE"] );
    fprintf(logfile,"state_RamControl = %d\n", sliceInfo["DAQ_RAM_CTRL_STATE"] );
    
    slice_empty = sliceInfo["DAQ_SLICE_EMPTY"] ;
    fprintf(stdout, "slice_empty = %d\n",(int)slice_empty);
    fprintf(logfile,"slice_empty = %d\n",(int)slice_empty);
    
    // Empty the slices, one at a time.
    uint32_t slice_data;
    uint32_t slice_id;
    uint32_t channel;
    uint32_t num_expected;
    uint32_t k;

    uint32_t cap_err0, cap_err1;
    uint32_t cap_id0, cap_id1;
    uint32_t data0, data1;

    // Loop over the slices.
    for (slice_id=0; slice_id<num_slices; slice_id++) {
      for (channel=0; channel<num_channels; channel++) {

	num_expected = num_slice_accept[slice_id]*(num_fibers*nsamples)/2;
	fprintf(stdout, "\nEmpty a channel: (expecting %d words per channel)\n",
		(int)num_expected);
	fprintf(logfile,"\nEmpty a channel: (expecting %d words per channel)\n",
		(int)num_expected);

	uHTR->pipeline_set_slice( pipe_id, slice_id ) ;
	fprintf(stdout, "slice_id = %d\n",(int)slice_id);
	fprintf(logfile,"slice_id = %d\n",(int)slice_id);
	
	uHTR->pipeline_set_channel( pipe_id, (channel+16)  ) ;
	fprintf(stdout, "channel = %d\n",(int)channel);
	fprintf(logfile,"channel = %d\n",(int)channel);
	
        sliceInfo.clear() ;
        sliceInfo = uHTR->pipeline_status( pipe_id ) ; 
	slice_empty = ( sliceInfo["DAQ_SLICE_EMPTY"] >> slice_id) & 1;
	fprintf(stdout, "slice_empty[%d][%d] = %d\n",(int)slice_id, (int)channel, (int)slice_empty);
	fprintf(logfile,"slice_empty[%d][%d] = %d\n",(int)slice_id, (int)channel, (int)slice_empty);

	if (slice_empty == 0) {
	  fprintf(stdout, "                                     ");
	  fprintf(stdout, "e1 id dat1    e0 id dat0\n");
	  fprintf(logfile,"                                     ");
	  fprintf(logfile,"e1 id dat1    e0 id dat0\n");
	}

	k = 0;
	while ( slice_empty == 0 ) {
	  k++;
	  if ( k > num_expected+10 ) {
	    fprintf(stdout, "ERROR: TOO MANY.\n");
	    fprintf(logfile,"ERROR: TOO MANY.\n");
	    break ;
	  }
	  
	  slice_data = uHTR->pipeline_Data( pipe_id,  "DAQ_SLICE_DATA" );
	  fprintf(stdout, "slice_data[%d][%d][%3d] = %08x     ",
		  (int)slice_id, (int)channel, (int)k, (int)slice_data);
	  fprintf(logfile,"slice_data[%d][%d][%3d] = %08x     ",
		  (int)slice_id, (int)channel, (int)k, (int)slice_data);
	  
	  cap_err1 = (slice_data >> 21) & 0x1;
	  cap_id1  = (slice_data >> 19) & 0x3;
	  data1    = (slice_data >> 11) & 0xFF;
	  cap_err0 = (slice_data >> 10) & 0x1;
	  cap_id0  = (slice_data >>  8) & 0x3;
	  data0    = (slice_data >>  0) & 0xFF;
	  
	  fprintf(stdout, " %x  %x   %02x    ",
		  (int)cap_err1, (int)cap_id1, (int)data1);
	  fprintf(stdout, " %x  %x   %02x\n",
		  (int)cap_err0, (int)cap_id0, (int)data0);
	  fprintf(logfile," %x  %x   %02x    ",
		  (int)cap_err1, (int)cap_id1, (int)data1);
	  fprintf(logfile," %x  %x   %02x\n",
		  (int)cap_err0, (int)cap_id0, (int)data0);
	  
	  uHTR->pipeline_advance( pipe_id, (1 << slice_id) , "SLICE" ) ; //strobe
	  usleep(10);

          sliceInfo.clear() ;
          sliceInfo = uHTR->pipeline_status( pipe_id ) ; 
	  slice_empty = ( sliceInfo["DAQ_SLICE_EMPTY"] >> slice_id) & 1;
	}
	
        sliceInfo.clear() ;
        sliceInfo = uHTR->pipeline_status( pipe_id ) ; 
        slice_empty = ( sliceInfo["DAQ_SLICE_EMPTY"] >> slice_id) & 1;

	fprintf(stdout, "slice_empty[%d][%d] = %d\n",(int)slice_id,(int)channel,(int)slice_empty);
	fprintf(logfile,"slice_empty[%d][%d] = %d\n",(int)slice_id,(int)channel,(int)slice_empty);
	
	if (k != num_expected) {
	  fprintf(stdout, "ERROR in SLICE[%d][%d].\n",(int)slice_id,(int)channel);
	  fprintf(logfile,"ERROR in SLICE[%d][%d].\n",(int)slice_id,(int)channel);
	}
	else {
	  fprintf(stdout, "SLICE[%d][%d]: OK\n",(int)slice_id,(int)channel);
	  fprintf(logfile,"SLICE[%d][%d]: OK\n",(int)slice_id,(int)channel);
	}
	fprintf(stdout, "\n");
	fprintf(logfile,"\n");
      }
    }

    // Print internal values from RAM Control.
    uint32_t raddr, waddr, val;

    std::map<std::string, uint32_t> ctrl_info = uHTR->pipeline_ctrl( pipe_id ) ;
    raddr = ctrl_info["DAQ_READ_ADDR"] ;
    fprintf(stdout, "read address = %04x\n",(int)raddr);
    fprintf(logfile,"read address = %04x\n",(int)raddr);

    waddr = ctrl_info["DAQ_WRITE_ADDR"] ;
    val = raddr + pipe_length - npresamples + fudge;
    fprintf(stdout, "write address = %04x (vs. %04x)\n",
	    (int)waddr,(int)(val));
    fprintf(logfile,"write address = %04x (vs. %04x)\n",
	    (int)waddr,(int)(val));

    val = ctrl_info["DAQ_WRITE_PTR"] ;
    fprintf(stdout, "write ptr = %06x\n",(int)(val));
    fprintf(logfile,"write ptr = %06x\n",(int)(val));
    
    // Print internal counters from slices.
    uint32_t which_slice, count, expected;
    std::map<std::string, uint32_t> cnt_info ;
    for (which_slice=0; which_slice < num_slices; which_slice++) {

      //fprintf(stdout, "which_slice = %d\n",(int)which_slice);
      //fprintf(logfile,"which_slice = %d\n",(int)which_slice);
      
      uHTR->pipeline_set_slice ( pipe_id, which_slice ) ;
      cnt_info.clear() ;
      cnt_info = uHTR->pipeline_cnt( pipe_id ) ;

      count = cnt_info["DAQ_ERROR_COUNT"] ; 
      fprintf(stdout, "slice[%d] data errors = %d\n",
	      (int)which_slice,(int)count);
      fprintf(logfile,"slice[%d] data errors = %d\n",
	      (int)which_slice,(int)count);
	
      count = cnt_info["DAQ_SLICE_COUNTER"] ; 
      expected = num_slice_accept[which_slice] * nsamples;
      fprintf(stdout, "slice[%d] counter = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] counter = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
	
      count = cnt_info["DAQ_TR"] ; 
      expected = num_slice_accept[which_slice] * (num_fibers*nsamples);
      fprintf(stdout, "slice[%d] transfer states = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] transfer states = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
	
      count = cnt_info["DAQ_WCNT"] ; 
      expected = num_slice_accept[which_slice] * (num_fibers*nsamples);
      fprintf(stdout, "slice[%d] fifo writes = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] fifo writes = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      
      count = cnt_info["DAQ_RCNT"] ; 
      expected = num_slice_accept[which_slice] * (num_fibers*num_channels*nsamples/2);
      fprintf(stdout, "slice[%d] fifo reads = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] fifo reads = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
    }	
	
    enable_accept = 0;
    uHTR->pipeline_test_set( pipe_id, "SLICE", slice_hold, enable_accept  ) ;
    fprintf(stdout, "enable_accept = %d\n", (int)enable_accept );
    fprintf(logfile,"enable_accept = %d\n", (int)enable_accept );
    
    count = uHTR->pipeline_overflow( pipe_id );
    fprintf(stdout, "pipeline overflow flag = %d\n",(int)count);
    fprintf(logfile,"pipeline overflow flag = %d\n",(int)count);

    fprintf(logfile, "\n------------------\n\n" ) ;
    fclose( logfile ) ; 
    //return;
    exit(0);
}

void uHTRtool::pipeline_fifo_test( uHTR* uHTR ) { 
    
    FILE *logfile = fopen( "fifo.log", "a" );

    fprintf(stdout, "Debugging FIFO.\n");
    fprintf(logfile,"Debugging FIFO.\n");

    // Check debug flag first and get pipeline setup info  
    uint32_t pipe_debug_flags = uHTR->pipeline_debug_check( pipe_id ) ;
    std::map<std::string, uint32_t> pipeInfo = uHTR->pipelineInfo( pipe_id ) ;
    uint32_t num_slices   = pipeInfo["DAQ_NUM_SLICES"] ;
    uint32_t num_fibers   = pipeInfo["DAQ_NUM_FIBERS"] ;
    uint32_t num_channels = pipeInfo["DAQ_NUM_CHANNELS"] ;
    uint32_t nsamples     = pipeInfo["DAQ_NSAMPLES"] ;     
    uint32_t npresamples  = pipeInfo["DAQ_NPRESAMPLES"] ;
    uint32_t pipe_length  = pipeInfo["DAQ_PIPELINELENGTH"] ; 
    uint32_t fudge        = pipeInfo["DAQ_PIPE_FUDGE"] ; 
   
    uint32_t num_accept = 1;
    if ((pipe_debug_flags & 2) > 0) num_accept = 10*num_slices;
    else num_accept = tool_readline_int("num_accept ?  ", num_accept );
    fprintf(stdout, "num_accept = %d\n",(int)num_accept);
    fprintf(logfile,"num_accept = %d\n",(int)num_accept);
    
    uint32_t num_slice_accept[100];
    for (uint32_t j=0; j<num_slices; j++) {
      num_slice_accept[j] = num_accept / num_slices;
      if ((num_accept % num_slices) > j) num_slice_accept[j]++;
    }

    uint32_t fifo_hold = 1;
    fprintf(stdout, "fifo_hold = %d\n",(int)fifo_hold);
    fprintf(logfile,"fifo_hold = %d\n",(int)fifo_hold);
    
    uint32_t enable_accept = 1;
    fprintf(stdout, "enable_accept = %d\n",(int)enable_accept);
    fprintf(logfile,"enable_accept = %d\n",(int)enable_accept);

    uHTR->pipeline_test_set( pipe_id, "FIFO", fifo_hold, enable_accept ) ;
    
    std::map<std::string, uint32_t> pipeStat = uHTR->pipeline_status( pipe_id );
    uint32_t fifo_empty = pipeStat["DAQ_FIFO_EMPTY"];
    fprintf(stdout, "fifo_empty = %d\n",(int)fifo_empty);
    fprintf(logfile,"fifo_empty = %d\n",(int)fifo_empty);
    
    // Issue the swL1A signals.
    sleep(1);
    for (uint32_t j=0; j<num_accept; j++) { 
      uHTR->pipeline_SWL1A( pipe_id ) ;
      usleep(10);
      fprintf(stdout, "swL1A[%d] sent.\n",(int)j+1);
      fprintf(logfile,"swL1A[%d] sent.\n",(int)j+1);
    } 
    sleep(1);
    
    pipeStat.clear() ;
    pipeStat = uHTR->pipeline_status( pipe_id );
    uint32_t accept_counter = pipeStat["DAQ_ACCEPT_COUNTER"] ;
    fprintf(stdout, "accept_counter = %d\n",(int)accept_counter);
    fprintf(logfile,"accept_counter = %d\n",(int)accept_counter);
    
    uint32_t state_RamControl = pipeStat["DAQ_RAM_CTRL_STATE"] ;
    fprintf(stdout, "state_RamControl = %d\n",(int)state_RamControl);
    fprintf(logfile,"state_RamControl = %d\n",(int)state_RamControl);

    // Turn the PipelineFIFO state machine on, if it has been disabled.
    if ((pipe_debug_flags & 4096) != 0) {
      pipe_debug_flags ^= 4096;
      uHTR->pipeline_debug( pipe_id, pipe_debug_flags )  ;
      fprintf(stdout, "pipe_debug_flags = %08x\n",(int) pipe_debug_flags);
      fprintf(logfile,"pipe_debug_flags = %08x\n",(int) pipe_debug_flags);
      sleep(1);
    }
    
    // Empty the fifos.
    uint32_t fifo_data;
    uint32_t num_expected;
    uint32_t k;

    num_expected = num_accept*(num_fibers*nsamples*num_channels/2);
    fprintf(stdout, "\nEmpty the fifos: expecting %d words\n",
	    (int)num_expected);
    fprintf(logfile,"\nEmpty the fifos: expecting %d words\n",
	    (int)num_expected);

    pipeStat.clear() ;
    pipeStat = uHTR->pipeline_status( pipe_id );
    fifo_empty = pipeStat["DAQ_FIFO_EMPTY"] ;
    fprintf(stdout, "fifo_empty = %d\n",(int)fifo_empty);
    fprintf(logfile,"fifo_empty = %d\n",(int)fifo_empty);
    
    uint32_t start, ch_id;
    uint32_t zero0, zero1;
    uint32_t gtx_err0, gtx_err1;
    uint32_t cap_err0, cap_err1;
    uint32_t cap_id0, cap_id1;
    uint32_t data0, data1;

    //if (fifo_empty == 0) {
    //  fprintf(stdout, "\n                            ");
    //  fprintf(stdout, "st z1 e1 e1 id dat1 ch z0 e0 e0 id dat0\n");
    //  fprintf(logfile,"\n                            ");
    //  fprintf(logfile,"st z1 e1 e1 id dat1 ch z0 e0 e0 id dat0\n");
    //}


    k = 0;
    while ( fifo_empty == 0 ) {
      k++;
      if ( k > num_expected+10 ) {
	fprintf(stdout, "ERROR: TOO MANY.\n");
	fprintf(logfile,"ERROR: TOO MANY.\n");
	break ;
      }
      
      if (((k-1) % (num_channels*nsamples/2 )) == 0) {
	fprintf(stdout, "\n                            ");
	fprintf(stdout, "st z1 e1 e1 id dat1 ch z0 e0 e0 id dat0\n");
	fprintf(logfile,"\n                            ");
	fprintf(logfile,"st z1 e1 e1 id dat1 ch z0 e0 e0 id dat0\n");
      }

      fifo_data = uHTR->pipeline_Data( pipe_id,  "FIFO" );
      fprintf(stdout, "fifo_data[%3d] = %08x   ",(int)k, (int)fifo_data);
      fprintf(logfile,"fifo_data[%3d] = %08x   ",(int)k, (int)fifo_data);
      
      start    = (fifo_data >> 31) & 0x1;
      zero1    = (fifo_data >> 28) & 0x1;
      gtx_err1 = (fifo_data >> 27) & 0x1;
      cap_err1 = (fifo_data >> 26) & 0x1;
      cap_id1  = (fifo_data >> 24) & 0x3;
      data1    = (fifo_data >> 16) & 0xFF;
      ch_id    = (fifo_data >> 13) & 0x7;
      zero0    = (fifo_data >> 12) & 0x1;
      gtx_err0 = (fifo_data >> 11) & 0x1;
      cap_err0 = (fifo_data >> 10) & 0x1;
      cap_id0  = (fifo_data >>  8) & 0x3;
      data0    = (fifo_data >>  0) & 0xFF;

      fprintf(stdout, " %x  %x  %x  %x ",
	      (int)start, (int)zero1, (int)gtx_err1, (int)cap_err1);
      fprintf(stdout, " %x   %02x ",
	      (int)cap_id1, (int)data1);
      fprintf(stdout, " %x  %x  %x  %x ",
	      (int)ch_id, (int)zero0, (int)gtx_err0, (int)cap_err0);
      fprintf(stdout, " %x   %02x\n",
	      (int)cap_id0, (int)data0);

      fprintf(logfile," %x  %x  %x  %x ",
	      (int)start, (int)zero1, (int)gtx_err1, (int)cap_err1);
      fprintf(logfile," %x   %02x ",
	      (int)cap_id1, (int)data1);
      fprintf(logfile," %x  %x  %x  %x ",
	      (int)ch_id, (int)zero0, (int)gtx_err0, (int)cap_err0);
      fprintf(logfile," %x   %02x\n",
	      (int)cap_id0, (int)data0);

      uHTR->pipeline_advance( pipe_id, 0, "FIFO" ) ; //strobe
      usleep(10);
      pipeStat.clear() ;
      pipeStat = uHTR->pipeline_status( pipe_id );
      fifo_empty = pipeStat["DAQ_FIFO_EMPTY"] ;
    }

    fprintf(stdout, "\n");
    fprintf(logfile,"\n");
    
    pipeStat.clear() ;
    pipeStat = uHTR->pipeline_status( pipe_id );
    fifo_empty = pipeStat["DAQ_FIFO_EMPTY"];
    fprintf(stdout, "fifo_empty = %d\n",(int)fifo_empty);
    fprintf(logfile,"fifo_empty = %d\n",(int)fifo_empty);
    
    if (k != num_expected) {
      fprintf(stdout, "ERROR in FIFO.\n");
      fprintf(logfile,"ERROR in FIFO.\n");
    }
    else {
      fprintf(stdout, "FIFO: OK\n");
      fprintf(logfile,"FIFO: OK\n");
    }
  
    // Print internal values from RAM Control.
    uint32_t raddr, waddr, val;

    std::map<std::string, uint32_t> ctrl_info = uHTR->pipeline_ctrl( pipe_id ) ;
    
    raddr = ctrl_info["DAQ_READ_ADDR"] ;
    fprintf(stdout, "read address = %08x\n",(int)raddr);
    fprintf(logfile,"read address = %08x\n",(int)raddr);
    
    waddr = ctrl_info["DAQ_WRITE_ADDR"] ;
    val = raddr + pipe_length - npresamples + fudge;
    fprintf(stdout, "write address = %04x (vs. %04x)\n",
	    (int)waddr,(int)(val));
    fprintf(logfile,"write address = %04x (vs. %04x)\n",
	    (int)waddr,(int)(val));
    
    val = ctrl_info["DAQ_WRITE_PTR"] ;
    fprintf(stdout, "write ptr = %06x\n",(int)(val));
    fprintf(logfile,"write ptr = %06x\n",(int)(val));
    
    // Print internal counters from slices.

    uint32_t which_slice, count, expected;
    std::map<std::string, uint32_t> cnt_info ;
    for (which_slice=0; which_slice < num_slices; which_slice++) {

      //fprintf(stdout, "which_slice = %d\n",(int)which_slice);
      //fprintf(logfile,"which_slice = %d\n",(int)which_slice);

      uHTR->pipeline_set_slice ( pipe_id, which_slice ) ;
      cnt_info.clear() ;
      cnt_info = uHTR->pipeline_cnt( pipe_id ) ;

      count = cnt_info["DAQ_ERROR_COUNT"] ;
      fprintf(stdout, "slice[%d] data errors = %d\n",
	      (int)which_slice,(int)count);
      fprintf(logfile,"slice[%d] data errors = %d\n",
	      (int)which_slice,(int)count);
	
      count = cnt_info["DAQ_SLICE_COUNTER"] ;
      expected = num_slice_accept[which_slice] * nsamples;
      fprintf(stdout, "slice[%d] counter = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] counter = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
	
      count = cnt_info["DAQ_TR"] ;
      expected = num_slice_accept[which_slice] * (num_fibers*nsamples);
      fprintf(stdout, "slice[%d] transfer states = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] transfer states = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
	
      count = cnt_info["DAQ_WCNT"] ;
      expected = num_slice_accept[which_slice] * (num_fibers*nsamples);
      fprintf(stdout, "slice[%d] fifo writes = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] fifo writes = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      
      count = cnt_info["DAQ_RCNT"] ;
      expected = num_slice_accept[which_slice] * (num_fibers*num_channels*nsamples/2);
      fprintf(stdout, "slice[%d] fifo reads = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
      fprintf(logfile,"slice[%d] fifo reads = %d (expected %d)\n",
	      (int)which_slice,(int)count,(int)expected);
    }	
	
    cnt_info.clear() ;
    cnt_info = uHTR->pipeline_cnt( pipe_id ) ;
    // Print internal counters from fifo.
    count = cnt_info["DAQ_FIFO_TR"];
    expected = num_accept * (num_fibers*nsamples*num_channels/2);
    fprintf(stdout, "fifo transfer states = %d (expected %d)\n",(int)count,(int)expected);
    fprintf(logfile,"fifo transfer states = %d (expected %d)\n",(int)count,(int)expected);

    count = cnt_info["DAQ_FIFO_WCNT"];
    expected = num_accept * (num_fibers*nsamples*num_channels/2);
    fprintf(stdout, "fifo writes = %d (expected %d)\n",(int)count,(int)expected);
    fprintf(logfile,"fifo_writes = %d (expected %d)\n",(int)count,(int)expected);

    enable_accept = 0;
    uHTR->pipeline_test_set( pipe_id,  "FIFO", fifo_hold, enable_accept ) ;
    fprintf(stdout, "enable_accept = %d\n",(int)enable_accept);
    fprintf(logfile,"enable_accept = %d\n",(int)enable_accept);
    
    count = uHTR->pipeline_overflow( pipe_id );
    fprintf(stdout, "pipeline overflow flag = %d\n",(int)count);
    fprintf(logfile,"pipeline overflow flag = %d\n",(int)count);

    fprintf(logfile, "\n------------------\n\n" ) ;
    fclose( logfile ) ; 
    //return;
    exit(0);

}

void uHTRtool::dram_test( uHTR* uHTR ) { 

    int TotalRamSize = 1000000 ;
    int TestSize     =    1000 ;
    int nTest        = TotalRamSize / TestSize ;   

    std::vector<int> mpool;
    for(int i=0; i< nTest ; i++) {
       mpool.push_back( i ) ;
    }

    srand( time(NULL) ) ;  // initial random number generator 
    for(int i= nTest-1; i>1; i--) {
       int k = rand() % i  ;
       std::swap( mpool[k], mpool[i]); //swaps randomly
    }

    bool pass = true ;
    for ( size_t i=0; i< mpool.size() ; i++ ) {
        bool pass_ = uHTR->dram_test( mpool[i] , TestSize ) ;
        if ( pass_ ) pass = false ;
    }
 
    if ( !pass ) printf(" DRAM Test Fail !!! \n" ) ;
    else         printf(" DRAM Test Pass !!! \n" ) ;
     
}
