#include <stdio.h>
#include <stdlib.h>
#include "hcal/uhtr/uHTRtool.hh"
#include "boost/thread.hpp"

using namespace hcal::uhtr;

void uHTRtool::RunMenu( uHTRV * uHTRs ) {

     typedef Menu< uHTRV > uMenu ;
     typedef Menu< uHTRV>::Line uMenuLine ;

     uMenuLine menu_link_lines[] = { 
             uMenuLine("INIT", "Initialize Links", &link_init),
	     uMenuLine("QUIT","Back to top menu" ),
	     uMenuLine()
     };
     uMenu menu_link(menu_link_lines);

     uMenuLine menu_lumi_lines[] = { 
             uMenuLine("SETUP", "Setup the Lumi DAQ", &lumi_setup),
	     //uMenuLine("LUTS", "Program and read LUTs", &lumi_luts),
             uMenuLine("READ", " Lumi read test ", &lumi_read_test),
             uMenuLine("RESET", " Lumi link reset ", &lumi_link_reset),
	     uMenuLine("QUIT","Back to top menu"),
	     uMenuLine()
     };
     uMenu menu_lumi(&lumi_render,menu_lumi_lines);

     uMenuLine menu_uflash_lines[] = { 
             uMenuLine("FPGA", "Toggle which FPGA to target", &flash_set ),
             //uMenuLine("READ", "Read the flash contents into a file", &flash_read ),
	     uMenuLine("PROG", "Reprogram the flash from an MCS file", &flash_prog ),
	     //uMenuLine("VERIFY", "Verify the flash contents against an existing MCS file", &flash_verify),
	     uMenuLine("RELOAD", "Reload the FPGA from FLASH", &flash_reconfig),
	     uMenuLine("QUIT","Back to top menu"),
	     uMenuLine()
     };
     uMenu menu_uflash(&flash_render,menu_uflash_lines);

     uMenuLine menu_utop_lines[] = { 
	     uMenuLine("STATUS","Status summary of all uHTR cards", &uHTRs_status),
	     uMenuLine("CLOCK","Check all clock rates", &check_clk_rates),
	     uMenuLine("FLASH", "Flash programming and readback menu", &menu_uflash  ),
	     uMenuLine("LUMI", "LUMI-DAQ work", &menu_lumi  ),
	     uMenuLine("LINK","Status and control of frontend links",&menu_link) ,
	     uMenuLine("EXIT", "Exit this tool") ,
	     uMenuLine() 
     };

     uMenu menu_utop(menu_utop_lines);

     menu_utop.steer( uHTRs ) ;

}

void uHTRtool::uHTRs_status(uHTRV * uHTRs)
{
   std::vector<int> okV(uHTRs->size(),0);
   //boost::function< void (uHTR*,int*)> func = uHTR_status_thread;
   //thread_func(uHTRs,func,okV);

   for(size_t i = 0; i< uHTRs->size(); i++)
      uHTR_status_thread(uHTRs->at(i),&okV[i]);

   bool okay = true;
   for(size_t i =0; i<okV.size();i++)
   {
      if(!okV[i])
      {
         if(okay)
         {
            printf("  *** Could not connect to all uHTRs *** \n");
            okay = false;
         }
         printf("  *** Check (%d,%d)",uHTRs->at(i)->crate(),uHTRs->at(i)->slot());
      }
   }
   if(okay) 
      printf("  Successfully connected to all uHTRs");
}

void uHTRtool::uHTR_status_thread(uHTR * uHTR_, int * ok)
{
   uint8_t patch,major,minor,flavor;
   bool back_ok, front_ok;
   printf(" Connecting to (%d,%d) ",uHTR_->crate(),uHTR_->slot());
   try {
      uHTR_->firmwareRevision( true,  flavor,major,minor,patch);
      printf(" Front-FW:(%02X) %02x.%02x.%02x",flavor,major,minor,patch);
      front_ok=true;
   } catch (...) {
      printf(" *** Problem communicating with front FPGA *** \n");
      front_ok=false;
   }

   try {
      uHTR_->firmwareRevision( false, flavor,major,minor,patch);
      printf(" Back-FW: (%02X) %02x.%02x.%02x \n",flavor,major,minor,patch);
      back_ok=true;
   } catch (...) {
      printf(" *** Problem communicating with back FPGA *** \n");
      back_ok=false;
   }
   *ok =  int(front_ok && back_ok);
}

