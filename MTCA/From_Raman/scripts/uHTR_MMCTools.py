#!/usr/bin/python

import os, sys
from optparse import OptionParser # Command line parsing

class IPMITool:
	IPMITOOLARGS = '-H %s -P "" -T 0x82 -B 0 -b 7'
	TARGET = 0x82

	CMD_SetBootMode=0x06
	CMD_FPGAConfigNVHeaderWrite=0x35
	CMD_FPGAConfigNVHeaderRead=0x36
	CMD_GetNVAreaInfo=0x40
	CMD_RawNVWrite=0x41
	CMD_RawNVRead=0x42
	CMD_EEPromErase=0x44

	IPMIBASE = "ipmitool %s -t %i raw 0x32"
	
	def __init__(self, target, mchip):
		self.TARGET = target
		if(mchip == "-99"):
			print("Must specify MCH IP address with -m")
			sys.exit(0)
		if(self.TARGET < 0):
			print("Must specify target address with -t")
			sys.exit(0)
		self.IPMITOOLARGS = self.IPMITOOLARGS%mchip
		self.IPMIBASE = self.IPMIBASE%(self.IPMITOOLARGS, self.TARGET)

	def getNVAreaInfo(self, page = 0):
		f = os.popen("%s %s %i"%(self.IPMIBASE, self.CMD_GetNVAreaInfo, page))
		return f.readline()
	
	def fpgaConfigNVHeaderWrite(self, flags, extendedFlags, byteOffset0, byteOffset1, byteOffset2, byteOffset3):
		os.system("%s %s %i %i %i %i %i %i %i"%(self.IPMIBASE, self.CMD_FPGAConfigNVHeaderWrite, flags, extendedFlags, 
		          byteOffset0, byteOffset1, byteOffset2, byteOffset3, self.__xsum([flags, extendedFlags, 
		          byteOffset0, byteOffset1, byteOffset2, byteOffset3])))
		          
	def rawNVWrite(self, startByteLS, startByteMS, data):
		catData = ""
		for i in data:
			catData = "%s %s"%(catData, i)
		os.system("%s %s %i %i %i %s"%(self.IPMIBASE, self.CMD_RawNVWrite, startByteLS, startByteMS, len(data), catData))
	
	def eraseEEPROM(self):
		f = os.popen("%s %s 0xaa"%(self.IPMIBASE, self.CMD_EEPromErase))
		eraseCodes=f.readline().split()
		os.system("%s %s 0x55 0x%s 0x%s 0x%s 0x%s"%(self.IPMIBASE, self.CMD_EEPromErase, eraseCodes[0], eraseCodes[1], eraseCodes[2], eraseCodes[3]))
	
	def labelCrate(self, crate = 0x73, slot = 0x80):
		if(crate < 0):
			print("Must specify crate ID with -c")
			sys.exit(0)
		self.fpgaConfigNVHeaderWrite(0x0c, 0x00, 0xff, 0x10, 0xff, 0xff)
		nvaData = self.getNVAreaInfo().split()
		nvcaAdd = (int(nvaData[14],16) << 8) + int(nvaData[13],16) + 16
		nvcaAdd_LS = nvcaAdd & 0x00ff
		nvcaAdd_MS = (nvcaAdd & 0xff00) >> 8
		recxsum = self.__xsum((slot, crate))
		hdrxsum = self.__xsum((0x00, 0x00, 0x02, recxsum, 0x01))
		self.rawNVWrite(nvcaAdd_LS, nvcaAdd_MS, (0x00, 0x00, 0x02, recxsum, 0x01, hdrxsum, slot, crate))
	
	def setBootMode(self, mode):
		if(mode < 0):
			print("Must specify boot mode with -b")
			sys.exit(0)
		elif(not (mode == 0) and not (mode == 1)):
			print("-b only allows options 0 and 1")
			sys.exit(0)
		os.system("%s %s %i"%(self.IPMIBASE, self.CMD_SetBootMode, mode))
	
	def __xsum(self, chars):
		sum = 0
		for i in chars:
			sum = sum + i
		return (1+~sum)&0xff
		

##### START OF CODE
""" Allows command line options to be parsed. Called first in order to let functions use them.  """

usage = "usage: %prog [Options] command (labelCrate, eraseEeprom, setBootMode)"
version = "%prog Version 0.1.3\n"
parser = OptionParser(usage=usage,version=version)
parser.add_option("-t", "--target", action="store", type="string", dest="target", default="-99", help="set target uHTR i2c address")
parser.add_option("-c", "--crate", action="store", type="string", dest="crateLabel", default="-99", help="set crate id (for use with labelCrate)")
parser.add_option("-m", "--mchip", action="store", type="string", dest="mchip", default="-99", help="set the MCH IP address")
parser.add_option("-b", "--bootMode", action="store", type="int", dest="bootMode", default=-99, help="set the boot mode (0 for Trigger mode, 1 for maintenance mode)")

(options, args) = parser.parse_args()

if(len(args) < 1):
	print "Must specify a valid command.  Use '-h' for a list of avaliable commands."
	sys.exit(0)
elif(len(args) > 1):
	print "May only specify one valid command.  Use '-h' for a list of avaliable commands."
	sys.exit(0)

ipmit = IPMITool(int(options.target, 0), options.mchip)

if(args[0] == "labelCrate"):
	ipmit.labelCrate(int(options.crateLabel, 0))
elif(args[0] == "eraseEeprom"):
	ipmit.eraseEEPROM()
elif(args[0] == "setBootMode"):
	ipmit.setBootMode(options.bootMode)
else:
	print "Invalid command.  Use '-h' for a list of avaliable commands."





