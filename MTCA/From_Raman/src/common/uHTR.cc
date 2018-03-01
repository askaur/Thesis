
#include "hcal/uhtr/uHTRsensors.hh"
#include "hcal/uhtr/uHTR.hh"
#include "uhal/HwInterface.hpp"
#include <unistd.h>
#include "uhal/ConnectionManager.hpp"
#include "uhal/ValMem.hpp"
#include "uhal/uhal.hpp"

namespace hcal {
  namespace uhtr {
  
    using namespace toolbox ;

    std::string uHTR::firmwareFlavorToString(uint8_t flavor) {
      const char* rvs=0;
      switch (flavor) {
      case (0x00) : rvs="Generic-1600"; break;
      case (0x01) : rvs="HF-1600"; break;
      case (0x41) : rvs="HF-4800"; break;
      case (0x42) : rvs="BHM"; break;
      default: rvs="Unknown"; break;
      };
      return std::string(rvs);
    }


    uHTR::uHTR(const std::string& name, std::string targetIP, uhal::HwInterface& hw_) : 
      u_hw(new uhal::HwInterface(hw_)),
      u_front(u_hw->getNode("FRONT")),
      u_back(u_hw->getNode("BACK")) 
    {

	card_ip   = targetIP ;
	card_name = name  ;
	slot_id   = -1 ;
	crate_id  = -1 ;
        theFirmwareRevFront = 0 ;   
        theFirmwareRevBack = 0 ;   
	flashCode = 0  ;
	firmwareRevisionRaw(true);
	firmwareRevisionRaw(false);

	if (is4800()||isBHM()) n_channels_per_fiber_=6;
	else n_channels_per_fiber_=3;
	sensors_=new uHTRsensors(this,u_back.getNode("SENSORS"));
    }

    uHTR::uHTR( const std::string& name, uhal::HwInterface& hw_ ) : 
      u_hw(new uhal::HwInterface(hw_)), 
      u_front(u_hw->getNode("FRONT")),
      u_back(u_hw->getNode("BACK")),
      card_name(name), 
      flashCode(0) { 

        theFirmwareRevFront = 0 ;   
        theFirmwareRevBack = 0 ;   
        for ( int i=0; i< NFIBERS; i++) {
	    m_fibers[i].n_BadData_rollovers = 0 ;
        }
	firmwareRevisionRaw(true);
	firmwareRevisionRaw(false);
	if (is4800()||isBHM()) n_channels_per_fiber_=6;
	else n_channels_per_fiber_=3;
	sensors_=new uHTRsensors(this,u_back.getNode("SENSORS"));



    }

    void uHTR::dispatch() { u_hw->dispatch(); }

    uHTR::~uHTR() {

    }

    // Card Identities
    void uHTR::setCrateSlot(int crate, int slot) {
         crate_id = crate;
         slot_id  = slot;
    }

    // Make connection with hardware 
    void uHTR::getHardware( std::string mapPath ) {

        std::string addressTable("file://");
        addressTable += mapPath;

        std::string deviceString("ipbusudp-1.3://");
        deviceString+= card_ip ;
        deviceString+=":50001";

    }

    static void unpackFirmwareRev(uint32_t val, uint8_t& flavor, uint8_t& major, uint8_t& minor, uint8_t& patch) {
	flavor=uint8_t((val>>24)&0xFF);
	major=uint8_t((val>>16)&0xFF);
	minor=uint8_t((val>>8)&0xFF);
	patch=uint8_t((val>>0)&0xFF);
    }


    // Return the raw firmware version
    uint32_t uHTR::firmwareRevision(bool front, uint8_t& flavor, uint8_t& major, uint8_t& minor, uint8_t& patch) {
      uhal::Node& fpga=(front)?(u_front):(u_back);

      uhal::ValWord< uint32_t > firmwareRev = fpga.getNode("FIRMWARE_REVISION").read();
      dispatch();
      
      if (firmwareRev.valid()) {
	uint32_t val= firmwareRev.value(); 
	unpackFirmwareRev(val,flavor,major,minor,patch);
	if (front) theFirmwareRevFront=val;
	else theFirmwareRevBack=val;
	return val ;
      } else {
	flavor= 0 ; 
	major = 0 ;
	minor = 0 ;
	patch = 0 ; 
	return 0;
      }
    }
    
    uint32_t uHTR::firmwareRevisionRaw(bool front) {

      uhal::Node& fpga=(front)?(u_front):(u_back);

      uhal::ValWord< uint32_t > firmwareRev = fpga.getNode("FIRMWARE_REVISION").read();
      uhal::ValWord< uint32_t > pcbVersion = fpga.getNode("BOARDIDENTIFIER").read();
      uhal::ValWord< uint32_t > serialNo = u_back.getNode("SERIAL_NUMBER").read();
      dispatch();

       if (firmwareRev.valid()) {
	 uint32_t val=firmwareRev.value();
	 if (front) theFirmwareRevFront=val;
	 else {
	   theFirmwareRevBack=val;
	   pcb_revision=pcbVersion.value();
	   serial_number=serialNo.value();
	 }
	 return val ;
       } else { 
          return 0 ;
       }
    }

    bool uHTR::is4800() const {
      uint8_t flavor, major, minor, patch;
      unpackFirmwareRev(theFirmwareRevFront,flavor,major,minor,patch);      
      return ((flavor&0xC0)==0x40);
    }

    bool uHTR::is1600() const {
      uint8_t flavor, major, minor, patch;
      unpackFirmwareRev(theFirmwareRevFront,flavor,major,minor,patch);      
      return ((flavor&0xC0)==0x00);
    }

    bool uHTR::isBHM() const {
      uint8_t flavor, major, minor, patch;
      unpackFirmwareRev(theFirmwareRevFront,flavor,major,minor,patch);      
      return (flavor==0x42);
    }

    std::vector<std::pair<double,double> > uHTR::getClockRates(bool front) {
      uhal::Node& fpga=(front)?(u_front):(u_back);

      static const int NCLOCKS=14;
      static const int NVOTES=5;
      std::vector<uint32_t> values[NCLOCKS];
      uint32_t median[NCLOCKS];
      
      for (int ivote=0; ivote<NVOTES; ivote++) {
	uhal::ValVector< uint32_t > clockRates = fpga.getNode("CLOCK_RATES").readBlock(NCLOCKS);
	dispatch();

	for (int j=0;j<NCLOCKS; j++) 
	  values[j].push_back(clockRates[j]);
      }	

      for (int j=0; j<NCLOCKS; j++) {
	std::sort(values[j].begin(),values[j].end());
	median[j]=values[j][NVOTES/2];
      }	
    
      std::vector<std::pair<double,double> > retval;

      const double expected_back[] = {25.0,100.0,40.08,40.08*2,40.08*3,40.08*4,40.08*6,40.08*8,11e-3,11e-5,-1,40.08,40.08,40.08*6};
      const double expected_front[] = {25.0,100.0,40.08,40.08*2,40.08*3,40.08*4,40.08*6,40.08*8,11e-3,11e-5,-1,40.08,40.08,40.08*6};
      const double* expected=(front)?(expected_front):(expected_back);

      for (int i=0; i<NCLOCKS; i++) {
	if (expected[i]<0) continue;
	if (i==8 || i==9)
	  retval.push_back(std::pair<double,double>(expected[i],  median[i]/1e6)); // reference clock here is 1 MHz, not 100 MHz
	else
	  retval.push_back(std::pair<double,double>(expected[i],  median[i]/1e4));
      } 
      return retval;
    }


    // Firmware Configuration
    void uHTR::reloadFromFlash( bool front ) {
      if (front) {
	u_back.getNode("FLASH_RELOAD_FRONT").write(1);
	dispatch();
	u_back.getNode("FLASH_RELOAD_FRONT").write(0);
	dispatch();
      } else {
	u_back.getNode("FLASH_RELOAD_BACK").write(1);
	dispatch();
      }
    }

    void uHTR::readFirmware(bool front, std::vector<unsigned char>& image, int verbosity) {
         int nsectors=13;
	 if ( flashCode==0 ) readFlashCode(front);
	 if ( (flashCode&0xFF)==0x16 || (flashCode&0xFF)==0x17 ) nsectors*=4;

	 for (int ns=0; ns<nsectors; ns++) 
	   readFirmwareSector(front, ns,image,verbosity);
    }

    void uHTR::readFlashCode( bool front ) {
         static const uint32_t CMD_RDID = 0x9F;
	 //         uhal::Node& fpga=( front )?(u_front):(u_back);
         uhal::Node& fpga=(u_back);

	 if (front) fpga.getNode("FLASH_BRIDGE_FRONT").write(1);
	 else fpga.getNode("FLASH_BRIDGE_FRONT").write(0);

	 fpga.getNode("FLASH_SPI_DIVISOR").write(2);
	 fpga.getNode("FLASH_OUT_BUFFER").write(CMD_RDID);
	 fpga.getNode("FLASH_BYTE_COUNT").write(4);
	 fpga.getNode("FLASH_START").write(1);
	 dispatch();

	 uhal::ValVector<uint32_t> spiReadMem = fpga.getNode("FLASH_IN_BUFFER").readBlock(4);
	 dispatch();

         if (spiReadMem.valid())  {
            flashCode = spiReadMem[1]&0xFF;
	    flashCode = flashCode<<8;
	    flashCode |= spiReadMem[2]&0xFF;
	    flashCode = flashCode<<8;
	    flashCode |= spiReadMem[3]&0xFF;
	 } else {
            XCEPT_RAISE(hcal::exception::Exception,"Error during attempt to read flash identifier code");
         }
      
    }

