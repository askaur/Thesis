#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <algorithm>
#include "uhal/ConnectionManager.hpp"
#include "hcal/uhtr/uHTRtool.hh"
#include "hcal/uhtr/uHTRsensors.hh"
#include "hcal/uhtr/LinkFormatTools.hh"
#include "TRandom.h"
#include "toolbox/TimeVal.h"

using namespace hcal::uhtr;

static const double clockFreq=1.6e9; // Hz
static int mId = 0 ;
std::vector<uHTR*> theUHTRs ;

static std::list<std::string> cmdTextQueue;

float edges10[257] = {
        1.58,   4.73,   7.88,   11.0,   14.2,   17.3,   20.5,   23.6,
        26.8,   29.9,   33.1,   36.2,   39.4,   42.5,   45.7,   48.8,
        53.6,   60.1,   66.6,   73.0,   79.5,   86.0,   92.5,   98.9,
        105,    112,    118,    125,    131,    138,    144,    151,
        157,    164,    170,    177,    186,    199,    212,    225,
        238,    251,    264,    277,    289,    302,    315,    328,
        341,    354,    367,    380,    393,    406,    418,    431,
        444,    464,    490,    516,    542,    568,    594,    620,
        569,    594,    619,    645,    670,    695,    720,    745,
        771,    796,    821,    846,    871,    897,    922,    947,
        960,    1010,   1060,   1120,   1170,   1220,   1270,   1320,
        1370,   1430,   1480,   1530,   1580,   1630,   1690,   1740,
        1790,   1840,   1890,   1940,   2020,   2120,   2230,   2330,
        2430,   2540,   2640,   2740,   2850,   2950,   3050,   3150,
        3260,   3360,   3460,   3570,   3670,   3770,   3880,   3980,
        4080,   4240,   4450,   4650,   4860,   5070,   5280,   5490,
        5080,   5280,   5480,   5680,   5880,   6080,   6280,   6480,
        6680,   6890,   7090,   7290,   7490,   7690,   7890,   8090,
        8400,   8810,   9220,   9630,   10000,  10400,  10900,  11300,
        11700,  12100,  12500,  12900,  13300,  13700,  14100,  14500,
        15000,  15400,  15800,  16200,  16800,  17600,  18400,  19300,
        20100,  20900,  21700,  22500,  23400,  24200,  25000,  25800,
        26600,  27500,  28300,  29100,  29900,  30700,  31600,  32400,
        33200,  34400,  36100,  37700,  39400,  41000,  42700,  44300,
        41100,  42700,  44300,  45900,  47600,  49200,  50800,  52500,
        54100,  55700,  57400,  59000,  60600,  62200,  63900,  65500,
        68000,  71300,  74700,  78000,  81400,  84700,  88000,  91400,
        94700,  98100,  101000, 105000, 108000, 111000, 115000, 118000,
        121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
        168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
        230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
        291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000, 398000
};

//**tool_readline functions
//helper functions for Menu to handle user input
//works for _float and _int. Parameters (prompt, defval={"",0} , bool preserve_last_blank = false)
//overloaded so you can leave out the last param or last 2 params

std::string tool_readline(const std::string& prompt, const std::string& defval, bool preserve_last_blank) {
  std::string retval;
  std::string trueprompt(prompt);
  if (!defval.empty()) trueprompt+=" ["+defval+"] ";


  if (!cmdTextQueue.empty()) {
    retval=cmdTextQueue.front();
    printf("%s %s\n",trueprompt.c_str(),retval.c_str());

    if (!retval.empty() && retval[0]=='~') {
      retval.erase(0,1);
      retval.insert(0,getenv("HOME"));
    }
    cmdTextQueue.pop_front();
  } else {
    char* res=readline(trueprompt.c_str());
    retval=std::string(res);
    free(res);
    if (retval.empty()) retval=defval;
    else if (!preserve_last_blank && isspace(retval[retval.size()-1])) retval.erase(retval.size()-1);

    if (!retval.empty() && retval[0]=='~') {
      retval.erase(0,1);
      retval.insert(0,getenv("HOME"));
    }
    
    std::string rvwork;
    bool lastWasWhite=true;
    for (size_t i=0; i<retval.size(); i++) {
      if (isspace(retval[i])) {
	if (!lastWasWhite) {
	  cmdTextQueue.push_back(rvwork);
	  rvwork="";
	}	  
	lastWasWhite=true;
      } else {
	rvwork+=retval[i];
	lastWasWhite=false;
      }
    }
    if (!rvwork.empty()) cmdTextQueue.push_back(rvwork);
    if (!cmdTextQueue.empty()) {
      retval=cmdTextQueue.front();
      cmdTextQueue.pop_front();    
    }
  }
  return retval;
}


std::string tool_readline_nosplit(const std::string& prompt, const std::string& defval) {
  std::string retval;
  std::string trueprompt(prompt);
  if (!defval.empty()) trueprompt+=" ["+defval+"] ";


  char* res=readline(trueprompt.c_str());
  retval=std::string(res);
  free(res);
  
  if (retval.empty()) retval=defval;
  else if (isspace(retval[retval.size()-1])) retval.erase(retval.size()-1);

  return retval;
}


std::string tool_readline(const std::string& prompt, const std::string& defval) { return tool_readline(prompt,defval,false); }
std::string tool_readline(const std::string& prompt) { return tool_readline(prompt,"",false); }

int tool_readline_int(const std::string& prompt) {
  return strtol(tool_readline(prompt).c_str(),0,0);
}

double tool_readline_float(const std::string& prompt) {
  return atof(tool_readline(prompt).c_str());
}

int tool_readline_int(const std::string& prompt,int aval) {
  char buffer[50];
  sprintf(buffer,"%d",aval);
  return strtol(tool_readline(prompt,buffer).c_str(),0,0);
}

static bool gl_useUDP=false;

std::string hcal::uhtr::uhalDeviceString(std::string ipAddress) {
  std::string deviceString;
  if (gl_useUDP) {
    deviceString="ipbusudp-1.3://";
  } else {
    deviceString="chtcp-1.3://localhost:10203?target=";  
  }
  deviceString+=ipAddress;
  deviceString+=":50001";
  return deviceString;
}

//Makes sure that IP has 3 periods and is made up of numbers and .'s
bool check_ip(std::string ip) {
   bool is_ip = true;
   if( std::count(ip.begin(),ip.end(),'.') != 3 ) is_ip = false;
   else if(ip.find_first_not_of("1234567890.")!=std::string::npos) is_ip = false;
   if(!is_ip) std::cout << "Invalid IP  [" << ip  << "]" << std::endl;
   return is_ip;
}


int main(int argc, char* argv[]) {
   //if not enough arguments output usage
  if (argc<2 || (getenv("IPBUS_MAP_PATH")==0 && getenv("HCAL_XDAQ_ROOT")==0)) {
    printf("Usage: uHTRtool [hostname/ip for uHTR] [-u] [-s script]\n");
    printf("       uHTRtool -f [ip list file] [-u]\n\n");
    printf("   -u : use direct UDP rather than connecting via the ControlHub at localhost\n");
    printf("   -s : execute the given script on every uHTR\n");
    printf("   -p : special functions to run in parralel\n");
    printf("   Uses environment variable IPBUS_MAP_PATH to get directory for IPBUS files\n");
    printf("   If IPBUS_MAP_PATH is unset, will use ${HCAL_XDAQ_ROOT}/ipbus/hcal\n");
    printf("   Ip List File Format: 1 IP address per line (# are comments) \n");             
    printf("   Example : \n") ;
    printf("      > uHTRTool.exe  192.168.1.254   \n");             
    //    printf("      > uHTRTool.exe  0 -t dtc   \n");             
    return 1;
  }

  std::vector<std::string> ipV ;

  ipV.clear() ;
  std::string line;
  bool parallel = false;

  uhal::setLogLevelTo( uhal::Error() );
 
 try {

    static int sz = argc ;
    bool skip[sz] ; 
    for ( int i =0 ; i < argc; i++)  {  skip[i] = false ; }
    
    for ( int i = 1 ; i < argc ; i++ ) {
        if ( skip[i] ) continue ;

        if ( strcasecmp( argv[i],"-u")==0 ) {
	  gl_useUDP=true;	  
        }  else if ( strcasecmp(argv[i],"-f") == 0 ) {
                skip[i+1] = true ;
                fstream ipFile( argv[i+1] );

                line.clear() ;

                if (ipFile == 0) {
	           printf("\nUnable to open htr list file '%s'\n",argv[i+1]);
                   return 2;
                }

                while ( getline( ipFile, line) ) {
                   if ( line[0] == '#' ) continue ;
                   // trim the space
                   std::string::iterator end_ipos = std::remove( line.begin(), line.end(), ' ');
                   line.erase(end_ipos, line.end());
                   std::size_t end_cpos = line.find("#");
                   if( end_cpos !=std::string::npos) line.erase(line.begin()+end_cpos, line.end());

                   if(!check_ip(line)) continue;
                   ipV.push_back( line );
                }
                ipFile.close() ;
        }  else if ( strcasecmp(argv[i],"-s") == 0 ) {
                skip[i+1] = true ;
                fstream sFile( argv[i+1] );

                line.clear() ;

                if (sFile == 0) {
	           printf("\nUnable to open script file '%s'\n",argv[i+1]);
                   return 2;
                }

                while ( getline( sFile, line) ) {
		  if ( line[0] == '#' ) continue ;
		  cmdTextQueue.push_back(line);
                }
                sFile.close() ;
        } else if ( strcasecmp(argv[i],"-p") == 0) {
           parallel = true;   
        }
        else {
           std::string ip(argv[i]);
           if(!check_ip(std::string(argv[i]))) break; 
           ipV.push_back( argv[i] ) ;
        }
        
    }
    

     
    std::string mappath;
    if (getenv("IPBUS_MAP_PATH")!=0) { 
        mappath=getenv("IPBUS_MAP_PATH");
    }
    else {
      mappath=getenv("HCAL_XDAQ_ROOT");
      mappath+="/ipbus/hcal";
    }
    
    std::string addressTable_uHTR("file://");
    addressTable_uHTR+=mappath;
    addressTable_uHTR+="/uHTR.xml";

    bool exitMenu = false ;
    uHTRtool* mtool = new uHTRtool( ) ;     
    if(!parallel)
    {
       do {

          do {

             if ( ipV.size()==0) {
                std::cout << "No IP's loaded" << std::endl;
                mId = -1;
                break;
             }
             for ( size_t k=0; k< ipV.size(); k++) {  
                //std::cout<<" ("<<k<<")  IP["<< ipV[k] <<"]  Type:"<< typeV[k] << std::endl ; 
                printf(" ID[%d] IP[%s]  Type: %s \n", (int)k, ipV[k].c_str(),"uHTR" ) ;
             }

             mId = tool_readline_int(" ID of uHTR (-1 for exiting the tool) :: ", mId );

             if( (mId < int(ipV.size()) && mId >= 0) || mId == -1)
                break;

             //mId = tool_readline_int(" ID of mCTR/uHTR ( -1 for exiting the tool ) : ", mId );

             std::cout << "Not a Valid ID\n";
          } while (1);

          if ( mId == -1 ) break ;

          uhal::HwInterface hw(uhal::ConnectionManager::getDevice(ipV[ mId ], uhalDeviceString(ipV[ mId ]), addressTable_uHTR));
          uHTR* theUHTR_ =new uHTR("uHTR",ipV[ mId ], hw);
          mtool->crate_slot_from_ip(theUHTR_,ipV[mId]);
          mtool->uHTR_status( theUHTR_ ) ;
          mtool->RunMenu( theUHTR_  ) ;
          delete theUHTR_ ;

          static std::string RunOrExit = "Exit" ;
          RunOrExit = tool_readline(" Choose a new card(new) or Exit(exit) ? ", RunOrExit );
          if (strncasecmp( RunOrExit.c_str(), "NEW",  1)==0) exitMenu = false ;
          if (strncasecmp( RunOrExit.c_str(), "EXIT", 1)==0) exitMenu = true ;

       } while( !exitMenu ) ;

       delete mtool;
    }
    else
    {
       if ( ipV.size()==0) {
          std::cout << "No IP's loaded" << std::endl;
          return 1;
       }
       std::vector<uHTR*> uHTRs;
       for(std::vector<std::string>::iterator ip = ipV.begin();
             ip != ipV.end();
             ip++)
       {
          //init HwInterface
          uhal::HwInterface hw(uhal::ConnectionManager::getDevice(*ip,uhalDeviceString(*ip),addressTable_uHTR));
          uHTRs.push_back(new uHTR("uHTR",*ip,hw));
          mtool->crate_slot_from_ip(uHTRs.back(),*ip);
       }
       mtool->uHTRs_status( &uHTRs ) ;
       mtool->RunMenu( &uHTRs  ) ;
    }

  } catch (xcept::Exception& e) {
    fprintf(stderr, "Exception!  %s\n",e.message().c_str());
  }
  return 0;
}

uHTRtool::uHTRtool(){

}

