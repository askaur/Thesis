#include "uHTRPowerMezzInterface.h"
#include <cstdio>

// i2c slave addresses
#define I2C_SADDRESS_BASEMUX   0x70
#define I2C_SADDRESS_EEPROM    0x50
#define I2C_SADDRESS_MODADC_26 0x68
#define I2C_SADDRESS_MODADC_28 0x6A
#define I2C_SADDRESS_MARGCTRL  0x20

#define V2_I2C_SADDRESS_BASE_MUX    0x70
#define V2_I2C_SADDRESS_PMBASE_ADC  0x6e
#define V2_I2C_SADDRESS_PMBASE_GPIO 0x27
#define V2_I2C_SADDRESS_APMBASE_ADC  0x1d
#define V2_I2C_SADDRESS_APMBASE_GPIO 0x27

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

// i2c margin control values
#define I2C_MARGCTRL_INPUT_REG  0x00
#define I2C_MARGCTRL_OUTPUT_REG 0x01
#define I2C_MARGCTRL_IPINV_REG  0x02
#define I2C_MARGCTRL_CTRL_REG   0x03

#define I2C_MARGCTRL_CTRL_MARG 0xf0
#define I2C_MARGCTRL_OUTPUT_C0 0x01
#define I2C_MARGCTRL_OUTPUT_C1 0x02
#define I2C_MARGCTRL_OUTPUT_C2 0x04
#define I2C_MARGCTRL_OUTPUT_C3 0x08
#define I2C_MARGCTRL_OUTPUT_C4 0x10
#define I2C_MARGCTRL_OUTPUT_C5 0x20
#define I2C_MARGCTRL_OUTPUT_C6 0x40
#define I2C_MARGCTRL_OUTPUT_C7 0x80

// i2c eeprom addresses
#define I2C_EEPROM_DATA 0x00
#define I2C_EEPROM_MAC  0x80

//SUB-20 gpio channels
#define S20_GPIO_C12 0x1000


template<class T>
static void binary(T b)
{
    for(int i = sizeof(T) * 8 - 1; i >= 0; i--) printf("%d", ((1 << i) & b)?1:0);
    printf("\n");
}

uHTRPowerMezzInterface::uHTRPowerMezzInterface(int idev, bool v2) : uHTRMezzInterface(idev, v2){ }

void uHTRPowerMezzInterface::configADC128()
{
    if(isV2_)
    {
    	// Wait until chip is ready
    	buff_[0] = 0x0c; //location of busy status register
    	sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 1);
    	do
    	{
        	sub_i2c_read(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 1);
        } while((buff_[0] & 0x02) && !usleep(100000));
        
        // make sure adc is off
        buff_[0] = 0x00; //location of config register
        buff_[1] = 0x80; //enable
        sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 2);
    	
        // Set conversion rate register 
        buff_[0] = 0x07; //location of conversion rate register
        buff_[1] = 0x01; //continous conversion rate 
        sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 2);

        // Mask unused channels
        buff_[0] = 0x08; //location of mask register
        buff_[1] = 0x60; //mask channels 5 and 6
        sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 2);
        
        // Advanced config
        buff_[0] = 0x0b; //location of advanced config register
        buff_[1] = 0x00; //set internal ref
        sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 2);
        
        // Start adc
        buff_[0] = 0x00; //location of config register
        buff_[1] = 0x01; //enable
        sub_i2c_write(sh_, V2_I2C_SADDRESS_APMBASE_ADC, 0, 0, (char*)buff_, 2);
        
    }
    
    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;
}

