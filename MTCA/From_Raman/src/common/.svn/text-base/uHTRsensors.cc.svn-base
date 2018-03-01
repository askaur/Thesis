#include "hcal/uhtr/uHTRsensors.hh"
#include "hcal/uhtr/uHTR.hh"
#include <string.h>
#include "uhal/uhal.hpp"

namespace hcal {
  namespace uhtr {

    static const int MUX_LAYER0_ADDR = 0x70;
    static const int MUX_LAYER1_ADDR = 0x71;

    static const uint32_t PCB_REV_9545 = 0x00030003;

    uHTRsensors::SubBus uHTRsensors::str2SB(const std::string& str) {
      if (!strcasecmp(str.c_str(),"pm_1VA")) return pm_1VA;
      if (!strcasecmp(str.c_str(),"pm_1VB")) return pm_1VB;
      if (!strcasecmp(str.c_str(),"pm_3_3V")) return pm_3_3V;
      if (!strcasecmp(str.c_str(),"apm_1_8V")) return apm_1_8V;
      if (!strcasecmp(str.c_str(),"apm_2_5V")) return apm_2_5V;
      if (!strcasecmp(str.c_str(),"ppod_rx0")) return ppod_rx0;
      if (!strcasecmp(str.c_str(),"ppod_rx1")) return ppod_rx1;
      if (!strcasecmp(str.c_str(),"ppod_tx")) return ppod_tx;
      if (!strcasecmp(str.c_str(),"sfp0")) return sfp0;
      if (!strcasecmp(str.c_str(),"sfp1")) return sfp1;
      if (!strcasecmp(str.c_str(),"cpld_mezz")) return cpld_mezz;
      if (!strcasecmp(str.c_str(),"jtag_mezz")) return jtag_mezz;
      if (!strcasecmp(str.c_str(),"flash_mezz")) return flash_mezz;
      if (!strcasecmp(str.c_str(),"back_sysmon")) return back_sysmon;
      if (!strcasecmp(str.c_str(),"front_sysmon")) return front_sysmon;
      if (!strcasecmp(str.c_str(),"mac_id_prom")) return mac_id_prom;
      return Unknown;
    }
    std::string uHTRsensors::SB2str(SubBus sb) {
      switch (sb) {
      case (pm_1VA) : return "pm_1VA";
      case (pm_1VB) : return "pm_1VB";
      case (pm_3_3V) : return "pm_3_3V";
      case (apm_1_8V) : return "apm_1_8V";
      case (apm_2_5V) : return "apm_2_5V";
      case (ppod_rx0) : return "ppod_rx0";
      case (ppod_rx1) : return "ppod_rx1";
      case (ppod_tx) : return "ppod_tx";
      case (sfp0) : return "sfp0";
      case (sfp1) : return "sfp1";
      case (cpld_mezz) : return "cpld_mezz";
      case (flash_mezz) : return "flash_mezz";
      case (jtag_mezz) : return "jtag_mezz";
      case (back_sysmon) : return "back FPGA (sysmon)";
      case (front_sysmon) : return "front FPGA (sysmon)";
      case (mac_id_prom) : return "MAC/id PROM";
      default : return "Unknown";
      }
    }
    int uHTRsensors::muxLayer0(SubBus sb) const {
      if (uhtr_->pcbRevision()<PCB_REV_9545) {
	switch (sb) {
	case (pm_1VA) : return 0x4|0;
	case (pm_1VB) : return 0x4|0;
	case (apm_1_8V) : return 0x4|0;
	case (apm_2_5V) : return 0x4|0;
	case (pm_3_3V) : return 0x4|1;
	case (ppod_rx0) : return 0x4|1;
	case (ppod_rx1) : return 0x4|1;
	case (ppod_tx) : return 0x4|1;
	case (sfp0) : return 0x4|2;
	case (sfp1) : return 0x4|2;
	case (jtag_mezz) : return 0x4|2;
	case (flash_mezz) : return 0x4|2;
	default : return 0x4|0;
	}
      } else {
	switch (sb) {
	case (pm_1VA) : return 1;
	case (pm_1VB) : return 1;
	case (apm_1_8V) : return 1;
	case (apm_2_5V) : return 1;
	case (pm_3_3V) : return 2;
	case (ppod_rx0) : return 2;
	case (ppod_rx1) : return 2;
	case (ppod_tx) : return 2;
	case (sfp0) : return 3;
	case (sfp1) : return 3;
	case (jtag_mezz) : return 3;
	case (flash_mezz) : return 3;
	default : return 0;
	}
      }
    }
    int uHTRsensors::muxLayer1(SubBus sb) const {
      if (uhtr_->pcbRevision()<PCB_REV_9545) {
	switch (sb) {
	case (pm_1VA) : return 0x4|0;
	case (pm_1VB) : return 0x4|1;
	case (apm_1_8V) : return 0x4|2;
	case (apm_2_5V) : return 0x4|3;
	case (pm_3_3V) : return 0x4|0;
	case (ppod_tx) : return 0x4|1;
	case (ppod_rx0) : return 0x4|2;
	case (ppod_rx1) : return 0x4|3;
	case (sfp0) : return 0x4|0;
	case (sfp1) : return 0x4|1;
	case (flash_mezz) : return 0x4|2;
	case (jtag_mezz) : return 0x4|3;
	default : return 0;
	}
      } else {
	switch (sb) {
	case (pm_1VA) : return 1;
	case (pm_1VB) : return 2;
	case (apm_1_8V) : return 4;
	case (apm_2_5V) : return 8;
	case (pm_3_3V) : return 1;
	case (ppod_tx) : return 2;
	case (ppod_rx0) : return 4;
	case (ppod_rx1) : return 8;
	case (sfp0) : return 1;
	case (sfp1) : return 2;
	case (flash_mezz) : return 4;
	case (jtag_mezz) : return 8;
	default : return 0;
	}
      }
    }
    int uHTRsensors::targetAddr(Target t) {
      if (t==mac_eeprom) return 0x50;
      if (t==adc) return 0x68;
      if (t==sfp_ctl) return 0x51;
      if (t==sfp) return 0x50;
      if (t==ppod) return 0x28;
      return 0;
    }