void uHTRtool::RunMenu( uHTR* uHTR_ ) {

     typedef Menu<uHTR> uMenu ;
   
     uMenu::Line menu_uflash_lines[] = { 
             uMenu::Line("FPGA", "Toggle which FPGA to target",  &flash_set ),
             uMenu::Line("READ", "Read the flash contents into a file",  &flash_read ),
	     uMenu::Line("PROG", "Reprogram the flash from an MCS file",  &flash_prog ),
	     uMenu::Line("VERIFY", "Verify the flash contents against an existing MCS file",  &flash_verify),
	     uMenu::Line("RELOAD", "Reload the FPGA from FLASH",  &flash_reconfig),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_uflash(&flash_render,menu_uflash_lines);

     uMenu::Line menu_sensor_lines[] = {
       uMenu::Line("MEZZ", "Dump out the information on the mezzanines",&sensor_action),
       uMenu::Line("STATUS", "Status summary",&sensor_action),
       uMenu::Line("I2CTUNE", "Tune the I2C settings",&sensor_action),
       uMenu::Line("READ","Generic SFP/PPOD I2C read",&sensor_ppod),
       uMenu::Line("WRITE","Generic SFP/PPOD I2C write",&sensor_ppod),
       uMenu::Line("MANU","Device manufacturing information",&sensor_ppod),
       uMenu::Line("QUIT","Back to top menu"),
       uMenu::Line()
     };
     uMenu menu_sensor(menu_sensor_lines);

     /*
     uMenu::Line menu_loopback_lines[] = { 
             uMenu::Line("LOAD", "Load SimData from files",  &load_sim_data ),
	     uMenu::Line("RUN",  "Run Loopback ",   &run_loopback ),
	     uMenu::Line("STOP", "Stop Loopback ",  &stop_loopback ),
	     uMenu::Line("QUIT",  "Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_loopback(menu_loopback_lines); 
     */ 

     uMenu::Line menu_fe_ram_lines[] = { 
       uMenu::Line("SETUP","Setup FE RAM parameters and enable/disable use",  &link_fe_rams),
       uMenu::Line("FIBER","Program one fiber's FE RAM with defined patterns",  &link_fe_rams),
       uMenu::Line("ISOPULSE","Program one channel with an energy pulse at one bx and zero for all other channels",  &link_fe_rams),
       uMenu::Line("LOAD","Load the FE RAMs from a file",  &link_fe_rams),
       uMenu::Line("DUMP","Dump the FE RAMs to a file",  &link_fe_rams),
       uMenu::Line("QUIT","Back to top menu"),
       uMenu::Line()
     };
     uMenu menu_fe_ram(menu_fe_ram_lines);

     uMenu::Line menu_link_histo_lines[] = { 
       uMenu::Line("RUN","Start the histogramming process",  &link_histo),
       uMenu::Line("STOP","Stop the histogramming process",  &link_histo),
       uMenu::Line("INTEGRATE","Run an integration for a fixed number of orbits",  &link_histo),
       uMenu::Line("CLEAR","Clear the histograms",  &link_histo),
       uMenu::Line("PRINT","Print out a histogram",  &link_histo),
       uMenu::Line("SAVE","Save histograms to a ROOT file",  &link_histo),
       uMenu::Line("QUIT","Back to top menu"),
       uMenu::Line()
     };
     uMenu menu_link_histo(menu_link_histo_lines);
             
     uMenu::Line menu_link_l1a_capture_lines[] = { 
       uMenu::Line("INIT","Initialise to wait on L1A",  &link_l1a_capture),
       uMenu::Line("RESET","Errase spy for next L1A",  &link_l1a_capture),
       uMenu::Line("READ","Read spy buffer",  &link_l1a_capture),
       uMenu::Line("STATUS","Status",  &link_l1a_capture),
       uMenu::Line("AUTORUN","Captures events and prints",  &link_l1a_capture),
       uMenu::Line("QUIT","Back to top menu"),
       uMenu::Line()
     };
     uMenu menu_link_l1a_capture(menu_link_l1a_capture_lines);
             
     uMenu::Line menu_link_lines[] = { 
	     uMenu::Line("INIT",  "Initialize Links",  &init_link ),
	     uMenu::Line("CDR",   "Reset CDR for Link",  &link_just_reset ),
	     uMenu::Line("STATUS","Link status", &link_status ),
	     uMenu::Line("ERRORS","Error rates check", &link_bad_data_rate ),
	     uMenu::Line("MASK",  "Bad data counter mask", &link_counter_mask ),
	     uMenu::Line("HISTO","Link Histogram", &menu_link_histo),
	     uMenu::Line("SPY","Spy on a link",  &link_spy),
	     uMenu::Line("L1ACAPTURE","Capture spy info on L1A",  &menu_link_l1a_capture),
	     uMenu::Line("FE_RAMS","Set up the use of the FE RAMs", &menu_fe_ram),
	     //uMenu::Line("Histogram","Histogram of Spy Buffer",  &link_spy_histogram),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_link(menu_link_lines);
     

     uMenu::Line menu_clock_lines[] = { 
             uMenu::Line("RATES", "Clock rate summary",  &clk_rates),
             uMenu::Line("SETUP", "Setup the Si53XX from SW",&clk_setup),
             uMenu::Line("EEPROM", "Program the EEPROMs on the CPLD mezzanine",&clk_setup),
             uMenu::Line("LOAD", "Load the Si53XX from EEPROMs",&clk_setup),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_clock(menu_clock_lines);

     uMenu::Line menu_sfp_lines[] = { 
       uMenu::Line("STATUS", "SFP Link Status",  &sfp_actions),
       uMenu::Line("RESETS", "SFP Link resets",  &sfp_actions),
       uMenu::Line("ERRORS", "SFP Link error counter resets",  &sfp_actions),
       uMenu::Line("TX", "SFP Trigger spy Tx",  &sfp_spy_tx),
       uMenu::Line("RX", "SFP Trigger spy Rx",  &sfp_spy_rx),
       uMenu::Line("QUIT","Back to top menu"),
       uMenu::Line()
     };
     uMenu menu_sfp(menu_sfp_lines);

     uMenu::Line menu_orm_lines[] = { 
             uMenu::Line("STATUS", "oRM Status/counters",  &trigger_orm),
             uMenu::Line("START", "Send a START",  &trigger_orm),
             uMenu::Line("STOP", "Send a STOP",  &trigger_orm),
             uMenu::Line("CMD", "Send a command",  &trigger_orm),
	     uMenu::Line("QUIT","Back to top menu"),
     	     uMenu::Line()
     };
     uMenu menu_orm(menu_orm_lines);

     uMenu::Line menu_trig_lines[] = { 
             uMenu::Line("LUTS", "Program and read LUTs",  &trigger_luts),
             uMenu::Line("SPY", "Spy on a trigger link",  &trigger_spy),
             uMenu::Line("SELF", "Self-trigger activities",  &trigger_self),
	     uMenu::Line("ORM","oRM commands (start/stop/command)",&menu_orm),
             uMenu::Line("SFP", "SFP operations", &menu_sfp),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_trig(menu_trig_lines);

     uMenu::Line menu_daq_lines[] = { 
             uMenu::Line("STATUS", "Status of the DAQ path",  &daq_status),
             uMenu::Line("SPY", "Read the DAQ path spy",  &daq_spy),
             uMenu::Line("CTL", "Control the DAQ path",  &daq_ctl),
             uMenu::Line("F2B", "F2B DAQ Link Operations", &daq_f2b),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_daq(menu_daq_lines);

     uMenu::Line menu_lumi_lines[] = { 
             uMenu::Line("SETUP", "Setup the Lumi DAQ", &lumi_setup),
             uMenu::Line("LUTS", "Program and read LUTs", &lumi_luts),
             uMenu::Line("READ", " Lumi read test ", &lumi_read_test),
             uMenu::Line("RESET", " Lumi link reset ", &lumi_link_actions),
             uMenu::Line("LINKSPY", " Lumi link spy ", &lumi_link_actions),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_lumi(&lumi_render,menu_lumi_lines);

     uMenu::Line menu_bhm_lines[] = { 
             uMenu::Line("SETUP", "Setup the BHM DAQ", &bhm_setup),
             uMenu::Line("READ", " BHM read test ", &bhm_read_test),
             uMenu::Line("RESET", " Lumi link reset ", &lumi_link_actions),
             uMenu::Line("LINKSPY", " Lumi link spy ", &lumi_link_actions),
	     uMenu::Line("QUIT","Back to top menu"),
	     uMenu::Line()
     };
     uMenu menu_bhm(&bhm_render,menu_bhm_lines);

     uMenu::Line menu_test_lines[] = {
             uMenu::Line("DDR3",     "DDR3 Read/Write test",   &ddr3_test ) ,
             uMenu::Line("PIPEINIT", "Pipeline Debugger Initialization", &ejf_test ) ,
             uMenu::Line("QUEUE",    "Pipeline Queue test",  &pipeline_queue_test ) , 
             uMenu::Line("CAPTURE",  "Pipeline Capture test", &pipeline_capture_test ) ,
             uMenu::Line("SLICE",    "Pipeline Slice test", &pipeline_slice_test ) , 
             uMenu::Line("FIFO",     "Pipeline FIFO test",  &pipeline_fifo_test ) ,
	     uMenu::Line("WRITEID",  "Write the ID/SN block", &sn_write),
             uMenu::Line("QUIT","Back to top menu") ,
             uMenu::Line()
     };
     uMenu menu_test(menu_test_lines);

     uMenu::Line menu_dtc_lines[] = {
       uMenu::Line("STATUS","Status of the DTC/TTC",  &dtc_Info ) ,
       uMenu::Line("ERR_RESET", "Reset the error counters",&dtc_Control ),
       uMenu::Line("DELAY", "Set the delay of the TTC stream manually",&dtc_Control ),
       uMenu::Line("QUIT","Back to top menu") ,
             uMenu::Line()
     };
     uMenu menu_dtc(menu_dtc_lines);


     uMenu::Line menu_utop_lines[] = { 
       uMenu::Line("STATUS","Status summary of the uHTR card", &uHTR_status),
	     uMenu::Line("LINK","Status and control of frontend links",&menu_link) ,
	     uMenu::Line("DTC","TTC link and information received from AMC13/DTC", &menu_dtc ),
	     uMenu::Line("CLOCK", "Clock module work", &menu_clock  ),
	     uMenu::Line("SENSOR", "I2C sensors and controls", &menu_sensor  ),
	     uMenu::Line("TRIG", "Trigger-path work", &menu_trig  ),
	     uMenu::Line("DAQ", "DAQ-path work", &menu_daq  ),
	     uMenu::Line("TEST", "Functionality tests of uHTR Board", &menu_test),
	     uMenu::Line("FLASH", "Flash programming and readback menu", &menu_uflash  ),
	     uMenu::Line() 
     };
     uMenu menu_utop(menu_utop_lines);
     if (uHTR_->isBHM()) {
       menu_utop.addLine(uMenu::Line("BHM", "Beam Halo Monitor LUMI-DAQ work", &menu_bhm ));
     } else {
       menu_utop.addLine(uMenu::Line("LUMI", "LUMI-DAQ work", &menu_lumi ));
     }


     menu_utop.addLine(uMenu::Line("EXIT", "Exit this tool" ));

     menu_utop.steer( uHTR_ ) ;

}


void uHTRtool::uHTR_status( uHTR* theUHTR_  ) {
  uint8_t patch,major,minor,flavor;
  
  printf(" uHTR baseboard V1.%d   Serial number %d\n",theUHTR_->pcbRevision()&0xFFF,theUHTR_->serialNumber());

  //  bool back_ok, front_ok;
  try {
    theUHTR_->firmwareRevision( true,  flavor,major,minor,patch);
    printf(" Front Firmware revision : %s (%02X) %02x.%02x.%02x \n",uHTR::firmwareFlavorToString(flavor).c_str(),flavor,major,minor,patch);
    //    front_ok=true;
  } catch (...) {
    printf(" *** Problem communicating with front FPGA *** \n");
    //  front_ok=false;
  }

  try {
    theUHTR_->firmwareRevision( false, flavor,major,minor,patch);
    printf(" Back Firmware revision : %s (%02X) %02x.%02x.%02x \n",uHTR::firmwareFlavorToString(flavor).c_str(),flavor,major,minor,patch);
    //    back_ok=true;
  } catch (...) {
    printf(" *** Problem communicating with back FPGA *** \n");
    // back_ok=false;
  }

  clk_rates(theUHTR_);

}

bool uHTRtool::readMCS(std::vector<unsigned char>& image, std::string& mcsFilename)  {

  std::string mcsFile=tool_readline("MCS file: ");
  bool cleanup=false;

  if (mcsFile.find(".xz")!=std::string::npos) {
    std::string tempFile="/tmp/uHTR_";
    tempFile+=getenv("USER");
    tempFile+=".mcs";
    printf("Unpacking XZ %s into %s\n",mcsFile.c_str(),tempFile.c_str());
    std::string cmd="cp "+mcsFile+" "+tempFile+".xz";
    system(cmd.c_str());
    cmd="unxz "+tempFile+".xz";
    system(cmd.c_str());

    mcsFile=tempFile;
    cleanup=true;
  }

  std::vector<std::string> lines;
  FILE* f=fopen(mcsFile.c_str(),"r");
  if (f==0) {
    printf("  \nError opening '%s'\n",mcsFile.c_str());
    return false;
  }
  char buffer[1024];
  while (!feof(f)) {
    buffer[0]=0;
    fgets(buffer,1000,f);

    if (buffer[0]!=0) {
      for (int i=strlen(buffer)-1; i>=0; i--)
	if (isspace(buffer[i])) buffer[i]=0;
	else break;
    }
    if (strlen(buffer)>4)
      lines.push_back(buffer);
  }
  fclose(f);

  if (cleanup) {
    unlink(mcsFile.c_str());
  }
  printf("Line count: %d\n",int(lines.size()));

  hcal::toolbox::unpackMCS(lines,image);

  mcsFilename=mcsFile;
  return true;
}

bool uHTRtool::readMCS(std::vector<unsigned char>& image)  {
  std::string dummy;
  return readMCS(image, dummy);
}


static bool which_fpga_for_flash=false; // back is default

void uHTRtool::flash_render(uHTR* uHTR_ ) { 
  printf(" FLASH target: %s\n",(which_fpga_for_flash)?("FRONT"):("BACK"));
}


void uHTRtool::flash_set( uHTR* uHTR_ ) { 
  which_fpga_for_flash=!which_fpga_for_flash;
}

void uHTRtool::flash_read( uHTR* uHTR_ ) { 
  std::string ofname=tool_readline("Output file name: ");
  FILE* f=fopen(ofname.c_str(),"wt");
  std::vector<unsigned char> imageFlash;

  uHTR_->readFirmware(which_fpga_for_flash, imageFlash,30);

  for (size_t i=0; i<imageFlash.size(); i++) {
    if ((i%32)==0) fprintf(f,"\n%06x  ",int(i));
    fprintf(f,"%02x ",imageFlash[i]);
  }
  fprintf(f,"\n");

  fclose(f);
}

void uHTRtool::flash_prog( uHTR* uHTR_ ) { 
  std::vector<unsigned char> image;
  std::string fname;
  if (readMCS(image,fname)) {
    if (which_fpga_for_flash && strcasestr(fname.c_str(),"BACK")) {
      std::string resp=tool_readline("Programming front FPGA and filename contains 'BACK', are you sure?","n");
      if (resp.empty() || (resp[0]!='Y' && resp[0]!='y')) return;
    }
    if (!which_fpga_for_flash && strcasestr(fname.c_str(),"FRONT")) {
      std::string resp=tool_readline("Programming back FPGA and filename contains 'FRONT', are you sure?","n");
      if (resp.empty() || (resp[0]!='Y' && resp[0]!='y')) return;
    }

    uHTR_->programFirmware(which_fpga_for_flash,image,25);
  }
}

void uHTRtool::flash_verify( uHTR* uHTR_ ) { 
  std::vector<unsigned char> image, imageFlash;
  readMCS(image);

  printf("\nReading flash...\n");
  uHTR_->readFirmware(which_fpga_for_flash,imageFlash,30);

  bool anyProblems=false;

  for (std::vector<unsigned char>::size_type j=0; j<image.size(); j++) {
    if (image[j]!=imageFlash[j]) {
      printf("Mismatch 0x%06x %02x!=%02x (file!=device)\n",(unsigned int)j,image[j],imageFlash[j]);
      anyProblems=true;
    } else if ((j&0xFF)>=0xfc) {
      //      printf("Ok       0x%6x %02x==%02x\n",j,image[j],imageFlash[j]);
    }
  }
  if (!anyProblems) printf("Verify ok!\n");

}


void uHTRtool::flash_reconfig( uHTR* uHTR_ ) { 
  printf("Initiating reload from flash (requires ~10 seconds)...\n");
  uHTR_->reloadFromFlash( which_fpga_for_flash);
  sleep(20);
}

void uHTRtool::dtc_Info( uHTR* uHTR_ ) {

  float f_dtc_rate_40mhz, f_dtc_rate_orbit;
  uint32_t f_dtc_bunch_count, f_dtc_event_number, f_dtc_bc0_error, f_dtc_single_error, f_dtc_double_error, f_dtc_ttc_stream_phase, f_dtc_ttc_stream_phase_locked, f_lumi_nibble, f_lumi_section, f_cms_run, f_lhc_fill;
  float b_dtc_rate_40mhz, b_dtc_rate_orbit;
  uint32_t b_dtc_bunch_count, b_dtc_event_number, b_dtc_bc0_error, b_dtc_single_error, b_dtc_double_error, b_dtc_ttc_stream_phase, b_dtc_ttc_stream_phase_locked, b_lumi_nibble, b_lumi_section, b_cms_run, b_lhc_fill;

    uHTR_->get_dtc_info(true ,f_dtc_rate_40mhz, f_dtc_rate_orbit, f_dtc_bunch_count, f_dtc_event_number, f_dtc_bc0_error, f_dtc_single_error, f_dtc_double_error, f_dtc_ttc_stream_phase, f_dtc_ttc_stream_phase_locked) ; 
    uHTR_->get_dtc_info(false,b_dtc_rate_40mhz, b_dtc_rate_orbit, b_dtc_bunch_count, b_dtc_event_number, b_dtc_bc0_error, b_dtc_single_error, b_dtc_double_error, b_dtc_ttc_stream_phase, b_dtc_ttc_stream_phase_locked) ; 
    uHTR_->get_run_info( true,  f_lumi_nibble, f_lumi_section, f_cms_run, f_lhc_fill);
    uHTR_->get_run_info( false, b_lumi_nibble, b_lumi_section, b_cms_run, b_lhc_fill);
    printf(" ================================================ \n")  ;
    printf("                     Front FPGA:   Back FPGA:   \n");
    printf(" Event Num        :  %11d%13d \n", f_dtc_event_number, b_dtc_event_number ) ; 
    printf(" Lumi Nibble      :  %11d%13d \n", f_lumi_nibble, b_lumi_nibble ) ; 
    printf(" Lumi Section     :  %11d%13d \n", f_lumi_section, b_lumi_section ) ; 
    printf(" CMS Run          :  %11d%13d \n", f_cms_run, b_cms_run ) ; 
    printf(" LHC Fill         :  %11d%13d \n", f_lhc_fill, b_lhc_fill ) ; 
    printf(" RATE_40MHz (MHz) :  %11.2f%13.2f \n", f_dtc_rate_40mhz, b_dtc_rate_40mhz ) ; 
    printf(" RATE_ORBIT (kHz) :  %11.2f%13.2f \n", f_dtc_rate_orbit, b_dtc_rate_orbit ) ; 
    printf(" Bunch Count      :  %11d%13d \n", f_dtc_bunch_count, b_dtc_bunch_count ) ; 
    printf(" BC0 Error        :  %11d%13d \n", f_dtc_bc0_error, b_dtc_bc0_error ) ; 
    printf(" Single Error     :  %11d%13d \n", f_dtc_single_error, b_dtc_single_error ) ; 
    printf(" Double Error     :  %11d%13d \n", f_dtc_double_error, b_dtc_double_error ) ; 
    printf(" TTC Stream Phase :  %11d%13d \n", f_dtc_ttc_stream_phase, b_dtc_ttc_stream_phase ) ; 
    printf(" TTC Stream Phase :  %11s%13s \n", f_dtc_ttc_stream_phase_locked==0 ? "Unlocked":"Locked", b_dtc_ttc_stream_phase_locked==0 ? "Unlocked":"Locked");

}

void uHTRtool::dtc_Control( const std::string& cmd, uHTR* uHTR_ ) {
    static int delay=0;
    static bool override=false;
    static int threshold=16;

    if (cmd=="DELAY") {
   
      int fb= tool_readline_int(" Front(1) or Back(0) FPGA : " );    
      bool isFront = ( fb == 1 ) ? true : false ;
      int od=tool_readline_int(" Override delay? (0/1) ",(override)?(1):(0));
      if (od) {
	delay=tool_readline_int(" Delay for TTC stream (0-3): ",delay);
	override=true;
      } else override=false;
      threshold=tool_readline_int(" Threshold for auto-tune: ",threshold);
      uHTR_->setDTCPhase(isFront,override,delay,threshold);
    } else {
      uHTR_->dtc_reset(false,true);
    }
}

//------------------------------------------------------------------------------------------------
//   Link Status Function
//------------------------------------------------------------------------------------------------- /


void uHTRtool::link_status( uHTR* theUHTR ) {
  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  theUHTR->link_query();
  ::usleep(10000);
  theUHTR->link_query();
  std::vector<uint32_t> gtxErrV = theUHTR->link_gtx_reset_count() ;

  int i;
  printf("\nLink          ");
  for (i=0; i<12; i++) printf("     [%2d]",i);
  printf("\nBadCounter    ");
  for (i=0; i<12; i++) {
      if ( theUHTR->info_link_BadDataCounterOn(i) == 1 ) 
                       printf("      ON " );
      else 
                       printf("       X " );
  }
  printf("\nBad Data      ");
  for (i=0; i<12; i++) printf("%8u ",theUHTR->info_link_nBadData(i));
  printf("\nRollover Count");
  for (i=0; i<12; i++) printf("%8u ",theUHTR->info_link_nBadDataRollover(i) );
  printf("\nBad data rate ");
  for (i=0; i<12; i++) printf("%8.1e ",theUHTR->info_link_BadDataRate(i));
  printf("\nGTX Reset Cnt ");
  for (i=0; i<12; i++) printf("%8d ", gtxErrV[i] );
  printf("\n              ");
  for (i=0; i<12; i++) printf("---------" );
  printf("\nDTC exist     ");
  for (i=0; i<12; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 6) & 1);
  printf("\nOrbAlign exist");
  for (i=0; i<12; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 4) & 1);
  printf("\nAlignment Done");
  for (i=0; i<12; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 1) & 1);

  printf("\n              ");
  for (i=0; i<12; i++) printf("---------" );
  printf("\nReset Done    ");
  for (i=0; i<12; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 0)  );
  printf("\nPLL Lock      ");
  for (i=0; i<12; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 1)  );
  printf("\nByte Aligned  ");
  for (i=0; i<12; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 2)  );
  printf("\n              ");
  for (i=0; i<12; i++) printf("---------" );

  printf("\nAlign occ     ");
  for (i=0; i<12; i++) printf("%8d ",theUHTR->info_link_alignOccupancy(i));
  printf("\nAlign delta   ");
  for (i=0; i<12; i++) printf("%8d ",theUHTR->info_link_alignDelta(i));
  printf("\nOrbitRate(kHz)");
  for (i=0; i<12; i++) {
      float obRate = theUHTR->info_link_orbitRate(i) ;
      if ( obRate != 0.0 )  printf("%8.2e ", obRate );
      else                  printf("       0 " );
  }
  printf("\nBad align     ");
  for (i=0; i<12; i++) printf("%8d ",theUHTR->info_link_nBadAlign(i));
  printf("\n\n");

  printf("\nLink          ");
  for (i=12; i<24; i++) printf("     [%2d]",i);
  printf("\nBadCounter    ");
  for (i=12; i<24; i++) {
      if ( theUHTR->info_link_BadDataCounterOn(i) == 1 ) 
                       printf("      ON " );
      else 
                       printf("       X " );
  }
  printf("\nBad Data      ");
  for (i=12; i<24; i++) printf("%8u ",theUHTR->info_link_nBadData(i));
  printf("\nRollover Count");
  for (i=12; i<24; i++) printf("%8u ",theUHTR->info_link_nBadDataRollover(i) );
  printf("\nBad data rate ");
  for (i=12; i<24; i++) printf("%8.1e ",theUHTR->info_link_BadDataRate(i));
  printf("\nGTX Reset Cnt ");
  for (i=12; i<24; i++) printf("%8d ", gtxErrV[i] );
  printf("\n              ");
  for (i=0; i<12; i++) printf("---------" );
  printf("\nDTC exist     ");
  for (i=12; i<24; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 6) & 1);
  printf("\nOrbAlign exist");
  for (i=12; i<24; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 4) & 1);
  printf("\nAlignment Done");
  for (i=12; i<24; i++) printf("%8d ",( theUHTR->info_link_alignStatus(i) >> 1) & 1);

  printf("\n              ");
  for (i=12; i<24; i++) printf("---------" );
  printf("\nReset Done    ");
  for (i=12; i<24; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 0)  );
  printf("\nPLL Lock      ");
  for (i=12; i<24; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 1)  );
  printf("\nByte Aligned  ");
  for (i=12; i<24; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 2)  );
  printf("\n              ");
  for (i=12; i<24; i++) printf("---------" );

  printf("\nAlign occ     ");
  for (i=12; i<24; i++) printf("%8d ",theUHTR->info_link_alignOccupancy(i));
  printf("\nAlign delta   ");
  for (i=12; i<24; i++) printf("%8d ",theUHTR->info_link_alignDelta(i));
  printf("\nOrbitRate(kHz)");
  for (i=12; i<24; i++) {
      float obRate = theUHTR->info_link_orbitRate(i) ;
      if ( obRate != 0.0 )  printf("%8.2e ", obRate );
      else                  printf("       0 " );
  }
  printf("\nBad align     ");
  for (i=12; i<24; i++) printf("%8d ",theUHTR->info_link_nBadAlign(i));
  printf("\n\n");


}