double uHTRPowerMezzInterface::readMezzADC(const int adc, const int chan)
{
    if(!openSuccessful_) return -999.9;

    double retval = 0.0;

    if(isV2_)
    {
        unsigned int saddress = 0;
        if(adc < 2)
        {
            if(adc == 0) saddress = I2C_SADDRESS_MODADC_26;
            else if (adc == 1) saddress = V2_I2C_SADDRESS_PMBASE_ADC;

            switch(chan)
            {
                case 1:
                    buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN1;
                    break;
                case 2:
                    buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN2;
                    break;
                case 3:
                    buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN3;
                    break;
                case 4:
                    buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN4;
                    break;
                default:
                    printf("INVALID ADC CHANNEL\n");
                    break;
            }

            sub_i2c_write(sh_, saddress, 0, 0, (char*)buff_, 1);
            sub_i2c_read(sh_, saddress, 0, 0, (char*)buff_, 3);  //12 or 16 bit ADC val are both 2 bytes long and the 3rd byte is status info

            retval = double(int((I2C_MODADC_READMASK_UPBYTE_12 & buff_[0]) << 8) | (int)buff_[1]);
        }
        else if (adc == 2)
        {
            saddress = V2_I2C_SADDRESS_APMBASE_ADC;

            if(chan >= 8) printf("INVALID ADC CHANNEL (ADC128)\n");
            // 0x20 is start of output registers 
            buff_[0] = 0x20 + chan;
            sub_i2c_write(sh_, saddress, 0, 0, (char*)buff_, 1);  //set result register
            sub_i2c_read(sh_, saddress, 0, 0, (char*)buff_, 2);  //read result register (2 bytes for 12 bit vaule)
            
            if(chan == 7) retval = ((buff_[1] & 1)?(double(512 - buff_[0]/2)):(double(buff_[0])/2)); //exception for temp reading
            else          retval = double((int(buff_[0] << 8) | (int)(buff_[1])) >> 4);
        }
    }
    else
    {
        switch(chan)
        {
            case 1:
                buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN1;
                break;
            case 2:
                buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN2;
                break;
            case 3:
                buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN3;
                break;
            case 4:
                buff_[0] = I2C_MODADC_WRITE_DEFAULT | I2C_MODADC_WRITE_CHAN4;
                break;
            default:
                printf("INVALID ADC CHANNEL\n");
                break;
        }


        unsigned int saddress = 0;
        if(adc == 0) saddress = I2C_SADDRESS_MODADC_28;
        else if (adc == 1) saddress = I2C_SADDRESS_MODADC_26;
        else printf("INVALID ADC NUMBER\n");
        //printf("write retval: %d    I2C status: %#x   buffval: %x\n", sub_errno,sub_i2c_status, buff_[0]);

        sub_i2c_write(sh_, saddress, 0, 0, (char*)buff_, 1);
        sub_i2c_read(sh_, saddress, 0, 0, (char*)buff_, 3);  //12 or 16 bit ADC val are both 2 bytes long and the 3rd byte is status info

        retval = double(int((I2C_MODADC_READMASK_UPBYTE_12 & buff_[0]) << 8) | (int)buff_[1]);
    }

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    //if(!(buff_[3] & I2C_MODADC_READMASK_RDY)) return -99999.9;
    //printf("adc read retval: %d    I2C status: %#x   buffval: %x %x\n", sub_errno,sub_i2c_status, buff_[0]&0xFF, buff_[1]&0xFF);

    return retval;
}

double uHTRPowerMezzInterface::readSUB20ADC(const int mux_switch)
{
    if(!openSuccessful_) return -999;

    int mux = 0;
    switch(mux_switch)
    {
        case 0:
            mux = ADC_S0;
            break;
        case 1:
            mux = ADC_S1;
            break;
        case 2:
            mux = ADC_S2;
            break;
        case 3:
            mux = ADC_S3;
            break;
        case 4:
            mux = ADC_S4;
            break;
        case 5:
            mux = ADC_S5;
            break;
        case 6:
            mux = ADC_S6;
            break;
        case 7:
            mux = ADC_S7;
            break;
    }

    int adcval;
    sub_adc_single(sh_, &adcval, mux);

    errval_ = sub_errno;

    return adcval * 2.56 / 1023;
}

