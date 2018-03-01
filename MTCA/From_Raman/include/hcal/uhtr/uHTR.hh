#ifndef HCAL_UHTR_UHTR_H_INCLUDED
#define HCAL_UHTR_UHTR_H_INCLUDED 1

#include "hcal/toolbox/RateTool.hh"
#include "hcal/exception/Exception.hh"
#include <stdint.h>
#include <sys/time.h>

namespace uhal {
  class Node;
  class HwInterface;
}


namespace hcal {
  namespace uhtr {

    class uHTRsensors;
    
    class uHTR {
    public:

      explicit uHTR(const std::string& name, std::string ip, uhal::HwInterface& hw_);
      explicit uHTR(const std::string& name, uhal::HwInterface& hw_ );

      virtual ~uHTR() ;

      // Card Identity 
      inline int slot() const { return slot_id; }
      inline int crate() const { return crate_id; }
      const std::string& name() const { return card_name; }
      std::string hostname() const { return card_hostname; }

      inline void setIP( std::string ipAddr ) { card_ip = ipAddr ; }
      inline void setHostname( std::string hostname_ ) {  card_hostname = hostname_ ; } 

      void setCrateSlot(int crate, int slot);
      void getHardware( std::string mapPath ) ;
      uint32_t firmwareRevision(bool front, uint8_t& flavor, uint8_t& major, uint8_t& minor, uint8_t& patch);
      uint32_t firmwareRevisionRaw(bool front);
      uint32_t pcbRevision() const { return pcb_revision; }
      uint32_t serialNumber() const { return serial_number; }
      bool is4800() const;
      bool is1600() const;
      bool isBHM() const;

      static std::string firmwareFlavorToString(uint8_t flavor);

      // Firmware Configuration
      void programFirmware(bool front, const std::vector<unsigned char>& image, int verbosity=0);
      void readFirmware(bool front, std::vector<unsigned char>& image, int verbosity=0 );
      void readFirmwareSector(bool front, int isector, std::vector<unsigned char>& image, int verbosity=0);
      void reloadFromFlash( bool front =false);

      /// Talk to the ID/MAC EEPROM
      bool idSPI(const std::vector<unsigned char>& dataIn, std::vector<unsigned char>& dataOut, int nread);

      std::vector<std::pair<double, double> > getClockRates(bool front=false);
      /// target=1 ("320 53XX"),  target=2 ("240 53XX"), target=3 ("S/N EEPROM"), target=4 ("Program EEPROM"), target=5 ("CPLD"), target=6 ("MISC SPI")
      bool clockSPI(int target, const std::vector<unsigned char>& dataIn, std::vector<unsigned char>& dataOut, int nread=-1);
      /// target=1 ("320 53XX" direct),  target=2 ("240 53XX" direct), target=3 (Program EEPROM for "320"), target=4 (Program EEPROM for "240"), target=6 ("MISC_SPI EEPROM")
      void setupClock(int target, int image);
      bool eepromClock(bool reload=false);

      // Link functions
      std::vector<int> link_init_status( bool printout = true ) ;
      std::vector<int> link_init_status( uhal::Node& linkNode, bool printout = true ) ;
      void link_init(uint32_t orbit_delay, bool auto_align, bool do_resets=false, bool text_info=false);     
      void link_init_setup(uint32_t orbit_delay, bool auto_align);
      void* link_init_multistep(void* last_step_ptr=0, bool text_info=false);

      void link_reset_error_counters();
      void link_reset_error_counters( uhal::Node& linkNode );
      void link_query();
      void link_set_gtx_error_threshold( uint32_t threshold_ = 0xFFFFF, uint32_t mask_ = 0xFFFFFF ) ;
      std::vector<uint32_t> link_gtx_reset_count() ;

      void link_all_reset() ;
      void link_GTX_reset( const std::vector<int>& ilink ) ;
      void link_CDR_reset( const std::vector<int>& ilink ) ;
      void link_buffer_reset( const std::vector<int>& ilink ) ;

      // Link functions
      static const int NFIBERS = 24;
      int n_channels_per_fiber() const { return n_channels_per_fiber_; }

      uint32_t info_link_ResetInfo(int ilid, int itemId) ;
      double   info_link_BadDataRate(int lid);
      uint32_t info_link_BadDataCounterOn(int lid);
      uint32_t info_link_nBadData(int lid);
      uint32_t info_link_nBadDataRollover(int lid);
      uint32_t info_link_nBadAlign(int lid);
      uint32_t info_link_alignStatus(int lid);
      uint32_t info_link_alignOccupancy(int lid);
      uint32_t info_link_alignDelta(int lid);
      float    info_link_orbitRate(int lid);
      float    info_clk_rates(int clk_id);
      
