#include "hcal/uhtr/LinkFormatTools.hh"

namespace hcal {
  namespace uhtr {
    
    void pack_link_16(int c0, int c1, int c2, int q0, int q1, int q2, uint16_t& firstW, uint16_t& secondW) {
      firstW=0x3 | ((c2&0x3)<<3) | ((c1&0x3)<<5) | ((c0&0x3)<<7) | ((q2&0x7F)<<9);
      secondW=((q0&0x7F)<<9) | ((q1&0x7F)<<1);
    }
    void unpack_link_16(const uint16_t firstW, const uint16_t secondW, int& c0, int& c1, int& c2, int& q0, int& q1, int& q2) {
      c0=(firstW>>7)&0x3;
      c1=(firstW>>5)&0x3;
      c2=(firstW>>3)&0x3;
      q0=(secondW>>9)&0x7F;
      q1=(secondW>>1)&0x7F;
      q2=(firstW>>9)&0x7F;
    }


    bool unpack_link_6chan(const uint16_t* ptr, int& cap, bool& bc0, bool& ce, uint8_t* adcs, uint8_t* tdcs) {
      cap=-1; bc0=false; ce=false; 
      if ((ptr[0]&0xFF)!=0xBC) return false;
      cap=(ptr[0]>>10)&0x3;
      bc0=(ptr[0]&0x100)!=0;
      ce=(ptr[0]&0x200)!=0;
      adcs[0]=uint8_t(ptr[1]);
      adcs[1]=uint8_t(ptr[1]>>8);
      adcs[2]=uint8_t(ptr[2]);
      adcs[3]=uint8_t(ptr[2]>>8);
      adcs[4]=uint8_t(ptr[3]);
      adcs[5]=uint8_t(ptr[3]>>8);
      tdcs[0]=uint8_t(((ptr[0]>>12)&0xF)|((ptr[4]&0x3)<<4));
      tdcs[1]=uint8_t((ptr[4]>>2)&0x3F);      
      tdcs[2]=uint8_t((ptr[4]>>8)&0x3F);
      tdcs[3]=uint8_t(((ptr[4]>>14)&0x3)|((ptr[5]&0xF)<<2));
      tdcs[4]=uint8_t((ptr[5]>>4)&0x3F);
      tdcs[5]=uint8_t((ptr[5]>>10)&0x3F);
      return true;
    }

    bool unpack_link_6chan_debug(const uint16_t* ptr, int& cap, bool& bc0, bool& ce, uint8_t* adcs, uint8_t* tdcs) {
      cap=-1; bc0=false; ce=false; 
      if ((ptr[0]&0xFF)!=0xBC) return false;
      cap=(ptr[0]>>10)&0x3;
      bc0=(ptr[0]&0x100)!=0;
      ce=(ptr[0]&0x200)!=0;
      adcs[0]=uint8_t(ptr[1]);
      adcs[1]=uint8_t(ptr[1]>>8);
      adcs[2]=uint8_t(ptr[2]);
      adcs[3]=uint8_t(ptr[2]>>8);
      adcs[4]=uint8_t(ptr[3]);
      adcs[5]=uint8_t(ptr[3]>>8);
      tdcs[0]=uint8_t(((ptr[0]>>12)&0xF)|((ptr[4]&0x3)<<4));
      tdcs[1]=uint8_t((ptr[4]>>2)&0x3F);      
      tdcs[2]=uint8_t((ptr[4]>>8)&0xFF); // this is a counter
      tdcs[3]=uint8_t((ptr[5])&0x3);     // capId for ch0
      tdcs[4]=uint8_t((ptr[5]>>2)&0x3);
      tdcs[5]=uint8_t((ptr[5]>>4)&0x3);
      tdcs[6]=uint8_t((ptr[5]>>6)&0x3);  // zero
      tdcs[7]=uint8_t((ptr[5]>>8)&0x3);
      tdcs[8]=uint8_t((ptr[5]>>10)&0x3);
      tdcs[9]=uint8_t((ptr[5]>>12)&0x3); 
      tdcs[10]=uint8_t((ptr[5]>>14)&0x3); // zero
      return true;
    }
    
  }
}