    static const int I2C_PRE0 = 0;
    static const int I2C_PRE1 = 1;
    static const int I2C_CTR = 2;
    static const int I2C_TXR = 3;
    static const int I2C_RXR = 3;
    static const int I2C_CR  = 4;
    static const int I2C_SR  = 4;

    static const int I2C_STA = 0x80;
    static const int I2C_STO = 0x40;
    static const int I2C_RD  = 0x20;
    static const int I2C_WR  = 0x10;
    static const int I2C_NACK= 0x08;
    static const int I2C_IACK= 0x01;

    static uint32_t i2c_write(int addr, int value) { return 0x1000|((addr&0xF)<<8)|(value&0xFF); }
    //    static uint32_t i2c_writewait(int addr, int value) { return 0x3000|((addr&0xF)<<8)|(value&0xFF); }
    static uint32_t i2c_write_addr(int waddr) { return 0x1000|(I2C_TXR<<8)|((waddr&0x7F)<<1); }
    static uint32_t i2c_read_addr(int waddr) { return 0x1000|(I2C_TXR<<8)|((waddr&0x7F)<<1)|1; }
    static uint32_t i2c_command(int cmd,bool wait=true) { return 0x1000|(I2C_CR<<8)|(cmd&0xFF)|I2C_IACK|(wait?(0x2000):(0)); }
    static uint32_t i2c_read(uint16_t handle=0) { return (handle<<16)|(I2C_RXR<<8); }
    static uint32_t i2c_status() { return (I2C_SR<<8); }