void uHTRtool::link_bad_data_rate( uHTR* theUHTR ) {
  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  int waitTime = 1000 ;
  waitTime = tool_readline_int("Integration time (ms): ", waitTime );
  int alignTime = 88000 ;
  alignTime = tool_readline_int("Time for alignment done (us): ", alignTime );

  // initialize link first to set counters to zero
  //init_link( theUHTR );
  int doInit = 1 ;
  doInit = tool_readline_int("Initialize Links (1 for yes, 0 for No) : ", doInit );
  if ( doInit > 0 ) init_link( theUHTR );
  //usleep(88000); // wait for alignment...
  usleep( alignTime ); // wait for alignment...
  theUHTR->link_reset_error_counters(); // reset the error counter

  // read out counters after specified time
  printf("Integrating %d ms.\n", waitTime);
  theUHTR->link_query();
  usleep(1000*waitTime);
  theUHTR->link_query();
  /*
  for ( int i=0; i<= waitTime ; i++ ) {
      if ( i% 100 == 0 ) printf(" Updating at %d ms \n", i ) ;
      usleep(1000);
      theUHTR->link_query();
  }
  */
  std::vector<uint32_t> gtxErrV = theUHTR->link_gtx_reset_count() ;


  // normalize bad data rate by number of clock cycles
  // wait time is in ms so a conversion to seconds is necessary
  double normFactor=(clockFreq*waitTime*0.001);

  int i;
  printf("\nLink           ");
  for (i=0; i<12; i++) printf("     [%2d]",i);
  printf("\nBadCounter     ");
  for (i=0; i<12; i++) {
      if ( theUHTR->info_link_BadDataCounterOn(i) == 1 ) 
                       printf("      ON " );
      else 
                       printf("       X " );
  }
  printf("\nBad Data       ");
  for (i=0; i<12; i++) printf("%8u ",theUHTR->info_link_nBadData(i));
  printf("\nBad data  rate ");
  for (i=0; i<12; i++) printf(" %1.1e ",theUHTR->info_link_nBadData(i)/normFactor);
  printf("\nByte Aligned   ");
  for (i=0; i<12; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 2)  );
  printf("\n\n");

  printf("\nLink           ");
  for (i=12; i<24; i++) printf("     [%2d]",i);
  printf("\nBadCounter     ");
  for (i=12; i<24; i++) {
      if ( theUHTR->info_link_BadDataCounterOn(i) == 1 ) 
                       printf("      ON " );
      else 
                       printf("       X " );
  }
  printf("\nBad Data       ");
  for (i=12; i<24; i++) printf("%8d ",theUHTR->info_link_nBadData(i));
  printf("\nBad data rate  ");
  for (i=12; i<24; i++) printf(" %1.1e ",theUHTR->info_link_nBadData(i)/normFactor);
  printf("\nByte Aligned   ");
  for (i=12; i<24; i++) printf("%8d ", theUHTR->info_link_ResetInfo(i, 2)  );
  printf("\n\n");


}


void uHTRtool::clk_rates( uHTR* theUHTR ) {
  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  std::vector<std::pair<double,double> > clocksF, clocksB;
  clocksB=theUHTR->getClockRates(false);
  clocksF=theUHTR->getClockRates(true);

  for (size_t i=0; i<clocksB.size(); i++) {
    if (clocksB[i].first<0.5) printf("  Clock expected at %10.4f kHz : %10.4f kHz (front)  %10.4f kHz (back) \n",clocksB[i].first*1000,clocksF[i].second*1000,clocksB[i].second*1000);
    else printf("  Clock expected at %10.4f MHz : %10.4f MHz (front)  %10.4f MHz (back) \n",clocksB[i].first,clocksF[i].second,clocksB[i].second);
  }
  printf("\n\n");
}

void uHTRtool::clk_setup( const std::string& cmd, uHTR* uHTR ) {

  if (cmd=="SETUP") {
    int standard=tool_readline_int(" Standard 1.6 Gbps setup (1), Standard 4.8 Gbps setup (2) or custom (3)? ",1);
    

    if (standard==1) {
      uHTR->setupClock(1,240);
      uHTR->setupClock(2,320);
    }
    if (standard==2) {
      uHTR->setupClock(1,320);
      uHTR->setupClock(2,240);
    }
    if (standard==3) {

      int which=tool_readline_int("  Which 53XX? (1=schematic 320,2=schematic 240) ");
      int image=tool_readline_int("  Which image? (320/240) ");
  
      uHTR->setupClock(which,image);
    }

    usleep(50000); // 5000 too short; 10000 works
    printf("  Resetting all GTX transceivers after clock setup...");
    uHTR->link_all_reset();

  }
  if (cmd=="EEPROM") {
    int standard=tool_readline_int(" Standard 1.6 Gbps setup (1), Standard 4.8 Gbps setup (2) or custom (3)? ",1);

    if (standard==1) {
      uHTR->setupClock(3,240);
      uHTR->setupClock(4,320);
    }
    if (standard==2) {
      uHTR->setupClock(3,320);
      uHTR->setupClock(4,240);
    }
    if (standard==3) {

      int which=tool_readline_int("  Which 53XX? (1=schematic 320,2=schematic 240) ");
      int image=tool_readline_int("  Which image? (320/240) ");
  
      uHTR->setupClock(which+2,image);
    }
  } 
  if (cmd=="DUMP") {
    //    uHTR
  }
  if (cmd=="LOAD") {
    uHTR->eepromClock(true);
  }

}


void uHTRtool::init_link( uHTR* theUHTR ) {
   if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  static int autoAlign = 1 ; 
  autoAlign = tool_readline_int("  Set Auto_Realign (0:NO, 1:Yes): ", autoAlign );

  static int orbDelay = 92 ; 
  orbDelay = tool_readline_int("  Set Orbit Delay : ", orbDelay );   

  theUHTR->link_init( orbDelay, autoAlign!=0, true, true) ;

}