static bool which_fpga_for_flash=false; // back is default

void uHTRtool::flash_render(uHTRV * uHTRs ) { 
  printf(" FLASH target: %s\n",(which_fpga_for_flash)?("FRONT"):("BACK"));
}

void uHTRtool::flash_set( uHTRV * uHTRs ) { 
  which_fpga_for_flash=!which_fpga_for_flash;
}

void uHTRtool::flash_prog( uHTRV* uHTRs ) { 
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

      boost::function<void (uHTR *)> func = boost::bind(&uHTR::programFirmware, _1,which_fpga_for_flash, image,30);

      thread_func(uHTRs,func);
   }
}

void uHTRtool::lumi_render( uHTRV* uHTRs ) {
   printf("Find out what useful info we need here\n");
   //for(size_t i = 0; i< uHTRs->size();i++)
   //{
      //printf(" uHTR(%d,%d)\n",uHTRs->at(i)->crate(),uHTRs->at(i)->slot());
      //lumi_render(uHTRs->at(i));
   //}
}

void uHTRtool::lumi_setup( uHTRV* uHTRs ) {
  int cmd;
  int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
  std::vector<bool> mask_lhc, mask_cms;
  bool set_none = true;

  //std::vector<setup> setupV(uHTRs->size());
  
  uHTRs->at(0)->lumi_read_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
  printf("Setup for first board: uHTR(%d,%d)\n",uHTRs->at(0)->crate(),uHTRs->at(0)->slot());
  lumi_render(uHTRs->at(0));

  do {
    printf("\n============================================== \n");

    printf("\n");  
    printf(" (1) Set LHC orbit integration period\n");
    printf(" (2) Set CMS orbit integration period\n");
    printf(" (3) Set LHC threshold     (4) Set CMS Threshold 1\n");
    printf(" (5) Set CMS threshold 2   (6) Adjust LHC mask bit\n");
    printf(" (7) Adjust CMS mask bit\n");
    printf("\n ( Changes will be applied when the menu is quit )\n\n");
  

    cmd=tool_readline_int(" Selection : ",-1);

    //thread_func(uHTRs,&lumi_read_setup,setupV);
    switch (cmd) {
    case 1 : 
      n_lhc_orb=tool_readline_int("  New LHC integration period in orbits : ", n_lhc_orb);
      set_none = false;
      break;
    case 2 : 
      n_cms_orb=tool_readline_int("  New CMS integration period in orbits : ", n_cms_orb);
      set_none = false;
      break;
    case 3 : 
      lhc_threshold=tool_readline_int("  New LHC hit threshold : ",lhc_threshold);
      set_none = false;
      break;
    case 4 : 
      cms1_threshold=tool_readline_int("  New CMS hit threshold 1 : ",cms1_threshold);
      set_none = false;
      break;
    case 5 : 
      cms2_threshold=tool_readline_int("  New CMS hit threshold 2 : ",cms2_threshold);
      set_none = false;
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
	    for (ic=0; ic<3; ic++) {
	      int index=ifiber*3+ic;
	      mask_lhc[index]=!mask_lhc[index];
	    }
	  } else {
	    int index=ifiber*3+ic;
	    mask_lhc[index]=!mask_lhc[index];
	  }
	}
      }
      set_none = false;
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
	    for (ic=0; ic<3; ic++) {
	      int index=ifiber*3+ic;
	      mask_cms[index]=!mask_cms[index];
	    }
	  } else {
	    int index=ifiber*3+ic;
	    mask_cms[index]=!mask_cms[index];
	  }
	}
      }
      set_none = false;
      break;
    default: break;
    }
  } while (cmd>0);
  if(!set_none)
  {
     printf("Applying changes...\n");
     boost::function< void (uHTR*)> f  = boost::bind(&uHTR::lumi_setup,_1,n_lhc_orb,n_cms_orb,lhc_threshold,cms1_threshold,cms2_threshold,mask_lhc,mask_cms);
     thread_func(uHTRs,f);
   for(size_t i = 0; i< uHTRs->size();i++)
   {
      printf(" uHTR(%d,%d)\n",uHTRs->at(i)->crate(),uHTRs->at(i)->slot());
      lumi_render(uHTRs->at(i));
   }
  }
}

