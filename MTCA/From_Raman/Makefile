##
#
# This is the hcal/UHTR Package Makefile
#
##

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include ../config.hcal

CACTUS_HOME=/opt/cactus

#
# Packages to be built
#
Project=hcal
Package=hcalUHTR
LongPackage=hcalUHTR
ShortPackage=uhtr
PackageName=uhtr

Sources=version.cc \
	uHTR.cc \
	uHTR_lumi.cc \
	uHTR_daq.cc \
	uHTR_trigger.cc \
	uHTRsensors.cc \
	uHTRLumi.cc \
	uHTRLumiWeb.cc \
	uHTRManager.cc \
	uHTRMonitoring.cc \
	uHTRMonitoringLumi.cc \
	uHTRManagerWeb.cc \
        uHTRtool_debug.cc \
        uHTRtool_bhm.cc \
        uHTRtool_thread.cc \
	LinkFormatTools.cc \
	CrateConnections.cc \
	uHTRSource.cc \
	uHTRSpyCapture.cc

Executables=uHTRtool.cc

IncludeDirs = \
	$(BUILD_HOME)/hcal/hcalBase/include \
	$(BUILD_HOME)/hcal/hcalAux/include \
	$(BUILD_HOME)/hcal/hcalTrig/include \
	$(BUILD_HOME)/$(Project)/$(Package)/include \
	$(BUILD_HOME)/$(Project)/$(Package)/amc13/include \
	$(BUILD_HOME)/include \
	$(CACTUS_HOME)/include \
	$(CACTUS_HOME)/include/boost \
	$(XDAQ_ROOT)/include $(XDAQ_ROOT)/include/hal \
	$(HCAL_XDAQ_ROOT)/include \
        $(ROOTSYS)/include

LibraryDirs = 	$(XDAQ_ROOT)/lib \
	$(CACTUS_HOME)/lib \
	$(HCAL_XDAQ_ROOT)/lib

DependentLibraryDirs = \
	$(BUILD_HOME)/hcal/hcalBase/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(BUILD_HOME)/hcal/hcalAux/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(BUILD_HOME)/hcal/hcalUpgrade/amc13/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(CACTUS_HOME)/lib \
	$(XDAQ_ROOT)/lib $(HCAL_XDAQ_ROOT)/lib ${ROOTSYS}/lib


UserSourcePath = 

UserCFlags = -O 
UserCCFlags = -Wno-write-strings -D__USE_XOPEN_2K8
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags = -lconfig -llog4cplus -lxcept -lcactus_uhal_uhal -lcactus_uhal_log -lboost_system -L $(BUILD_HOME)/hcal/hcalUpgrade/ipbus/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) -L $(BUILD_HOME)/hcal/hcalUpgrade/amc13/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) -L $(BUILD_HOME)/hcal/hcalBase/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) -L$(CACTUS_HOME)/lib -L $(BUILD_HOME)/hcal/hcalAux/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)  ${EXTRA_BOOST_LIBS} -luuid -llogudpappender -llogxmlappender -lxgi -ltoolbox -lasyncresolv -lpeer -lreadline -lncurses -lhcalBase -lhcalAux `${ROOTSYS}/bin/root-config --glibs`

# These libraries can be platform specific and
# potentially need conditional processing
#
DependentLibraries = xdaq toolbox xoap xerces-c boost_system cactus_uhal_uhal hcalBase hcalAux 
DependentLibraries +=  Core Cint Hist Gpad Graf Postscript Matrix MathCore RIO 
#
# Compile the source files and create a shared library
#
DynamicLibrary= hcalUHTR
StaticLibrary=

LD_LIBRARY_PATH=${XDAQ_ROOT}/lib:${CACTUS_HOME}/lib:/home/daqowner/dist/lib:${ROOTSYS}/lib:${HCAL_XDAQ_ROOT}/lib

include $(XDAQ_ROOT)/config/Makefile.rules
include $(BUILD_HOME)/hcal/mfRPM_hcal.rules
include $(XDAQ_ROOT)/config/mfRPM.rules