    void uHTR::readFirmwareSector(bool front, int isector, std::vector<unsigned char>& image, int verbosity) {

      uhal::Node& fpga=(u_back);
      
      if (front) fpga.getNode("FLASH_BRIDGE_FRONT").write(1);
      else fpga.getNode("FLASH_BRIDGE_FRONT").write(0);
      dispatch();
      
      static const uint32_t CMD_RD = 0x03;
      int sizeSector=0x40000;
      
      if (flashCode==0) readFlashCode(front);
      if ((flashCode&0xFF)==0x16 || (flashCode&0xFF)==0x17 ) sizeSector=0x10000; // small flash on these boards...
      
      const int readSize=256;
      const int nreads=1;
      unsigned char tries[2][256];
      
      for (int i=(isector)*sizeSector; i<(isector+1)*sizeSector; i+=readSize) {
	bool readsMatch=true;
	do {
	  readsMatch=true;
	  for (int j=0; j<nreads; j++) {
	    
	    try {
	      uhal::ValWord<uint32_t> spiStart ;
	      fpga.getNode("FLASH_SPI_DIVISOR").write(1);
	      
	      std::vector<uint32_t> cmd_vector ;
	      cmd_vector.push_back( CMD_RD ) ;
	      cmd_vector.push_back( (i>>16)&0xFF ) ;
	      cmd_vector.push_back( (i>>8)&0xFF ) ;
	      cmd_vector.push_back( i&0xFF ) ;
	      fpga.getNode("FLASH_OUT_BUFFER").writeBlock( cmd_vector );
	      
	      fpga.getNode("FLASH_BYTE_COUNT").write( 4+readSize );
	      fpga.getNode("FLASH_START").write( 1 );
	      spiStart = fpga.getNode("FLASH_BUSY").read();
	      dispatch();
	      
	      bool ok1=spiStart.valid();
	      if (verbosity>50) printf("Step 1 %d : %d\n",i,ok1);
	      
	      bool readDone;
	      do {
		
		spiStart = fpga.getNode("FLASH_BUSY").read();
		dispatch();
		bool ok=spiStart.valid();
		
		if (!ok) {
		  printf("Unable to read from device (status)\n");
		  readDone=false;
		} else {
		  if (verbosity>50) printf("Step 2 %d : %d %d\n",i,ok,spiStart.value() );
		  readDone=(spiStart.value()==0);
		}
	      } while (!readDone);
	      
	      uint32_t spiReadMemAddress = fpga.getNode("FLASH_IN_BUFFER").getAddress();
	      uhal::ValVector<uint32_t> spiReadMem = fpga.getClient().readBlock(spiReadMemAddress, readSize+4);
	      dispatch();
	      bool ok2=spiReadMem.valid();
	      if (verbosity>50) printf("Step 3 %d : %d\n",i,ok2);	
	      for (int k=0; k<readSize; k++) tries[j][k]=uint8_t(spiReadMem[k+4]);
	    } catch (std::exception& e) {
	      j--;
	      printf( "Exception!  %s\n",e.what() );
	    }	    
	  }
	  for (int k=0; nreads>1 && k<readSize; k++) {
	    if (tries[0][k]!=tries[1][k]) {
	      readsMatch=false;
	      //printf("read mismatch %x %x %d\n",tries[0][k],tries[1][k],k);
	    }
	  }
	  
	} while (!readsMatch);
	
	for (int k=0; k<readSize; k++) image.push_back(tries[0][k]);
	
      }
    }
    
    void uHTR::programFirmware(bool front, const std::vector<unsigned char>& image, int verbosity) {
         static const uint32_t CMD_WREN = 0x06;
	 static const uint32_t CMD_PP   = 0x02;
	 //static const uint32_t CMD_RDSR = 0x05;
	 static const uint32_t CMD_SE   = 0xD8;
	 std::vector<uint32_t> buffer;
	 size_t sectorSize=0x40000;
         uhal::Node& fpga=(u_back);
	 const bool validateWrite=false;
	 
         if (flashCode==0) readFlashCode(front);
         if ((flashCode&0xFF)==0x16 || (flashCode&0xFF)==0x17) sectorSize=0x10000; // small flash on these boards...

         int nsectors=(image.size()+sectorSize-1)/sectorSize;

	 if (verbosity>10) {
	    printf("Sector-by-sector programming (%d sectors)\n",nsectors);
            fflush(stdout);
         }

	 if (front) fpga.getNode("FLASH_BRIDGE_FRONT").write(1);
	 else fpga.getNode("FLASH_BRIDGE_FRONT").write(0);
	 dispatch();

         for (int ns=0; ns<=nsectors; ns++) {
	
  	     bool needErase=true;
             bool doneSector=false;

	     while (!doneSector) {

	           if (verbosity>20) {
		      printf("Sector %02d : ",ns);	
		      fflush(stdout);
	           }

             	   if (needErase) {

                      if (verbosity>20) {
                         printf(" erasing... ");	
                         fflush(stdout);
	              }

 	              // make the initial request to erase	    
                      try {
                          fpga.getNode("FLASH_SPI_DIVISOR").write(2);
			  fpga.getNode("FLASH_OUT_BUFFER").write(CMD_WREN);
			  fpga.getNode("FLASH_BYTE_COUNT").write(1);
			  fpga.getNode("FLASH_START").write(1);

			  // the reads just for small delay - may need to be tuned again 
			  for (int i=0; i<50; i++) 
			    fpga.getNode("FLASH_BUSY").read();

			  buffer.clear();
			  buffer.push_back(CMD_SE);

			  if (sectorSize==0x40000)
			    buffer.push_back(ns*4);
	    	          else
			    buffer.push_back(ns);
			  buffer.push_back(0);
			  buffer.push_back(0);

			  fpga.getNode("FLASH_OUT_BUFFER").writeBlock(buffer);
			  fpga.getNode("FLASH_BYTE_COUNT").write(4);	    
			  fpga.getNode("FLASH_START").write(1);
			  fpga.getNode("FLASH_BUSY").read();
			  dispatch();
		      } catch (xcept::Exception& e) {
			      printf( "Exception!  %s\n",e.message().c_str() );
		      }

	              // do status reads 
		      bool eraseDone = SPI_Status( 100000 ) ;
                      if ( !eraseDone ) std::cout<<" Erase Fail "<<std::endl;                      

		      /*
		      std::vector<unsigned char> imageVerify;
		      
		      readFirmwareSector(ns,imageVerify,verbosity);
		      for (std::vector<unsigned char>::size_type j=0; j<imageVerify.size(); j++)
			if (imageVerify[j]!=0xFF) printf("Not erased!\n");
		      */
	              needErase=false;
	           }

   	           if (verbosity>10) {
                      printf(" writing data...");
             	      fflush(stdout);
	           }

	           std::vector<unsigned char>::size_type sizeChunk=256;
   	           size_t sectorBase=sectorSize*ns;

	           for (std::vector<unsigned char>::size_type i=0; i<sectorSize; i+=sizeChunk) {
                       size_t writeBA=sectorBase+i;
	      
	               bool allMatched=true;
	               int nfail=0;
                       do {
             	          allMatched=true;
	      
                          try {
	   		      fpga.getNode("FLASH_OUT_BUFFER").write(CMD_WREN);
			      fpga.getNode("FLASH_BYTE_COUNT").write(1);
			      fpga.getNode("FLASH_START").write(1);
			      fpga.getNode("FLASH_BUSY").read();

			      dispatch();

			      buffer.clear();

			      buffer.push_back(CMD_PP);
			      buffer.push_back((writeBA>>16)&0xFF);
			      buffer.push_back((writeBA>>8)&0xFF);
			      buffer.push_back((writeBA)&0xFF);

			      for (size_t j=0; j<sizeChunk; j++)
				if (writeBA+j<image.size()) buffer.push_back(image[writeBA+j]);
				else buffer.push_back(0xFF);

			      fpga.getNode("FLASH_SPI_DIVISOR").write(1);
			      fpga.getNode("FLASH_OUT_BUFFER").writeBlock(buffer);
			      
			      fpga.getNode("FLASH_BYTE_COUNT").write(sizeChunk+4);
			      dispatch();
		          } catch (xcept::Exception& e) {
			          printf( "Exception!  %s\n",e.message().c_str() );
		          }

			  if (validateWrite) {

			    uhal::ValVector<uint32_t> spiMemRead = fpga.getNode("FLASH_OUT_BUFFER").readBlock(sizeChunk+4);
			    dispatch();

			    for (size_t kk=0; kk<sizeChunk+4; kk++) {
			      if (buffer[kk]!=spiMemRead[kk]) {
				//printf("%x (read) req=%x read=%x \n",uint32_t(writeBA+kk),buffer[kk],spiMemRead[kk]);
				allMatched=false;
		    	      }
			    }

			    if (!allMatched) {
			      nfail++;
			      if (nfail>20) {
                   	        printf("Unable to write valid data into device at 0x%x\n",uint32_t(writeBA));
                      	        return; // exception some day!
			      }
			      usleep(1000);
			    }

			  } else allMatched=true; 
		       } while (!allMatched);
	    
	               fpga.getNode("FLASH_START").write(1);
		       dispatch();
		       bool writeDone = SPI_Status( 1000 ) ;
                       if ( !writeDone ) std::cout<<" Write Fail !! "<<std::endl;
	           }
	      	      
	           if (verbosity>10) {
		      printf(" verifying... ");
		      //  printf(" verifying (%d)... ",kkk);
		      fflush(stdout);
		   }
	           std::vector<unsigned char> imageVerify;
	  
	           readFirmwareSector(front,ns,imageVerify,verbosity);
	  
	           doneSector=true;
	  
	           bool failureIsOnes=true;
	  
	           for (std::vector<unsigned char>::size_type j=0; j<sectorSize && j+sectorBase<image.size(); j++) {
	               if (image[j+sectorBase]!=imageVerify[j]) {
             	          if (verbosity>30) {
               		     printf("\n   Mismatch 0x%06x %02x(prog)!=%02x(readback)",uint32_t(j+sectorBase),image[j+sectorBase],imageVerify[j]);
             	          }
			  if (imageVerify[j]!=0xFF) failureIsOnes=false;
			  if (!failureIsOnes) needErase=true;
			  doneSector=false;			 
	               }
	           }
	
	           if (verbosity>10) {	 
	              if (!doneSector) printf(" (mismatch, must repeat)\n");
		      else printf("(ok)\n");
		   }
	     }
         }
    }

    
    bool uHTR::SPI_Status( int delay_us, bool front ) {

	static const uint32_t CMD_RDSR = 0x05;
        uhal::Node& fpga=(u_back);
	// do status reads
	int jj=0;
	bool status = false;
	do {
	   try {
	     uhal::ValVector<uint32_t> spiReadMemValue;
	     usleep( delay_us );
	     
	     uhal::ValWord<uint32_t> busy= fpga.getNode("FLASH_BUSY").read();
	     dispatch();
	     //	     printf("%x\n",busy.value());
	     if (busy.value()==0) {
	       fpga.getNode("FLASH_OUT_BUFFER").write(CMD_RDSR);
	       fpga.getNode("FLASH_BYTE_COUNT").write(2);
	       fpga.getNode("FLASH_START").write(1);
	       
	       for (int kk=0; kk<30; kk++) { 
	           fpga.getNode("FLASH_BUSY").read();
	       }
	       uhal::ValVector<uint32_t> spiReadMemValue = fpga.getNode("FLASH_IN_BUFFER").readBlock(2);
	       dispatch();	
	       status = spiReadMemValue.valid() && ((spiReadMemValue[1]&0x1)==0);	      
	     } else status=false;
	     jj++;
           } catch (xcept::Exception& e) {
	     printf( "Exception!  %s\n",e.message().c_str() );
	   }

	} while (jj<100 && !status );

        return status ;
    }