    /*
    uhal::ValWord<uint8_t> uHTRsensors::readTarget(SubBus sb, Target t, const std::vector<uint8_t>& wwords) {
      if (stack_.empty()) stack_.push_back(i2c_write(I2C_CTR,0xC0)); // enable I2C and interrupts
      // set LAYER 0 
      muxLayer0(sb,stack_);
      // set LAYER 1
      muxLayer1(sb,stack_);

      if (!wwords.empty()) {
	stack_.push_back(i2c_write_addr(targetAddr(sb,t)));
	stack_.push_back(i2c_command(I2C_STA|I2C_WR)); // send the address
	for (size_t i=0; i+1<wwords.size(); i++) {
	  stack_.push_back(i2c_write(I2C_TXR,wwords[i]));
	  stack_.push_back(i2c_command(I2C_WR)); // send the data
	}
	stack_.push_back(i2c_write(I2C_TXR,wwords[wwords.size()-1]));
	stack_.push_back(i2c_command(I2C_STO|I2C_WR)); // send the data
      }

      stack_.push_back(i2c_read_addr(targetAddr(sb,t)));
      stack_.push_back(i2c_command(I2C_STA|I2C_WR)); // send the address
      stack_.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK));
      stack_.push_back(i2c_read(nextHandle_));
      
      uhal::ValWord<uint8_t> vw;
      singles_.insert(std::pair<uint16_t, uhal::ValWord<uint8_t> >(nextHandle_,vw));
      nextHandle_++;
      return vw;
    }

    uhal::ValWord<uint8_t> uHTRsensors::readTarget(SubBus sb, Target t) {
      std::vector<uint8_t> empty;
      return readTarget(sb,t,empty);
    }

    uhal::ValVector<uint8_t> uHTRsensors::readTarget(SubBus sb, Target t, int words) {
      std::vector<uint8_t> empty;
      return readTarget(sb,t,words,empty);
    }

    uhal::ValVector<uint8_t> uHTRsensors::readTarget(SubBus sb, Target t, int words, const std::vector<uint8_t>& wwords) {
      if (stack_.empty()) stack_.push_back(i2c_write(I2C_CTR,0xC0)); // enable I2C and interrupts
      // set LAYER 0 
      muxLayer0(sb,stack_);
      // set LAYER 1
      muxLayer1(sb,stack_);

      if (!wwords.empty()) {
	stack_.push_back(i2c_write_addr(targetAddr(sb,t)));
	stack_.push_back(i2c_command(I2C_STA|I2C_WR)); // send the address
	for (size_t i=0; i+1<wwords.size(); i++) {
	  stack_.push_back(i2c_write(I2C_TXR,wwords[i]));
	  stack_.push_back(i2c_command(I2C_WR)); // send the data
	}
	stack_.push_back(i2c_write(I2C_TXR,wwords[wwords.size()-1]));
	stack_.push_back(i2c_command(I2C_STO|I2C_WR)); // send the data
      }

      stack_.push_back(i2c_read_addr(targetAddr(sb,t)));
      stack_.push_back(i2c_command(I2C_STA|I2C_WR)); // send the address
      for (int i=0; i<(words-1); i++) {
	stack_.push_back(i2c_command(I2C_RD));
	stack_.push_back(i2c_read(nextHandle_));
      }
      stack_.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK));
      stack_.push_back(i2c_read(nextHandle_));
      
      uhal::ValVector<uint8_t> vv;
      vectors_.insert(std::pair<uint16_t, uhal::ValVector<uint8_t> >(nextHandle_,vv));
      nextHandle_++;
      return vv;
    }

    void uHTRsensors::writeTarget(SubBus sb, Target t, uint8_t word) {
      if (stack_.empty()) stack_.push_back(i2c_write(I2C_CTR,0xC0)); // enable I2C and interrupts
      // set LAYER 0 
      muxLayer0(sb,stack_);
      // set LAYER 1
      muxLayer1(sb,stack_);
      stack_.push_back(i2c_write_addr(targetAddr(sb,t)));
      stack_.push_back(i2c_command(I2C_STA|I2C_WR)); // send the address
      stack_.push_back(i2c_write(I2C_TXR,word));
      stack_.push_back(i2c_command(I2C_STO|I2C_WR));      
    }
    
    void uHTRsensors::dispatch() {
      if (!stack_.empty()) {
	stack_.push_back(i2c_write(I2C_CTR,0)); // disable the I2C when done
	uhtr_->i2cStart(stack_);
	stack_.clear();
	done_=false;
      }
    }
    bool uHTRsensors::done() {
      if (done_) return true;
      if (uhtr_->i2cDone()) {
	std::vector<uint32_t> res=uhtr_->i2cResults();
	for (std::vector<uint32_t>::const_iterator i=res.begin(); i!=res.end(); i++) {
	  uint16_t handle=(*i>>16);
	  std::map<uint16_t, uhal::ValVector<uint8_t> >::iterator iv=vectors_.find(handle);
	  std::map<uint16_t, uhal::ValWord<uint8_t> >::iterator is=singles_.find(handle);
	  if (iv!=vectors_.end()) {
	    iv->second.push_back(uint8_t(*i&0xFF));
	  } else if (is!=singles_.end()) {
	    is->second=uint8_t(*i&0xFF);
	    is->second.valid(true);
	  }	  
	}
	//
	singles_.clear();
	for (std::map<uint16_t, uhal::ValVector<uint8_t> >::iterator iv=vectors_.begin();
	     iv!=vectors_.end(); iv++) iv->second.valid(true);
	vectors_.clear();

	done_=true;
	return true;
      }      
      return false;
    }
    */