void uHTRPowerMezzInterface::setMarginControl(const bool m0, const bool m1, const bool m2, const bool m3)
{
    if(!openSuccessful_) return;

    buff_[0] = I2C_MARGCTRL_OUTPUT_REG;  //register address byte
    buff_[1] = (m0?I2C_MARGCTRL_OUTPUT_C0:0) | (m1?I2C_MARGCTRL_OUTPUT_C1:0) | (m2?I2C_MARGCTRL_OUTPUT_C2:0) | (m3?I2C_MARGCTRL_OUTPUT_C3:0);  //set all registers as output
    sub_i2c_write(sh_, I2C_SADDRESS_MARGCTRL, 0, 0, (char*)buff_, 2);

    buff_[0] = I2C_MARGCTRL_CTRL_REG;  //register address byte
    buff_[1] = I2C_MARGCTRL_CTRL_MARG;  //set all margin registers as output
    sub_i2c_write(sh_, I2C_SADDRESS_MARGCTRL, 0, 0, (char*)buff_, 2);

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::releaseMarginControl()
{
    if(!openSuccessful_) return;

    buff_[0] = I2C_MARGCTRL_CTRL_REG;  //register address byte
    buff_[1] = 0xff;  //set all registers as input
    sub_i2c_write(sh_, I2C_SADDRESS_MARGCTRL, 0, 0, (char*)buff_, 2);

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::readMarginPGood(bool* margup, bool* margdn, bool* pgood)
{
    if(!openSuccessful_) return;

    buff_[0] = I2C_MARGCTRL_INPUT_REG;  //register address byte
    sub_i2c_write(sh_, I2C_SADDRESS_MARGCTRL, 0, 0, (char*)buff_, 1);  //set register
    sub_i2c_read(sh_, I2C_SADDRESS_MARGCTRL, 0, 0, (char*)buff_, 1);  //read input register

    *margup = I2C_MARGCTRL_OUTPUT_C2 & buff_[0];
    *margdn = I2C_MARGCTRL_OUTPUT_C3 & buff_[0];
    *pgood  = I2C_MARGCTRL_OUTPUT_C7 & buff_[0];

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::togglePowerMezzs(const bool state)
{
    if(!openSuccessful_) return;

    if(isV2_)
    {
        // Set register for output
        buff_[0] = I2C_MARGCTRL_CTRL_REG;  //register address byte
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 1);  //set register
        sub_i2c_read(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)(buff_ + 1), 1);  //read register
        buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C0;  //set C0 to output
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 2);  //set register

        // Set output value
        buff_[0] = I2C_MARGCTRL_OUTPUT_REG;  //register address byte
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 1);  //set register
        sub_i2c_read(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)(buff_ + 1), 1);  //read register
        if(state) buff_[1] |=  I2C_MARGCTRL_OUTPUT_C0;  //state byte
        else      buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C0;  //state byte
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 2 );  //set register
    }
    else
    {
        int ib;
        sub_gpio_write(sh_, state?S20_GPIO_C12:0, &ib, S20_GPIO_C12); //sets power mezzaniens on if state is true
    }

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::toggleMezzsLoad(const int channel, const bool state)
{
	if(!openSuccessful_) return;

    if(isV2_)
    {
        // Set register for output
        buff_[0] = I2C_MARGCTRL_CTRL_REG;  //register address byte
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 1);  //set register
        sub_i2c_read(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)(buff_ + 1), 1);  //read register
        switch(channel)
        {
            case 1:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C1;  //set C1 to output
            	break;
            case 2:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C2;  //set C2 to output
            	break;
            case 3:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C3;  //set C3 to output
            	break;
            case 4:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C4;  //set C4 to output
            	break;
            case 5:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C5;  //set C5 to output
            	break;
            case 6:
            	buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C6;  //set C6 to output
            	break;
            default:
            	printf("Invalid mosfet channel\n");
            	return;
            	
        }
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 2);  //set register

        // Set output value
        buff_[0] = I2C_MARGCTRL_OUTPUT_REG;  //register address byte
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 1);  //set register
        sub_i2c_read(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)(buff_ + 1), 1);  //read register
        switch(channel)
        {
            case 1:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C1):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C1);  //state byte
            	break;
            case 2:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C2):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C2);  //state byte
            	break;
            case 3:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C3):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C3);  //state byte
            	break;
            case 4:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C4):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C4);  //state byte
            	break;
            case 5:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C5):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C5);  //state byte
            	break;
            case 6:
            	state?(buff_[1] |= I2C_MARGCTRL_OUTPUT_C6):(buff_[1] &= ~I2C_MARGCTRL_OUTPUT_C6);  //state byte
            	break;
            default:
            	printf("Invalid mosfet channel\n");
            	return;   	
        }
        sub_i2c_write(sh_, V2_I2C_SADDRESS_PMBASE_GPIO, 0, 0, (char*)buff_, 2 );  //set register
    }
    else
    {
        // This feature does not exist on V1 boards
    }

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::writeMezzEeprom(const EEPROM_data data)
{
    if(!openSuccessful_) return;

    i2c_status_ = 0;

    for(unsigned int dpos = 0; dpos < 0x80; dpos += 0x08)
    {
        buff_[0] = I2C_EEPROM_DATA + dpos;  // Data must be written in 8 byte pages 
        for(unsigned int i = 0; (i < 8) && (dpos + i < EEPROM_DATA_SIZE); i++) buff_[i + 1] = *(((char*)&data) + dpos + i);
        //printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x\n", buff_[0], buff_[1], buff_[2], buff_[3], buff_[4], buff_[5], buff_[6], buff_[7], buff_[8]);
        sub_i2c_write(sh_, I2C_SADDRESS_EEPROM, 0, 0, (char*)buff_, 9);
        i2c_status_ |= sub_i2c_status;
        usleep(10000);
    }

    errval_ = sub_errno;

    return;
}

void uHTRPowerMezzInterface::readMezzEeprom(EEPROM_data* data)
{
    if(!openSuccessful_) return;

    buff_[0] = I2C_EEPROM_DATA;
    sub_i2c_write(sh_, I2C_SADDRESS_EEPROM, 0, 0, (char*)buff_, 1);  // Set address of initial read
    sub_i2c_read(sh_, I2C_SADDRESS_EEPROM, 0, 0, (char*)data, EEPROM_DATA_SIZE);  // Set address of initial read

    errval_ = sub_errno;
    i2c_status_ = sub_i2c_status;

    return;
}

void uHTRPowerMezzInterface::printEEPROM_data(const EEPROM_data data, const bool hex)
{
    if(hex)
    {
        for(unsigned int i = 0; i < EEPROM_DATA_SIZE; i++)
        {
            printf("%02x ", *((uint8_t*) & data + i));
            if((i % 8) == 7) printf("\n");
        }
    }
    else
    {
        printf("Data format version: %d\n", data.data_format_version);
        printf("Mezz type code:      %d\n", data.mezz_type_code);
        printf("Mezz subtype code:   %d\n", data.mezz_subtype_code);
        printf("Mezz type (string):  %s\n", data.mezz_type);
        printf("Serial number:       %d\n", 0xffff & ((((int)(0xff & data.serial_number[1])) << 8) + (int)(0xff & data.serial_number[0])));
        printf("Manufacture date:    %s\n", data.manu_date);
        printf("Manufacture site:    %s\n", data.manu_site);
        printf("Manufacture tester:  %s\n", data.manu_tester);
        printf("Test release:        %s\n", data.test_release);
    }

    return;
}
