#include <cstdlib>
#include <cstdio>
#include <unistd.h> 
#include <getopt.h>
#include <time.h>
#include <map>
#include <string>
#include <vector>
#include "uHTRPowerMezzInterface.h"
#include "uHTRMezzanines.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>


const int TimeToN = 6;

int N_margup = 180; //30 min
int N_margdn = 180; //30 min
int N_nom    = 6840; //19 hours 
int N_nom_lh = 6840; //19 hours 
const bool will_sleep = true;

double PM_VOUT_NOM = 1.0;
const double PM_VA_NOM = 12.0;
const double PM_VB_NOM = 12.0;

double APM_VOUT_NOM = 1.8;
const double APM_VC_NOM = 12.0;

boost::mutex s20mtx_;

int test_mezzanines(uHTRPowerMezzInterface& s20, std::vector<Mezzanine*>& mezzanines);
void displayAndSleep(uHTRPowerMezzInterface& s20, std::vector<Mezzanine*>& mezzanines);
void help();

unsigned int in_[] = {0x6e6f420c, 0x72756f6a, 0x20202021, 0x00202020, 0x6d614d0c, 0x64206e61, 0x6e655265, 0x00202065,
    0x7473650c, 0x20697320, 0x6f726720, 0x00202c73, 0x6175710c, 0x2020646e, 0x6c6c6520, 0x00202065,
    0x756f620c, 0x20206567, 0x20656c20, 0x006c7563, 0x6c6c650c, 0x720a2065, 0x69757165, 0x00747265,
    0x7565640c, 0x760a2078, 0x6761796f, 0x00217365, 0x2020200c, 0x20202020};

void help()
{
    printf("Usage: ./uHTR_PowerMod_Test.exe modeFlags [options]\n");
    printf("Mode Flags - These can be used in conjunction but will always evaluate in the order listed below.\n");
    printf(" --echo,         -e   Echo string to sub20\n");
    printf(" --enable,            Enable all Mezzanines\n");
    printf(" --disable,      -d   Disable all Mezzanines\n");
    printf(" --readEeproms,  -r   Read power mezzanine eeproms\n");
    printf(" --labelPM,      -p   Program power mezzanine eeprom\n");
    printf(" --labelAPM,     -a   Program aux power mezzanine eeprom\n");
    printf(" --runTest,      -t   Run power mezzanine test\n");
    printf("Option Flags\n");
    printf(" --margUpTime,   -M   Sub20 number (default is 0)\n");
    printf(" --margUpTime,   -U   Margin up test length (in min)\n");
    printf(" --margDnTime,   -D   Margin down test length (in min)\n");
    printf(" --nomTime,      -T   Nominal test length (in min)\n");
    printf(" --highLoadTime, -L   High load test length (in min)\n");
    printf(" --pmVolt,       -P   PM voltage (in V)\n");
    printf(" --apmVolt,      -A   APM voltage (in V)\n");
    printf(" --testSite,     -S   Test site name (<= 15 characters)\n");
    printf(" --testerName,   -N   Tester name (<= 15 characters\n");
    printf(" --version1,     -V   Tester name (<= 15 characters\n");
    printf(" --skipSNBlock,       The SN assignment will skip a block of SNs of specified size\n");
    printf(" --help,         -h   help (This menu)\n");
}