    bool uHTR::idSPI(const std::vector<unsigned char>& dataIn, std::vector<unsigned char>& dataOut, int nread) {

      uint32_t request=0;
      if (dataIn.size()>0) request=dataIn[0];  
      request=request<<8; 
      if (dataIn.size()>1) request|=dataIn[1];
      request=request<<8; 
      if (dataIn.size()>2) request|=dataIn[2];      
      
      u_back.getNode("MAC_SPI_TO_PROM").write(request);
      u_back.getNode("MAC_SPI_LEN").write((dataIn.size()+std::max(0,nread))*8);
      u_back.getNode("MAC_SPI_START").write(1);
      dispatch();

      // check for completion
      int jj=0;
      bool spiDone=false;
      uint32_t retword=0;

      do {

	uhal::ValWord<uint32_t> pollAsk = u_back.getNode("MAC_SPI_DONE").read();
	uhal::ValWord<uint32_t> rw = u_back.getNode("MAC_SPI_FROM_PROM").read();
	dispatch();
	
	if (!pollAsk.valid()) {
	  printf("Unable to read from device (status)\n");
	  return false;
	}
	  
	jj++;
	spiDone=(pollAsk.value()==1);
	if (spiDone) retword=rw.value();
	if (!spiDone) usleep(1000);	
      } while (jj<100 && !spiDone);
      if (!spiDone) return false;
      
      if (nread>0) {
	int offset=int(dataIn.size());
	if (nread>=4) offset=0;
	dataOut.clear();
	for (int jj=0; jj<nread; jj++)
	  dataOut.push_back(uint8_t(retword>>(8*(jj+offset))));
      }
      
      return true;
    }
   

    bool uHTR::clockSPI(int target, const std::vector<unsigned char>& dataIn, std::vector<unsigned char>& dataOut, int nread) {

      u_back.getNode("CLOCK_SPI_SELECT_TARGET").write(target);


      if (target==6) u_back.getNode("CLOCK_SPI_DIVISOR").write(0xF);
      else u_back.getNode("CLOCK_SPI_DIVISOR").write(0xE);

      std::vector<uint32_t> dataInConverted;
      for (size_t i=0; i<dataIn.size(); i++) dataInConverted.push_back(static_cast<uint32_t>(dataIn[i]));
      u_back.getNode("CLOCK_OUT_BUFFER").writeBlock(dataInConverted);
      u_back.getNode("CLOCK_BYTE_COUNT").write(dataIn.size()+std::max(0,nread));
      u_back.getNode("CLOCK_SPI_START").write(1);
      dispatch();


      uhal::ValVector<uint32_t> check = u_back.getNode("CLOCK_OUT_BUFFER").readBlock(dataIn.size());
      dispatch();
      int nmis=0;
      for (size_t i=0; i<dataIn.size(); i++)
	if (dataIn[i]!=check[i]) {
	  nmis++;
	}
      if (nmis!=0) printf("Mismatch in output buffer!\n");
      //      else printf("Output buffer checks\n");

      // check for completion
      int jj=0;
      bool spiDone=false;

      do {

	uhal::ValWord<uint32_t> pollAsk = u_back.getNode("CLOCK_SPI_BUSY").read();
	dispatch();
	
	if (!pollAsk.valid()) {
	  printf("Unable to read from device (status)\n");
	  return false;
	}
	  
	jj++;
	spiDone=(pollAsk.value()==0);
	if (!spiDone) usleep(1000);	
      } while (jj<100 && !spiDone);
      if (!spiDone) return false;
      
      if (nread>0) {
	int offset=int(dataIn.size());
	dataOut.clear();
	uhal::ValVector<uint32_t> request = u_back.getNode("CLOCK_IN_BUFFER").readBlock(nread+offset);
	dispatch();
	for (int jj=0; jj<nread; jj++)
	  dataOut.push_back(uint8_t(request[jj+offset]));
      }
      
      return true;
    }

    const uint8_t clock_image_240[] = {45,
      0,0x14,1,0xE4,2,0xA2,3,0x15,4,0x12,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x42,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0x7E,22,0xDF,23,0x1F,24,0x3F,25,0xE0,31,0x00,32,0x00,33,0x01,
      34,0x00,35,0x00,36,0x01,40,0x60,41,0x01,42,0x8B,43,0x00,44,0x00,
      45,0x14,46,0x00,47,0x00,48,0x14,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40};
    
    const uint8_t clock_image_320[] = { 45,
      0,0x14,1,0xE4,2,0xA2,3,0x15,4,0x12,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x42,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0xFE,22,0xDF,23,0x1F,24,0x3F,25,0x00,31,0x00,32,0x00,33,0x03,
      34,0x00,35,0x00,36,0x03,40,0x80,41,0x01,42,0x4F,43,0x00,44,0x00,
      45,0x14,46,0x00,47,0x00,48,0x14,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40};

    const uint8_t clock_image_fr[] = { 45,
      0,0x54,1,0xE4,2,0x24,3,0x15,4,0x92,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x40,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0x7E,22,0xDF,23,0x1F,24,0x3F,25,0xA0,31,0x00,32,0x00,33,0x0D,
      34,0x00,35,0x00,36,0x0D,40,0xA0,41,0x85,42,0x37,43,0x00,44,0x09,
      45,0x83,46,0x00,47,0x1B,48,0x2F,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40};

    void uHTR::setupClock(int target, int imagechoice) {
      static const uint8_t SILABS_CMD_SETADDRESS=(0x00);
      static const uint8_t SILABS_CMD_WRITE=(0x40);

      std::vector<unsigned char> cmdStream,dummy;
      const uint8_t* imagesequence=0;
      if (imagechoice==1 || imagechoice==320) imagesequence=clock_image_320;
      if (imagechoice==2 || imagechoice==240) imagesequence=clock_image_240;
      if (imagechoice==3) imagesequence=clock_image_fr;

      if (!imagesequence) return;
       
      for (uint8_t i=0; i<imagesequence[0]; i++) {
	//if (!(i%10)) cmdStream.clear();
	cmdStream.push_back(SILABS_CMD_SETADDRESS);
	cmdStream.push_back(imagesequence[2*i+1]);
	cmdStream.push_back(SILABS_CMD_WRITE);
	cmdStream.push_back(imagesequence[2*i+2]);
      }      
      if (target==1 || target==2) {
	clockSPI(target,cmdStream,dummy,0);
      } else if (target==3 || target==4 || target==6) {
	uint8_t uaddr=0;
	if (target==4) uaddr=0x20;
	int spi_target=4;
	if (target==6) spi_target=6;
	std::vector<uint8_t> di, dout;
	int ptr=0;

	// write the image
	do {
	  di.clear();
	  di.push_back(0x06); // write-enable
	  clockSPI(spi_target, di, dout, 0); // enable the SPI

	  di.clear();
	  di.push_back(0x02); // write instruction
	  di.push_back(uaddr|((ptr&0x7F00)>>8)); // pick the sector
	  di.push_back(uint8_t(ptr)); 
	  
	  for (int i=0; i<64; i++) {
	    if (i+ptr<int(cmdStream.size())) di.push_back(cmdStream[i+ptr]);
	    else di.push_back(0x0);
	  }
	  if (di.size()>3) {	    
	    clockSPI(spi_target, di, dout, 0);
	    double startT=hcal::toolbox::RateTool::decimalTime();
	    while ((hcal::toolbox::RateTool::decimalTime()-startT)<0.1){
	      ::usleep(1000);
	    }	    
	  }
	  ptr+=64;
	} while (ptr<int(cmdStream.size()) || ptr<1024 );
	// read-and-verify the image
	di.clear();
	di.push_back(0x03); // read instruction
	di.push_back(uaddr);
	di.push_back(0);
	
	clockSPI(spi_target,di,dout,cmdStream.size());
        
	for (size_t i=0; i<cmdStream.size() ; i++) {
	  if (dout[i]!=cmdStream[i]) printf("Mismatch %d %x %x\n",int(i),dout[i],cmdStream[i]);
	}
	
      }
    }


    bool uHTR::eepromClock(bool reload) {
      uhal::ValWord<uint32_t> vw=u_back.getNode("CLOCK_CPLD_RESET").read();
      if (reload) {
	u_back.getNode("CLOCK_CPLD_RESET").write(1);
      }
      dispatch();
      if (reload) {
	u_back.getNode("CLOCK_CPLD_RESET").write(0);
	dispatch();
      }
      return vw.value()==1;
    }

    // set up link for initialization
     std::vector<int> uHTR::link_init_status( bool printout ) {
         uhal::Node& linkNode=u_front.getNode("LINKS");
         std::vector<int> statV = link_init_status( linkNode, printout ) ;
         return statV ;
    }

    std::vector<int> uHTR::link_init_status( uhal::Node& linkNode, bool printout ){

         std::vector<int> init_st(24, 0) ;
         //uhal::Node& linkNode=u_front.getNode("LINKS");
         char reset_st[25], pll_st[25], idle_st[25];
         std::vector< uhal::ValWord<uint32_t> > sV0 ;
         std::vector< uhal::ValWord<uint32_t> > sV1 ;
         std::vector< uhal::ValWord<uint32_t> > sV2 ;
         for ( int i=0 ; i< NFIBERS; i++) {
             sprintf( reset_st, "RX%d_RESETDONE", i) ;
	     sprintf( pll_st,   "RX%d_PLLLKDET", i) ;
	     sprintf( idle_st,  "RX%d_ALIGNED", i) ;
	     uhal::ValWord<uint32_t>  sWord0 = linkNode.getNode( reset_st ).read() ;
	     uhal::ValWord<uint32_t>  sWord1 = linkNode.getNode( pll_st ).read() ;
	     uhal::ValWord<uint32_t>  sWord2 = linkNode.getNode( idle_st ).read() ;
             sV0.push_back( sWord0 ) ;
             sV1.push_back( sWord1 ) ;
             sV2.push_back( sWord2 ) ;
          }
 	  dispatch() ;
 
          
          for ( int i=0 ; i< NFIBERS; i++) {
              if ( sV0[i].value() == 1 ) init_st[i] += 1 ;
              if ( sV1[i].value() == 1 ) init_st[i] += 2 ;
              if ( sV2[i].value() == 1 ) init_st[i] += 4 ;
          }

          if ( printout ) {
             printf("\nLink  |") ;
             for ( int i=0; i< 24 ; i++)     printf(" %2d |", i ) ;
             printf("\nStatus|") ;
             for ( int i=0; i< 24 ; i++)     printf(" %2d |", init_st[i] ) ;
             printf("\n") ;
          }
          return init_st ;
    }

    // Initialize the fiber links
    void uHTR::link_init( uint32_t orbit_delay, bool auto_align, bool do_resets, bool text_info) {
      link_init_setup(orbit_delay, auto_align);
      
      void* cycling=link_init_multistep(0);
      while (cycling!=0) {
	cycling=link_init_multistep(cycling, text_info);
	::usleep(8000);
      }      
    } 

    void uHTR::link_init_setup(uint32_t orbit_delay, bool auto_align) {
      
      u_front.getNode( "ALIGN_DELAY").write( (uint32_t)(orbit_delay) );      
      dispatch();
      if (  auto_align ) u_front.getNode( "ALIGN_AUTOALIGN_MASK").write((uint32_t)(0xFFFFFF) ) ;   
      else u_front.getNode( "ALIGN_AUTOALIGN_MASK").write((uint32_t)(0x0) ) ;   
      dispatch();
    }
    