void uHTRtool::lumi_read(uHTRV * uHTRs, int end, std::vector<std::pair<int,int> > &ret) {
   printf("  Reading Histograms for %i seconds\n",end);

   uHTR::LumiHistogram hV ; 

   time_t start;
   time(&start);
   //int i = 0;
   //typedef boost::posix_time::ptime Time;

   for(size_t i = 0; i < ret.size();i++)
      ret[i]=std::pair<int,int>(0,0);
   
   while(time(NULL) - start < end)
   {
      for(size_t i = 0; i<uHTRs->size();i++)
      {
         for(int hist_num = 0; uHTR::LUMI_HISTONAMES[hist_num] != 0; hist_num ++)
         {
            uHTRs->at(i)->lumi_read_histogram(uHTR::LUMI_HISTONAMES[hist_num], hV);
            if (!hV.h.empty()) 
            { 
               hV.h.clear();
               ret[i].first ++;
               ret[i].second += hV.overflow;
               sched_yield();
            }
         }
      }
      //Time t2 = boost::posix_time::microsec_clock::universal_time();
      //printf("Time diff : %s\n",boost::posix_time::to_simple_string(t2-t1).c_str());
   }
}

void uHTRtool::lumi_read_one(uHTR* theuHTR, int end, std::pair<int,int> * ret) {

   std::vector<std::pair<int,int> > o_i(1,std::pair<int,int>(0,0));
   std::vector<uHTR*> uHTRs(1,theuHTR);
   lumi_read(&uHTRs,end,o_i);
   *ret = o_i[0];
}

std::pair<int,int> sum_vec(std::vector<std::pair<int,int> > & vec)
{
   std::pair<int,int> sum_of_elems;
   sum_of_elems.first = 0;
   sum_of_elems.second = 0;
   for(std::vector<std::pair<int,int> >::iterator j=vec.begin();j!=vec.end();++j)
   {
      sum_of_elems.first += j->first;
      sum_of_elems.second += j->second;
   }
   return sum_of_elems;
}

void uHTRtool::lumi_read_test(uHTRV* uHTRs)
{
   static int duration = 30;
   duration = tool_readline_int("  Read for how long?",duration);  
   static int mode = 0;
   mode = tool_readline_int("Set Mode  (0:Threaded or 1:Iterative)",mode);  

   std::vector<std::pair<int,int> > ret(uHTRs->size());
   std::pair<int,int> total_ret;

   int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
   std::vector<bool> mask_lhc, mask_cms;
   uHTRs->at(0)->lumi_read_setup(n_lhc_orb, n_cms_orb,lhc_threshold, cms1_threshold, cms2_threshold, mask_lhc, mask_cms );
   int n_orbits = n_lhc_orb;

   int n_hists = 6;
   switch(mode)
   {
      case 1:
         lumi_read(uHTRs,duration,ret);
         break;
      case 0:
         //std::vector<int> it = overflow.begin();
         //std::vector<uHTR*> iuHTR = uHTRs.begin();
         printf("Starting %i threads\n",int(uHTRs->size()));
         {
            boost::thread_group thrds;
            for(size_t i = 0; i < uHTRs->size(); i++)
            {
               thrds.create_thread(
                     boost::bind(
                        &lumi_read_one,
                        uHTRs->at(i),duration,&ret[i]));
            }
            thrds.join_all();
         }
         break;
      default:
         break;
   }
   total_ret = sum_vec(ret);

   double orbit_freq = 1/(3564*25e-9);
   printf ("Total Histograms is: %i\n", total_ret.first);
   printf ("Total overflow is: %i\n", total_ret.second);
   printf ("Expected about Histograms: %li\n", int(n_hists*uHTRs->size()*duration*orbit_freq/n_orbits));
   printf ("Try using n_orbits = %li\n", int(n_hists*uHTRs->size()*duration*orbit_freq/(n_hists*uHTRs->size()*duration*orbit_freq/n_orbits - total_ret.second)));

}

