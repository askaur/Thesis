#ifndef HCAL_UHTR_UHTRTOOL_H_INCLUDED
#define HCAL_UHTR_UHTRTOOL_H_INCLUDED 1

#include "hcal/toolbox/mcs.hh"
#include "hcal/uhtr/uHTR.hh"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>
#include <malloc.h>
#include <memory>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <list>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TFile.h>


std::string tool_readline(const std::string& prompt) ; 
std::string tool_readline(const std::string& prompt, const std::string& defval);
int tool_readline_int(const std::string& prompt) ; 
int tool_readline_int(const std::string& prompt, int aval) ; 

namespace hcal {
   namespace uhtr {

      std::string uhalDeviceString(std::string ipAddress);

      template<class ctr_T>
      class Menu {
      public:
	
	class Line {
	public:
	  Line(const char* n, const char* d, void (*f)( ctr_T* mCTR_ )) : name(n),desc(d),subMenu(0),func(f),func2(0), isNull(false) { }
	  Line(const char* n, const char* d, void (*f2)( const std::string& cmd, ctr_T* mCTR_ )) : name(n),desc(d),subMenu(0),func(0),func2(f2), isNull(false) { }
	  Line(const char* n, const char* d, Menu* sb ) : name(n),desc(d),subMenu(sb), func(0),func2(0), isNull(false) { }
	  Line(const char* n, const char* d) : name(n),desc(d),subMenu(0), func(0),func2(0), isNull(false) { }
	  Line() : isNull(true) { }
	  bool null() const { return isNull; }
	  const char* name;
	  const char* desc;
	  Menu* subMenu;
	  void (*func)( ctr_T* mCTR_ ) ;
	  void (*func2)( const std::string& cmd, ctr_T* mCTR_ ) ;
	private:
	  bool isNull;
	};

	Menu(void (*renderf)( ctr_T* mCTR_ ), const Line* tlines) : renderFunc(renderf) { 
	  for (size_t i=0; !tlines[i].null(); i++) lines.push_back(tlines[i]);
	}
	
	Menu(const Line* tlines) : renderFunc(0) {
	  for (size_t i=0; !tlines[i].null(); i++) lines.push_back(tlines[i]);
	}
	
	void addLine(const Line& line) { lines.push_back(line); }
	
	void (*renderFunc)( ctr_T* mCTR_ );

	std::vector<Line> lines;

               void steer( ctr_T* mCTR_  ) ;
         };

      template<class ctr_T>
         void Menu<ctr_T>::steer( ctr_T* mCTR_ ) {

            const Line* theMatch=0;
            do {
	      printf("\n");
               if (renderFunc!=0) {
                  this->renderFunc( mCTR_ ) ;
               }
	       for (size_t i=0; i<lines.size(); i++) {
		 printf("   %-12s %s\n",lines[i].name,lines[i].desc);	       
               }
               std::string request=tool_readline(" > ");
               theMatch=0;
               // check for a unique match...
               int nmatch=0;
               for (size_t i=0; i<lines.size(); i++) 
		 if (strncasecmp(request.c_str(),lines[i].name,request.length())==0) {
		   theMatch=&(lines[i]);
		   nmatch++;
		 }
               if (nmatch>1) theMatch=0;
               // ok
               if (theMatch==0) printf("  Command '%s' not understood.\n\n",request.c_str());
               else add_history(theMatch->name);
               // actions
               if (theMatch!=0 && theMatch->subMenu!=0)          theMatch->subMenu->steer( mCTR_ );
               else if (theMatch!=0 && theMatch->func!=0 )       theMatch->func( mCTR_ ) ;
               else if (theMatch!=0 && theMatch->func2!=0 )       theMatch->func2( theMatch->name, mCTR_ ) ;

            } while (theMatch==0 || theMatch->subMenu!=0 || (theMatch->func!=0 || theMatch->func2 !=0) );
         }


      class uHTRtool {
         
         typedef std::vector<uHTR*> uHTRV;
         public:

            uHTRtool( ) ;  
            ~uHTRtool() { } 


            /* 
               template<typename ctr_T>
               static void spicfg_render( ctr_T* mCTR_ );
               template<typename ctr_T>
               static void spicfg_setip( ctr_T* theCTR_ = 0  );
               template<typename ctr_T>
               static void spicfg_setmac( ctr_T* theCTR_ = 0 );
               template<typename ctr_T>
               static void spicfg_settest( ctr_T* theCTR_ = 0 );
               template<typename ctr_T>
               static void spicfg_write( ctr_T* mCTR_ );
            */

            static void ejf_test( uHTR* uHTR ); 
            static void pipeline_queue_test( uHTR* uHTR );
            static void pipeline_capture_test( uHTR* uHTR );
            static void pipeline_slice_test( uHTR* uHTR );
            static void pipeline_fifo_test( uHTR* uHTR );
            static void dram_test( uHTR* uHTR ) ;
            static void ddr3_test( uHTR* uHTR ) ;

            static void dtc_Info( uHTR* uHTR );
	static void dtc_Control( const std::string& cmd, uHTR* uHTR );
            //static void CTR2_status( upgrade::mCTR2* mCTR2_ );
            static void uHTR_status( uHTR* uHTR );
            static void sn_write( uHTR* uHTR );