    struct link_init_state {
      link_init_state() {
	step=0;
	cycles=0;
	for (int i=0; i<uHTR::NFIBERS; i++) chON[i]=false;
      }
      int step;
      int cycles;
      bool chON[uHTR::NFIBERS];
    };

    
    void* uHTR::link_init_multistep(void* last_step_ptr, bool text_info) {
      uhal::Node& linkNode=u_front.getNode("LINKS");
      char buffer[100] ;
      const bool verbose=false;


      link_init_state* state=(link_init_state*)last_step_ptr;
      if (state==0) state=new link_init_state;
      if (verbose) printf("State.step=%d\n",state->step);


      // steps 0->20 are to do GTX Resets looking for byte alignment
      if (state->step>=0 && state->step<20) {
	if ((state->step%2)==0) { // here we reset things

	 
	  if (verbose) printf("Reseting ");
	  for (int i=0; i<NFIBERS; i++) {
	    if ( state->chON[i] ) continue ;
	    if (verbose) printf("%d ",i);
	    sprintf(buffer,"RX%d_RESET",i);	 
	    linkNode.getNode(buffer).write(1);
	  }
	  for (int i=0; i<NFIBERS; i++) {
	    if ( state->chON[i] ) continue ;
	    sprintf(buffer,"RX%d_RESET",i);	 
	    linkNode.getNode(buffer).write(0);
	  }
	  
	  dispatch();
	  if (verbose) printf("\n");
	  state->step++;
	} else { // here we check status
	  bool allpass=true;
	  std::vector<int> init_st = link_init_status( linkNode, false ) ;
          
	  int reset_done = 0 ;
	  for (int i=0; i<NFIBERS; i++) {
	    //if ( init_st[i] == 7 ) onepass = true ;
	    if ( init_st[i] >= 3  ) reset_done += 1 ;
	    if ( init_st[i] == 7 ) state->chON[i] = true ;
	    if ( init_st[i] != 7 ) {
	      if (allpass && verbose) printf("\n   Not status 7 on ");
	      if (verbose) printf(" %d ",i);
	      allpass = false ;
	    }
	  }
	  if (verbose) printf("Checking for reset and status %d %d\n",reset_done,allpass);

	  if (allpass || state->step==19) state->step=100; // done!
          else if (reset_done==24 && state->step==19) state->step=100; // go on anyway, maybe we don't have all the fibers connected
	  else if (reset_done==24) state->step++; // try resetting again!
	  // otherwise, we come back here to wait...	  
	}
      } else if (state->step==100) {
	if (text_info) {
	  printf("\nLink  |") ;
	  for ( int i=0; i<NFIBERS ; i++)     printf(" %2d |", i ) ;
	  printf("\nStatus|") ;
	  for ( int i=0; i<NFIBERS ; i++)  {
	    if (  state->chON[i] )   printf(" ON |" ) ;
	    if ( !state->chON[i] )   printf("  X |" ) ;
	  }
	  printf("\n\n") ;
	}
	state->step++;	 
	
      } else if (state->step==101) {
	
	u_front.getNode( "ALIGN_RESET_FIBERS").write((uint32_t)(0xFFFFFF) ) ;   
	dispatch();
	u_front.getNode( "ALIGN_RESET_FIBERS").write((uint32_t)(0) ) ;   
	dispatch();
	state->step=200;
      } else if (state->step>=200) {
	if ((state->step%10)==0) {	
	  // reset counters
	  link_reset_error_counters( linkNode ) ;
	  state->step++;
	} else if ((state->step%10)==9) {
	  // check error counters and reset bad links
	  int nbad=0;

	  std::vector<uhal::ValWord<uint32_t> > error_counters;
	  for (int i=0; i<NFIBERS; i++) {
	    sprintf(buffer,"RX%d_ERROR_COUNTER",i);
	    error_counters.push_back(linkNode.getNode(buffer).read());
	  }
	  dispatch();

	  for (int i=0; i<NFIBERS; i++) {
	    if ( error_counters[i].value() > 100 )  {
	      nbad++;
	      state->chON[i]=false;
	    }
	  }
	  if (nbad>0) {
	    if (state->cycles>3) state->step=-1; // done anyway
	    else {
	      state->step=0; 
	      state->cycles++;
	      if (verbose) printf("trying again (%d)\n",nbad);
	    }
	  } else state->step=-1; // done for happy reasons!
	} else state->step++; // just a wait...
      } else {
      
	state->step=-1;
      }
      

      if (state->step==-1) { // we're done!
	delete state;
	state=0;
      }
      return state;
    }


    void uHTR::link_set_gtx_error_threshold( uint32_t threshold_ , uint32_t mask_ ) {

         // Default value = 0xFFFFF
         uhal::Node& linkNode=u_front.getNode("LINKS");
         linkNode.getNode( "GTX_ERR_THRESHOLD").write( threshold_ ) ;   
         dispatch();
         
         // bit 0~23 represent 24 links
         linkNode.getNode( "BAD_DATA_RESET").write( mask_ ) ;   
         dispatch();
    }

    std::vector<uint32_t> uHTR::link_gtx_reset_count() {

         // Default value = 0xFFFFF
         uhal::Node& linkNode=u_front.getNode("LINKS");
         uhal::ValVector<uint32_t> cnt = linkNode.getNode("GTX_RESET_COUNTER").readBlock(24);
         dispatch();
         
         std::vector<uint32_t> cntV ;
         for ( int i=0; i<24 ; i++ ) cntV.push_back( cnt[i] )  ;

         return cntV ;
    }


    void uHTR::link_all_reset( ) {

         uhal::Node& linkNode=u_front.getNode("LINKS"); 
         char rname[25] ;
	 std::vector<std::string> names;
	
         for (int i =0; i < 24 ; i++ ) {
    	     sprintf( rname,"RX%d_RESET", i );	 
	     names.push_back(rname);
	     if (i<12) {
	       sprintf( rname,"TX%d_RESET", i );	 
	       names.push_back(rname);
	     }
	     if (i<4) {
	       sprintf( rname,"XTRA%d_RESET", i );	 
	       names.push_back(rname);
	     }
	     if (i<4) {
	       sprintf( rname,"F2B%d_RESET", i );	 
	       names.push_back(rname);
	     }
	     if (i<2) {
	       sprintf( rname,"SFP%d_RESET", i );	 
	       names.push_back(rname);
	     }	     
         }

	 for (std::vector<std::string>::const_iterator i=names.begin(); i!=names.end(); i++) {
             linkNode.getNode( *i ).write( 1 ) ;   
	 }

         dispatch();
	 link_init_status( linkNode, true ) ;


	 for (std::vector<std::string>::const_iterator i=names.begin(); i!=names.end(); i++) {
             linkNode.getNode( *i ).write( 0 ) ;   
	 }

         dispatch();
	 link_init_status( linkNode, true ) ;
    }

    void uHTR::link_GTX_reset(const std::vector<int>& ilink ) {

         uhal::Node& linkNode=u_front.getNode("LINKS"); 
         char rname[25] ;
         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_RESET", ilink[i] );	 
             linkNode.getNode( rname ).write( 1 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;

         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_RESET", ilink[i] );	 
             linkNode.getNode( rname ).write( 0 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;
    }

    void uHTR::link_CDR_reset(const std::vector<int>& ilink ) {

         uhal::Node& linkNode=u_front.getNode("LINKS"); 
         char rname[25] ;
         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_CDRRESET", ilink[i]);	 
             linkNode.getNode( rname ).write( 1 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;

         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_CDRRESET", ilink[i] );	 
             linkNode.getNode( rname ).write( 0 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;
    }

    void uHTR::link_buffer_reset(const std::vector<int>& ilink ) {

         uhal::Node& linkNode=u_front.getNode("LINKS"); 
         char rname[25] ;
         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_BUFRESET", ilink[i] );	 
             linkNode.getNode( rname ).write( 1 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;

         for ( size_t i =0; i < ilink.size() ; i++ ) {
    	     sprintf( rname,"RX%d_BUFRESET", ilink[i] );	 
             linkNode.getNode( rname ).write( 0 ) ;   
         }
         dispatch();
	 if ( ilink.size() > 0 ) link_init_status( linkNode, true ) ;
    }

    // Initialize the error counter for fiber links
    void uHTR::link_reset_error_counters() {
         uhal::Node& linkNode=u_front.getNode("LINKS");
         link_reset_error_counters( linkNode ) ;
    }

    void uHTR::link_reset_error_counters( uhal::Node& linkNode ) {
      char buffer[100];
      //uhal::Node& linkNode=u_front.getNode("LINKS");
      for (int i=0; i<NFIBERS; i++) {
          sprintf(buffer,"RX%d_RESET_ERR_COUNTER",i);	 
	  linkNode.getNode(buffer).write(1);
      }
      dispatch();

      for (int i=0; i<NFIBERS; i++) {

          sprintf(buffer,"RX%d_RESET_ERR_COUNTER",i);	 
 	  linkNode.getNode(buffer).write(0);
	
   	  m_fibers[i].badDataRate.resetHistory();
	  m_fibers[i].rx_link_error_rate=-1;
	  m_fibers[i].n_BadData=0;
	  m_fibers[i].n_BadData_rollovers=0;
	  m_fibers[i].badDataCounterOn=0;
      }
      dispatch();
    }

    void uHTR::link_query() {
      char buffer[100];
      std::vector<uhal::ValWord<uint32_t> > error_counters;

      char reset_st[25], pll_st[25], idle_st[25];
      std::vector< uhal::ValWord<uint32_t> > rstV ;
      std::vector< uhal::ValWord<uint32_t> > pllV ;
      std::vector< uhal::ValWord<uint32_t> > cmaV ;

      uhal::Node& linkNode=u_front.getNode("LINKS");

      for (int i=0; i<NFIBERS; i++) {
	sprintf(buffer,"RX%d_ERROR_COUNTER",i);
	error_counters.push_back(linkNode.getNode(buffer).read());

	sprintf( reset_st, "RX%d_RESETDONE", i) ;
	sprintf( pll_st,   "RX%d_PLLLKDET", i) ;
	sprintf( idle_st,  "RX%d_ALIGNED", i) ;
	rstV.push_back( linkNode.getNode( reset_st ).read() );
	pllV.push_back( linkNode.getNode( pll_st ).read() );
	cmaV.push_back( linkNode.getNode( idle_st ).read() );
      }
      
      uhal::ValWord<uint32_t>   counter_enable = linkNode.getNode("RX_ERROR_COUNTER_ENABLE").read() ;
      uhal::ValVector<uint32_t> align_occupancy=u_front.getNode("ALIGN_OCCUPANCY").readBlock(24);
      uhal::ValVector<uint32_t> align_delta=u_front.getNode("ALIGN_DELTA").readBlock(24);
      uhal::ValVector<uint32_t> align_status=u_front.getNode("ALIGN_STATUS").readBlock(24);
      uhal::ValVector<uint32_t> n_BadAlign=u_front.getNode("ALIGN_BAD_COUNT").readBlock(24);
      uhal::ValVector<uint32_t> orbit_rate=u_front.getNode("ALIGN_FIBER_RATE").readBlock(24);

      dispatch();
      usleep(1000) ;

      int nreset=0;
      for ( int i=0; i<NFIBERS; i++) {
	m_fibers[i].n_BadAlign      = n_BadAlign[i]  ;
	m_fibers[i].align_status    = align_status[i] ;
	m_fibers[i].orbit_rate      = orbit_rate[i];
	m_fibers[i].align_occupancy = align_occupancy[i]  ;
	m_fibers[i].align_delta     = align_delta[i]  ;
	uint64_t theBadDataSum = (m_fibers[i].n_BadData_rollovers*16777215) + error_counters[i].value();
	//m_fibers[i].n_BadData       = theBadDataSum   ;
	m_fibers[i].n_BadData       = error_counters[i].value() ;
	m_fibers[i].badDataRate.addObservation( theBadDataSum ) ;
	m_fibers[i].badDataCounterOn = (counter_enable.value() >> i) & 1  ;
	m_fibers[i].rx_link_error_rate = m_fibers[i].badDataRate.getRate() ; 
	//printf("   rate    :   %f \n", m_fibers[i].badDataRate->getRate()  ) ;

        m_fibers[i].reset_done   = rstV[i].value() ;
        m_fibers[i].pll_lock     = pllV[i].value() ;
        m_fibers[i].byte_aligned = cmaV[i].value() ;
        
	if ( error_counters[i].value() == 16777215 ) {
	  m_fibers[i].n_BadData_rollovers    += 1    ;
	  sprintf(buffer,"RX%d_RESET_ERR_COUNTER",i);
	  linkNode.getNode(buffer).write(1);
	  linkNode.getNode(buffer).write(0);
	  nreset++;
	}
      }
      if (nreset>0) dispatch();
    }


    uint32_t uHTR::info_link_ResetInfo(int ilid, int itemId) {

      uint32_t reset_val = 9 ;
      if ( ilid>=0 && ilid<NFIBERS ) {
         if ( itemId == 0 ) reset_val = (uint32_t)m_fibers[ilid].reset_done ;
         if ( itemId == 1 ) reset_val = (uint32_t)m_fibers[ilid].pll_lock ;
         if ( itemId == 2 ) reset_val = (uint32_t)m_fibers[ilid].byte_aligned ;
      } 
      return reset_val ;
    }

    uint32_t uHTR::info_link_BadDataCounterOn(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? (uint32_t)m_fibers[ilid].badDataCounterOn : 0 ;
    }

    double uHTR::info_link_BadDataRate(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].badDataRate.getRate() : 0 ;
    }

    uint32_t uHTR::info_link_nBadData(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].n_BadData : 0 ;
    }

    uint32_t uHTR::info_link_nBadDataRollover(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].n_BadData_rollovers : 0 ;
    }

    uint32_t uHTR::info_link_nBadAlign(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].n_BadAlign : 0 ;
    }

    uint32_t uHTR::info_link_alignStatus(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].align_status : 0 ;
    }

