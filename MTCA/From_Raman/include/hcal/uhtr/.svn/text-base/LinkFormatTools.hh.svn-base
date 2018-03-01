#ifndef HCAL_UHTR_LINKFORMATTOOLS_HH
#define HCAL_UHTR_LINKFORMATTOOLS_HH

#include <stdint.h>

namespace hcal {
  namespace uhtr {
    
    void pack_link_16(int c0, int c1, int c2, int q0, int q1, int q2, uint16_t& firstW, uint16_t& secondW);
    void unpack_link_16(const uint16_t firstW, const uint16_t secondW, int& c0, int& c1, int& c2, int& q0, int& q1, int& q2);

    // all arrays must be of length 6!
    bool unpack_link_6chan(const uint16_t* ptr, int& cap, bool& bc0, bool& ce, uint8_t* adcs, uint8_t* tdcs);

    // only for testbeam debug
    bool unpack_link_6chan_debug(const uint16_t* ptr, int& cap, bool& bc0, bool& ce, uint8_t* adcs, uint8_t* tdcs);

  }
}

#endif // HCAL_UHTR_LINKFORMATTOOLS_HH