      void link_setup_query( uint32_t& align_v , bool& auto_align) ;
      std::vector<uint32_t> link_spyRead(int id );
      std::vector<uint32_t> link_spyCaptureRead(int id, uint32_t address, int nBX_before, uint32_t nBX_after, int &trigger);
      std::vector<uint32_t> link_spyCaptureRead(int id, uint32_t address, uint32_t nBX_before, uint32_t nBX_after, uint32_t &trigger);
      void link_spySetup(bool enable_capture_sw, bool enable_capture_dtc, bool orbit_no_wait, bool raw_words, int pre, int delay);
      void link_spyCaptureSetup(int howMany);
      void link_spyExecute();
      uint32_t link_spyState();
      uint32_t link_spy_writeptr_L1A_read();
      uint32_t link_accept_pulse_rate();
      void link_spySetStart(uint32_t setstart);

      /// use 0 to disable orbit integration
      void link_histo_orbit_integrate(uint32_t orbits);
      void link_histo_orbit_integrate_info(uint32_t& orbits, bool& active);
      void link_histo_setSeparateCapIDs(bool separateCapIDs);
      bool link_histo_ready();
      void link_histo_start();
      void link_histo_stop();
      void link_histo_clear();
      bool link_histo_clear_done();
      std::vector<uint32_t> link_histo_read(int nhisto);
      void link_histo_read_FirstAndLastOrbits(uint32_t& firstorbit, uint32_t& lastorbit);

      // FE RAM functions
      void fe_ram_setup(bool enabled, bool ttc_aligned, uint32_t ttc_delay);
      void fe_ram_setup_query(bool& enabled, bool& ttc_aligned, uint32_t& ttc_delay);
      void fe_ram_program_fiber(int ifiber, const std::vector<uint32_t>& data);
      void fe_ram_read_fiber(int ifiber, std::vector<uint32_t>& data);

      // Data Pipeline
      void pipeline_setup(  bool pipe_id, uint32_t nsamples, uint32_t npresamples, uint32_t pipe_length, uint32_t phase_delay );
      void pipeline_enable( bool pipe_id );
      void pipeline_debug( bool pipe_id, uint32_t debug_flag );
      void pipeline_set_slice( bool pipe_id, uint32_t theSlice ) ;
      void pipeline_set_fiber( bool pipe_id, uint32_t theFiber ) ; 
      void pipeline_set_channel( bool pipe_id, uint32_t theChannel ) ;
      void pipeline_test_set( bool pipe_id, std::string test_hold, uint32_t q_hold, uint32_t q_accept ) ;
      void pipeline_advance( bool pipe_id, uint32_t adv, std::string advance_name ) ;
      void pipeline_delay_setup( bool pipe_id, std::string registerName, uint32_t delay ) ;
      void pipeline_SWL1A( bool pipe_id ) ;

      uint32_t pipeline_Data( bool pipe_id, std::string data_source ) ;
      uint32_t pipeline_debug_check( bool pipe_id ) ;
      uint32_t pipeline_overflow( bool pipe_id ) ; 
      std::map<std::string, uint32_t>  pipelineInfo(bool pipe_id ) ; 
      std::map<std::string, uint32_t > pipeline_status( bool pipe_id ) ;
      std::map<std::string, uint32_t > pipeline_ctrl( bool pipe_id ) ;
      std::map<std::string, uint32_t > pipeline_cnt( bool pipe_id ) ;

      // DRAM Test
      bool dram_test(  int id, int chunk ) ;
      // DDR3 Read and Write
      void ddr3_reset() ;
      void check_ddr3_stat( int check = -1 ) ;
      std::vector<uint32_t> ddr3_read( std::vector<uint32_t> base_addrV ) ;
      void ddr3_write( uint32_t base_addr, std::vector<uint32_t>& buffer ) ;
 

      // **********************
      //  DAQ Functionalities 
      // **********************

      void daq_query();
      void daq_path_setup_query(uint32_t& moduleId, uint32_t& bcOffset, uint32_t& nsamples, uint32_t& npre, uint32_t& n_tp_samples, uint32_t& ntp_pre, uint32_t& pipeline, bool& enabled);
      void daq_path_setup(uint32_t moduleId, uint32_t bcOffset, uint32_t nsamples, uint32_t npre, uint32_t n_tp_samples, uint32_t ntp_pre, uint32_t pipeline);
      void daq_zs_setup(bool nzs, bool markAndPass, uint32_t zs_mask, bool sumByTwo, const std::vector<uint32_t>& thresholds, bool tp_zs_enable, bool tp_zs_soi_only);
      void daq_zs_setup_query(bool& nzs, bool& markAndPass, uint32_t& zs_mask, bool& sumByTwo,  std::vector<uint32_t>& thresholds, bool& tp_zs_enable, bool& tp_zs_soi_only);
      void daq_path_reset();