    float uHTR::info_link_orbitRate(int ilid) {
      float  orbit_rate_ = m_fibers[ilid].orbit_rate / 1000. ;
      return (ilid>=0 && ilid<NFIBERS) ? orbit_rate_ : 0 ;
    }

    uint32_t uHTR::info_link_alignOccupancy(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].align_occupancy : 0 ;
    }
    uint32_t uHTR::info_link_alignDelta(int ilid) {
      return (ilid>=0 && ilid<NFIBERS) ? m_fibers[ilid].align_delta : 0 ;
    }

    float uHTR::info_clk_rates(int clk_id) {
      float clk_rate_ = clk_rates[clk_id] / 10000. ;
      return (clk_id>=0 && clk_id<=8)? clk_rate_:0.;
    }
 
    // Ckeck align_orbit_delay value 
    void uHTR::link_setup_query( uint32_t& align_delay, bool& auto_align) {

      uhal::ValWord<uint32_t>  t1 = u_front.getNode("ALIGN_ORBIT_DELAY").read();
      dispatch();

      align_delay = t1.value() ;
    }

    void uHTR::link_spySetup(bool enable_capture_sw, bool enable_capture_dtc, bool orbit_no_wait, bool rawwords, int pre, int delay) {
      u_front.getNode("SPY_RAM_ERASE_ENABLE").write(1);
      u_front.getNode("FE_SPY_PULSE_PRE").write(pre);
      u_front.getNode("FE_SPY_PULSE_DELAY").write(delay);
      u_front.getNode("FE_SPY_ENABLE_CAPTURE_SW").write(enable_capture_sw?(1):(0));
      u_front.getNode("FE_SPY_ENABLE_CAPTURE_DTC").write(enable_capture_dtc?(1):(0));
      u_front.getNode("FE_SPY_ORBIT_NO_WAIT").write(orbit_no_wait?(0):(1));
      u_front.getNode("FE_SPY_RAW_WORDS").write(rawwords?(1):(0));
      u_front.getNode("SPY_CAPTURE_ENABLE").write(0);
      dispatch();
    }
 
    void uHTR::link_spyCaptureSetup(int howMany) {
      u_front.getNode("SPY_RAM_ERASE_ENABLE").write(1);
      u_front.getNode("SPY_CAPTURE_ENABLE").write(1);
      u_front.getNode("FE_SPY_PULSE_PRE").write(0);
      u_front.getNode("FE_SPY_PULSE_DELAY").write(0);
      if (is1600()){u_front.getNode("FE_SPY_CAPTURE_BX_PADDING").write(howMany*2);}
      else{u_front.getNode("FE_SPY_CAPTURE_BX_PADDING").write(howMany*6);}
      u_front.getNode("FE_SPY_ENABLE_CAPTURE_SW").write(0);
      u_front.getNode("FE_SPY_ENABLE_CAPTURE_DTC").write(1);
      u_front.getNode("FE_SPY_ORBIT_NO_WAIT").write(1);
      u_front.getNode("FE_SPY_RAW_WORDS").write(0);
      dispatch();
    }
 
    void uHTR::link_spyExecute() {

      // Set the control to idle  
      u_front.getNode("FE_SPY_START").write(0xAA55);
      uhal::ValWord<uint32_t> spy_rate = u_front.getNode("SPY_PULSE_RATE").read() ;
      uhal::ValWord<uint32_t> accept_rate = u_front.getNode("ACCEPT_PULSE_RATE").read() ;
      dispatch();
      // printf(" Spy pulse rate: %d , Accept pulse rate: %d \n", spy_rate.value() , accept_rate.value() ) ;

      // Check erase pulse
      uhal::ValWord<uint32_t> erase_set = u_front.getNode("SPY_RAM_ERASE_ENABLE").read() ;
      dispatch();
 
      if ( erase_set.value() != 0 ) {  
	//        uhal::ValWord<uint32_t> eraseCount = u_front.getNode("FE_ERASE_PULSE_COUNTER").read() ;
	 uhal::ValWord<uint32_t> spyState = u_front.getNode("FE_SPY_RAM_STATE").read() ;
	 dispatch();
	 // printf(" Before Erase Pulse count : %d , state : %d \n", eraseCount.value() , spyState.value() ) ;

	 u_front.getNode("FE_SPY_START").write(0x5555);
	 dispatch();

	 //	 uhal::ValWord<uint32_t> eraseCount1 = u_front.getNode("FE_ERASE_PULSE_COUNTER").read() ;      
	 uhal::ValWord<uint32_t> spyState1 = u_front.getNode("FE_SPY_RAM_STATE").read() ;
	 dispatch();
	 //printf(" After Erase Pulse count  : %d, state : %d \n", eraseCount1.value(), spyState1.value() ) ;
      }
      // Check spy pulse
      uhal::ValWord<uint32_t> spyCount = u_front.getNode("FE_SPY_PULSE_COUNTER").read() ;      
      uhal::ValWord<uint32_t> spyState = u_front.getNode("FE_SPY_RAM_STATE").read() ;      
      dispatch();
      //printf(" Before Spy Pulse count : %d , state : %d \n", spyCount.value() , spyState.value() ) ;

      // issue ipbus spy pulse
      u_front.getNode("FE_SPY_START").write(0xAAAA);
      uhal::ValWord<uint32_t> spy_rate1 = u_front.getNode("SPY_PULSE_RATE").read() ;
      uhal::ValWord<uint32_t> accept_rate1 = u_front.getNode("ACCEPT_PULSE_RATE").read() ;
      dispatch();
      //printf(" Spy pulse rate: %d , Accept pulse rate: %d \n", spy_rate1.value() , accept_rate1.value() ) ;
 
      // the sleep is wait for orbit pulse at 100 Hz
      // for immediate mode , sleep is not necessary .. 
      usleep(100000) ;

      uhal::ValWord<uint32_t> spyCount1 = u_front.getNode("FE_SPY_PULSE_COUNTER").read() ;      
      uhal::ValWord<uint32_t> spyState1 = u_front.getNode("FE_SPY_RAM_STATE").read() ;      
      dispatch();
      //printf(" After Spy Pulse count : %d, state : %d \n", spyCount1.value(), spyState1.value() ) ;

    }

    std::vector<uint32_t> uHTR::link_spyRead(int id ) {

      std::vector<uint32_t> buffer;
      // first, we have to get the pointer to the relevant spy buffer
      //uhal::ValVector<uint32_t> getPtr = u_front.getNode("FE_SPY_RAM_PTRS").readBlock( NFIBERS );
      u_front.getNode("FE_FIBER_ID").write( id ) ;
      dispatch();
      uhal::ValWord<uint32_t> words= u_front.getNode("FE_SPY_RAM_WORDS").read();
      dispatch();

      uhal::ValWord<uint32_t> fiber_id = u_front.getNode("FE_FIBER_ID").read();
      dispatch();
      printf(" ***  Reading fiber (%d) ***\n", fiber_id.value() ) ;
      //uint32_t ramPtr=getPtr[ int(id) ];
      uint32_t ramPtr= 0x80200030 ;

      // now we read it, in blocks of 256 words
      // sc: NUM_SPY_RAM from verilog
      int chunk=256;
      for (size_t i=0; i< words.value()/chunk; i++) {
	uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock(ramPtr+i*chunk,chunk);
	dispatch();
	for (int j=0; j<chunk; j++) {
	    buffer.push_back( readBlock[j] );
        } 
      }      
      return buffer ;
    }

