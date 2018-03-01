#ifndef UHTR_MOD_INTERFACE
#define UHTR_MOD_INTERFACE

#include <stdint.h>
#include "libsub.h"
#include <string>

enum MUXChannels
{
    MUX_POWERMOD = 0,
    MUX_AUXPOWERMOD = 1
} ;

enum V2MUXChannels
{
    V2_MUX_PM_3_3  = 1,
    V2_MUX_PM_1_A  = 5,
    V2_MUX_PM_1_B  = 6,
    V2_MUX_APM_2_5 = 2,
    V2_MUX_APM_1_6 = 4    
} ;

class uHTRMezzInterface
{
public:
    const bool isV2_;
    
	uHTRMezzInterface(int, bool);
    virtual ~uHTRMezzInterface();
	int setMUXChannel(const int);
	virtual int readMezzMAC();
	void printMezzMAC();
	void copyMezzMAC(char MAC[6]);

	void updateSUB20Display(const char *);
	void togglePowerMezzs(const bool); 
	
	inline unsigned int getErrVal()
	{
		return errval_;
	}
	inline unsigned int geti2cStatus()
	{
		return i2c_status_;
	}
	
protected:
	sub_device sd_;
	sub_handle sh_;
	unsigned int errval_, i2c_status_;
	bool openSuccessful_;
	unsigned char buff_[128], MAC_[6];
};

#endif