void uHTRtool::link_just_reset( uHTR* theUHTR ) {

   if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
   }

   std::string ilink = tool_readline(" links for reset ( O ~ 23, separated multiple links by ',' : ) ", ilink );
   char* linkStr = new char[ilink.length()+1];
   std::strcpy ( linkStr, ilink.c_str());

   int rtype = 0 ;
   rtype = tool_readline_int(" Type of link reset , 0: GTX, 1:CDR, 2:Buffer : ", rtype );

   std::vector<int> ilinkV ;
   char* pch;
   pch = strtok ( linkStr," ,");
   while (pch != NULL) {
         if ( pch != NULL )  ilinkV.push_back( strtol(pch, 0, 0) ) ;
         pch = strtok (NULL, " ,");
   }

   printf("\n Reseting : ") ;
   for ( size_t i=0 ; i < ilinkV.size() ; i++ ) printf( " %d,", ilinkV[i]  ) ;
   printf("\n") ;

   if ( ilinkV.size() > 0 ) {
      if ( rtype == 0 ) theUHTR->link_GTX_reset( ilinkV ) ;
      if ( rtype == 1 ) theUHTR->link_CDR_reset( ilinkV ) ;
      if ( rtype == 2 ) theUHTR->link_buffer_reset( ilinkV ) ;
      usleep(8000) ;
   }
   theUHTR->link_init_status() ;

}
void uHTRtool::link_counter_mask( uHTR* theUHTR ) {

   if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  static uint32_t cntMask = 0xFFFFFF ; 
  cntMask = tool_readline_int("  Set Counter Mask for reset (bit 0~23) : ", cntMask );   

  static uint32_t cntThreshold = 0xFFFFFF ; 
  if ( cntMask != 0 ) cntThreshold = tool_readline_int("  Set Error Reset Threshold : ", cntThreshold );   

  theUHTR->link_set_gtx_error_threshold( cntThreshold, cntMask ) ;

}

void uHTRtool::link_histo(const std::string& cmd, uHTR* uHTR) {

  std::string fname;

  if (cmd=="RUN") {
    static bool separateCapIDs=0;
    separateCapIDs=tool_readline_int("  Separate different cap IDs (yes=1, no=0)",separateCapIDs);
    uHTR->link_histo_setSeparateCapIDs(separateCapIDs);
    uHTR->link_histo_start();
  } else if (cmd=="STOP") {
    uHTR->link_histo_stop();
  } else if (cmd=="CLEAR") {
    uHTR->link_histo_stop();
    uHTR->link_histo_clear();
    while (!uHTR->link_histo_clear_done()) {
      printf("   Waiting for clear to be done...\n");
    }
  } else if (cmd=="INTEGRATE") {

    static int orbits=0;
    static bool separateCapIDs=0;
    
    orbits=tool_readline_int("  How many orbits? ",orbits);
    separateCapIDs=tool_readline_int("  Separate different cap IDs (yes=1, no=0)",separateCapIDs);
    uHTR->link_histo_stop();
    uHTR->link_histo_clear();
    uHTR->link_histo_orbit_integrate(10);
    uHTR->link_histo_clear();
    uHTR->link_histo_setSeparateCapIDs(separateCapIDs);
    uHTR->link_histo_orbit_integrate(orbits);
    uHTR->link_histo_clear();
    while (!uHTR->link_histo_clear_done()) {
      printf("   Waiting for clear to be done...\n");
    }


    fname=tool_readline("  Histogram output filename: ");
    ::sleep(1);
    while (!uHTR->link_histo_ready()) {
      printf("  Integrating...\n");
      sleep(5);
    }
    
  } else if (cmd=="PRINT") {
    static int nhisto=0;
    static int channelfiber=0;
    channelfiber=tool_readline_int("  (0) Histogram for a channel or (1) for a fiber ",channelfiber);
    if (channelfiber==0){
      nhisto=tool_readline_int("  Histogram from which channel? ",nhisto);
      std::vector<uint32_t> histo=uHTR->link_histo_read(nhisto);
      for (size_t i=0; i<histo.size(); i++) 
	printf("%03d : %d\n",int(i),histo[i]);
    } else if (channelfiber==1){
      nhisto=tool_readline_int("  Histogram from which fiber? ",nhisto);
      int nchannel=uHTR->n_channels_per_fiber();
      std::vector<std::vector<uint32_t> > histovec;
      histovec.clear() ;
      for (int nc=0; nc<nchannel; nc++){
	std::vector<uint32_t> histo=uHTR->link_histo_read(nhisto*nchannel+nc);
	histovec.push_back(histo);
      }
      for (uint i=0; i<histovec[0].size(); i++) { // 256 values
	printf("%03d : ",i);
	for (uint j=0; j<histovec.size();j++){ // 6 channels
	  printf("%15d ",histovec[j][i]);
	}
	printf("\n");
      }
    }
  } else if (cmd=="SAVE") {
    fname=tool_readline("  Histogram output filename: ");
  } 

  if (cmd=="SAVE" || cmd=="INTEGRATE") {    
    TFile f(fname.c_str(),"RECREATE");

    int xUnit = tool_readline_int(" ADC (0)or fC (1)? ", xUnit);

    for (int nh=0; nh<1000; nh++) {
      std::vector<uint32_t> histo=uHTR->link_histo_read(nh);
      if (histo.empty()) break;
      char histoname[1000];
      sprintf(histoname,"h%d",nh);
      
      if ( xUnit == 0 ) {
         TH1* h=new TH1D(histoname,histoname,256,-0.5,255.5);
         for (size_t i=0; i<256; i++)
       	     h->SetBinContent(i+1,histo[i]);
      }
      if ( xUnit == 1 ) {
         TH1* h=new TH1F(histoname,histoname, 256, edges10 );
         for (size_t i=0; i<256; i++)
	  h->SetBinContent( i+1 , histo[i]  );
      }
    }
    f.Write();
    f.Close();
  }

}