    void uHTRsensors::muxLayer0(SubBus sb,std::vector<uint32_t>& istack) const {
      istack.push_back(i2c_write_addr(MUX_LAYER0_ADDR)); // addr for MUX layer 0
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_write(I2C_TXR,muxLayer0(sb))); // value for MUX layer 0
      istack.push_back(i2c_command(I2C_STO|I2C_WR)); // start the write and generate a stop

      istack.push_back(i2c_read_addr(MUX_LAYER0_ADDR)); // addr for MUX layer 0
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_read(0xbe0)); // value for MUX layer 0
      istack.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK)); // start the write and generate a stop

      istack.push_back(i2c_read_addr(MUX_LAYER1_ADDR)); // addr for MUX layer 0
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_read(0xbe1)); // value for MUX layer 0
      istack.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK)); // start the write and generate a stop

      istack.push_back(i2c_read_addr(MUX_LAYER0_ADDR)); // addr for MUX layer 0
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_read(0xbe2)); // value for MUX layer 0
      istack.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK)); // start the write and generate a stop
    }

    void uHTRsensors::muxLayer1(SubBus sb,std::vector<uint32_t>& istack) const {
      istack.push_back(i2c_write_addr(MUX_LAYER1_ADDR)); // addr for MUX layer 1
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_write(I2C_TXR,muxLayer1(sb))); // value for MUX layer 1
      istack.push_back(i2c_command(I2C_STO|I2C_WR)); // start the write and generate a stop

      istack.push_back(i2c_read_addr(MUX_LAYER1_ADDR)); // addr for MUX layer 0
      istack.push_back(i2c_command(I2C_STA|I2C_WR)); // start the write
      istack.push_back(i2c_read(0xbe0)); // value for MUX layer 0
      istack.push_back(i2c_command(I2C_STO|I2C_RD|I2C_NACK)); // start the write and generate a stop
    }

    bool uHTRsensors::dispatch_and_wait(bool ackExpect) {
      uhtr_->dispatch();
      bool done=false;
      int tries=100;
      do {
	u_sense.getNode("DIRECT").write(i2c_status());
	u_sense.getNode("DIRECT_START").write(1);
	uhal::ValWord<uint32_t> rw=u_sense.getNode("DIRECT.DATA_IN").read();
	uhtr_->dispatch();
	if (!(rw.value()&0x2)) done=true;
	else if (tries<5) {
	  //	  printf("need to wait (0x%x)\n",rw.value());
	 //	  ::sleep(1);
	}
	if (ackExpect && done && (rw.value()&0x80)) {
	  fprintf(stderr,"uHTR I2C transaction problem : no acknowledge from targetted device\n");
	  return false;
	}
	tries--;
      } while (!done && tries>0);
      if (!done) printf("Timed out\n");
      return done;
    }

    std::vector<uint8_t> uHTRsensors::read(Target t, int words, const std::vector<uint8_t>& wwords) {
      std::vector<uint8_t> rv, rz;
      uint8_t p0, p1;
      p0=uint8_t(i2cPrescale_);
      p1=uint8_t(i2cPrescale_>>8);
      u_sense.getNode("DIRECT").write(i2c_write(I2C_PRE0,p0));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();
      u_sense.getNode("DIRECT").write(i2c_write(I2C_PRE1,p1));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();

      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x80));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();

      if (!wwords.empty()) {
	u_sense.getNode("DIRECT").write(i2c_write_addr(targetAddr(t)));
	u_sense.getNode("DIRECT_START").write(1);
	u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
	u_sense.getNode("DIRECT_START").write(1);
	if (!dispatch_and_wait()) {
	  u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
	  u_sense.getNode("DIRECT_START").write(1);
	  uhtr_->dispatch();
	  return rz;
	}

	for (size_t i=0; i<wwords.size(); i++) {
	  u_sense.getNode("DIRECT").write(i2c_write(I2C_TXR,wwords[i]));
	  u_sense.getNode("DIRECT_START").write(1);
	  u_sense.getNode("DIRECT").write(i2c_command(I2C_WR,false));
	  u_sense.getNode("DIRECT_START").write(1);
	  if (!dispatch_and_wait()) {
	    u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
	    u_sense.getNode("DIRECT_START").write(1);
	    uhtr_->dispatch();
	    return rz;
	  }
	}	
      }      

      u_sense.getNode("DIRECT").write(i2c_read_addr(targetAddr(t)));
      u_sense.getNode("DIRECT_START").write(1);
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
      u_sense.getNode("DIRECT_START").write(1);
      if (!dispatch_and_wait()) {
	u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
	u_sense.getNode("DIRECT_START").write(1);
	uhtr_->dispatch();
	return rz;
       }
      for (int i=0; i<words; i++) {
	if (i+1==words)
	  u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_RD|I2C_NACK,false));
	else
	  u_sense.getNode("DIRECT").write(i2c_command(I2C_RD,false));
	u_sense.getNode("DIRECT_START").write(1);
	if (!dispatch_and_wait((i+1)!=words)) {
          u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
          u_sense.getNode("DIRECT_START").write(1);
          uhtr_->dispatch();
          return rz;
 	}
	u_sense.getNode("DIRECT").write(i2c_read());
	u_sense.getNode("DIRECT_START").write(1);
	uhal::ValWord<uint32_t> rw=u_sense.getNode("DIRECT.DATA_IN").read();
	uhtr_->dispatch();	
	rv.push_back(uint8_t(rw.value()));
      }
      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();
      return rv;
    }

    void uHTRsensors::write(Target t, const std::vector<uint8_t>& wwords) {
      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x80));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();

      u_sense.getNode("DIRECT").write(i2c_write_addr(targetAddr(t)));
      u_sense.getNode("DIRECT_START").write(1);
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
      u_sense.getNode("DIRECT_START").write(1);

      if (dispatch_and_wait()) {
      
	for (size_t i=0; i<wwords.size(); i++) {
	  u_sense.getNode("DIRECT").write(i2c_write(I2C_TXR,wwords[i]));
	  u_sense.getNode("DIRECT_START").write(1);
	  if (i+1==wwords.size())
	    u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_WR,false));
	  else 
	    u_sense.getNode("DIRECT").write(i2c_command(I2C_WR,false));
	  u_sense.getNode("DIRECT_START").write(1);
	  dispatch_and_wait();	 
	}	
      }

      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();
    }

    void uHTRsensors::readSensorsPM(double& temp, double& Vout) {
      temp=-100;
      Vout=-100;
      // assuming the muxes are ok...
      std::vector<uint8_t> cmd(1), res, empty;
      cmd[0]=0x18; // command to 16-bit/single

      write(adc,cmd);
      cmd[0]=0x98; // start the conversion

      write(adc,cmd);

      // wait for conversion
      do {
	::usleep(10000);
	res=read(adc,3,empty);
	if (res.empty()) return;
      } while (res[2]&0x80);
   
      temp=res[0]*256+res[1];
      temp*=2048.0/32768; // counts to millivolts
      // 10 mV/C and 750 mV at 25C
      temp=(temp-750)/10+25;
   
      cmd[0]=0x38; // command to 16-bit/single/chan 1

      write(adc,cmd);
      cmd[0]=0xB8; // start the conversion

      write(adc,cmd);

      // wait for conversion
      do {
	::usleep(10000);
	res=read(adc,3,empty);
	if (res.empty()) return;
      } while (res[2]&0x80);
      
      Vout=res[0]*256+res[1];
      Vout*=2.048/32768;
      Vout*=(392+118)/118.0;
    }

    void uHTRsensors::readSensorsSFP(double& temp, double& Vout) {
      temp=-100;
      Vout=-100;

      // assuming the muxes are ok..
      std::vector<uint8_t> cmd(1), res;
      cmd[0]=96; 
      res=read(sfp_ctl,4,cmd);

      temp=res[0]+res[1]/256.0;
   
      Vout=res[2]*256+res[3];
      Vout/=10000;
    }

    void uHTRsensors::readSensorsPPOD(double& temp, double& V1, double& V2) {
      // assuming the muxes are ok...
      std::vector<uint8_t> cmd(1), res;
      cmd[0]=28; 

      res=read(ppod,8,cmd);
      if (res.empty()) {
	temp=-100;
	V1=-100;
	V2=-100;
	return;
      }
   
      temp=res[0]+res[1]/256.0;
      V1=(res[4]*256+res[5])*100e-6;
      V2=(res[6]*256+res[7])*100e-6;
    }

    void uHTRsensors::readPPOD_ADC( double& V25, double& V33 ) {
   
      // assuming the muxes are ok...
      std::vector<uint8_t> cmd(1), res, empty;
      cmd[0]=0x18; // command to 16-bit for channel 1
      write(adc,cmd);

      cmd[0]=0x98; // start the conversion
      write(adc,cmd);

      // wait for conversion
      do {
	::usleep(10000);
	res=read(adc,3,empty);
      } while (res[2]&0x80);

      V25= (res[0] << 8) | res[1] ;
      V25*=2.048/32768;
      V25*=2.;
      res.clear() ;

      cmd[0]=0x38; // command to 16-bit for channel 2
      write(adc,cmd);

      cmd[0]=0xB8; // start the conversion
      write(adc,cmd);

      do {
	::usleep(10000);
	res=read(adc,3,empty);
      } while (res[2]&0x80);
      V33= (res[0] << 8) | res[1] ;

      V33*=2.048/32768;
      V33*=2.;
    }

    void uHTRsensors::readSensorsFPGA(SubBus sb, double& tempC, double& V1, double& V2) {
      if (sb!=back_sysmon && sb!=front_sysmon) return;

      uhal::Node& n=(sb==back_sysmon)?(uhtr_->u_back):(uhtr_->u_front);

      tempC=0;
      V1=0;
      V2=0;

      const int cycles=4;

      for (int i=0; i<cycles; i++) {
	n.getNode("SYSMON.TEMPCORE").read();
	uhal::ValWord<uint32_t> at=n.getNode("SYSMON.TEMPCORE").read();

	n.getNode("SYSMON.VOLTCORE").read();
	uhal::ValWord<uint32_t> av1=n.getNode("SYSMON.VOLTCORE").read();

	n.getNode("SYSMON.VOLTAUX").read();
	uhal::ValWord<uint32_t> av2=n.getNode("SYSMON.VOLTAUX").read();

	uhtr_->dispatch();

	tempC+=at.value()/64;
	V1+=av1.value()/64;
	V2+=av2.value()/64;       
      }

      V1/=4;
      V2/=4;
      tempC/=4;

      tempC=(tempC*503.975)/(1024)-273.15;

      V1*=3.0/1024;
      V2*=3.0/1024;

    }

    void uHTRsensors::setMuxes( SubBus sb ) {
      // set the divider
      u_sense.getNode("DIRECT").write(i2c_write(0,255));
      u_sense.getNode("DIRECT_START").write(1);

      // enable the I2C
      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x80));
      u_sense.getNode("DIRECT_START").write(1);

      int tries=3;
      do {

	// set mux layer 0
	u_sense.getNode("DIRECT").write(i2c_write_addr(MUX_LAYER0_ADDR));
	u_sense.getNode("DIRECT_START").write(1);
	u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
	u_sense.getNode("DIRECT_START").write(1);
	dispatch_and_wait();
	
	u_sense.getNode("DIRECT").write(i2c_write(I2C_TXR,muxLayer0(sb)));
	u_sense.getNode("DIRECT_START").write(1);
	u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_WR,false));
	u_sense.getNode("DIRECT_START").write(1);
	dispatch_and_wait();
	
	
	// read mux layer 0
	u_sense.getNode("DIRECT").write(i2c_read_addr(MUX_LAYER0_ADDR));
	u_sense.getNode("DIRECT_START").write(1);
	u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
	u_sense.getNode("DIRECT_START").write(1);
	dispatch_and_wait();
	
	u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_RD|I2C_NACK,false));
	u_sense.getNode("DIRECT_START").write(1);
	dispatch_and_wait(false);
	u_sense.getNode("DIRECT").write(i2c_read());
	u_sense.getNode("DIRECT_START").write(1);
	uhal::ValWord<uint32_t> rv=u_sense.getNode("DIRECT.DATA_IN").read();
	uhtr_->dispatch();

	if (int(rv.value()&0xF)!=int(muxLayer0(sb))) {
	  if (tries==1) printf("Layer 0 : asked for %x , found %x\n",muxLayer0(sb),rv.value());
	} else break;
	tries--;
      } while (tries>0);

      // set mux layer 1
      u_sense.getNode("DIRECT").write(i2c_write_addr(MUX_LAYER1_ADDR));
      u_sense.getNode("DIRECT_START").write(1);
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
      u_sense.getNode("DIRECT_START").write(1);
      dispatch_and_wait();


      u_sense.getNode("DIRECT").write(i2c_write(I2C_TXR,muxLayer1(sb)));
      u_sense.getNode("DIRECT_START").write(1);
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_WR,false));
      u_sense.getNode("DIRECT_START").write(1);
      dispatch_and_wait();

      
      // read mux layer 0
      u_sense.getNode("DIRECT").write(i2c_read_addr(MUX_LAYER1_ADDR));
      u_sense.getNode("DIRECT_START").write(1);
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STA|I2C_WR,false));
      u_sense.getNode("DIRECT_START").write(1);
      dispatch_and_wait();
      u_sense.getNode("DIRECT").write(i2c_command(I2C_STO|I2C_RD|I2C_NACK,false));
      u_sense.getNode("DIRECT_START").write(1);
      dispatch_and_wait(false);    
      u_sense.getNode("DIRECT").write(i2c_read());
      u_sense.getNode("DIRECT_START").write(1);
      uhal::ValWord<uint32_t> rv2=u_sense.getNode("DIRECT.DATA_IN").read();
      uhtr_->dispatch();
      if (int(rv2.value()&0xF)!=int(muxLayer1(sb)))
	printf("Layer 1 : asked for %x , found %x\n",muxLayer1(sb),rv2.value());

      u_sense.getNode("DIRECT").write(i2c_write(I2C_CTR,0x00));
      u_sense.getNode("DIRECT_START").write(1);
      uhtr_->dispatch();
    }


    uHTRsensors::MezzIdStruct uHTRsensors::readId(uHTRsensors::SubBus sb) {
      MezzIdStruct retval;

      if (sb==cpld_mezz)  { // special, not I2C but SPI
	std::vector<unsigned char> di, dout;
	di.push_back(0x3);
	di.push_back(0xFA);
	uhtr_->clockSPI(3,di,dout,sizeof(uHTRsensors::MezzIdStruct));
	memcpy(&retval,&(dout[0]),sizeof(MezzIdStruct));	
      } else if (sb==mac_id_prom)  { // special, not I2C but SPI
	std::vector<unsigned char> di, dout;
	di.push_back(0x3);
	di.push_back(0xFA);
	uint8_t buffer[sizeof(MezzIdStruct)];
	for (size_t i=0; i<sizeof(MezzIdStruct); i+=4) {
	  di[1]=(0xFA+i)%256;
	  if (uhtr_->idSPI(di,dout,4)) 
	    for (int j=0; j<4; j++)
	      buffer[i+j]=dout[j];
	}
	memcpy(&retval,buffer,sizeof(MezzIdStruct));	
      } else {
	setMuxes(sb);/*
	printf("Sleep\n");
	sleep(1);
	printf("proceed\n");*/
	std::vector<uint8_t> cmd(1,0xFA);
	std::vector<uint8_t> rv=read(mac_eeprom,sizeof(uHTRsensors::MezzIdStruct),cmd);
	if (rv.size()>=sizeof(MezzIdStruct)) 
	  memcpy(&retval,&(rv[0]),sizeof(MezzIdStruct));
      }
      return retval;
    }


    bool uHTRsensors::writeId(uHTRsensors::SubBus sb, const uHTRsensors::MezzIdStruct& id, bool force) {
      MezzIdStruct current=readId(sb);
      if (current.data_format_version!=0xFF && !force) { // already programmed?
	return false;
      }

      const uint8_t* buf=(const uint8_t*)(&id);
      
      if (sb==cpld_mezz)  { // special, not I2C but SPI
	std::vector<unsigned char> di, dout;
	for (size_t i=6; i<sizeof(MezzIdStruct); i+=4) {
	  di.clear();
	  di.push_back(0x6); // WREN
	  uhtr_->clockSPI(3,di,dout,0);
	  di.clear();
	  di.push_back(0x2); // WR
	  di.push_back(uint8_t(i-6)); // ADDR
	  di.push_back(buf[i]);
	  di.push_back(buf[i+1]);
	  di.push_back(buf[i+2]);
	  di.push_back(buf[i+3]);
	  uhtr_->clockSPI(3,di,dout,0);
	  

	  di.clear();
	  di.push_back(0x5); // RDSR
	  do {
	    uhtr_->clockSPI(3,di,dout,1);
	    // waiting for WIP to fall
	  } while (dout[0]&0x1);
	}
      } else if (sb==mac_id_prom)  { // special, not I2C but SPI
	std::vector<unsigned char> di, dout;
	for (size_t i=6; i<sizeof(MezzIdStruct); i++) {
	  bool ok=false;
	  do {
	    di.clear();
	    di.push_back(0x6); // WREN
	    uhtr_->idSPI(di,dout,0);
	    di.clear();
	    di.push_back(0x2); // WR
	    di.push_back(uint8_t(i-6)); // ADDR
	    di.push_back(buf[i]);
	    uhtr_->idSPI(di,dout,0);
	    
	    di.clear();
	    di.push_back(0x5); // RDSR
	    do {
	      uhtr_->idSPI(di,dout,1);
	      // waiting for WIP to fall
	    } while (dout[0]&0x1);
	    
	    di.clear();
	    di.push_back(0x3); // RD
	    di.push_back(uint8_t(i-6));
	    uhtr_->idSPI(di,dout,4);
	    
	    if (dout[0]==buf[i]) ok=true;
	    //	    else printf("Trying again %d %x!=%x\n",i,dout[0],buf[i]);
	  } while (!ok);
	}
      } else {
	// no implementation at this time
      }
      return true;
    }

  }
}