int main(int argc, char* argv[])
{
    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"readEeproms",       no_argument, 0, 'r'},
        {"runTest",           no_argument, 0, 't'},
        {"margUpTime",  required_argument, 0, 'U'},
        {"margDnTime",  required_argument, 0, 'D'},
        {"nomTime",     required_argument, 0, 'T'},
        {"highLoadTime",required_argument, 0, 'L'},
        {"pmVolt",      required_argument, 0, 'P'},
        {"apmVolt",     required_argument, 0, 'A'},
        {"labelPM",           no_argument, 0, 'a'},
        {"labelAPM",          no_argument, 0, 'p'},
        {"testSite",    required_argument, 0, 'S'},
        {"testerName",  required_argument, 0, 'N'},
        {"skipSNBlock", required_argument, 0, 'B'},
        {"sub20Num",    required_argument, 0, 'M'},
        {"echo",        required_argument, 0, 'e'},
        {"version1",          no_argument, 0, 'V'},
        {"enable",            no_argument, 0, 's'},
        {"disable",           no_argument, 0, 'd'},
        {"r00t",              no_argument, 0, 'k'},
        {"help",              no_argument, 0, 'h'}
    };

    bool runTest = false, labelPM = false, labelAPM = false, readEeproms = false, siteSet = false, testerSet = false;
    bool echo = false, k = false, isV2 = true, pSet = false, aSet = false, enable = false, disable = false;
    char site[16], tester[16], echoString[32];
    unsigned int sbSize = 0, sub20Num = 0;

    if(argc == 1)
    {
        help();
        return 0;
    }

    while((opt = getopt_long(argc, argv, "rapthe:dq:M:U:D:T:L:P:A:S:N:V", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 't':
                runTest = true;
                break;
            case 'U':
                N_margup = int(atoi(optarg) * TimeToN);
                break;
            case 'D':
                N_margdn = int(atoi(optarg) * TimeToN);
                break;
            case 'T':
                N_nom = int(atoi(optarg) * TimeToN);
                break;
            case 'L':
                N_nom_lh = int(atoi(optarg) * TimeToN);
                break;
            case 'P':
                PM_VOUT_NOM = atof(optarg);
                pSet = true;
                break;
            case 'A':
                APM_VOUT_NOM = atof(optarg);
                aSet = true;
                break;
            case 'S':
                if(strlen(optarg) > 15)
                {
                    printf("Site name restricted to 15 character.");
                    return 0;
                }
                siteSet = true;
                sprintf(site, "%s", optarg);
                break;
            case 'N':
                if(strlen(optarg) > 15)
                {
                    printf("Tester name restricted to 15 character.");
                    return 0;
                }
                testerSet = true;
                sprintf(tester, "%s", optarg);
                break;
            case 'B':
                sbSize = int(atoi(optarg));
                break;
            case 'M':
                sub20Num = int(atoi(optarg));
                break;
            case 'V':
                isV2 = false;
                break;
            case 'p':
                labelPM = true;
                break;
            case 'a':
                labelAPM = true;
                break;
            case 'r':
                readEeproms = true;
                break;
            case 'e':
                echo = true;
                echoString[0] = '\f';
                for(unsigned int i = 1; i < 17 && i <= strlen(optarg); i++) echoString[i] = optarg[i - 1];
                break;
            case 's':
                enable = true;
                break;
            case 'd':
                disable = true;
                break;
            case 'k':
                k = true;
                break;
            case 'h':
            default:
                help();
                return 0;
            
            case 'q':
                uHTRPowerMezzInterface s20(sub20Num, isV2);
                
                int chan = int(atoi(optarg));
                int adc = 0;
                if(chan == 0)      
                {
                	chan = V2_MUX_PM_1_A;
                	adc = 1;
                }
                else if(chan == 1) 
                {
                	chan = V2_MUX_APM_1_6;
                	adc = 2;
                }
                else if(chan == 2) 
                {
                	chan = V2_MUX_PM_1_B;
                	adc = 1;
                }
                else if(chan == 3) 
                {
                	chan = V2_MUX_APM_2_5;
                	adc = 2;
                }
                else if(chan == 4) 
                {
                	chan = V2_MUX_PM_3_3;
                	adc = 1;
                }
                
                s20.setMUXChannel(chan);
                printf("MUX STATUS: %x\n", s20.geti2cStatus());
                s20.togglePowerMezzs(true);
                printf("TOG STATUS: %x\n", s20.geti2cStatus());
                if(adc == 2) s20.configADC128();
                sleep(1);
                //s20.readMezzADC(1, 1);
                if(adc == 2) printf("CFG STATUS: %x\n", s20.geti2cStatus());
                double v2 = s20.readMezzADC(chan, 1);
                sleep(1);
                printf("ADC STATUS: %x %f\n", s20.geti2cStatus(), v2/1600);
                double v1 = s20.readMezzADC(0, 2);
                printf("V_O STATUS: %x %f\n", s20.geti2cStatus(), 4.32*v1);
                s20.togglePowerMezzs(false);
            	return 0;
        }
    }

    // Initialize sub20 device
    uHTRPowerMezzInterface s20(sub20Num, isV2);

    // Initialize list of mezzanines 
    std::vector<Mezzanine*> mezzanines;
    if(isV2)
    {
        mezzanines.push_back(new PM(s20, V2_MUX_PM_3_3, 3.3, isV2));
        //mezzanines.push_back(new APM(s20, V2_MUX_APM_2_5, 2.5, isV2));
	mezzanines.push_back(new APM(s20, V2_MUX_APM_2_5, 1.8, isV2));//changed by Bhaw/Ram to run 1.8V one on 2nd slot
        mezzanines.push_back(new PM(s20, V2_MUX_PM_1_B, 1.0, isV2));
	//mezzanines.push_back(new APM(s20, V2_MUX_APM_2_5, 2.5, isV2));
        mezzanines.push_back(new APM(s20, V2_MUX_APM_1_6, 1.8, isV2));
        mezzanines.push_back(new PM(s20, V2_MUX_PM_1_A, 1.0, isV2));
    }
    else
    {
    	mezzanines.push_back(new PM(s20, MUX_POWERMOD, PM_VOUT_NOM, isV2));
        mezzanines.push_back(new APM(s20, MUX_AUXPOWERMOD, APM_VOUT_NOM, isV2));
    }

    if(echo)
    {
        s20.updateSUB20Display(echoString);

        if(readEeproms || labelPM || labelAPM || runTest) sleep(10);
    }

    if(k)
    {
        for(unsigned int i = 0; i < sizeof(in_) / sizeof(unsigned int); i += 4)
        {
            s20.updateSUB20Display((char *)(in_ + i));
            sleep(1);
        }
    }

    std::vector<Mezzanine*>::iterator iM;
    std::vector<Mezzanine*>::iterator iMBegin = mezzanines.begin();
    std::vector<Mezzanine*>::iterator iMEnd = mezzanines.end();
    
    if(enable)
    {
        for(iM = iMBegin; iM != iMEnd; ++iM) 
        {
            (*iM)->setPrimaryLoad(false, false);
            if(!(*iM)->isPM) (*iM)->setSecondaryLoad(false, false, false, false);
            (*iM)->setRun(true);
        }
    }
    
    if(disable)
    {
        for(iM = iMBegin; iM != iMEnd; ++iM) 
        {
            (*iM)->setPrimaryLoad(false, false);
            if(!(*iM)->isPM) (*iM)->setSecondaryLoad(false, false, false, false);
            (*iM)->setRun(false);
        }
    }

    if(readEeproms)
    {
        for(iM = iMBegin; iM != iMEnd; ++iM) (*iM)->readEeprom();
        s20.updateSUB20Display("\fEEPROMs\nread");
    }

    // read in SN file
    if(labelPM || labelAPM)
    {
        if(!siteSet && !testerSet)
        {
            printf("Site name and tester name must both be specified to program eeprom (-h for options)\n");
            return 0;
        }
    }

    if(labelPM) for(iM = iMBegin; iM != iMEnd; ++iM) if((*iM)->isPM) (*iM)->programEeprom(tester, site);

    if(labelAPM) for(iM = iMBegin; iM != iMEnd; ++iM) if(!(*iM)->isPM) (*iM)->programEeprom(tester, site);

    if(runTest)
    {
        int retval = test_mezzanines(s20, mezzanines);
        char retmessage[32];
        Mezzanine::Summary::translateStatus(retval, retmessage);
        if(retval) printf("\nExit with value: %s\n", retmessage);
    }
}