            static void flash_render( uHTR* uHTR ) ;
            static void flash_set( uHTR* uHTR ) ;
            static void flash_prog( uHTR* uHTR );
            static void flash_read( uHTR* uHTR );
            static void flash_verify( uHTR* uHTR );
            static void flash_reconfig( uHTR* uHTR );
	
	    static void clk_rates( uHTR* uHTR );
	    static void clk_setup( const std::string& cmd, uHTR* uHTR );

	    static void lumi_setup( uHTR* uHTR );
	    static void lumi_luts( uHTR* uHTR );
	    static void lumi_render( uHTR* uHTR );
	    static void lumi_read_test( uHTR* uHTR );
	    static void lumi_link_actions(const std::string& cmd, uHTR* uHTR );

	    static void bhm_render( uHTR* uHTR );
	    static void bhm_setup( uHTR* uHTR );
	    static void bhm_read_test( uHTR* uHTR ) ;

	    static void init_link( uHTR* uHTR );
	    static void link_just_reset( uHTR* uHTR );
	    static void link_status( uHTR* uHTR );
	    static void link_spy( uHTR* uHTR );
	    static void link_spy_histogram( uHTR* uHTR );
	    static void link_histo(const std::string& cmd, uHTR* uHTR);
	    static void link_l1a_capture(const std::string& cmd,  uHTR* uHTR );

	    static void link_bad_data_rate(uHTR*);
	    static void link_counter_mask( uHTR* uHTRs ) ;
	    static void link_fe_rams(const std::string& cmd, uHTR* uHTR);
	    static void daq_spy(uHTR* uHTR );          
	    static void daq_status(uHTR* uHTR );
	    static void daq_ctl(uHTR* uHTR );
	    static void daq_f2b(uHTR* uHTR );

	    static void trigger_luts( uHTR* uHTR ); 
	    static void trigger_spy( uHTR* uHTR ); 
	    static void trigger_self( uHTR* uHTR );
  	    static void trigger_orm( const std::string&, uHTR* uHTR );

	    static void sfp_spy_tx( uHTR* uHTR ); 
	    static void sfp_spy_rx( uHTR* uHTR ); 
	static void sfp_actions( const std::string&, uHTR* uHTR ); 

	static void sensor_action(const std::string& cmd, uHTR* uHTR);
	static void sensor_ppod(const std::string& cmd, uHTR* uHTR);

	    static bool readMCS(std::vector<unsigned char>& image, std::string& fname) ;
	    static bool readMCS(std::vector<unsigned char>& image) ;


	    static void RunMenu( uHTR* uHTR_  )  ;

	    static void crate_slot_from_ip(uHTR* theuHTR,std::string ip);
	    // Functions for parrallel functionality
	    //
	    static void RunMenu( uHTRV * uHTRs  );
	    static void uHTRs_status( uHTRV * uHTRs );

	    static void flash_render( uHTRV* uHTRs ) ;
	    static void flash_set( uHTRV* uHTRs ) ;
	    static void flash_prog( uHTRV* uHTRs );
	    static void flash_reconfig( uHTRV* uHTRs );

	    static void lumi_setup( uHTRV* uHTRs );
	    static void lumi_render( uHTRV* uHTRs );
	    static void lumi_link_reset(uHTRV* uHTRs );
	    static void lumi_read_test( uHTRV* uHTR );
	    static void lumi_read(uHTRV * uHTRs, int end, std::vector<std::pair<int,int> > &ret);
	    static void lumi_read_one(uHTR* theuHTR, int end, std::pair<int,int>  * ret);

	    static void link_init(uHTRV* uHTRs ) ;
	    static void check_clk_rates( uHTRV* uHTRs);

            //creates threads for each uhtr, executing the given function
            //storing the result in a pointer
            template <class T>
            //static void thread_func(uHTRV * uHTRs,void (*func)(uHTR*, T*), std::vector<T> & ret)
            static void thread_func(uHTRV * uHTRs,boost::function<void (uHTR*, T*)> func, std::vector<T> & ret)
            {
               if(ret.empty())
                  ret.resize(uHTRs->size());
               else if(ret.size() != uHTRs->size())
               {
                  printf("  Return vector wrong size!!\n");
                  printf("  Resizing...\n");

                  ret.resize(uHTRs->size());
               }

               boost::thread_group thrds;
               for(size_t i = 0;i<uHTRs->size();i++)
               {
                  thrds.create_thread(
                        boost::bind(
                           func,
                           uHTRs->at(i),
                           &ret[i])
                        );
               }
               thrds.join_all();
            }
             
            static void thread_func(uHTRV * uHTRs, boost::function<void (uHTR*)> func)
            {
               boost::thread_group thrds;
               for(size_t i = 0;i<uHTRs->size();i++)
               {
                  thrds.create_thread(
                        boost::bind(
                           func,
                           uHTRs->at(i))
                        );
               }
               thrds.join_all();
            }

         private:
            /*struct setup
            {
               int n_lhc_orb, n_cms_orb, lhc_threshold, cms1_threshold, cms2_threshold;
               std::vector<bool> mask_lhc, mask_cms;
            };*/
            static void uHTR_status_thread(uHTR * uHTR_, int * ok);
            static void check_clk_rates_thread( uHTR* theuHTR, int * okay);
            //static void lumi_read_setup( uHTR* uHTR, setup * set);

      };


   }
}


#endif // HCAL_UHTR_UHTRTOOL_H_INCLUDED
