#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <algorithm>
#include "hcal/uhtr/uHTRtool.hh"
#include "hcal/uhtr/uHTR.hh"

using namespace hcal::uhtr;

//============================================================================
//   BHM Functionalities
//============================================================================

void uHTRtool::bhm_render( uHTR* uHTR ) {

  int n_orb ;
  std::vector<uint8_t> thresholds, tdcmap ;

  uHTR->bhm_read_setup( n_orb, thresholds, tdcmap );

  
  float lbr,tbr; // lumi BC0 and trigger BC0 rate
  uint32_t link_state, errors;
  uHTR->lumi_link_status(link_state, lbr, tbr, errors) ;

  printf(" Link Status ( 0x3f is happy ) : 0x%02x  Error count: %d\n", link_state, errors ) ;
  printf(" BHM Lumi BC0 Rate : %.3f kHz   Local trigger BC0 Rate : %.3f kHz\n", lbr*1000, tbr*1000);
  
  printf(" N of Orbits = %d \n",n_orb  );

  printf(" [Threshold] :\n");
  for ( int i=0; i < (int)thresholds.size() ; i++ ) {
      if      ( i%6 == 0 ) printf(" Channel (%2d) :  %3u ,", i , thresholds[i] );
      else if ( i%6 == 5 ) printf(" %3u \n", thresholds[i] );
      else                 printf(" %3u ,",  thresholds[i] );
  }
  printf("\n");

  printf(" [TDC MAP] :\n");
  for ( int i=0; i < (int)tdcmap.size() ; i++ ) {
      if      ( i%8 == 0 ) printf(" TDC [%2d] %8u,", i, tdcmap[i] ) ;
      else if ( i%8 == 7 ) printf("  %8u \n", tdcmap[i] ) ;
      else                 printf("  %8u,"  , tdcmap[i] );
  }
  printf("\n");

}

void uHTRtool::bhm_setup( uHTR* uHTR ) {

  int cmd, n_orb ;
  std::vector<uint8_t> thresholds, tdcMap;

  FILE* fptr;

  do {
    printf("\n============================================== \n");
    bhm_render(uHTR);
    printf("\n============================================== \n");
    printf("\n");
    printf(" (1) Set BHM orbit integration period\n");
    printf(" (2) Set thresholds for histograms   \n");
    printf(" (3) Set TDC MAP                     \n");
    printf("\n (  Anything else will just return to the original menu )\n\n");

    cmd=tool_readline_int(" Selection : ",-1);

    uHTR->bhm_read_setup( n_orb, thresholds, tdcMap );

    int th ;
    ////std::vector<uint8_t> newVals ;
    std::string fname ;
    switch (cmd) {
    case 1 :
      n_orb=tool_readline_int("  New integration period in orbits : ", n_orb);
      uHTR->bhm_setup( n_orb, thresholds, tdcMap );
      break;
    case 2 :
      fname = tool_readline(" Input Threshold filename : ");
      fptr=fopen(fname.c_str(),"r");
      if (fptr==0) {
         printf(" !!! Unable to open '%s'.\n\n",fname.c_str());
      } else {
 
         int k =0 ;
         rewind(fptr);
         thresholds.clear() ;
         while (!feof(fptr)) {
              fscanf( fptr," %u", &th );
              //printf(" (%d) %u \n", k, th ) ;
              if ( ! feof(fptr) )  thresholds.push_back( th ) ;
              k++ ;
         }
         printf(" Update %d thresholds \n", (int) thresholds.size() ) ;
      }
      fclose(fptr);
      uHTR->bhm_setup( n_orb, thresholds, tdcMap );

      break;
    case 3 :
      fname = tool_readline(" Input TDC Map filename : ");
      fptr=fopen(fname.c_str(),"r");
      if (fptr==0) {
         printf(" !!! Unable to open '%s'.\n\n",fname.c_str());
      } else {
 
         int k =0 ;
         rewind(fptr);
         tdcMap.clear() ;
         while (!feof(fptr)) {
              fscanf( fptr," %u", &th );
              if ( ! feof(fptr) )  tdcMap.push_back( th ) ;
              k++ ;
         }
         printf(" Update %d TDC values \n", (int) tdcMap.size() ) ;
      }
      fclose(fptr);
      uHTR->bhm_setup( n_orb, thresholds, tdcMap );
      break ;
    default: break;
    }
  } while (cmd>0);
}

void uHTRtool::bhm_read_test( uHTR* uHTR ) {

     // setup the root file name 
     TFile* hfile=0;
     std::string hFileStr=tool_readline(" ROOT histogram file (blank for no output) : ");
     int nRead = 100 ;
     nRead = tool_readline_int(" Number of read : ", nRead );
     if (!hFileStr.empty()) hfile=new TFile(hFileStr.c_str(), "RECREATE");

     std::vector<bool> aM ;    // available 
     std::vector<bool> oM ;    // overflow 
     std::vector<uHTR::LumiHistogram> hV ;

     int n_orb ;
     std::vector<uint8_t> thresholds, tdcMap;
     uHTR->bhm_read_setup( n_orb, thresholds, tdcMap );
     int read_period = n_orb*90 ;  // each orbit is about 89.1 (us) micro-second

     for (int i=0; i < nRead ;  i++ ) {

         // 1. Check availability
         aM.clear();
         oM.clear();
         uHTR->bhm_histo_status( aM, oM ) ;
	 bool any_avail=false;
         for ( int j=0; j < (int)aM.size() ; j++   ) {
	   if (j==0) printf("Histo[%02d] = Available : %u , Overflow : %u \n",  j , (uint8_t)aM[j], (uint8_t)oM[j] ) ;

             if ( !aM[j] ) continue ;
	     any_avail=true;
	 }
	 if (!any_avail)   {
	   usleep( read_period ) ;
	   continue;
	 }

	 // this hV contains 24 (4 links x6 channels or 3 sets x 8 reads ) histogram objects
	 uHTR->bhm_read_histograms( hV );

         for ( int j=0; j < (int)aM.size() ; j++   ) {

	   if (j!=0) continue; //
	   if ( hfile != 0 && aM[j] ) {
	     
	     char hNameStr[128] ;
	     sprintf( hNameStr,  "h%02d_%d_%d", j , hV[j].orb_init , i ) ;
	     TH1D*  h1  = new TH1D( hNameStr, hNameStr , 3564*4, 0, 3564*4 ) ;
	     
	     for ( size_t k=1; k<= hV[j].h.size(); k++ ) {
	       h1->Fill( k, hV[j].h[k-1] ) ;
	     }
	     
	     h1->Write();
	   }
         }
         printf(" ============================================== \n") ;
     }
     if ( hfile != 0 ) {
         hfile->Close();
         delete hfile ;
     }
}