    std::vector<uint32_t> uHTR::link_spyCaptureRead(int id, uint32_t address, uint32_t nBX_before, uint32_t nBX_after, uint32_t &trigger) {

         int trigger_ ;
         int nBx_before_ ;
         std::vector<uint32_t> buf = link_spyCaptureRead( id, address, nBx_before_ , nBX_after, trigger_ ) ;
         nBX_before =  (uint32_t) nBx_before_ ;
         trigger =  (uint32_t) trigger_ ;
         return buf ;
    }

    std::vector<uint32_t> uHTR::link_spyCaptureRead(int id, uint32_t address, int nBX_before, uint32_t nBX_after, int &trigger) {

      uint16_t wordsPerBX=6;
      if (is1600())wordsPerBX=2;
      
      std::vector<uint32_t> buffer;
      // first, we have to get the pointer to the relevant spy buffer
      u_front.getNode("FE_FIBER_ID").write( id ) ;
      dispatch();
      // buffer size: 2048 words ->  2048/6 = 341 bx depth for 4.8 Gbps , 1024 bx for 1.6 Gbps
      // -->  is word 16 bit or 32 bit ?  
      uhal::ValWord<uint32_t> words      = u_front.getNode("FE_SPY_RAM_WORDS").read();
      uhal::ValWord<uint32_t> fiber_id   = u_front.getNode("FE_FIBER_ID").read();
      uhal::ValWord<uint32_t> nBX_padding=u_front.getNode("FE_SPY_CAPTURE_BX_PADDING").read();
      dispatch();

      printf(" ***  Reading fiber (%d) ***\n", fiber_id.value() ) ;
      //uint32_t ramPtr=getPtr[ int(id) ];
      if ( nBX_padding.value()/wordsPerBX<nBX_after ) {nBX_after=nBX_padding.value()/wordsPerBX;}
      uint32_t startbufferPtr= 0x80200030 ;
      uint32_t endbufferPtr  = 0x80200030+words.value() ;
      uint32_t triggerPtr= address;
      uint32_t width = (nBX_before + nBX_after + 1)*wordsPerBX ;
      uint32_t startPtr = triggerPtr-(nBX_before*wordsPerBX) ;
      if ( startPtr > endbufferPtr ) startPtr = startbufferPtr + ( startPtr - endbufferPtr ) ;

      // use block read  - hope it will be faster
      //uint32_t width = (nBX_before+nBX_after+1)*wordsPerBX ;
      if ( width > words.value() ) { 
         width = words.value() ; // readout window must be smaller than spyRam size
         printf(" Out-of-range, set read window size = buffer size (%d Bx) \n", width/wordsPerBX ) ;
      }

      if (startPtr<startbufferPtr) {
         startPtr=endbufferPtr-(startbufferPtr-startPtr) ; 
         uint32_t width1 = startbufferPtr-startPtr ;
         uint32_t width2 = width - width1 ;
         uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock( startPtr, width1 );
         dispatch();
         for ( size_t i=0; i< readBlock.size() ; i++ ) buffer.push_back( readBlock[i] ) ;  

         uhal::ValVector<uint32_t> readBlock1 = u_hw->getClient().readBlock( startbufferPtr, width2 );
         dispatch();
         for ( size_t i=0; i< readBlock1.size() ; i++ ) buffer.push_back( readBlock1[i] ) ;  
      }
      else if ( startPtr > startbufferPtr && startPtr+width > endbufferPtr ) { 
         uint32_t width1 = endbufferPtr - startPtr ;
         uint32_t width2 = width - width1 ;
         uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock( startPtr, width1 );
         dispatch();
         for ( size_t i=0; i< readBlock.size() ; i++ ) buffer.push_back( readBlock[i] ) ;  

         uhal::ValVector<uint32_t> readBlock1 = u_hw->getClient().readBlock( startbufferPtr, width2 );
         dispatch();
         for ( size_t i=0; i< readBlock1.size() ; i++ ) buffer.push_back( readBlock1[i] ) ;  

      } else {

         uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock(startPtr, width );
         dispatch();
         for ( size_t i=0; i< readBlock.size() ; i++ ) buffer.push_back( readBlock[i] ) ;  
      }
      // report the position of trigger word , if < 0 which means readout window is behind trigger
      trigger = triggerPtr - startPtr ;

      /*  
      for (size_t i=0; i< (nBX_before+nBX_after+1)*wordsPerBX; i++) {
	if (startPtr==triggerPtr){trigger=i;}
	uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock(startPtr,1);
	dispatch();
	buffer.push_back( readBlock[0] );
	startPtr++;
	if (startPtr==endbufferPtr){startPtr=startbufferPtr;}
      } 
      */     
      return buffer ;
    }

    uint32_t uHTR::link_spyState(){
      uhal::ValWord<uint32_t> spyState = u_front.getNode("FE_SPY_RAM_STATE").read();
      dispatch();
      return spyState.value();
    }

    uint32_t uHTR::link_spy_writeptr_L1A_read() {
      uhal::ValWord<uint32_t> writeptr_L1A = u_front.getNode("FE_SPY_WRITEPTR_L1A").read();
      dispatch();
      return writeptr_L1A.value();
    }

    uint32_t uHTR::link_accept_pulse_rate() {
      uhal::ValWord<uint32_t> pulse = u_front.getNode("ACCEPT_PULSE_RATE").read();
      dispatch();
      return pulse.value();
    }


    void uHTR::link_spySetStart(uint32_t setstart){
      u_front.getNode("FE_SPY_START").write(setstart);
      dispatch();
    }
    void uHTR::link_histo_start() {
      u_front.getNode("LINK_HISTO.RUN").write(1);
      dispatch();
    }
    void  uHTR::link_histo_stop() {
      u_front.getNode("LINK_HISTO.RUN").write(0);
      dispatch();
    }

    void uHTR::link_histo_orbit_integrate(uint32_t orbits) {      
      if (orbits==0) {
	u_front.getNode("LINK_HISTO.INTEGRATE").write(0);
	u_front.getNode("LINK_HISTO.ORBITS").write(orbits);
      } else {
        u_front.getNode("LINK_HISTO.RUN").write(0);
	u_front.getNode("LINK_HISTO.INTEGRATE").write(1);
	u_front.getNode("LINK_HISTO.ORBITS").write(orbits);
      }
      dispatch();
    }

    void uHTR::link_histo_orbit_integrate_info(uint32_t& orbits, bool& active) {
      uhal::ValWord<uint32_t> ia=u_front.getNode("LINK_HISTO.INTEGRATE").read();
      uhal::ValWord<uint32_t> ors=u_front.getNode("LINK_HISTO.ORBITS").read();

      dispatch();

      orbits=ors.value();
      active=(ia.value()!=0);
    }

    void uHTR::link_histo_setSeparateCapIDs(bool separateCapIDs) {
      u_front.getNode("LINK_HISTO.CAPIDSMSB").write(separateCapIDs);
    }

    bool uHTR::link_histo_ready() {
      uhal::ValWord<uint32_t> ia=u_front.getNode("LINK_HISTO.INTEGRATIONDONE").read();

      dispatch();

      return (ia.value()!=0);
    }
    void uHTR::link_histo_read_FirstAndLastOrbits(uint32_t& firstorbit, uint32_t& lastorbit) {
      uhal::ValWord<uint32_t> fo=u_front.getNode("LINK_HISTO.FIRSTORBIT").read();
      uhal::ValWord<uint32_t> lo=u_front.getNode("LINK_HISTO.LASTORBIT").read();
      dispatch();
      firstorbit=fo.value();
      lastorbit=lo.value();
    }

    void uHTR::link_histo_clear() {
      u_front.getNode("LINK_HISTO.CLEAR").write(1);
      dispatch();
      u_front.getNode("LINK_HISTO.CLEAR").write(0);
      dispatch();
    }
    bool uHTR::link_histo_clear_done() { 
      uhal::ValWord<uint32_t> dv=u_front.getNode("LINK_HISTO.CLEARING").read();
      dispatch();
      return dv.value()==0;
    }

    std::vector<uint32_t> uHTR::link_histo_read(int nhisto) {
      std::vector<uint32_t> histogram;
      uhal::ValWord<uint32_t> nh=u_front.getNode("LINK_HISTO.NHIST").read();
      dispatch();
      if (nhisto>=int(nh.value()) || nhisto<0) return histogram; // out of range

      histogram.reserve(256);
      uint32_t baseAddress = u_front.getNode("LINK_HISTO.HISTOGRAM").getAddress();
      baseAddress+=(nhisto*256);
      uhal::ValVector<uint32_t> rh = u_front.getClient().readBlock(baseAddress, 256);
      dispatch();
      for (int i=0; i<256; i++)
	histogram.push_back(rh[i]);
      return histogram;
    }


    void uHTR::i2cStart(const std::vector<uint32_t>& cmds) {
      u_back.getNode("SENSORS.ENABLE_ENGINE").write(0);
      u_back.getNode("SENSORS.INSTRUCTION_FIFO").writeBlock(cmds);
      u_back.getNode("SENSORS.ENABLE_ENGINE").write(1);
      dispatch();
    }
    bool uHTR::i2cDone() {
      uhal::ValWord<uint32_t> dv=u_back.getNode("SENSORS.ENGINE_BUSY").read();
      dispatch();
      return dv.value()==0;
    }
    std::vector<uint32_t> uHTR::i2cResults() {
      u_back.getNode("SENSORS.ENABLE_ENGINE").write(0);
      uhal::ValWord<uint32_t> wc=u_back.getNode("SENSORS.RESULT_WORDS").read();
      dispatch();
      printf("Reading %d words\n",wc.value());
      static const uint32_t blockSize=128;
      std::vector<uint32_t> retval;

      for (uint32_t ptr=0; ptr<wc.value(); ptr+=blockSize) {
	uhal::ValWord<uint32_t> rv[blockSize];
	for (uint32_t i=0; i<blockSize && ptr+i<wc.value(); i++) 
	  rv[i]=u_back.getNode("SENSORS.RESULT_FIFO").read();
	dispatch();
	for (uint32_t i=0; i<blockSize && ptr+i<wc.value(); i++)
	  retval.push_back(rv[i].value());
      }
      return retval;
    }