      static const int N_DAQ_F2B=3;

      void daq_f2b_links_query();
      void daq_f2b_links_reset();
      void daq_f2b_links_status(std::vector<uint32_t>& linkStatus, std::vector<uint32_t>& linkErrors, std::vector<double>& linkErrorRate);
      void daq_f2b_links_data(std::vector<uint64_t>& linkDataVolume, std::vector<double>& linkDataRate);
      std::vector<uint32_t> daq_f2b_links_spy(int ilink);

      void daq_fifo_query(std::vector<uint32_t>& occupancy);


      void info_evn_orn(uint32_t& evn, uint32_t& orn);


      void daq_path_enable(bool enable);
      
      uint32_t daq_path_headerOccupancy(bool peak=false);
      
      uint32_t daq_spy_eventSize(); //< returns zero if there is no event to read.
      void daq_spy_read(std::vector<uint32_t>& buffer, bool reset_after_read=true); //< read the spy buffer

      // ************************* 
      // TRIGGER Functionalities
      // ************************* 
      void trigger_query_lut_type(int group, int& n_addr_bits, int& n_data_bits, int& n_luts, uint32_t& base_address, uint32_t& stride, bool& last_group);
      /// Program LUT for Trigger
      void trigger_program_lut(int group, int index, const std::vector<uint32_t>& image);
      /// Read LUT for Trigger
      void trigger_read_lut(int group, int index, std::vector<uint32_t>& image);
      /// setup the trigger link spy
      void trigger_spy_execute(uint32_t active_mask, uint32_t orbit_delay);
      /// setup the trigger spy read
      void trigger_spy_read(int trigger_link_id,std::vector<uint32_t>& buffer);
      
      void trigger_self_set_thresholds(const std::vector<uint32_t>& thresholds);
      std::vector<uint32_t> trigger_self_get_thresholds();
      /// read the self-trigger histogram
      std::vector<uint32_t> trigger_self_read_histogram(int bit);

      // sfp spy 
      void sfp_spySetup(bool enable_capture_sw, bool enable_capture_dtc, bool orbit_no_wait, int pre, int delay, int adelay);
      void sfp_setup( int link, int TxFPGA = 1 );
      void sfp_query( uint32_t& ctl0, uint32_t& st0, uint32_t& err0, float& clk0,
			  uint32_t& ctl1, uint32_t& st1, uint32_t& err1, float& clk1 ) ;

      void sfp_resets(bool link, bool ec);
      void sfp_spyExecute();
      void sfp_spySetupQuery() ;
      std::vector<uint32_t> sfp_spyRead(int id );
      
      // ************************* 
      // LUMI Functionalities
      // ************************* 
      struct LumiHistogram {
           std::vector<uint32_t> h ;
           std::string type ; 
   	   int index ;
           uint32_t orb_init ;
           uint32_t orb_last ;
           uint32_t overflow ;
	uint32_t lumi_nibble;
	uint32_t lumi_section;
	uint32_t cms_run;
	uint32_t lhc_fill;
      } ;
      // applies to BHM also
      void lumi_link_reset() ; 
      // applies to BHM also
      void lumi_link_status(uint32_t& status, float& lumi_bc0_rate, float& trig_bc0_rate, uint32_t& errors) ;
      // applies to BHM also
      std::vector<uint32_t> lumi_link_spy();

      void lumi_setup( int n_lhc_orb, int n_cms_orb, int lhc_threshold, int cms1_threshold, int cms2_threshold, const std::vector<bool>& mask_lhc, const std::vector<bool>& mask_cms ); 
      void lumi_read_setup(int& n_lhc_orb, int& n_cms_orb, int& lhc_threshold, int& cms1_threshold, int& cms2_threshold, std::vector<bool>& mask, std::vector<bool>& mask_cms ); 
      /// Program LUT for LUMI (ifiber = 0..23, ichan = 0..2)
      void lumi_program_lut(int ifiber, int ichan, const std::vector<uint16_t>& image);
      /// Read LUT for LUMI (ifiber = 0..23, ichan = 0..2)
      void lumi_read_lut(int ifiber, int ichan, std::vector<uint16_t>& image);
      
      void lumi_histo_status( std::map<std::string, bool>& hAvailable, std::map<std::string, bool>& hOverflow ) ; 
      int lumi_histo_overflow( int monitoringhist ) ; 
      void lumi_read_histogram( const std::string& hName, LumiHistogram& ilumi ) ; 