void uHTRtool::lumi_link_reset(uHTRV* uHTRs ) {

   printf("  Resetting all Lumi Links...\n");
   thread_func(uHTRs,&uHTR::lumi_link_reset);
}

void uHTRtool::link_init(uHTRV* uHTRs ) {

  static int autoAlign = 0 ; 
  autoAlign = tool_readline_int("  Set Auto_Realign (0:NO, 1:Yes): ", autoAlign );

  static int orbDelay = 93 ; 
  orbDelay = tool_readline_int("  Set Orbit Delay : ", orbDelay );   

  boost::function<void (uHTR *)> func = boost::bind(&uHTR::link_init, _1,orbDelay, autoAlign!=0,true,true);
   thread_func(uHTRs,func);
}

void uHTRtool::check_clk_rates( uHTRV* uHTRs) {

   std::vector<int> okV;
   boost::function< void(uHTR*,int*)> func = check_clk_rates_thread; 
   thread_func(uHTRs,func,okV);

   bool okay = true;
   for(size_t i =0; i<okV.size();i++)
   {
      okay = okV[i] && okay;
   }
   if(okay) 
      printf("  All Clocks Okay");
   else
   {
      std::string resp=tool_readline("Clocks not okay. Check status for problem uHTRs?","y");
      if (resp.empty() || (resp[0]=='y' || resp[0]=='Y')) 
      {
         printf("hmm\n");
         for(size_t i =0; i<okV.size();i++)
         {
            if(!okV[i])
            {
               printf(" uHTR(%d,%d)\n",uHTRs->at(i)->crate(),uHTRs->at(i)->slot());
               uHTR_status(uHTRs->at(i));
            }
         }
      }
   }
}

bool check_double(double x,double y)
{
   int j =0;
   double p;
   for (p=pow(10,j); x*p-round(x*p) != 0 && j <15 ;j++)
      p = pow(10,j);

   //printf("%f %f %f \n",x*p,y*p,fabs(x*p - round(y*p)));
   return fabs(x*p - round(y*p)) <= 1;
}

void uHTRtool::check_clk_rates_thread( uHTR* theuHTR, int * okay) {

   std::vector<std::pair<double,double> > clocksF, clocksB;
   clocksB=theuHTR->getClockRates(false);
   clocksF=theuHTR->getClockRates(true);
   std::vector<std::pair<double,double> >::iterator it;

   
   bool clocks_ok = true;
   if(!check_double(40.08*6,clocksF.back().second))
   {
      //printf(" Front Clock: %10.4f MHz, setting to 240 MHz\n",clocksF.back().second);
      theuHTR->setupClock(1,240);
   }
   if(!check_double(40.08*8,clocksB.back().second))
   {
      //printf(" Back Clock: %10.4f MHz, setting to 320 MHz\n",clocksF.back().second);
      theuHTR->setupClock(2,320);
   }

   for(it=clocksB.begin();it != clocksB.end()-1 ;it++)
   {
      if (!check_double( (*it).first, (*it).second))
      {
         clocks_ok = false;
         //printf (" Back: Expected %10.4f MHz, got %10.4f MHz \n",(*it).first, (*it).second);
      }
   }
   for(it=clocksF.begin();it != clocksF.end()-1;it++)
   {
      if (!check_double( (*it).first, (*it).second))
      {
         //printf (" Front: Expected %10.4f MHz, got %10.4f MHz\n",(*it).first, (*it).second);
         clocks_ok = false;
      }
   }

   if(!check_double(40.08*6,clocksF.back().second))
      clocks_ok = false;
   if(!check_double(40.08*8,clocksB.back().second))
      clocks_ok = false;

   if (clocks_ok) printf(" Clocks ok for uHTR(%d,%d)\n",theuHTR->crate(),theuHTR->slot());
   else printf(" Problem with clocks on  uHTR(%d,%d)\n",theuHTR->crate(),theuHTR->slot());

   *okay = int(clocks_ok);
}

void uHTRtool::flash_reconfig( uHTRV* uHTRs ) { 
  printf("Initiating reload from flash (requires ~10 seconds)...\n");

   thread_func(uHTRs,boost::bind(&uHTR::reloadFromFlash,_1,which_fpga_for_flash)); 
  sleep(20);
}

