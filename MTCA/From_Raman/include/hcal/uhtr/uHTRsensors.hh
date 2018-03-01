#ifndef HCAL_UHTR_UHTRSENSORS_H_INCLUDED
#define HCAL_UHTR_UHTRSENSORS_H_INCLUDED 1

#include <string>
#include <vector>
#include <stdint.h>
#include <map>
#include <uhal/ValMem.hpp>


namespace uhal {
  class Node;
}

namespace hcal {
  namespace uhtr {

    class uHTR;

    class uHTRsensors {
    public:
      enum SubBus { Unknown=0, 
		    pm_1VA=1, pm_1VB=2, pm_3_3V=3, apm_1_8V=4, apm_2_5V=5, 
		    ppod_rx0=6, ppod_rx1=7, ppod_tx=8, 
		    sfp0=9, sfp1=10,
		    jtag_mezz=11, flash_mezz=12, cpld_mezz=13, 
		    back_sysmon=14, front_sysmon=15, mac_id_prom=16};
      enum Target { mac_eeprom, adc, ppod, sfp, sfp_ctl };

      static SubBus str2SB(const std::string& str);
      static std::string SB2str(SubBus sb);

      void setMuxes( SubBus sb );
      
      std::vector<uint8_t> read(Target t, int words, const std::vector<uint8_t>& wwords);
      void write(Target t, const std::vector<uint8_t>& wwords);

      void readSensorsPM(double& tempC, double& Vout);
      void readSensorsPPOD(double& tempC, double& V1, double& V2);
      void readPPOD_ADC( double& V25, double& V33);
      void readSensorsFPGA(SubBus sb, double& tempC, double& V1, double& V2);
      void readSensorsSFP(double& tempC, double& Vout);

      struct MezzIdStruct {
	uint8_t macid[6];
	uint8_t data_format_version; // this is version 1
	uint8_t mezz_type_code;      // see codes in the sections below
	uint8_t mezz_subtype_code;   // see codes in the sections below
	uint8_t mezz_type[16];       // String version of mezzanine type+sub type

	uint8_t serial_number[2];    // construction serial number ([0]+256*[1])
	uint8_t manu_date[11];       // DAY-MON-YEAR (zero-terminated)
	uint8_t manu_site[16];       // Site of manufacture (zero-terminated)
	uint8_t manu_tester[16];     // Name of tester (zero-terminated)
	uint8_t test_release[8];     // Release version of test code used
	uint8_t notes[56];           // String notes field in this version,
      };
      MezzIdStruct readId(SubBus sb);
      bool writeId(SubBus sb,const MezzIdStruct& id, bool force=false);

      //      void writeTarget(SubBus sb, Target t, const std::vector<uint8_t>& words)
      
      //      void dispatch();
      //bool done();      

      uHTRsensors(uHTR* theUHTR, uhal::Node& sensors) : uhtr_(theUHTR), u_sense(sensors), i2cPrescale_(0xF0) { }
      //uHTRsensors(uHTR* theUHTR, uhal::Node& sensors) ;

      void setI2Cprescale(uint32_t val) { i2cPrescale_=val; }
      uint32_t i2cPrescale() const { return i2cPrescale_; }
    private:
      bool dispatch_and_wait(bool ackExpect=true);
      uHTR* uhtr_;
      uhal::Node& u_sense;
      bool done_;
      std::vector<uint32_t> stack_;
      uint16_t nextHandle_;
      std::map<uint16_t, uhal::ValVector<uint8_t> > vectors_;
      std::map<uint16_t, uhal::ValWord<uint8_t> > singles_;

      bool isNewUHTR ;
      void muxLayer0(SubBus sb, std::vector<uint32_t>& istack) const;
      void muxLayer1(SubBus sb, std::vector<uint32_t>& istack) const;
      int muxLayer0(SubBus sb) const;
      int muxLayer1(SubBus sb) const;
      static int targetAddr(Target t);

      uint32_t i2cPrescale_;

    };


  }
}


#endif // HCAL_UHTR_UHTRSENSORS_H_INCLUDED 