    void uHTR::fe_ram_setup(bool enabled, bool ttc_aligned, uint32_t ttc_delay) {
      if (enabled) {
	u_front.getNode("FE_RAM.ENABLE").write(0xFFFFFF);
	u_front.getNode("FE_RAM_USE").write(1);
      } else {
	u_front.getNode("FE_RAM.ENABLE").write(0);
	u_front.getNode("FE_RAM_USE").write(0);
      }
      u_front.getNode("FE_RAM.SYNC_TO_TTC").write((ttc_aligned)?(1):(0));
      u_front.getNode("FE_RAM.TTC_ORBIT_DELAY").write(ttc_delay);
      dispatch();
    }
    void uHTR::fe_ram_setup_query(bool& enabled, bool& ttc_aligned, uint32_t& ttc_delay) {
      uhal::ValWord<uint32_t> t1=u_front.getNode("FE_RAM.ENABLE").read();
      uhal::ValWord<uint32_t> t2=u_front.getNode("FE_RAM.SYNC_TO_TTC").read();
      uhal::ValWord<uint32_t> t3=u_front.getNode("FE_RAM.TTC_ORBIT_DELAY").read();
      dispatch();
      enabled=(t1.value()!=0);
      ttc_aligned=(t2.value()!=0);
      ttc_delay=t3.value();
    }
    void uHTR::fe_ram_program_fiber(int ifiber, const std::vector<uint32_t>& data) {
      // get the size of the volume and the pointer
      uhal::ValWord<uint32_t> nw=u_front.getNode("FE_RAM.NWORDS").read();
      uhal::ValVector<uint32_t> ptrs=u_front.getNode("FE_RAM.POINTERS").readBlock(24);
      dispatch();
      if (ifiber>=0 && ifiber<24 && data.size()<=nw.value()) {
	u_hw->getClient().writeBlock(ptrs[ifiber],data);
	u_hw->getClient().dispatch();
      }
    }
    void uHTR::fe_ram_read_fiber(int ifiber, std::vector<uint32_t>& data) {
      data.clear();
      uhal::ValWord<uint32_t> nw=u_front.getNode("FE_RAM.NWORDS").read();
      uhal::ValVector<uint32_t> ptrs=u_front.getNode("FE_RAM.POINTERS").readBlock(24);
      dispatch();
      if (ifiber>=0 && ifiber<24) {
	uhal::ValVector<uint32_t> res=u_hw->getClient().readBlock(ptrs[ifiber],nw.value());
	u_hw->getClient().dispatch();
	data.reserve(res.size());
	for (size_t i=0; i<res.size(); i++)
	  data.push_back(res[i]);
      }
    }

    void uHTR::dtc_reset(bool full, bool errors) {
      if (full) {
	u_front.getNode("DTC.RESET").write(1);
	u_back.getNode("DTC.RESET").write(1);
      } else if (errors) {
	u_front.getNode("DTC.RESET_ERROR_COUNTERS").write(1);
	u_back.getNode("DTC.RESET_ERROR_COUNTERS").write(1);
      }
      dispatch();
    }

    void uHTR::setDTCPhase(bool front, bool override, int phase, int threshold) {
         uhal::Node& fpga=(front)?(u_front):(u_back);

	 fpga.getNode("DTC.TTC_STREAM_PHASE_CONTROL").write(phase&0x3);
	 fpga.getNode("DTC.TTC_STREAM_PHASE_OVERRIDE").write(override?(1):(0));
	 fpga.getNode("DTC.TTC_STREAM_AUTO_THRESHOLD").write(threshold&0xFF);
	 dispatch();
    }

    void uHTR::get_dtc_info( bool front, float& dtc_rate_40mhz, float& dtc_rate_orbit, uint32_t& dtc_bunch_count, uint32_t& dtc_event_number, uint32_t& dtc_bc0_error, uint32_t& dtc_single_error, uint32_t& dtc_double_error, uint32_t& dtc_ttc_stream_phase, uint32_t& dtc_ttc_stream_phase_locked)  { 

         uhal::Node& fpga=(front)?(u_front):(u_back);

         uhal::ValWord<uint32_t> t1=fpga.getNode("DTC.RATE_40MHZ").read();
	 uhal::ValWord<uint32_t> t2=fpga.getNode("DTC.RATE_ORBIT").read();
	 uhal::ValWord<uint32_t> t3=fpga.getNode("DTC.BUNCH_COUNT").read();
	 uhal::ValWord<uint32_t> t4=fpga.getNode("DTC.EVENT_NUMBER").read();
	 uhal::ValWord<uint32_t> t5=fpga.getNode("DTC.BC0_ERROR").read();
	 uhal::ValWord<uint32_t> t6=fpga.getNode("DTC.SINGLE_ERROR").read();
	 uhal::ValWord<uint32_t> t7=fpga.getNode("DTC.DOUBLE_ERROR").read();
	 uhal::ValWord<uint32_t> t8=fpga.getNode("DTC.TTC_STREAM_PHASE").read();
	 uhal::ValWord<uint32_t> t9=fpga.getNode("DTC.TTC_STREAM_PHASE_LOCKED").read();
	 dispatch();
         dtc_rate_40mhz=t1.value()/1e4;
	 dtc_rate_orbit=t2.value()/1e3;
	 dtc_bunch_count=t3.value();
	 dtc_event_number=t4.value();
	 dtc_bc0_error=t5.value();
	 dtc_single_error=t6.value();
	 dtc_double_error=t7.value();
	 dtc_ttc_stream_phase=t8.value();
	 dtc_ttc_stream_phase_locked=t9.value();

     }

      //event/run info
    void uHTR::get_run_info(bool front, uint32_t& lumi_nibble,  uint32_t& lumi_section, uint32_t& cms_run, uint32_t& lhc_fill){    
         uhal::Node& fpga=(front)?(u_front):(u_back);
	 uhal::ValWord<uint32_t> t1=fpga.getNode("DTC.LUMI_NIBBLE").read();
	 uhal::ValWord<uint32_t> t2=fpga.getNode("DTC.LUMI_SECTION").read();
	 uhal::ValWord<uint32_t> t3=fpga.getNode("DTC.CMS_RUN").read();
	 uhal::ValWord<uint32_t> t4=fpga.getNode("DTC.LHC_FILL").read();
	 dispatch();
	 lumi_nibble=t1.value();
	 lumi_section=t2.value();
	 cms_run=t3.value();
	 lhc_fill=t4.value();
    }


    bool uHTR::dram_test( int id, int chunk )  { 

       uint32_t ramPtr = u_back.getNode("DAQ_DRAM_PTR").read() ;
       uint32_t addr   = ramPtr + (id*chunk) ;
       bool pass = true ;

       for ( int k=0 ; k < 2 ; k++ ) {

           //writing into the block 
           std::vector<uint32_t> wV ;
           for ( int i=0; i< chunk ; i++) {
               if ( k == 0 ) wV.push_back( 0xAAAA ) ;
               if ( k == 1 ) wV.push_back( 0x5555 ) ;
           }
	   u_hw->getClient().writeBlock( addr, wV );
	   dispatch();

	   //reading the block 
	   uhal::ValVector<uint32_t> readBlock = u_hw->getClient().readBlock( addr ,chunk);
	   dispatch();

	   std::vector<uint32_t> rV ;
	   for (int j=0; j<chunk; j++) {
	       rV.push_back( readBlock[j] );
           } 
        
           usleep(100) ;
	   //comparing input and output 
	   for ( int i=0 ; i < chunk; i++) {
               if ( rV[i] != wV[i] ) {
                  printf(" DRAM ERROR @  0x%X \n", ramPtr + i*chunk ) ;
                  pass = false ;
               } 
           }
       }
       return pass ; 
    }