//======================================================================
// Test procedures for uHTR POWER MODULE and uHTR AUX_POWER MODULE
//======================================================================

int test_mezzanines(uHTRPowerMezzInterface& s20, std::vector<Mezzanine*>& mezzanines)
{
    int i, status;

    // Turn power mezzanines on
    std::vector<Mezzanine*>::iterator iM;
    std::vector<Mezzanine*>::iterator iMBegin = mezzanines.begin();
    std::vector<Mezzanine*>::iterator iMEnd = mezzanines.end();
    for(iM = iMBegin; iM != iMEnd; ++iM) (*iM)->setRun(true);
    usleep(100000); // wait to ensure mezzaning power up complete

    //====================================================================
    // CHECK SUPPLY VOLTAGES.
    //====================================================================

    if(s20.isV2_)
    {
        // Read voltage from sub20 board
        
        // 12 V supply is read through a divide by 9.2
        if(9.2 * s20.readSUB20ADC(6) < 10.0) return RETVAL_BAD_SUPPLY_VOLTAGE;
        
        // 5 V supply is read through a dividr by 3.7
        if(3.7 * s20.readSUB20ADC(7) < 4.5) return RETVAL_BAD_3_3VOLT;
    }
    else
    {
        //--------------------------------------------------------------------
        // Set MUX to channel 1, AUX_POWER MODULE
        //--------------------------------------------------------------------
        // PCA9544A; SLAVE ADDRESS: 1110 000
        s20.setMUXChannel(MUX_AUXPOWERMOD);

        //--------------------------------------------------------------------
        // Read 12V supply voltage. If 12V supply reads less than 8V then 
        // exit with a failure.
        //--------------------------------------------------------------------
        // MCP3428; SLAVE ADDRESS: 1101 010
        // Channel 3: V12 = 18.647 * V_CH3
        // 2.048V reference -> 1 mV per LSB
        if(18.647 * s20.readMezzADC(ADC_28, 3) < 10000.0) return RETVAL_BAD_SUPPLY_VOLTAGE;

        //--------------------------------------------------------------------
        // Read 3.3V supply voltage. If 3.3V supply reads less than 3V then 
        // exit with a failure.
        //--------------------------------------------------------------------
        // MCP3428; SLAVE ADDRESS: 1101 010
        // Channel 4: V3.3 = 18.647 * V_CH4
        // 2.48V reference -> 1 mV per LSB
        if(18.647 * s20.readMezzADC(ADC_28, 4) < 3000.0) return RETVAL_BAD_3_3VOLT;
    }
    
    // Activate nominal load resistors 
    for(iM = iMBegin; iM != iMEnd; ++iM) 
    {
        (*iM)->setPrimaryLoad(true, false);
        if(!(*iM)->isPM) (*iM)->setSecondaryLoad(true, true, true, true);
    }
    usleep(100000); //wait for output to stabalize 

    // This thread updates the sub20 display
    boost::thread display(boost::bind(displayAndSleep, s20, mezzanines));

    //====================================================================
    // Set LOW voltage margins.
    //====================================================================

    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        if(!(*iM)->isPresent()) continue;
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setMargins(-1);
    }

    // Monitor for N loops at 10 seconds per loop.

    printf("Margin Down Test:");
    for (i = 0; i < N_margdn; i++)
    {
        boost::thread timer(boost::bind(sleep, 10));
        status = 0;
        printf(".");
        fflush(stdout);
        for(iM = iMBegin; iM != iMEnd; ++iM)
        {
            if(!(*iM)->isPresent()) continue;
            boost::mutex::scoped_lock l(s20mtx_);
            status |= (*iM)->monitor();
        }

        if(!s20.isV2_ && (status & RETVAL_ABORT)) return RETVAL_ABORT;

        // SLEEP 10 seconds.
        if(will_sleep) timer.join();
    }
    printf("\n");
    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->print();
    }

    //====================================================================
    // Set HIGH voltage margins.
    //====================================================================

    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        if(!(*iM)->isPresent()) continue;
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setMargins(1);
    }

    // Monitor for N loops at 10 seconds per loop.

    printf("Margin Up Test:");
    for (i = 0; i < N_margup; i++)
    {
        boost::thread timer(boost::bind(sleep, 10));
        status = 0;
        printf(".");
        fflush(stdout);
        for(iM = iMBegin; iM != iMEnd; ++iM)
        {
            if(!(*iM)->isPresent()) continue;
            boost::mutex::scoped_lock l(s20mtx_);
            status |= (*iM)->monitor();
        }

        if(!s20.isV2_ && (status & RETVAL_ABORT)) return RETVAL_ABORT;

        // SLEEP 10 seconds.
        if(will_sleep) timer.join();
    }
    printf("\n");
    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->print();
    }

    //====================================================================
    // Set NOMINAL voltage margins.
    //====================================================================

    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        if(!(*iM)->isPresent()) continue;
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setMargins(0);
    }

    // Monitor for N loops at 10 seconds per loop.
    printf("Nominal Test:");
    for (i = 0; i < N_nom; i++)
    {
        boost::thread timer(boost::bind(sleep, 10));
        status = 0;
        printf(".");
        fflush(stdout);
        for(iM = iMBegin; iM != iMEnd; ++iM)
        {
            if(!(*iM)->isPresent()) continue;
            boost::mutex::scoped_lock l(s20mtx_);
            status |= (*iM)->monitor();
        }

        if(!s20.isV2_ && (status & RETVAL_ABORT)) return RETVAL_ABORT;

        // SLEEP 10 seconds.
        if(will_sleep) timer.join();
    }
    printf("\n");
    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->print();
    }
    
    //====================================================================
    // Set HIGH LOAD voltage test.
    //====================================================================

    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        if(!(*iM)->isPresent()) continue;
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setMargins(0, 2);
    }
    
    // Set full load
    for(iM = iMBegin; iM != iMEnd; ++iM) 
    {
        if(!(*iM)->isPresent()) continue;
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setPrimaryLoad(true, true);
        if(!(*iM)->isPM) (*iM)->setSecondaryLoad(true, true, true, true);
    }
    usleep(100000); //give time for voltage to settle

    // Monitor for N loops at 10 seconds per loop.
    printf("High Load Test:");
    for (i = 0; i < N_nom_lh; i++)
    {
        boost::thread timer(boost::bind(sleep, 10));
        status = 0;
        printf(".");
        fflush(stdout);
        for(iM = iMBegin; iM != iMEnd; ++iM)
        {
            if(!(*iM)->isPresent()) continue;
            boost::mutex::scoped_lock l(s20mtx_);
            status |= (*iM)->monitor();
        }

        if(!s20.isV2_ && (status & RETVAL_ABORT)) return RETVAL_ABORT;

        // SLEEP 10 seconds.
        if(will_sleep) timer.join();
    }
    printf("\n");
    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->print();
    }
    
    // End of test cleanup
    
    // Turn power mezzanines off
    for(iM = iMBegin; iM != iMEnd; ++iM)
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setRun(false);
    }
    
    // Deactivate nominal load resistors 
    for(iM = iMBegin; iM != iMEnd; ++iM) 
    {
        boost::mutex::scoped_lock l(s20mtx_);
        (*iM)->setPrimaryLoad(false, false);
        if(!(*iM)->isPM) (*iM)->setSecondaryLoad(false, false, false, false);
    }

    display.~thread();

    // Set final status to screen
    boost::mutex::scoped_lock l(s20mtx_);
    s20.updateSUB20Display("\fTEST\nCOMPLETE");

    //====================================================================
    // RETURN
    //====================================================================
    return(RETVAL_SUCCESS);
}

