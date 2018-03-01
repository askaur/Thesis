#include "uHTRMezzInterface.h"
#include <cstdio>

// i2c slave addresses
#define I2C_SADDRESS_BASEMUX   0x70
#define I2C_SADDRESS_EEPROM    0x50
#define I2C_SADDRESS_MODADC_26 0x68
#define I2C_SADDRESS_MODADC_28 0x6A
#define I2C_SADDRESS_MARGCTRL  0x20

#define V2_I2C_SADDRESS_BASE_MUX    0x70
#define V2_I2C_SADDRESS_PMBASE_ADC  0x6e
#define V2_I2C_SADDRESS_PMBASE_GPIO 0x3f
#define V2_I2C_SADDRESS_APMBASE_ADC  0x1d
#define V2_I2C_SADDRESS_APMBASE_GPIO 0x3f

// i2c adc values
#define I2C_MODADC_WRITE_DEFAULT 0x90
#define I2C_MODADC_WRITE_CHAN1   0x00
#define I2C_MODADC_WRITE_CHAN2   0x20
#define I2C_MODADC_WRITE_CHAN3   0x40
#define I2C_MODADC_WRITE_CHAN4   0x60

#define I2C_MODADC_READMASK_RDY       0x80
#define I2C_MODADC_READMASK_CHAN      0x60
#define I2C_MODADC_READMASK_RATE      0x0B
#define I2C_MODADC_READMASK_GAIN      0x03
#define I2C_MODADC_READMASK_UPBYTE_12 0x0f

//SUB-20 gpio channels
#define S20_GPIO_C12 0x01000
#define S20_GPIO_C16 0x10000
#define S20_GPIO_C17 0x20000
#define S20_GPIO_C18 0x40000
#define S20_GPIO_C19 0x80000

uHTRMezzInterface::uHTRMezzInterface(int idev, bool v2) : isV2_(v2), sd_(0), sh_(0), openSuccessful_(false)
{
    int count = 0;
    for (sd_ = sub_find_devices(sd_); sd_ != 0; sd_ = sub_find_devices(sd_))
    {
        if(count == idev)
        {
            sh_ = sub_open(sd_);
            break;
        }
        count++;
    }

    //verify the device is open
    if(sh_ == 0) printf("DEVICE FAILED TO OPEN\n");
    else
    {
        openSuccessful_ = true;

        //configure i2c settingns
        int freq = 1000;  //THe ADCs cannot keep up with a faster clock rate than 1 kHz
        sub_i2c_freq(sh_, &freq);

        //configure adc sattings
        sub_adc_config(sh_, ADC_ENABLE | ADC_REF_2_56);

        //configure GPIO
        int ib;
        if(isV2_)
        {
            //configure channels 16-19 as inputs
            sub_gpio_config(sh_, 0, &ib, S20_GPIO_C16 | S20_GPIO_C17 | S20_GPIO_C18 | S20_GPIO_C19);
        }
        else
        {
            sub_gpio_config(sh_, S20_GPIO_C12, &ib, S20_GPIO_C12); //configures channel 12 for output (to control powermodule on/off)
            sub_gpio_write(sh_, 0, &ib, S20_GPIO_C12); //sets power modules to off by default 
        }
    }

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;
}

uHTRMezzInterface::~uHTRMezzInterface(){ }

int uHTRMezzInterface::setMUXChannel(const int channel)
{
    if(!openSuccessful_) return 999;

    if(isV2_)
    {
        buff_[0] = (char)(1 << channel);
        sub_i2c_write(sh_, V2_I2C_SADDRESS_BASE_MUX, 0, 0, (char*)buff_, 1);
    }
    else
    {
        buff_[0] = (char)(channel | 0x4);
        sub_i2c_write(sh_, I2C_SADDRESS_BASEMUX, 0, 0, (char*)buff_, 1);
    }

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return  sub_i2c_status;
}

int uHTRMezzInterface::readMezzMAC()
{
    if(!openSuccessful_) return 999;

    MAC_[0] = MAC_[1] = MAC_[2] = MAC_[3] = MAC_[4] = MAC_[5] = 0x00;

    buff_[0] = 0xfa; //address to start of MAC
    sub_i2c_write(sh_, I2C_SADDRESS_EEPROM, 0, 0, (char*)buff_, 1);
    sub_i2c_read(sh_, I2C_SADDRESS_EEPROM, 0, 0, (char*)MAC_, 6); //MAC is 6 bytes long

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    //printf("eprom read retval: %d    I2C status: %#x   buffval: %x %x %x %x %x %x\n", errval_,sub_i2c_status, MAC_[0]&0xFF,MAC_[1]&0xFF,MAC_[2]&0xFF,MAC_[3]&0xFF,MAC_[4]&0xFF,MAC_[5]&0xFF);

    return sub_i2c_status;
}

void uHTRMezzInterface::printMezzMAC()
{
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", MAC_[0]&0xFF, MAC_[1]&0xFF, MAC_[2]&0xFF, MAC_[3]&0xFF, MAC_[4]&0xFF, MAC_[5]&0xFF);
}

void uHTRMezzInterface::copyMezzMAC(char * MAC)
{
    for(int i = 0; i < 6; i++) MAC[i] = MAC_[i];
}

void uHTRMezzInterface::updateSUB20Display(const char * dbuf)
{
    if(!openSuccessful_) return;

    sub_lcd_write(sh_, (char*)dbuf);

    errval_ = sub_errno;

    return;
}