      // ************************* 
      // BHM Functionalities
      // ************************* 
      static const int BHM_N_THRESHOLDS=24;
      static const int BHM_N_TDCMAP=64;
      void bhm_setup(int n_orb, const std::vector<uint8_t>& thresholds, const std::vector<uint8_t>& tdcmap);
      void bhm_read_setup(int& n_orb, std::vector<uint8_t>& thresholds, std::vector<uint8_t>& tdcmap);
      void bhm_histo_status( std::vector<bool>& hAvailable, std::vector<bool>& hOverflow);
      void bhm_read_histograms(std::vector<LumiHistogram>& histos);
      void bhm_ampl_histo_control(bool run, bool clear);
      std::vector<uint32_t> bhm_ampl_histo_read(int ichannel,int tbin);


      // DTC Info
      void dtc_reset(bool full, bool errors);
      void get_dtc_info( bool front, float& dtc_rate_40mhz, float& dtc_rate_orbit, uint32_t& dtc_bunch_count, uint32_t& dtc_event_number, uint32_t& dtc_bc0_error, uint32_t& dtc_single_error, uint32_t& dtc_double_error, uint32_t& dtc_ttc_stream_phase, uint32_t& dtc_ttc_stream_phase_locked ) ;
      void setDTCPhase(bool front, bool override, int phase, int threshold);

      // I2C access point
      void i2cStart(const std::vector<uint32_t>& cmds);
      bool i2cDone();
      std::vector<uint32_t> i2cResults();
      uHTRsensors* sensors() { return sensors_; }

      //orm
      void get_orm_info(uint32_t& orm_cmd_queued,  uint32_t& orm_start_count, uint32_t& orm_stop_count, uint32_t& orm_cmd_count);
      void orm_control(bool orm_start, bool orm_stop, bool orm_send, uint32_t orm_cmd);

      //event/run info
      void get_run_info( bool front, uint32_t& lumi_nibble,  uint32_t& lumi_section, uint32_t& cms_run, uint32_t& lhc_fill);

      static const char* LUMI_HISTONAMES[] ; 

    protected:
      void dispatch();
      friend class uHTRsensors;
      uhal::HwInterface* u_hw;
      uhal::Node &u_front, &u_back ; 

      int crate_id;
      int slot_id;
      std::string card_ip ;
      std::string card_name;
      uint32_t pcb_revision, serial_number;
      mutable uint32_t theFirmwareRevFront,theFirmwareRevBack;
      int n_channels_per_fiber_; // depends on firmware version

    private:

      bool SPI_Status( int delay_us = 10000,  bool front =false) ;

      std::string card_hostname ;
      uint32_t flashCode;
      //void readFlashCode();

      uint32_t clk_rates[16];

      toolbox::RateTool* rateCalc ;
      uHTRsensors* sensors_;

      struct LinkInfo {
	//uint32_t rx_link_error_count;
	double rx_link_error_rate;
        uint32_t badDataCounterOn ;
	uint32_t rx_los_count;
        uint64_t n_BadData ;
        uint64_t n_BadData_rollovers ;
        uint32_t n_BadAlign ;
        uint32_t align_status ;
        uint32_t orbit_rate ;
        uint32_t align_occupancy ;
	uint32_t align_delta;

        uint32_t reset_done ;
        uint32_t pll_lock ;
        uint32_t byte_aligned ;
        toolbox::RateTool badDataRate  ;
      } ;

      LinkInfo m_fibers[NFIBERS] ;

      struct DaqInfo {
           uint32_t moduleID ;
           uint32_t orbit_Num ;
           uint32_t bcOffset ;
           uint32_t nsamples ;
           uint32_t npre ;
           uint32_t pipeline ;
           bool     formatter_enable ;
           uint32_t lastEvent ;
           uint32_t zsThreshold[128] ;
           uint32_t zsMask ;

	uint32_t n_tp_samples;
	uint32_t n_tp_presamples;

           bool     zs_disable ;
           bool     zs_MarkNPass ;
           bool     zs_sumbytwo ;
  	   bool     zs_tp_enable;
           bool     zs_tp_soi_only;

	// related to the link
	uint32_t f2b_status[N_DAQ_F2B];
	uint32_t f2b_errors[N_DAQ_F2B];
	toolbox::RateTool f2b_error_rate[N_DAQ_F2B];
	uint64_t f2b_words[N_DAQ_F2B];
	toolbox::RateTool f2b_data_rate[N_DAQ_F2B];
      } iDaq ;


      // variables for lumi      
      int n_LHC_Orbit ;
      int n_CMS_Orbit ;

      // protected functions
      void readFlashCode( bool front );
    };
  }
}


#endif // HCAL_UHTR_UHTR_H_INCLUDED