void displayAndSleep(uHTRPowerMezzInterface& s20, std::vector<Mezzanine*>& mezzanines)
{
    char displaybuff[20], error[10];

    std::vector<Mezzanine*>::iterator iM;
    std::vector<Mezzanine*>::iterator iMBegin = mezzanines.begin();
    std::vector<Mezzanine*>::iterator iMEnd = mezzanines.end();

    while(true)
    {
        for(iM = iMBegin; iM != iMEnd; ++iM)
        {
            if(!(*iM)->isPresent()) continue;
            {
                boost::mutex::scoped_lock l2(s20mtx_);
                if((*iM)->isPM) sprintf(displaybuff, "\fPM %0.1fV\nT: %0.1fC", (*iM)->VOUT_NOM, (*iM)->actTest->temp[4]);
                else            sprintf(displaybuff, "\fAPM %0.1fV\nT: %0.1fC", (*iM)->VOUT_NOM, (*iM)->actTest->temp[4]);
                s20.updateSUB20Display(displaybuff);
            }
            sleep(2);
            {
                boost::mutex::scoped_lock l2(s20mtx_);
                if((*iM)->isPM) sprintf(displaybuff, "\fPM %0.1fV\nV: %0.2fV", (*iM)->VOUT_NOM, (*iM)->actTest->vout[4]);
                else            sprintf(displaybuff, "\fAPM %0.1fV\nV: %0.2fV", (*iM)->VOUT_NOM, (*iM)->actTest->vout[4]);
                s20.updateSUB20Display(displaybuff);
            }
            sleep(2);
            {
                boost::mutex::scoped_lock l2(s20mtx_);
                Mezzanine::Summary::translateStatus((*iM)->actTest->pass, error);
                if((*iM)->isPM) sprintf(displaybuff, "\fPM %0.1fV\n%s", (*iM)->VOUT_NOM, error);
                else            sprintf(displaybuff, "\fAPM %0.1fV\n%s", (*iM)->VOUT_NOM, error);
                s20.updateSUB20Display(displaybuff);
            }
            sleep(2);
        }
    }
}