void uHTRtool::link_spy( uHTR* theUHTR ) {
  
  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  static int whichLink=0;
  static int howMany=32;
  static int presamples=1;
  static int offset=0;
  static int raww=0;
  whichLink=tool_readline_int("  Spy which link (0-23): ",whichLink);
  raww=tool_readline_int("  Rchannelfiberaw/immediate (0), Raw/orbit-synched (1), aligned/immediate (2), or aligned/orbit-synced(3) ?",raww);

  if (raww==1 || raww==3) {
    presamples=tool_readline_int("  How many spy presamples? ",presamples);
    offset=tool_readline_int("  Offset in the orbit? ",offset);
  }
  howMany = tool_readline_int("  How many samples? ",howMany);

  static int unpacker = 0 ;
  unpacker = tool_readline_int("  Which data unpacker? ( 0:Regular , 1:TestBeam) ", unpacker );

  theUHTR->link_spySetup(true, false,raww==1 || raww==3,(raww<2),presamples,offset);
  theUHTR->link_spyExecute();
  std::vector<uint32_t> buffer=theUHTR->link_spyRead(whichLink);

  size_t startPt=0;

  if (theUHTR->is1600()) {

    if ((buffer[startPt]&0x1)==0) startPt++;

    printf("\n      N  RAW0  RAW1  Q0  Q1  Q2  C0 C1 C2 \n");
  
    for (size_t i=startPt; i+1<buffer.size() && i<size_t(howMany*2); i+=2) {
      
      int qiea,qieb,qiec,capa,capb,capc;
      hcal::uhtr::unpack_link_16(uint16_t(buffer[i]),uint16_t(buffer[i+1]),capa,capb,capc,qiea,qieb,qiec);
      
      printf("   %04d %05X %05X %3d %3d %3d   %d  %d  %d\n",int(i),buffer[i],buffer[i+1],qiea,qieb,qiec,capa,capb,capc);
    }
  } else {

    printf("\n      N  RAW0  RAW1 \n");

    for (startPt=0; startPt<6; startPt++)
      if ((buffer[startPt]&0xFF)==0xBC && (buffer[startPt+6]&0xFF)==0xBC) break;
  
    
    for (size_t i=startPt; i+5<buffer.size() && i<size_t(howMany*2); i+=6) {
      
          int capid;
	  bool bc0, ce;
	  //uint8_t adcs[6], tdcs[6];
          // this change is only for test beam debug 
	  uint8_t adcs[6], tdcs[11];
	  uint16_t samples[6];
	  for (int j=0; j<6; j++) samples[j]=uint16_t(buffer[i+j]);


          if ( unpacker == 1 ) {

             // this change is only for test beam debug 
	     bool fmtissue=!hcal::uhtr::unpack_link_6chan_debug(samples,capid,bc0,ce,adcs,tdcs);
	     printf("   %04d %05X %05X %s CAP%d %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("ce=0")),((fmtissue)?("(Suspicious data format!)"):""));
	     printf("   %04d %05X %05X CAP: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[3],tdcs[4],tdcs[5],tdcs[7],tdcs[8],tdcs[9]);
	     printf("   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     printf("   Counter [%03d] TDC: %3d %3d Zero %3d %3d\n\n", tdcs[2], tdcs[0], tdcs[1], tdcs[6], tdcs[10] );

          } else {

	     bool fmtissue=!hcal::uhtr::unpack_link_6chan(samples,capid,bc0,ce,adcs,tdcs);
	     printf("   %04d %05X %05X %s CAP%d %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("ce=0")),((fmtissue)?("(Suspicious data format!)"):""));
	     printf("   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     printf("   %04d %05X %05X TDC: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[0],tdcs[1],tdcs[2],tdcs[3],tdcs[4],tdcs[5]);

          }
    }
  }
  printf("\n");
}


void uHTRtool::link_spy_histogram( uHTR* theUHTR ) {
  
  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  int maxBounches = 256;

  static int useDefault = 1 ;
  static int nOrbits=10;
  static std::string hFileName = "h_" ; 
  useDefault = tool_readline_int("  Use default setup ? ( 1: Yes 0: detail setups) ", useDefault );
  nOrbits    = tool_readline_int("  How many orbits? ",nOrbits );
  hFileName  = tool_readline("  Histogram File Name Header ? ",hFileName );

  static int howMany= maxBounches ;
  static int presamples=1;
  static int offset=0;
  static int link=12;
  static int needPlots=0;
  static int nbins= 128;
  if ( useDefault ==0 ) {
     presamples=tool_readline_int("  How many spy presamples? ",presamples);
     offset    =tool_readline_int("  Offset in the orbit? ",offset);
     howMany   =tool_readline_int("  How many bounches? ",howMany);
     link      =tool_readline_int("  How many fiber link? ",link );
     needPlots =tool_readline_int("  Output Plots (0:NO, 1:Yes) " , needPlots );
     nbins = tool_readline_int("  Number of ADC bin (between 0 ~ 128) ? ", nbins );
  }

  static std::string FolderName = hFileName.substr(0, hFileName.size() - 5 ) ; 
  if ( needPlots == 1 ) FolderName =tool_readline("  Folder to save plots? ",FolderName ); 
  if ( howMany > maxBounches ) howMany = maxBounches ;

  static int xMin = 0;
  static int xMax = 128;
  //xMin = tool_readline_int(" Min ADC value ? ", xMin );
  //xMax = tool_readline_int(" Max ADC value ? ", xMax );

  time_t t_ini, t_end ;
  time( &t_ini) ;
  
  std::string t_str = ctime (&t_ini) ;
  std::string day_str = ( t_str.substr(8,1) == " ") ? t_str.substr(9,1) : t_str.substr(8,2) ;  
  if ( day_str.size() == 1 ) day_str.insert(0,"0");
  hFileName += t_str.substr(4,3) ;  // month
  hFileName += day_str ; 
  hFileName += "_" ;
  hFileName += t_str.substr(11,2) ; // hour 
  hFileName += t_str.substr(14,2) ; // minute 
  hFileName += ".root" ;  
  printf ( "File Name: %s  \n", hFileName.c_str() );

  TFile* hfile = new TFile( hFileName.c_str() ,"RECREATE");

  const int nCh = link*3 ;
  TH2D* hBX_ADCs[ nCh ] ; 

  TString hNames[nCh] ;
  for ( int i = 0; i < nCh; i++) {
      char hName[8] ;
      sprintf(hName,"hBX_ADC%d", i) ;
      hNames[i] = hName ;
      hBX_ADCs[i] = new TH2D( hName, " BX(x) vs ADC(Y) ", howMany, 0, howMany, nbins, xMin, xMax );
  }

  std::vector<uint32_t> buffer;
  theUHTR->link_spySetup(true,true,true,false,presamples,offset);
  int k = 0 ;
  do {
     k++ ;
     theUHTR->link_spyExecute();
     if ( k%1000 == 0 && k > 0  ) std::cout<<" ==== Finished "<< k  <<" orbits ==== " << std::endl ;
     for ( int l = 0 ; l < link; l++) {
         buffer.clear() ;  
         buffer=theUHTR->link_spyRead(l);
         //std::cout<<" buffer size : "<< buffer.size() << std::endl ;
         for (size_t i=0; i<buffer.size() && i<size_t(howMany); i++) {
             if ( i == 0 ) continue ;
	     int qiea=(buffer[i]>>25)&0x7F;
	     int qieb=(buffer[i]>>17)&0x7F;
	     int qiec=(buffer[i]>>9)&0x7F;
	     hBX_ADCs[3*l+0]->Fill( i,qiea ) ;
	     hBX_ADCs[3*l+1]->Fill( i,qieb ) ;
	     hBX_ADCs[3*l+2]->Fill( i,qiec ) ;
         }
     }
  } while ( k < nOrbits ) ;

  if ( needPlots == 1 ) {
     TCanvas *c1 = new TCanvas("c1","c1",800,600);
     c1->SetFillColor(10);

     TString hfolder = FolderName ;
     gSystem->mkdir( hfolder );
     gSystem->cd( hfolder ) ;
     gStyle->SetPalette(1);
     gStyle->SetNumberContours(8);
     gStyle->SetStatX(0.9);
     for ( int i=0; i< nCh; i++) {
         c1->cd();
         hBX_ADCs[i]->Draw("COLZ");
	 c1->Update();
	 c1->Print( hNames[i]+".png") ;
     }
     gSystem->cd("../"); 
     delete c1 ;
  }

  for ( int i = 0; i < nCh; i++) {
      hBX_ADCs[i]->Write();
  }
  hfile->Close();  
 
  time( &t_end) ;
  double dT = difftime( t_end, t_ini) ;
  printf(" dT of %d orbit = %.3f", nOrbits, dT ) ;

}
void uHTRtool::link_l1a_capture(const std::string& cmd,  uHTR* uHTR ) {

  static int unpacker = 0 ;
  if (cmd=="INIT") {
    static int howMany=5;
    howMany=tool_readline_int(" How many BXs to store after trigger?", howMany);

    uHTR->link_spyCaptureSetup(howMany);
    //    if (uHTR->is1600()){printf("This application is currently only supported for 4800 FW");}
  }
  else if (cmd=="RESET"){
    uHTR->link_spySetStart(0x5555);
  }
  else if (cmd=="READ"){

    int triggerword=0;
    static int whichLink=0;
    static int howManybefore=5;

    whichLink=tool_readline_int("  Spy which link (0-23): ",whichLink);
    howManybefore=tool_readline_int(" How many BXs readout before trigger?", howManybefore);
    unpacker = tool_readline_int("  Which data unpacker? ( 0:Regular , 1:TestBeam) ", unpacker );

    uint32_t writeptr_L1A=uHTR->link_spy_writeptr_L1A_read();
    std::vector<uint32_t> buffer;
    if (writeptr_L1A==0xDEADBEEF) {
      printf("Still waiting for trigger \n");
    }
    else{
      //                                linkID     read_addr_in_spyRam     nBx_bf_trig   nBx_af_trig, trigWord  
      buffer=uHTR->link_spyCaptureRead(whichLink, 0x80200030+writeptr_L1A, howManybefore,   10000,   triggerword);
      size_t startPt=0;
      if(uHTR->is1600()){
	if ((buffer[startPt]&0x1)==0) startPt++;
      
	printf("\n      N  RAW0  RAW1  Q0  Q1  Q2  C0 C1 C2 \n");
      
	for (size_t i=startPt; i+1<buffer.size(); i+=2) {
	  
	  int qiea,qieb,qiec,capa,capb,capc;
	  hcal::uhtr::unpack_link_16(uint16_t(buffer[i]),uint16_t(buffer[i+1]),capa,capb,capc,qiea,qieb,qiec);
	  
	  printf("   %04d %05X %05X %3d %3d %3d   %d  %d  %d  %s\n",int(i),buffer[i],buffer[i+1],qiea,qieb,qiec,capa,capb,capc,((triggerword>=i&&triggerword<i+2)?("(<--- Triggered BX!)"):("")));
	  
	}
      }
      else{
	
	printf("\n      N  RAW0  RAW1 \n");
	
	for (startPt=0; startPt<6; startPt++)
	  if ((buffer[startPt]&0xFF)==0xBC && (buffer[startPt+6]&0xFF)==0xBC) break;
	
	for (size_t i=startPt; i+5<buffer.size(); i+=6) {

	  int capid;
	  bool bc0, ce;
	  uint8_t adcs[6], tdcs[11];
	  uint16_t samples[6];
          std::string isTrigger = "" ;
	  for (int j=0; j<6; j++) samples[j]=uint16_t(buffer[i+j]);

          if ( unpacker == 1 ) {
             // this change is only for test beam debug 
	     bool fmtissue=!hcal::uhtr::unpack_link_6chan_debug(samples,capid,bc0,ce,adcs,tdcs);
             isTrigger = (triggerword>=i&&triggerword<i+6) ? ("(<--- Triggered BX!)"):("") ;

	     printf("   %04d %05X %05X %s CAP%d %s %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("")),((fmtissue)?("(Suspicious data format!)"):("")), isTrigger.c_str() );
	     printf("   %04d %05X %05X CAP: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[3],tdcs[4],tdcs[5],tdcs[7],tdcs[8],tdcs[9]);
	     printf("   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     printf("   Counter [%03d] TDC: %3d %3d \n\n", tdcs[2], tdcs[0], tdcs[1]  );
	  } else {
	     bool fmtissue=!hcal::uhtr::unpack_link_6chan(samples,capid,bc0,ce,adcs,tdcs);
             isTrigger = (triggerword>=i&&triggerword<i+6) ? ("(<--- Triggered BX!)"):("") ;
   	     printf("   %04d %05X %05X %s CAP%d %s %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("")),((fmtissue)?("(Suspicious data format!)"):("")), isTrigger.c_str() );
	     printf("   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     printf("   %04d %05X %05X TDC: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[0],tdcs[1],tdcs[2],tdcs[3],tdcs[4],tdcs[5]);
          }

	}
	
      }
    }
  }
  else if (cmd=="STATUS"){
    uint32_t writeptr_L1A=uHTR->link_spy_writeptr_L1A_read();
    if (writeptr_L1A==0xDEADBEEF) printf("Waiting for trigger\n");
    else printf("Triggered on address %08X of spy buffer\n", writeptr_L1A);

    uint32_t pulse=uHTR->link_accept_pulse_rate();
    printf("Accpted pulse rate is: %d\n",pulse);

    uint32_t spystatus=uHTR->link_spyState();
    if (spystatus==0) printf("spy status is 0:Idle");
    if (spystatus==1) printf("spy status is 1:Init Erase");
    if (spystatus==2) printf("spy status is 2:Erase");
    if (spystatus==3) printf("spy status is 3:Wait for IPbus");
    if (spystatus==4) printf("spy status is 4:Wait for 1A");
    if (spystatus==5) printf("spy status is 5:Wait for Spy Pulse");
    if (spystatus==6) printf("spy status is 6:Init Spy");
    if (spystatus==7) printf("spy status is 7:Spy");
  }
  else if (cmd=="AUTORUN"){
    static int howManyEvents=1;
    static uint howManyBefore=5;
    static uint howManyAfter=5;
    howManyEvents=tool_readline_int(" How many events?", howManyEvents);
    howManyBefore=tool_readline_int(" How many BXs to store before trigger?", howManyBefore);
    howManyAfter=tool_readline_int(" How many BXs to store after trigger?", howManyAfter);
    unpacker = tool_readline_int("  Which data unpacker? ( 0:Regular , 1:TestBeam) ", unpacker );

    // Get read out fibers
    std::string ilink = tool_readline(" Read out fibers ( O ~ 23, separated multiple links by ',' : ) ", ilink );
    char* linkStr = new char[ilink.length()+1];
    std::strcpy ( linkStr, ilink.c_str());

    std::vector<int> ilinkV ;
    char* pch;
    pch = strtok ( linkStr," ,");
    while (pch != NULL) {
          if ( pch != NULL )  ilinkV.push_back( strtol(pch, 0, 0) ) ;
          pch = strtok (NULL, " ,");
    }

    std::string fname=tool_readline("Filename: ");
    FILE *fi =fopen(fname.c_str(), "w");
    if (fi==0) { printf("Unable to open '%s' for writing.\n\n",fname.c_str()); return; }
    if (uHTR->is1600()) { printf("This mode is only available for FW4800"); return; }

    uHTR->link_spyCaptureSetup(howManyAfter);
    for (int i=0; i<howManyEvents; i++){
      uHTR->link_spySetStart(0x5555);
      fprintf(fi,"---------------------------------------------------------------------------\n");
      fprintf(fi,"--------------------------- START EVENT %8d---------------------------\n",i);
      fprintf(fi,"---------------------------------------------------------------------------\n");
      uint32_t writeptr_L1A=uHTR->link_spy_writeptr_L1A_read();
      while(writeptr_L1A==0xDEADBEEF) {
	usleep(1000);
	writeptr_L1A=uHTR->link_spy_writeptr_L1A_read();
      }
      printf("Triggered on address %08X of spy buffer\n", writeptr_L1A);
      for (size_t f=0; f< ilinkV.size() ; f++){
	int triggerword=0;
	int fiber = ilinkV[f] ;
	std::vector<uint32_t> buffer=uHTR->link_spyCaptureRead(fiber, 0x80200030+writeptr_L1A, howManyBefore, 682, triggerword);
	size_t startPt=0;

	fprintf(fi,"\n      N  RAW0  RAW1             Reading Fiber:%d\n",fiber);
	
	for (startPt=0; startPt<6; startPt++)
	  if ((buffer[startPt]&0xFF)==0xBC && (buffer[startPt+6]&0xFF)==0xBC) break;
	
	for (size_t i=startPt; i+5<buffer.size(); i+=6) {
	  int capid;
	  bool bc0, ce ;
	  uint8_t adcs[6], tdcs[6];
	  uint16_t samples[6];
          std::string isTrigger = "" ;
	  for (int j=0; j<6; j++) samples[j]=uint16_t(buffer[i+j]);
	
          //
          if ( unpacker == 1 ) {
             // this change is only for test beam debug 
	     bool fmtissue=!hcal::uhtr::unpack_link_6chan_debug(samples,capid,bc0,ce,adcs,tdcs);
             isTrigger = (triggerword>=i&&triggerword<i+6) ? ("(<--- Triggered BX!)"):("") ;

	     fprintf(fi, "   %04d %05X %05X %s CAP%d %s %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("")),((fmtissue)?("(Suspicious data format!)"):("")), isTrigger.c_str() );
	     fprintf(fi, "   %04d %05X %05X CAP: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[3],tdcs[4],tdcs[5],tdcs[7],tdcs[8],tdcs[9]);
	     fprintf(fi, "   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     fprintf(fi, "   Counter [%03d] TDC: %3d %3d \n\n", tdcs[2], tdcs[0], tdcs[1]  );
	  } else {
	     bool fmtissue=!hcal::uhtr::unpack_link_6chan(samples,capid,bc0,ce,adcs,tdcs);
             isTrigger = (triggerword>=i&&triggerword<i+6) ? ("(<--- Triggered BX!)"):("") ;
   	     fprintf(fi, "   %04d %05X %05X %s CAP%d %s %s %s\n",int(i),buffer[i],buffer[i+1],((bc0)?("BX0"):("")),capid,((ce)?("CAPID-mismatch"):("")),((fmtissue)?("(Suspicious data format!)"):("")), isTrigger.c_str() );
	     fprintf(fi, "   %04d %05X %05X ADC: %3d %3d %3d %3d %3d %3d\n",int(i+2),buffer[i+2],buffer[i+3],adcs[0],adcs[1],adcs[2],adcs[3],adcs[4],adcs[5]);
	     fprintf(fi, "   %04d %05X %05X TDC: %3d %3d %3d %3d %3d %3d\n",int(i+4),buffer[i+4],buffer[i+5],tdcs[0],tdcs[1],tdcs[2],tdcs[3],tdcs[4],tdcs[5]);
          }
	  

	}
      }
      printf("read out event %d of %d \n",i,howManyEvents);
    }
    fclose(fi);
  }

}

void uHTRtool::trigger_luts( uHTR* uHTR ) {

  int cmd;

  std::vector<uint32_t> lut;
  int group, index;
  std::string fname;
  FILE* fptr;

  int n_addr_bits, n_data_bits, n_luts;
  uint32_t base_address,  stride;
  bool last_group;

  do {
    printf("\n============================================== \n");
    printf(" (1) Read one LUT      (2) Read all LUTs to file\n");
    printf(" (3) Set 1-1 LUT       (4) Set LUTs from file\n");
    printf(" (5) Zero LUTs \n");
    printf("\n (  Anything else will just return to the original menu )\n\n");
    
    cmd=tool_readline_int(" Selection : ",-1);
    if (cmd<0) return;

    group=tool_readline_int(" Which group of LUTs? (0-3) : ", 0);
    
    uHTR->trigger_query_lut_type(group,n_addr_bits, n_data_bits, n_luts, base_address,  stride, last_group);
    
    switch (cmd) {
    case 1 : 
      index=tool_readline_int("  LUT index : ");
      uHTR->trigger_read_lut(group, index, lut);
      printf("   INPUT    OUTPUT  \n");
      printf("   -----    --------\n");
      for (size_t i=0; i<lut.size(); i++)
	printf("    %3d    %d\n",int(i),lut[i]);
      printf("\n");
      break;
    case 2 : 
      fname=tool_readline("  Output filename : ");
      fptr=fopen(fname.c_str(),"w");
      if (fptr==0) {
	printf(" !!! Unable to open '%s' for writing.\n\n",fname.c_str());
      } else {
	fprintf(fptr,"# index  input output \n");
	fprintf(fptr,"#---------------------\n");
	for (index=0; index<n_luts; index++) {
	  uHTR->trigger_read_lut(group, index, lut);
	  for (size_t i=0; i<lut.size(); i++) 
	    fprintf(fptr," %7d  %4d %6d\n",index,int(i),lut[i]);
	}
	fclose(fptr);
      }
      break;
    case 3 : 
      lut.clear();

      for (uint32_t i=0; i<uint32_t(1<<n_addr_bits); i++) {
	if (i>=uint32_t(1<<n_data_bits)) lut.push_back((1<<n_data_bits)-1);
	else lut.push_back(i);
      }

      printf("Pick index for 1-1 LUTs\n");
      index=tool_readline_int("  Choose index (-1 for all) : ");
      if (index==-1) {
	for (index=0; index<n_luts; index++) {
	  uHTR->trigger_program_lut(group, index,lut);
	}
      } else {
	uHTR->trigger_program_lut(group, index,lut);
      }
      break;
    case 4 : 
      fname=tool_readline("  Input filename : ");
      fptr=fopen(fname.c_str(),"r");
      if (fptr==0) {
	printf(" !!! Unable to open '%s'.\n\n",fname.c_str());
      } else {
	char buffer[80];
	for (index=0; index<n_luts; index++) {
	  int aindex, ain, aout;
	  lut=std::vector<uint32_t>(1<<n_addr_bits,0);
	  rewind(fptr);
	  while (!feof(fptr)) {
	    fgets(buffer,75,fptr);
	    if (strchr(buffer,'#')==0) continue;
	    sscanf(buffer," %d %d %d ",&aindex, &ain, &aout);
	    if (aindex!=index || ain<0 || ain>=int(lut.size())) continue;
	    lut[ain]=aout;
	  }
	  uHTR->trigger_program_lut(group,index,lut);
	}
	fclose(fptr);
      }
      break;
    case 5 : 
      lut.clear();

      for (uint32_t i=0; i<uint32_t(1<<n_addr_bits); i++) lut.push_back(0);

      printf("Pick index for zero LUTs\n");
      index=tool_readline_int("  Choose index (-1 for all) : ");
      if (index==-1) {
	for (index=0; index<n_luts; index++) {
	  uHTR->trigger_program_lut(group, index,lut);
	}
      } else {
	uHTR->trigger_program_lut(group, index,lut);
      }
      break;

    default: break;
    }
  } while (cmd>0);
}

void uHTRtool::trigger_spy( uHTR* uHTR ) {

  static int orbit_delay=0;
  static int ilink=0;
  orbit_delay=tool_readline_int("Orbit delay : ",orbit_delay);
  ilink=tool_readline_int("Trigger link : ",ilink);

  uHTR->trigger_spy_execute(0x3FFF,orbit_delay);
  
  std::vector<uint32_t> buffer;

  uHTR->trigger_spy_read(ilink,buffer);
  for (size_t i=0; i<64; i++) {
    printf("%3d %08x\n",int(i),buffer[i]);
  }

  std::string of=tool_readline("Save to filename (blank for no save) : ");
  if (!of.empty()) {
    FILE* f=fopen(of.c_str(),"wt");
    if (f==0) printf("Unable to open '%s' for writing\n\n",of.c_str());
    else {
      for (size_t i=0; i<buffer.size(); i++) {
	fprintf(f,"%5d %08x\n",int(i),buffer[i]);
      }   
      fclose(f);
    }
  }
}

void uHTRtool::trigger_self( uHTR* uHTR ) {

  int cmd;

  std::vector<uint32_t> tool;
  int index, value;
  std::string fname;
  FILE* fptr;

  do {
    printf("\n============================================== \n");
    printf(" (1) Set thresholds    (2) Read thresholds\n");
    printf(" (3) Get rates         (4) Write histogram for bit\n");
    printf("\n (  Anything else will just return to the original menu )\n\n");
    
    cmd=tool_readline_int(" Selection : ",-1);
    if (cmd<0) return;

    switch (cmd) {
    case 1 : 
      index=tool_readline_int("  Which threshold (-1 for all) : ",-1);
      value=tool_readline_int("  Set to what value? ");

      tool=uHTR->trigger_self_get_thresholds();
      if (index==-1) 
	for (size_t i=0; i<tool.size(); i++) tool[i]=value;
      else tool[index]=value;

      uHTR->trigger_self_set_thresholds(tool);

      printf("\n");
      break;
    case 2 : 
      tool=uHTR->trigger_self_get_thresholds();

      printf(" I     VALUE\n------------------------\n");
      for (size_t i=0; i<tool.size(); i++) printf("   %d    %d\n",(int)i,int(tool[i]));
      break;
    case 4 :

      index=tool_readline_int("  Trigger bit: ");
      fname=tool_readline("  Output filename : ");
      fptr=fopen(fname.c_str(),"w");
      if (fptr==0) {
	printf(" !!! Unable to open '%s' for writing.\n\n",fname.c_str());
      } else {
	fprintf(fptr,"# BX    N \n");
	fprintf(fptr,"#---------------------\n");
	tool=uHTR->trigger_self_read_histogram(index);
	
	for (size_t i=0; i<tool.size(); i++) 
	    fprintf(fptr," %4d %6d\n",int(i),tool[i]);
      
	fclose(fptr);
      }
      break;
    default: break;
    }
  } while (cmd>0);
}

void uHTRtool::daq_spy(uHTR* uHTR ) {
  std::vector<uint32_t> buffer;
  uHTR->daq_spy_read(buffer,true); // one-time-spy
  
  if (buffer.empty()) {
    printf(" No DAQ words found\n\n");
    return;
  }
  
  static const int FL_TRIG = 4;
  static const int FL_QIE8 = 5;

  size_t ntp=0;
  int flavor=-1;
  for (size_t i=0; i<buffer.size(); i++) {
    printf("  %04x %x %04x ",uint32_t(i), (buffer[i]>>16)&0xF,buffer[i]&0xFFFF);
    uint16_t thisword=buffer[i]&0xFFFF;
    // interpretation!
    switch (i) {
    case (1) : 
      printf("Event number %d (0x%x)",(buffer[0]&0xFF)+(thisword<<8),(buffer[0]&0xFF)+(thisword<<8));
      break;
    case (3) :
      printf("Orbit number %d, submodule number %d (0x%x)",(thisword>>11)&0x1F,thisword&0x7FF,thisword&0x7FF); 
      break;
    case (4):
      printf("Format %d, BCN %d (0x%x)",(thisword>>12)&0xF,thisword&0xFFF,thisword&0xFFF);
      break;
    case (5):
      printf("Presamples %d, TP words %d ",(thisword>>3)&0x1F,(thisword>>8)&0xFF);
      ntp=(thisword>>8)&0xff;
      break;
    case (6):
      printf("Unsuppressed %d, compact mode %d, firmware rev 0x%x",(thisword>>15)&0x1,(thisword>>14)&0x1,thisword&0xFFF);
      break;
    case (7):
      printf("Flavor %d, Pipeline length %d ",(thisword>>8)&0x7F,thisword&0xFF);      
      break;

    default: 
      if (i>=buffer.size()-4) {
	if (i==buffer.size()-4) printf("NS %d WC %d",thisword>>11, thisword&0x7ff);
      } else if (i>=8+ntp) {
	if (thisword&0x8000) {
	  flavor=(thisword>>12)&0x7;
	  if (flavor==FL_TRIG) printf("TP Header (Flavor %d) Error bits %d %d Id %d (0x%02x) ",flavor,(thisword>>11)&0x1,(thisword>>10)&0x1,(thisword&0xFF),thisword&0xFF);
	  else printf("Channel Header Flavor %d Error bits %d %d Capid0 %d Fiber %d FiberChan %d ",flavor,(thisword>>11)&0x1,(thisword>>10)&0x1,(thisword>>8)&0x3,(thisword>>2)&0x1F,(thisword>>0)&0x3); 
	} else if (flavor==FL_TRIG) printf(" %d %d %s %s",(thisword>>8)&0x1,(thisword&0xFF),(thisword&0x2000)?("OK"):("ERR"),(thisword&0x4000)?("SOI"):(""));
	else if (flavor==FL_QIE8) printf("QIE %d %d",thisword&0x7F,(thisword>>8)&0x7f);
	//	else printf("QIE %d %d",thisword&0x7F,(thisword>>8)&0x7f);
      }
    }
    
    printf("\n");
  }
  
  
  
}

void uHTRtool::daq_status(uHTR* uHTR ) {


  std::vector<uint32_t> f2b_status, f2b_errors;
  std::vector<uint64_t> f2b_words;
  std::vector<double> f2b_words_rates, f2b_error_rates;

  uint32_t orn, evn, occupancy, peak_occupancy, n_tp_samples,n_tp_pre;
  uint32_t moduleid, bcOffset,nsamples, npre, pipeline, zsmask;
  bool nzs,enabled, mandp, sumbytwo, tp_zs, tp_zs_soi;
  std::vector<uint32_t> zsThresholds;
    	   
  uHTR->daq_path_setup_query(moduleid,bcOffset, nsamples, npre, n_tp_samples, n_tp_pre, pipeline,enabled);
  uHTR->daq_zs_setup_query(nzs, mandp, zsmask, sumbytwo, zsThresholds, tp_zs, tp_zs_soi);
  uHTR->daq_query() ; 
  uHTR->daq_f2b_links_query();

  uHTR->info_evn_orn(evn,orn);
  occupancy=uHTR->daq_path_headerOccupancy();
  peak_occupancy=uHTR->daq_path_headerOccupancy(true);
  uHTR->daq_f2b_links_status(f2b_status, f2b_errors, f2b_error_rates);
  uHTR->daq_f2b_links_data(f2b_words, f2b_words_rates);


  printf("\n");
  printf("DAQ F2B Links\n");
  for (size_t i=0; i<f2b_status.size(); i++)
    printf("  %lu : Status = %x  Errors = %u  (%e Hz)  Words = %lu (%e Hz)\n",
	   i, f2b_status[i],f2b_errors[i],f2b_error_rates[i],f2b_words[i],f2b_words_rates[i]);
  printf("DAQ Path : %s   %s  %s  %s\n",enabled?("ENABLED"):("DISABLED"),nzs?("NZS"):("")
	 ,mandp?("ZS(M&P)"):(""),sumbytwo?("ZS(SumBy2"):("ZS(per sample)"));
  printf("   Last EVN: %d   OrN : %d  Header Occupancy : %d  (Peak : %d)\n",evn,orn,occupancy,peak_occupancy);
  printf("   Samples: %d   Presamples : %d  Pipeline Length : %d\n",nsamples,npre,pipeline);
  printf("   ZS Mask (one means ignore) : 0x%4x \n",zsmask);
  printf("   TP Samples: %d   TP Presamples : %d  \n",n_tp_samples,n_tp_pre);
  printf("   TP ZS : %s %s \n",(tp_zs)?("TP_ZS"):("TP_NZS"),(tp_zs_soi)?("TP_ZS_SOIONLY"):(""));
  printf("   Module Id : %d (0x%x)   BC Offset : %d\n",moduleid,moduleid,bcOffset);
  printf("\n");
}

void uHTRtool::daq_ctl(uHTR* uHTR ) {
  char dummy[128];
  int cmd;
  do {
    daq_status(uHTR);

    printf("\n");  
    printf(" (1) Set Module Id  (2) Set BC Offset       (3) Set NSAMPLES\n");
    printf(" (4) Set PRESAMPLES (5) Set Pipeline Length (6) Set ZS Mask \n");
    printf(" (7) Enable DAQ Path (toggle)   (8) Reset DAQ Path \n");
    printf(" (9) Toggle NZS    (10) Toggle Mark-And-Pass ZS    (11) Toggle ZS Sum-By-Two\n");
    printf(" (12) Dump ZS Thresholds   (13) Edit ZS Thresholds   (14) Uniform ZS\n");
    printf(" (15) Set TP PRESAMPLES  (16) Set TP SAMPLES\n");
    printf(" (17) Toggle ZS for TP (18) Toggle SOI-only for TP\n");
    printf("\n (  Anything else will just return to the original menu )\n\n");
  
    cmd=tool_readline_int(" Selection : ",-1);
    
    uint32_t moduleid, bcOffset,nsamples, npre, pipeline, zsmask, n_tp_samples,n_tp_pre;
    bool nzs,enabled, mandp, sumbytwo, tp_zs, tp_zs_soi;
    std::vector<uint32_t> zsThresholds;

    static const int LIVE_CHANNELS=12*3*2; // appropriate for 1.6 Gbps uHTR
    	   
    uHTR->daq_path_setup_query(moduleid,bcOffset, nsamples, npre, n_tp_samples,n_tp_pre, pipeline,enabled);
    uHTR->daq_zs_setup_query(nzs, mandp, zsmask, sumbytwo, zsThresholds, tp_zs, tp_zs_soi);
    
    switch (cmd) {
    case 1 : moduleid=tool_readline_int("  New module id : ",moduleid);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 2 : bcOffset=tool_readline_int("  New bc offset : ",bcOffset);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 3 : nsamples=tool_readline_int("  New nsamples : ",nsamples);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 4 : npre=tool_readline_int("  New presamples : ",npre);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 5 : pipeline=tool_readline_int("  New pipeline length : ",pipeline);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 6 : zsmask=tool_readline_int("  New ZS Mask (bits set mean ignore-for-ZS) : ",zsmask);
      uHTR->daq_zs_setup(nzs,mandp,zsmask,sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); break;  
    case 7 : uHTR->daq_path_enable(!enabled); break;
    case 8 : uHTR->daq_path_reset(); break;
    case 9 : uHTR->daq_zs_setup(!nzs,mandp,zsmask,sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); break;
    case 10 : uHTR->daq_zs_setup(nzs,!mandp,zsmask,sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); break;
    case 11 : uHTR->daq_zs_setup(nzs,mandp,zsmask,!sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); break;
    case 12 : 
      printf(" Channel  ZS Threshold\n -------   ------------\n");
      for (int i=0; i<LIVE_CHANNELS; i++) 
	printf("  %6d  %5d\n",i,zsThresholds[i]);
      break;
    case 13 : 
      for (int i=0; i<LIVE_CHANNELS; i++) {
	snprintf(dummy,120,"Threshold for channel %d : ",i);
	zsThresholds[i]=tool_readline_int(dummy,zsThresholds[i]);
      }
      uHTR->daq_zs_setup(nzs,mandp,zsmask,!sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); 
      break;
    case 14 : {
      uint32_t threshold=tool_readline_int("Uniform ZS Threshold : ",0);
      for (int i=0; i<LIVE_CHANNELS; i++) {
	zsThresholds[i]=threshold;
      }
      uHTR->daq_zs_setup(nzs,mandp,zsmask,!sumbytwo,zsThresholds,  tp_zs, tp_zs_soi); 
    }
      break;
    case 15 :  n_tp_pre=tool_readline_int("  TP presamples : ",n_tp_pre);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;

    case 16 :  n_tp_samples=tool_readline_int("  TP samples : ",n_tp_samples);
      uHTR->daq_path_setup(moduleid,bcOffset,nsamples, npre, n_tp_samples,n_tp_pre, pipeline); break;
    case 17 : uHTR->daq_zs_setup(nzs,mandp,zsmask,sumbytwo,zsThresholds,  !tp_zs, tp_zs_soi); break;
    case 18 : uHTR->daq_zs_setup(nzs,mandp,zsmask,sumbytwo,zsThresholds,  tp_zs, !tp_zs_soi); break;
    default: break;
    }
  } while (cmd>0);

    
}

void uHTRtool::daq_f2b( uHTR* uHTR ) {
  printf("\n\n (1) Reset  (2) Spy Link 0  (3) Spy Link 1 (4) Spy Link 2\n");
  int action=tool_readline_int("Selection: ");
  if (action==1) uHTR->daq_f2b_links_reset();
  if (action==2) {
    std::vector<uint32_t> words=uHTR->daq_f2b_links_spy(0);
    for (size_t i=0; i<words.size(); i++)
      printf(" %02x : %08x \n",int(i),words[i]);
  }
  if (action==3) {
    std::vector<uint32_t> words=uHTR->daq_f2b_links_spy(1);
    for (size_t i=0; i<words.size(); i++)
      printf(" %02x : %08x \n",int(i),words[i]);
  }
  if (action==4) {
    std::vector<uint32_t> words=uHTR->daq_f2b_links_spy(2);
    for (size_t i=0; i<words.size(); i++)
      printf(" %02x : %08x \n",int(i),words[i]);
  }
}

void uHTRtool::lumi_setup( uHTR* uHTR ) {
  int cmd;
  int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
  std::vector<bool> mask_lhc, mask_cms;
  
  do {
    printf("\n============================================== \n");
    lumi_render(uHTR);

    printf("\n");  
    printf(" (1) Set LHC orbit integration period\n");
    printf(" (2) Set CMS orbit integration period\n");
    printf(" (3) Set LHC threshold     (4) Set CMS Threshold 1\n");
    printf(" (5) Set CMS threshold 2   (6) Adjust LHC mask bit\n");
    printf(" (7) Adjust CMS mask bit\n");
    printf("\n (  Anything else will just return to the original menu )\n\n");
  
    cmd=tool_readline_int(" Selection : ",-1);

    uHTR->lumi_read_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );

    switch (cmd) {
    case 1 : 
      n_lhc_orb=tool_readline_int("  New LHC integration period in orbits : ", n_lhc_orb);
      uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      break;
    case 2 : 
      n_cms_orb=tool_readline_int("  New CMS integration period in orbits : ", n_cms_orb);
      uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      break;
    case 3 : 
      lhc_threshold=tool_readline_int("  New LHC hit threshold : ",lhc_threshold);
      uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      break;
    case 4 : 
      cms1_threshold=tool_readline_int("  New CMS hit threshold 1 : ",cms1_threshold);
      uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      break;
    case 5 : 
      cms2_threshold=tool_readline_int("  New CMS hit threshold 2 : ",cms2_threshold);
      uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      break;
    case 6 :
      {
	printf("Inverting mask bit for LHC...\n");
	int ifiber=tool_readline_int("  Fiber (-1 for all): ");
	if (ifiber==-1) {
	  for (size_t index=0; index<mask_cms.size(); index++)
	    mask_lhc[index]=!mask_lhc[index];
	} else {
	  int ic=tool_readline_int("  Channel-in-Fiber ( -1 for all): ");
	  if (ic==-1) {
	    for (ic=0; ic<uHTR->n_channels_per_fiber(); ic++) {
	      int index=ifiber*uHTR->n_channels_per_fiber()+ic;
	      mask_lhc[index]=!mask_lhc[index];
	    }
	  } else {
	    int index=ifiber*uHTR->n_channels_per_fiber()+ic;
	    mask_lhc[index]=!mask_lhc[index];
	  }
	}
	uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      }
      break;
    case 7 :
      {
	printf("Inverting mask bit for CMS...\n");
	int ifiber=tool_readline_int("  Fiber (-1 for all): ");
	if (ifiber==-1) {
	  for (size_t index=0; index<mask_cms.size(); index++)
	    mask_cms[index]=!mask_cms[index];
	} else {
	  int ic=tool_readline_int("  Channel-in-Fiber ( -1 for all): ");
	  if (ic==-1) {
	    for (ic=0; ic<uHTR->n_channels_per_fiber(); ic++) {
	      int index=ifiber*uHTR->n_channels_per_fiber()+ic;
	      mask_cms[index]=!mask_cms[index];
	    }
	  } else {
	    int index=ifiber*uHTR->n_channels_per_fiber()+ic;
	    mask_cms[index]=!mask_cms[index];
	  }
	}
	uHTR->lumi_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
      }
      break;
    default: break;
    }
  } while (cmd>0);
}

void uHTRtool::lumi_render( uHTR* uHTR ) {
  int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
  std::vector<bool> mask_lhc, mask_cms;
  
  uHTR->lumi_read_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
  float lbr,tbr;
  uint32_t link_state, errors;
  uHTR->lumi_link_status(link_state, lbr, tbr, errors) ; 

  printf(" Link Status ( 0x3f is happy ) : 0x%02x   Errors=%d\n", link_state,errors ) ;
  printf("      Lumi BC0 Rate : %.3f kHz   Local trigger BC0 Rate : %.3f kHz\n", lbr*1000, tbr*1000);
  printf(" LHC: Orbits = %d   Threshold = %d  \n",n_lhc_orb, lhc_threshold);
  printf("     Mask:");

  for (int ic=0; ic<uHTR->n_channels_per_fiber(); ic++) {
    for (int ifiber=0; ifiber<24; ifiber++)
      printf("%d ",(mask_lhc[ifiber*uHTR->n_channels_per_fiber()+ic])?(1):(0));
    printf(" (fiber-channel=%d)\n          ",ic);
  }

  printf("\n CMS: Orbits = %d   Threshold 1 = %d  Threshold 2 = %d\n",n_cms_orb, cms1_threshold, cms2_threshold);
  printf("     Mask:");

  for (int ic=0; ic<uHTR->n_channels_per_fiber(); ic++) {
    for (int ifiber=0; ifiber<24; ifiber++)
      printf("%d ",(mask_cms[ifiber*uHTR->n_channels_per_fiber()+ic])?(1):(0));
    printf(" (fiber-channel=%d)\n          ",ic);
  }
  printf("\n");
}

void uHTRtool::lumi_luts( uHTR* uHTR ) {
  int cmd;

  std::vector<uint16_t> lut;
  int ifiber, ichan;
  std::string fname;
  FILE* fptr;

  do {
    printf("\n============================================== \n");
    printf(" (1) Read one LUT      (2) Read all LUTs to file\n");
    printf(" (3) Set 1-1 LUT       (4) Set LUTs from file\n");
    printf("\n (  Anything else will just return to the original menu )\n\n");
  
    cmd=tool_readline_int(" Selection : ",-1);

    switch (cmd) {
    case 1 : 
      ifiber=tool_readline_int("  Choose fiber : ");
      ichan=tool_readline_int("  Choose channel-in-fiber : ");
      uHTR->lumi_read_lut(ifiber,ichan,lut);
      printf("   QIE     LUMI ET\n");
      printf("   ----    --------\n");
      for (size_t i=0; i<lut.size(); i++)
	printf("    %3d    %d\n",int(i),lut[i]);
      printf("\n");
      break;
    case 2 : 
      fname=tool_readline("  Output filename : ");
      fptr=fopen(fname.c_str(),"w");
      if (fptr==0) {
	printf(" !!! Unable to open '%s' for writing.\n\n",fname.c_str());
      } else {
	fprintf(fptr,"# ifiber  ichan  qie  lumiET\n");
	fprintf(fptr,"#---------------------------\n");
	for (ifiber=0; ifiber<24; ifiber++) 
	  for (ichan=0; ichan<uHTR->n_channels_per_fiber(); ichan++) {
	    uHTR->lumi_read_lut(ifiber,ichan,lut);
	    for (size_t i=0; i<lut.size(); i++) 
	      fprintf(fptr," %7d  %5d %4d %6d\n",ifiber,ichan,int(i),lut[i]);
	  }
	fclose(fptr);
      }
      break;
    case 3 : 
      lut.clear();
      for (uint16_t i=0; i<128; i++) lut.push_back(i);

      printf("Pick fiber for 1-1 LUTs\n");
      ifiber=tool_readline_int("  Choose fiber (-1 for all) : ");
      if (ifiber==-1) {
	for (ifiber=0; ifiber<24; ifiber++) {
	  for (ichan=0; ichan<uHTR->n_channels_per_fiber(); ichan++) {
	    uHTR->lumi_program_lut(ifiber,ichan,lut);
	  }
	} 
      } else {
	ichan=tool_readline_int("  Choose channel-in-fiber : ");
	uHTR->lumi_program_lut(ifiber,ichan,lut);
      }
      break;
    case 4 : 
      fname=tool_readline("  Input filename : ");
      fptr=fopen(fname.c_str(),"r");
      if (fptr==0) {
	printf(" !!! Unable to open '%s'.\n\n",fname.c_str());
      } else {
	char buffer[80];
	for (ifiber=0; ifiber<24; ifiber++) 
	  for (ichan=0; ichan<uHTR->n_channels_per_fiber(); ichan++) {
	    int aqie, alumi, afiber, achan;
	    lut=std::vector<uint16_t>(128,0);
	    rewind(fptr);
	    while (!feof(fptr)) {
	      fgets(buffer,75,fptr);
	      if (index(buffer,'#')==0) continue;
	      sscanf(buffer," %d %d %d %d ",&afiber, &achan, &aqie, &alumi);
	      if (afiber!=ifiber || achan!=ichan) continue;
	      lut[aqie]=alumi;
	    }
	    uHTR->lumi_program_lut(ifiber,ichan,lut);
	  }
	fclose(fptr);
      }
      break;
    default: break;
    }
  } while (cmd>0);
}

void uHTRtool::lumi_link_actions(const std::string& actions, uHTR* uHTR ) {
  if (actions=="RESET") {
    printf("\n   Resetting lumi link...\n");
    uHTR->lumi_link_reset() ; 
  }
  if (actions=="LINKSPY") {
    std::vector<uint32_t> words=uHTR->lumi_link_spy();
    for (size_t i=0; i<words.size(); i++)
      printf(" %02x : %08x \n",int(i),words[i]);
  }

}


void uHTRtool::trigger_orm(const std::string& cmd, uHTR* uHTR) {

  if (cmd=="START") uHTR->orm_control(true,false,false,0);
  if (cmd=="STOP") uHTR->orm_control(false,true,false,0);
  if (cmd=="CMD") {
    uint32_t cmd=tool_readline_int("Command code: ",0);
    uHTR->orm_control(false,false,true,cmd);
  }
  if (cmd=="STATUS") {
    uint32_t q, start_c, stop_c, cmd_c;
    uHTR->get_orm_info(q,start_c,stop_c,cmd_c);
    printf("\n\n  STARTS = %d  STOPS = %d COMMANDS=%d QUEUED= %d\n",start_c,stop_c, cmd_c, q);
  }

}

void uHTRtool::sfp_spy_tx( uHTR* uHTR ) {
  
  static int whichLink=0;
  whichLink   = tool_readline_int("  Which SFP link (0,1): "    , whichLink );
  static int orbit_delay=0;
  orbit_delay=tool_readline_int("Orbit delay : ",orbit_delay);

  uHTR->trigger_spy_execute(0x3FFF,orbit_delay);
  
  std::vector<uint32_t> buffer;

  uHTR->trigger_spy_read(whichLink+12,buffer);
  for (size_t i=0; i<64; i++) {
    printf("%3d %08x\n",int(i),buffer[i]);
  }

}

void display_sfp_status( uHTR* theUHTR, int which_sfp) {

  uint32_t st_0, ctr_0, err_0;
  float clk_0;
  uint32_t st_1, ctr_1, err_1;
  float clk_1;

  theUHTR->sfp_query(ctr_0, st_0, err_0, clk_0,ctr_1, st_1, err_1, clk_1);

  
  printf(" =========== SFP%d ================================================================================\n", which_sfp ) ;
  printf("      FPGA  [Tx] RESET, RESET_Done, PLLLKDET | [Rx] RESET, RESET_Done, PLLLKDET, ALIGN , NotInTable,  Clk, ERRORS \n") ;
  if ( which_sfp == 0 || which_sfp == 2 )
    printf("SFP0: (%1x,%1x)      %3x       %3x        %3x          %3x        %3x        %3x     %3x       %3x     %5.3f    %d\n" , 
	   (st_0>>12)&1, (ctr_0>>24)&1, ctr_0 & 1,  (st_0>>1)&1, (st_0>>3)&1, (ctr_0>>1)&1, st_0&1 , (st_0>>2)&1, (st_0>>9)&1,  (st_0>>4)&1 , clk_0, err_0  ) ;
  
  if ( which_sfp == 1 || which_sfp == 2 )
    printf("SFP1: (%1x,%1x)      %3x       %3x        %3x          %3x        %3x        %3x     %3x       %3x     %5.3f    %d\n" , 
	   (st_1>>12)&1, (ctr_1>>24)&1, ctr_1 & 1,  (st_1>>1)&1, (st_1>>3)&1, (ctr_1>>1)&1, st_1&1 , (st_1>>2)&1, (st_1>>9)&1,  (st_0>>4)&1 , clk_1, err_1 ) ;
  
  
}

void uHTRtool::sfp_spy_rx( uHTR* theUHTR ) {
  
  if (theUHTR==0) {
     printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  static int whichLink=0;
  static int presamples=1;
  static int offset=0;
  static int nowait=0;
  static int align_delay = 0 ;
  whichLink   = tool_readline_int("  Spy which link (0,1, 2=0&1): "    ,whichLink);
  nowait      = tool_readline_int("  Immediate(1), Wait until next orbit (0) ?", nowait);
  align_delay = tool_readline_int("  Align delay ?", align_delay );

  if ( nowait == 0 ) {
    presamples=tool_readline_int("  How many spy presamples? ",presamples);
    offset=tool_readline_int("  Offset in the orbit? ",offset);
  }

  printf(" *** setup sfp spy \n") ; 
  theUHTR->sfp_spySetup(true,true, nowait , presamples, offset, align_delay );
  display_sfp_status(theUHTR, whichLink);

  printf(" *** execute sfp spy \n") ;
  theUHTR->sfp_spyExecute();
  display_sfp_status(theUHTR, whichLink);
  printf(" *** read sfp spy \n") ;
  std::vector<uint32_t> buffer=theUHTR->sfp_spyRead(whichLink);
  display_sfp_status(theUHTR, whichLink);
  printf("  display sfp spy = %d \n", (int)buffer.size() ) ;

  theUHTR->sfp_spySetupQuery( );

  for ( int i=0; i< 64 ; i++ ) {
      printf("(%2d) %08X \n", i, buffer[i] ) ;
  }

}

void uHTRtool::sfp_actions( const std::string& cmd, uHTR* theUHTR ) {
  if (cmd=="STATUS") 
    display_sfp_status(theUHTR,2);  
  else if (cmd=="RESET") {
    theUHTR->sfp_resets(true, true);
    display_sfp_status(theUHTR,2);  
  } else if (cmd=="ERRORS") {
    theUHTR->sfp_resets(false, true);
    display_sfp_status(theUHTR,2);  
  }
}


void uHTRtool::lumi_read_test( uHTR* uHTR ) {
     TFile* hfile=0;
     std::string hFileStr=tool_readline(" ROOT histogram file (blank for no output) : ");
     int nRead = 100 ;
     nRead = tool_readline_int(" Number of read : ", nRead );
     if (!hFileStr.empty()) hfile=new TFile(hFileStr.c_str(), "RECREATE");

     std::map<std::string, bool > aM ;
     std::map<std::string, bool > oM ;
     uHTR::LumiHistogram hV ; 
     for (int i=0; i < nRead ;  i++ ) { 
         sleep(1) ;
         aM.clear();
         oM.clear();
	 uHTR->lumi_histo_status( aM, oM ) ;
         for ( int j=0; uHTR::LUMI_HISTONAMES[j] != 0 ; j++   ) {
             printf("%10s = Available : %u ",  uHTR::LUMI_HISTONAMES[j], aM[uHTR::LUMI_HISTONAMES[j]] ) ; 
             printf(" Overflow : %u ",  oM[uHTR::LUMI_HISTONAMES[j]] ) ; 

             
             if ( aM[uHTR::LUMI_HISTONAMES[j]] ) {

                 uHTR->lumi_read_histogram( uHTR::LUMI_HISTONAMES[j] , hV );
                 printf("Bin 5 values: | %d | %x |", hV.h[5], hV.h[5]  ) ;
                 if ( hfile != 0 ) { 
                    
                    char hNameStr[128] ;
                    sprintf( hNameStr,  "h_%s_%d", uHTR::LUMI_HISTONAMES[j] , hV.orb_init ) ;
                    TH1D*  h1    = new TH1D( hNameStr, hNameStr , 3564, 0, 3564 ) ;

                    for ( size_t k=1; k<= hV.h.size(); k++ ) {
                        h1->Fill( k, hV.h[k-1] ) ;      
                    }

                    h1->Write();
                 }
             } 
	     printf("\n");
         }
         printf(" ============================================== \n") ;
     }
     if ( hfile != 0 ) {
         hfile->Close();  
         delete hfile ;
     }
}



void uHTRtool::sensor_action(const std::string& cmd, uHTR* uHTR) {
  using namespace hcal::uhtr;
  uHTRsensors* sensors=uHTR->sensors();


  if (cmd=="MEZZ") {

    uHTRsensors::MezzIdStruct id;
    
    static const uHTRsensors::SubBus sblist[]={
      uHTRsensors::mac_id_prom,
      uHTRsensors::pm_1VA,uHTRsensors::pm_1VB,uHTRsensors::pm_3_3V, 
      uHTRsensors::apm_1_8V,uHTRsensors::apm_2_5V, 
      uHTRsensors::jtag_mezz,
      uHTRsensors::flash_mezz,
      uHTRsensors::cpld_mezz,
      uHTRsensors::Unknown};
    
    for (int i=0; sblist[i]!=uHTRsensors::Unknown; i++) {
      uHTRsensors::SubBus sb=sblist[i];
      
      id=sensors->readId(sb);
      
      printf("  %s -- %02x:%02x:%02x:%02x:%02x:%02x (S/N = %d)\n",sensors->SB2str(sb).c_str(),
	     id.macid[0],id.macid[1],id.macid[2],id.macid[3],id.macid[4],id.macid[5],id.serial_number[0]+id.serial_number[1]*256);
      if (id.data_format_version==0xFF) printf("     Invalid ID block observed\n");
      else if (id.data_format_version!=1) {
	printf("     Id-Version = %d   Mezz Type = %d / %d \n",id.data_format_version,id.mezz_type_code,id.mezz_subtype_code);
      }
      else {
	printf("     Id-Version = %d   Mezz Type = %d / %d (%s)\n",id.data_format_version,id.mezz_type_code,id.mezz_subtype_code,id.mezz_type);
	printf("     Tested %s at '%s' by '%s' using release %s\n",id.manu_date,id.manu_site,id.manu_tester,id.test_release);
      }
    }  
  } else if (cmd=="STATUS") {

    double temp,v;

    // FPGA sensor info
    for (int i=14; i<=15; i++) {
      double v1,v2;
      hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
      
      sensors->readSensorsFPGA(sb,temp,v1,v2);
      
      printf("  %-12s -- %.2f V   %.2f V   %.1f degC \n",sensors->SB2str(sb).c_str(),
	     v1,v2,temp);
      
    }  
    
    // Power mezz sensor info
    for (int i=1; i<=5; i++) {
      hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
      
      sensors->setMuxes(sb);
      
      sensors->readSensorsPM(temp,v);
      
      printf("  %-12s -- %.2f V            %.1f degC \n",sensors->SB2str(sb).c_str(),
	     v,temp);
    }  
    
    // PPOD sensor info
    for (int i=6; i<=8; i++) {
      double v1,v2, v25, v33;
      if (i != 99) {
	hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
	
	sensors->setMuxes(sb);
	
	sensors->readSensorsPPOD(temp,v1,v2);
	sensors->readPPOD_ADC(v25,v33);
	
	printf("  %-12s -- %.2f V (%.2f)   %.2f V (%.2f)  %.1f degC \n",
	       sensors->SB2str(sb).c_str(),  v2,v25, v1,v33, temp);
      }
    }  
    
    // PPOD sensor info
    for (int i=9; i<=10; i++) {
      if (i != 99) {
	hcal::uhtr::uHTRsensors::SubBus sb=hcal::uhtr::uHTRsensors::SubBus(i);
	
	sensors->setMuxes(sb);
	
	sensors->readSensorsSFP(temp,v);
	
	printf("  %-12s -- %.2f V                %.1f degC \n",
	       sensors->SB2str(sb).c_str(),  v, temp);
      }
    }   
  } else if (cmd=="I2CTUNE") {
    int presc=tool_readline_int("I2C prescaler value?",sensors->i2cPrescale());
    sensors->setI2Cprescale(uint32_t(presc));
  }
}
					 
void uHTRtool::sensor_ppod(const std::string& cmd, uHTR* uHTR) {
  using namespace hcal::uhtr;

  static int which=0;
  
  which=tool_readline_int("Which device (0=RX0,1=RX1,2=TX,3=SFP0,4=SFP1)? ",which);
  uHTRsensors::SubBus sb;
  uHTRsensors::Target tg;
  if (which==0) {
    sb=uHTRsensors::ppod_rx0;
    tg=uHTRsensors::ppod;
  } else if (which==1) {
    sb=uHTRsensors::ppod_rx1;
    tg=uHTRsensors::ppod;
  } else if (which==2) {
    sb=uHTRsensors::ppod_tx;
    tg=uHTRsensors::ppod;
  } else if (which==3) {
    sb=uHTRsensors::sfp0;
    tg=uHTRsensors::sfp;
  } else if (which==4) {
    sb=uHTRsensors::sfp1;
    tg=uHTRsensors::sfp;
  }  else return;

  hcal::uhtr::uHTRsensors* sensors=uHTR->sensors();
  sensors->setMuxes(sb);

  if (cmd=="MANU") {
    if (tg==uHTRsensors::sfp) {
      char buffer[100];

      std::vector<uint8_t> cmdv, res;

      cmdv.push_back(20);
      res=sensors->read(tg,16,cmdv);
      for (int i=0; i<16; i++)
	buffer[i]=res[i];
      buffer[16]=0;

      printf("   Manufacturer: %s\n",buffer);

      cmdv[0]=40;
      res=sensors->read(tg,16,cmdv);
      for (int i=0; i<16; i++)
	buffer[i]=res[i];
      buffer[16]=0;

      printf("   Device type: %s\n",buffer);

      cmdv[0]=68;
      res=sensors->read(tg,16,cmdv);
      for (int i=0; i<16; i++)
	buffer[i]=res[i];
      buffer[16]=0;

      printf("   Serial number: %s\n",buffer);
      
      cmdv[0]=84;
      res=sensors->read(tg,8,cmdv);
      for (int i=0; i<8; i++)
	buffer[i]=res[i];
      buffer[8]=0;

      printf("   Date of manufacture : %s\n",buffer);

    }
    return;
  }

  static int addr=0;
  static int len=1;
  addr=tool_readline_int("  Address?",addr);

  if (cmd=="READ") {
    len=tool_readline_int("  Length?",len);
  
    std::vector<uint8_t> cmdv, res;
    cmdv.push_back(addr);
    res=sensors->read(tg,len,cmdv);
    unsigned char asText[120];
    asText[0]=0;
    
    printf("  %02x : ", addr);
    for (int i=0; i<len && i<int(res.size()); i++) {
      printf(" %02x ",res[i]);
      asText[i]=res[i];
      asText[i+1]=0;
    }
    printf("  \"%s\"\n",asText);
  } else if (cmd=="WRITE") {
    std::vector<uint8_t> cmdv;
    cmdv.push_back(addr);
    int val;
    val=tool_readline_int(" value to write :",-1);
    if (val>=0) cmdv.push_back(val);
    for (size_t i=0;i<cmdv.size(); i++)
      printf("%02x ",cmdv[i]);
    printf("\n");
    sensors->write(tg,cmdv);
  }

}

void uHTRtool::link_fe_rams(const std::string& cmd, uHTR* uHTR) {
  if (cmd=="SETUP") {
    bool enabled, ttc_aligned;
    uint32_t delay;
    uHTR->fe_ram_setup_query(enabled,ttc_aligned,delay);
    enabled=tool_readline_int("Enable? ",enabled);
    ttc_aligned=tool_readline_int("TTC aligned? ",ttc_aligned);
    delay=tool_readline_int("Delay between TTC orbit and pattern start ",delay);
    uHTR->fe_ram_setup(enabled,ttc_aligned,delay);
  } else if (cmd=="FIBER") {
    int ifiber=tool_readline_int("Fiber: ");

    std::vector<uint32_t> samples; // insert capid rotation
    for (size_t i=0; i<2048*4/2; i++) {
      samples.push_back(0x3 | ((i&0x3)<<3) | ((i&0x3)<<5)| ((i&0x3)<<7)); // LSB
      samples.push_back(0); // MSB
    }

    for (int fc=0; fc<uHTR->n_channels_per_fiber(); fc++) {
      int halfWord=(fc==2)?(0):(1);
      int shift=(fc==1)?(1):(9);
      printf("  Channel %d on the fiber\n",fc);
      printf("  (1) Constant    (2) Gaussian pedestal   (3) Linear ramp\n");
      int model=tool_readline_int("Select signal model: ");
      if (model==1) {
	int cval=tool_readline_int("Constant level: ");
	for (size_t i=0; i<2048*2; i++) 
	  samples[i*2+halfWord]|=(cval&0x7F)<<shift;
      } else if (model==2) {
	printf(" Warning, this version of the tool doesn't properly emulate the QIE bins above ADC=15\n");
	float mean=tool_readline_float("Pedestal mean: ");
	float sigma=tool_readline_float("Pedestal width: ");
	for (size_t i=0; i<2048*2; i++) {
	  int val=int(gRandom->Gaus(mean,sigma));
	  samples[i*2+halfWord]|=(val&0x7F)<<shift;
	}
      } else if (model==3) {
	for (size_t i=0; i<2048*2; i++) {
	  samples[i*2+halfWord]|=(i&0x7F)<<shift;
	}
      }
    }
    uHTR->fe_ram_program_fiber(ifiber,samples);
  } else if (cmd=="ISOPULSE") {
    int afiber=tool_readline_int("Fiber: ");
    int achan=tool_readline_int("Chan: ");
    int amp=tool_readline_int("Amplitude: ");
    if (amp>0x7F) amp=0x7f;
    if (amp<0) amp=0;

    int bx=tool_readline_int("Bunch: ");

    std::vector<uint32_t> samples; // insert capid rotation
    for (size_t i=0; i<2048*4/2; i++) {
      samples.push_back(0x3 | ((i&0x3)<<3) | ((i&0x3)<<5)| ((i&0x3)<<7)); // LSB
      samples.push_back(0); // MSB
    }

    // ZEROS
    for (int ifiber=0; ifiber<24; ifiber++) {
      uHTR->fe_ram_program_fiber(ifiber,samples);
    }

    int halfWord=(achan==2)?(0):(1);
    int shift=(achan==1)?(1):(9);

    samples[bx*2+halfWord]|=amp<<shift;
    uHTR->fe_ram_program_fiber(afiber,samples);

  } else if (cmd=="LOAD") {
    std::string fname=tool_readline("Filename: ");
    FILE *f =fopen(fname.c_str(), "r");
    if (f==0) { printf("Unable to open '%s' for reading.\n\n",fname.c_str()); return; }
    char buffer[120];
    uint32_t tool;
    std::vector<uint32_t> samples;
    int ifiber=-1;
    while (!feof(f)) {
      buffer[0]=0;
      fgets(buffer,120,f);
      if (buffer[0]=='#') {
	if (!samples.empty() && ifiber>=0) uHTR->fe_ram_program_fiber(ifiber,samples);
	samples.clear();
	sscanf(buffer,"# Fiber %d",&ifiber);
	continue;
      }
      if (sscanf(buffer,"%x",&tool)==1) samples.push_back(tool);
    }
    if (samples.size()>10 && ifiber>=0) uHTR->fe_ram_program_fiber(ifiber,samples);
    fclose(f);
  } else if (cmd=="DUMP") {
    std::string fname=tool_readline("Filename: ");
    FILE *f =fopen(fname.c_str(), "w");
    if (f==0) { printf("Unable to open '%s' for writing.\n\n",fname.c_str()); return; }
    std::vector<uint32_t> samples;
    for (int ifiber=0; ifiber<24; ifiber++) {
      fprintf(f,"# Fiber %d\n",ifiber);
      uHTR->fe_ram_read_fiber(ifiber,samples);
      for (size_t i=0; i<samples.size(); i++) 
	fprintf(f,"%05x\n",samples[i]);
    }
    fclose(f);
  }
}

void uHTRtool::crate_slot_from_ip(uHTR* theuHTR,std::string ip)
{
   int crate,slot;
   unsigned last = ip.rfind(".");
   slot = atoi(ip.substr(last+1).c_str())/4;
   crate = atoi(ip.substr(
            ip.substr(0,last).rfind(".")+1,last).c_str());

   theuHTR->setCrateSlot(crate,slot);
}
/*
void uHTRtool::ddr3_test( uHTR* theUHTR ) {

  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }
 
  static int nTest = 20 ;
  static uint32_t iniPos = 0 ;
  nTest  = tool_readline_int("  Number of Read/Write ?" , nTest );
  iniPos = tool_readline_int("  Starting location ?   " , iniPos );
  uint32_t base_addr = iniPos * 0x8;

  printf( " ***** Reset DDR3 **********\n") ;
  theUHTR->ddr3_reset() ;
  //theUHTR->check_ddr3_stat() ;

  printf( " ***** Start Writing **********\n") ;
  // Writing 
  uint32_t w_addr = base_addr ;
  uint32_t testVal = 0x0 ;
  std::vector<uint32_t> wVal ;
  std::vector<uint32_t> rAddr ;
  for ( int i = 0; i< nTest; i++ ) {

      testVal += 0x11 ;
      if ( testVal > 0xFF ) testVal = 0x11 ;
      wVal.push_back( testVal ) ;
      
      if ( i%4 == 0 ) rAddr.push_back( w_addr ) ;

      if ( i%4 == 0 ) printf(" ***** Writing to ddr3 addr(%08X) = [", w_addr  ) ;
      printf(" %X", testVal ) ;
      if ( i%4 == 3 || i == nTest-1 ) printf(" ] \n" ) ;
      //printf(" ***** Writing to ddr3 addr(%08X) = [%X, %X, %X, %X] \n", base_addr, wVal[3], wVal[2], wVal[1], wVal[0] ) ;
      if ( i%4 == 3 ) w_addr += 0x8 ;
     
  }
  printf(" **** Writing buffer size = %d , base address size = %d \n", (int)wVal.size(), (int)rAddr.size() ) ;
  theUHTR->ddr3_write( base_addr, wVal ) ;

  //printf( " ***** Reset DDR3 **********\n") ;
  //theUHTR->ddr3_reset() ;

  // Reading
  printf( " \n **** Start Reading **** \n") ;
  std::vector<uint32_t> buff = theUHTR->ddr3_read( rAddr ) ;
  printf(" Read out buffer size = %d \n", (int)(buff.size())  ) ;
  //theUHTR->check_ddr3_stat() ;

}
*/

void uHTRtool::ddr3_test( uHTR* theUHTR ) {

  if (theUHTR==0) {
    printf("You can only run this function with a uHTR, not a generic mCTR2!\n\n");
    return;
  }

  static int testTime  = tool_readline_int(" Testing time duration (second) ?" , testTime );

  printf( " ***** Reset DDR3 **********\n") ;
  theUHTR->ddr3_reset() ;

  /* initialize random seed: */
  srand (time(NULL));

  uint32_t base_addr ;
  std::vector<uint32_t> wVal ;
  uint32_t testVal = 0x0 ;
  int mpos = 0 ;
  std::vector<uint32_t> rAddr ;
  uint32_t r_addr ;

  double test_duration = 0;
  time_t t_ini, t_run ;
  int kk = 0 ; 
  bool doPrint = false ;
  do {

     time(&t_ini) ;
     doPrint = ( kk%10000 == 0  ) ? true : false ;

     // Writing pattern 
     testVal = 0x0 ;
     wVal.clear() ;
     int wsize = ( rand()%9 + 1) *4 ;
     for ( int i = 0; i< wsize; i++ ) {
         wVal.push_back( testVal ) ;
	 if ( testVal == 0xFFFFFFFF ) testVal = 0x0 ;
	 testVal += 0x11111111 ;
     }

     // Write to DDR3
     mpos = rand()%16777216 ;
     base_addr = mpos * 8 ;
     theUHTR->ddr3_write( base_addr, wVal ) ;

     // small delay between read and write
     int delay = rand()%100 + 1 ;
     usleep( delay ) ;

     // Read out from DDR3
     rAddr.clear() ;
     r_addr = base_addr ;
     if (doPrint) printf(" <%d>Addr(%d) [ ", kk,  mpos) ;
     for ( int i=0; i< (int)wVal.size() ; i++) { 
         rAddr.push_back( r_addr  ) ;
         if (doPrint) printf(" %08X", r_addr ) ;
         r_addr += 0x8 ;
     }
     if (doPrint) printf(" ]\n") ;
     std::vector<uint32_t> buff = theUHTR->ddr3_read( rAddr ) ;

     // Comparing result 
     bool pass = true ;
     for ( int j=0; j< (int)wVal.size(); j++) {
         if (doPrint) printf("(%d) [%08X -- %08X] \n", j ,  wVal[j] ,  buff[j] ) ;
         if ( wVal[j] != buff[j] ) pass = false ;
     }
     if ( !pass ) {
        printf(" Mismatching @ %d test !!! \n", kk ) ;
        break ;
     }

     time(&t_run) ;
     test_duration += difftime( t_run, t_ini ) ;
     kk++ ;
  } while ( test_duration < testTime ) ;
  printf(" one test is %.2f sec \n ", test_duration ) ;

  printf(" Total Number of test : %d \n", kk ) ;

}


void uHTRtool::sn_write( uHTR* theUHTR ) {
  using namespace hcal::uhtr;
  uHTRsensors* sensors=theUHTR->sensors();

  uHTRsensors::MezzIdStruct id;
  memset(&id,0,sizeof(id));

  id.data_format_version=1;
  id.mezz_type_code=0x10;
  id.mezz_subtype_code=theUHTR->pcbRevision();
  snprintf((char*)id.mezz_type,15,"UHTR v1.%d",id.mezz_subtype_code);
  
  int sn=tool_readline_int("Serial number: ");
  id.serial_number[0]=sn&0xFF;
  id.serial_number[1]=(sn>>8)&0xFF;

  ::toolbox::TimeVal tvnow=::toolbox::TimeVal::gettimeofday();
  std::string date=tvnow.toString("%Y-%m-%d",::toolbox::TimeVal::loc);
  std::string site=tool_readline_nosplit("Site: ","");
  std::string tester=tool_readline_nosplit("Tester: ","");
  const char* relstr=getenv("HCAL_DAQ_RELEASE");
  std::string rel;
  if (relstr!=0) rel=relstr;

  strncpy((char*)id.manu_date,date.c_str(),11);
  id.manu_date[11]=0;
  strncpy((char*)id.manu_site,site.c_str(),16);
  id.manu_site[15]=0;
  strncpy((char*)id.manu_tester,tester.c_str(),16);
  id.manu_tester[15]=0;
  strncpy((char*)id.test_release,rel.c_str(),8);
  id.test_release[7]=0;

  sensors->writeId(uHTRsensors::mac_id_prom,id,true);
}