    void uHTR::check_ddr3_stat( int check )  { 

         uhal::ValWord<uint32_t> ct0  = u_back.getNode("DDR.PHY_INIT_DONE").read() ;
         uhal::ValWord<uint32_t> ct1  = u_back.getNode("DDR.STATE").read() ;
         uhal::ValWord<uint32_t> ct2  = u_back.getNode("DDR.CONSECUTIVE_WRITES").read() ;
         uhal::ValWord<uint32_t> ct3  = u_back.getNode("DDR.COUNT_ENABLE_ADDR_W").read() ;
         uhal::ValWord<uint32_t> ct4a = u_back.getNode("DDR.COUNT_ENABLE_DATA0_W").read() ;
         uhal::ValWord<uint32_t> ct4b = u_back.getNode("DDR.COUNT_ENABLE_DATA1_W").read() ;
         uhal::ValWord<uint32_t> ct4c = u_back.getNode("DDR.COUNT_ENABLE_DATA2_W").read() ;
         uhal::ValWord<uint32_t> ct4d = u_back.getNode("DDR.COUNT_ENABLE_DATA3_W").read() ;
         uhal::ValWord<uint32_t> ct5  = u_back.getNode("DDR.COUNT_ENABLE_ADDR_R").read() ;
         uhal::ValWord<uint32_t> ct6a = u_back.getNode("DDR.COUNT_ENABLE_DATA0_R").read() ;
         uhal::ValWord<uint32_t> ct6b = u_back.getNode("DDR.COUNT_ENABLE_DATA1_R").read() ;
         uhal::ValWord<uint32_t> ct6c = u_back.getNode("DDR.COUNT_ENABLE_DATA2_R").read() ;
         uhal::ValWord<uint32_t> ct6d = u_back.getNode("DDR.COUNT_ENABLE_DATA3_R").read() ;
         uhal::ValWord<uint32_t> ct7a = u_back.getNode("DDR.COUNT_READ_DATA0_W").read() ;
         uhal::ValWord<uint32_t> ct7b = u_back.getNode("DDR.COUNT_READ_DATA1_W").read() ;
         uhal::ValWord<uint32_t> ct7c = u_back.getNode("DDR.COUNT_READ_DATA2_W").read() ;
         uhal::ValWord<uint32_t> ct7d = u_back.getNode("DDR.COUNT_READ_DATA3_W").read() ;
         uhal::ValWord<uint32_t> ct8  = u_back.getNode("DDR.COUNT_READ_ADDR_W").read() ;
         uhal::ValWord<uint32_t> ct9  = u_back.getNode("DDR.COUNT_READ_ADDR_R").read() ;

         uhal::ValWord<uint32_t> app0  = u_back.getNode("DDR.COUNT_APP_RD_DATA_END").read() ;
         uhal::ValWord<uint32_t> app1a = u_back.getNode("DDR.COUNT_APP_RD_VALID0").read() ;
         uhal::ValWord<uint32_t> app1b = u_back.getNode("DDR.COUNT_APP_RD_VALID1").read() ;
         uhal::ValWord<uint32_t> app1c = u_back.getNode("DDR.COUNT_APP_RD_VALID2").read() ;
         uhal::ValWord<uint32_t> app1d = u_back.getNode("DDR.COUNT_APP_RD_VALID3").read() ;
         uhal::ValWord<uint32_t> app2  = u_back.getNode("DDR.COUNT_APP_WDF_WREN").read() ;
         uhal::ValWord<uint32_t> app3  = u_back.getNode("DDR.COUNT_APP_WDF_END").read() ;
         uhal::ValWord<uint32_t> app4  = u_back.getNode("DDR.COUNT_APP_EN").read() ;

         uhal::ValWord<uint32_t> stat0 = u_back.getNode("DDR.COUNT_STATE_WRITE0").read() ;
         uhal::ValWord<uint32_t> stat1 = u_back.getNode("DDR.COUNT_STATE_WRITE1").read() ;
         uhal::ValWord<uint32_t> stat2 = u_back.getNode("DDR.COUNT_STATE_WRITE2").read() ;
         uhal::ValWord<uint32_t> stat3 = u_back.getNode("DDR.COUNT_STATE_WRITE3").read() ;
         uhal::ValWord<uint32_t> stat4 = u_back.getNode("DDR.COUNT_STATE_READ0").read() ;
         uhal::ValWord<uint32_t> stat5 = u_back.getNode("DDR.COUNT_STATE_READ1").read() ;
         uhal::ValWord<uint32_t> stat6 = u_back.getNode("DDR.COUNT_STATE_READ2").read() ;

         uhal::ValWord<uint32_t> cntw0 = u_back.getNode("DDR.CNT_RDY_WRITE0").read() ;
         uhal::ValWord<uint32_t> cntw1 = u_back.getNode("DDR.CNT_RDY_WRITE1").read() ;
         uhal::ValWord<uint32_t> cntw2 = u_back.getNode("DDR.CNT_RDY_WRITE2").read() ;
         uhal::ValWord<uint32_t> cntw3 = u_back.getNode("DDR.CNT_RDY_WRITE3").read() ;
         uhal::ValWord<uint32_t> cntr0 = u_back.getNode("DDR.CNT_RDY_READ0").read() ;
         uhal::ValWord<uint32_t> cntr1 = u_back.getNode("DDR.CNT_RDY_READ1").read() ;
         uhal::ValWord<uint32_t> cntr2 = u_back.getNode("DDR.CNT_RDY_READ2").read() ;

         uhal::ValWord<uint32_t> rate0 = u_back.getNode("DDR.RATE_CLK_FIFO").read() ;
         uhal::ValWord<uint32_t> rate1 = u_back.getNode("DDR.RATE_CLK_200MHZ").read() ;
         uhal::ValWord<uint32_t> rate2 = u_back.getNode("DDR.RATE_CLK_400").read() ;
         uhal::ValWord<uint32_t> rate3 = u_back.getNode("DDR.RATE_CLK_UI").read() ;

         uhal::ValWord<uint32_t> rate4 = u_back.getNode("DDR.RESET_CLK_FIFO").read() ;
         uhal::ValWord<uint32_t> rate5 = u_back.getNode("DDR.RESET_CLK_400").read() ;
         uhal::ValWord<uint32_t> rate6 = u_back.getNode("DDR.RESET_CLK_UI").read() ;
	 dispatch() ;

         printf("\n") ;
         if ( check == 0 ||  check == - 1 ) {
            printf("------------------------------------------------------------------------------------------------------- \n");
            printf("| phy_ini | state | consec_w | en_addrW | en_dataW | en_addrR | en_dataR |  r_dataW | r_addrW | r_addrR |\n") ;
            printf("|    %3X  |  %3X  |    %3X   |    %3X   |   %X%X%X%X   |    %2X    |   %X%X%X%X   |   %X%X%X%X   |    %3X  |    %3X  |\n ", 
                    ct0.value(), ct1.value(), ct2.value(), ct3.value(), 
                    ct4d.value(), ct4c.value(), ct4b.value(), ct4a.value(), ct5.value(), 
                    ct6d.value(), ct6c.value(), ct6b.value(), ct6a.value(), 
                    ct7d.value(), ct7c.value(), ct7b.value(), ct7a.value(), ct8.value(), ct9.value() ) ;
         }

         if ( check == 1 ||  check == - 1 ) {
            printf("------------------------------------------------------- \n");
            printf("| rd_data_end | rd_valid | wdf_wren | wdf_end | app_en | \n");  
            printf("|    %3X      |   %x%x%x%x   |    %3X   |    %3X  |   %3X  | \n",
                   app0.value(), app1d.value(), app1c.value(), app1b.value(), app1a.value(), 
                   app2.value(), app3.value(), app4.value()  ) ;

         }
         if ( check == 2 ||  check == - 1 ) {
            printf("------------------------------------------------------------------------\n");
            printf("| w0 | w1 | w2 | w3 | r0 | r1 | r2 | cw0| cw1| cw2| cw3| cr0| cr1| cr2|\n") ; 
            printf("| %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x | %02x |\n",
                  stat0.value() , stat1.value(), stat2.value(), stat3.value(), stat4.value(), stat5.value(), stat6.value(),
                  cntw0.value() , cntw1.value(), cntw2.value(), cntw3.value(), cntr0.value(), cntr1.value(), cntr2.value() ) ;
         }
         if ( check == 3 ||  check == - 1 ) {
            printf("------------------------------------------------------------------------------------ \n");
            printf("| clk_fifo |   200MHz  |  400MHZ  | clk_sync | reset_fifo | reset_400 | reset_ui |\n") ;
            printf("| %08d |  %08d | %08d | %08d |     %3d    |    %3d    |    %3d   |\n",  
                    rate0.value(), rate1.value(), rate2.value(), rate3.value(), rate4.value(), rate5.value(), rate6.value()) ;
         }
         printf("\n") ;
    }

    void uHTR::ddr3_reset()  { 
         u_back.getNode("DDR.RESET_DDR3").write( 1 ) ;
         u_back.getNode("DDR.USE_IPBUS").write( 1 ) ;
	 dispatch() ;
    }

    std::vector<uint32_t> uHTR::ddr3_read( std::vector<uint32_t> base_addrV )  { 
         
         // Check whether read command can be issued 
         bool is_full_r = false ;
         do {
               uhal::ValWord<uint32_t> full_r = u_back.getNode("DDR.FULL_READ").read() ;
	       dispatch() ;
               is_full_r = ( full_r.value() == 1 ) ? true : false ;
         } while ( is_full_r ) ;

         // ***** check *****
         //printf(" (R1) Is Full Read Done and Issue a read command \n") ;
         //check_ddr3_stat() ;         

         // Issue the read command
         for ( size_t i=0; i< base_addrV.size(); i++ ) {

             u_back.getNode("DDR.ADDR_READ").write( base_addrV[i] ) ;
	     dispatch() ;

	     uhal::ValWord<uint32_t> addr_ = u_back.getNode("DDR.ADDR_READ").read() ; 
	     dispatch() ;
	     //printf(" *** The (%d) ddr3 base read addr = %08X \n", (int)(i+1), addr_.value() ) ;

	     // ***** check *****
	     //printf(" (R2) Issue Address Read \n") ;
	     //check_ddr3_stat() ;         

	     u_back.getNode("DDR.ENABLE_ADDR_READ").write( 1 ) ;
	     dispatch() ;

	     // ***** check *****
	     //printf(" (R3) Enable Address Read \n") ;
	     //check_ddr3_stat() ; 

	     u_back.getNode("DDR.ENABLE_ADDR_READ").write( 0 ) ;
	     dispatch() ;

         }

         // Check whether read command is complete
         bool is_empty_r = false ;
         do {
               usleep(10) ;
               uhal::ValWord<uint32_t> empty_r = u_back.getNode("DDR.EMPTY_READ").read() ;
	       dispatch() ;
               is_empty_r = ( empty_r.value() == 1 ) ? true : false ;
               //printf("    empty read ? %d \n", empty_r.value() );
         } while ( is_empty_r ) ;
         
         // Read out the values
	 std::vector<uint32_t> buffer ;
         for ( size_t i=0 ; i< base_addrV.size(); i++ ) {

             uhal::ValWord<uint32_t> theWord0 = u_back.getNode("DDR.DATA0_READ").read() ;
             uhal::ValWord<uint32_t> theWord1 = u_back.getNode("DDR.DATA1_READ").read() ;
             uhal::ValWord<uint32_t> theWord2 = u_back.getNode("DDR.DATA2_READ").read() ;
             uhal::ValWord<uint32_t> theWord3 = u_back.getNode("DDR.DATA3_READ").read() ;
	     dispatch() ;

	     buffer.push_back( theWord0.value() );
	     buffer.push_back( theWord1.value() );
	     buffer.push_back( theWord2.value() );
	     buffer.push_back( theWord3.value() );

	     u_back.getNode("DDR.ENABLE_DATA0_READ").write( 1 ) ;
	     u_back.getNode("DDR.ENABLE_DATA1_READ").write( 1 ) ;
	     u_back.getNode("DDR.ENABLE_DATA2_READ").write( 1 ) ;
	     u_back.getNode("DDR.ENABLE_DATA3_READ").write( 1 ) ;
	     dispatch() ;

	     u_back.getNode("DDR.ENABLE_DATA0_READ").write( 0 ) ;
	     u_back.getNode("DDR.ENABLE_DATA1_READ").write( 0 ) ;
	     u_back.getNode("DDR.ENABLE_DATA2_READ").write( 0 ) ;
	     u_back.getNode("DDR.ENABLE_DATA3_READ").write( 0 ) ;
	     dispatch() ;

	     //printf(" Reading Addr[%08X] = (%X %X %X %X )\n", base_addrV[i], 
             //                              theWord3.value(), theWord2.value(), theWord1.value(), theWord0.value() ) ;

         }

         return buffer ;
    }
    
    void uHTR::ddr3_write( uint32_t base_addr, std::vector<uint32_t>& buffer )  { 
         
         //u_back.getNode("DDR.RESET_DDR3").write( 1 ) ;
	 //dispatch() ;

         // Wait until "FULL_WRITE == 0" 
         bool is_full_w = false ;
         do {
               usleep(1) ;
               uhal::ValWord<uint32_t> full_w = u_back.getNode("DDR.FULL_WRITE").read() ;
	       dispatch() ;
               is_full_w = ( full_w.value() == 1 ) ? true : false ;

         } while ( is_full_w ) ;


         // Make sure the writing buffer size is multiple of 4
         if ( buffer.size() % 4 > 0 ) {
            int n0 = 4 - (buffer.size()%4) ;
            printf("Buffer size from %d ", (int)buffer.size()  ) ;
            for (int i =0; i< n0 ; i++) buffer.push_back(0) ;
            printf(" -> %d \n", (int)buffer.size() ) ;
         }

         // Writing process 
         char RStr[32] ;
         uint32_t w_addr = base_addr ;
         for ( size_t i=0 ; i< buffer.size() ; i++ ) {
          
             int k = i%4 ;
             sprintf( RStr, "DDR.ENABLE_DATA%d_WRITE", k ) ;   

             u_back.getNode("DDR.DATA_WRITE").write( buffer[i] ) ;
	     dispatch() ;
	     u_back.getNode( RStr ).write( 1 ) ;
	     dispatch() ;
	     u_back.getNode( RStr ).write( 0 ) ;
	     dispatch() ;
              
             if ( k == 3 || i == buffer.size() -1 ) {
                u_back.getNode("DDR.ADDR_WRITE").write( w_addr ) ;
	        dispatch() ;
		u_back.getNode("DDR.ENABLE_ADDR_WRITE").write( 1 ) ;
		dispatch() ;
		u_back.getNode("DDR.ENABLE_ADDR_WRITE").write( 0 ) ;
		dispatch() ;
                w_addr += 0x8 ;
             }
         }

         // ***** check *****
         //check_ddr3_stat() ;         
    }

  }
}
