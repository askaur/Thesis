import os
import tpgFormat

class tpgTest(object):
    def __init__(self,
                 name="",
                 data=None,
                 expectedTps=None,
                 selfTriggerThreshold=None,
                 execute=False,
                 ip="",
                 triggerLinks=[0],
                 orbitDelay=0,
                 oneLine=False,
                 verifyData=True,
                 setLuts=True,
                 setupClocks=True,
                 ):

        assert name
        assert ip
        assert type(triggerLinks) is list, type(triggerLinks)
        
        self.outputDir = "/tmp/%s" % os.environ["USER"]
        os.system("mkdir %s >& /dev/null" % self.outputDir)
        
        self.dataFileNameIn = "%s/%s.data.in" % (self.outputDir, name)
        self.dataFileNameOut = "%s/%s.data.out" % (self.outputDir, name)
        self.logFileName = "%s/%s.log" % (self.outputDir, name)

        self.data = qieData(data).fibers()
        self.expectedTps = tpData(expectedTps)

        for item in ["name", "ip",  "triggerLinks", "orbitDelay",
                     "oneLine", "verifyData", "setLuts", "setupClocks",
                      "selfTriggerThreshold"]:
            setattr(self, item, eval(item))
        
        if execute:
            f = open(self.logFileName, "w")
            f.close()
            self.execute()

    def tpFileNameOut(self, triggerLink=None):
        return "%s/%s.tps_link%d.out" % (self.outputDir, self.name, triggerLink)

    def sfpFileNameOut(self, triggerLink=None):
        return "%s/%s.sfp_link%d.out" % (self.outputDir, self.name, triggerLink)

    def run(self, cmd=""):
        #print cmd
        #os.system("date >> %s" % self.logFileName)
        os.system("%s >> %s" % (cmd, self.logFileName))

    def runHtr(self, cmds=[], script=""):
        assert script

        f = open("%s/%s" % (self.outputDir, script), "w")
        f.write("\n".join(cmds))
        f.close()

        self.run("uHTRtool.exe %s -s %s/%s" % (self.ip, self.outputDir, script))
        
    def execute(self):
        self.writeDataFile()

        if self.setupClocks:
            self.setupClocks16()
        self.loadDataIntoHtr()
        if self.verifyData:
            self.dumpDataFromHtr()
            self.compareData()

        if self.setLuts:
            self.setIdentityLuts()
        if self.selfTriggerThreshold is not None:
            self.setSelfTriggerThreshold()

        self.enablePattern()
        self.dumpTpFromHtr()
        self.dumpSfpFromHtr()
        self.ormCmd(cmd="stop")
        self.ormCmd(cmd="start")

        for triggerLink in self.triggerLinks:
            tps, iByte0 = tpgFormat.tpsFromFile(self.tpFileNameOut(triggerLink))
            if iByte0 is None:
                print "WARNING (test %s link %d): iByte0 flag not found." % (self.name, triggerLink)
                tps, iByte0 = tpgFormat.tpsFromFile(self.tpFileNameOut(triggerLink), useK=True)
            if iByte0 is None:
                print "ERROR   (test %s link %d): Neither iByte0 flag nor k-char was found in TP stream." % (self.name, triggerLink)
            else:
                self.printTps(triggerLink, tps, iByte0)

    def writeDataFile(self):
        out = ""
        fiberPrev = -1
        for fiber in range(24):
            if fiberPrev != fiber:
                out += "# Fiber %d\n" % fiber
                fiberPrev = fiber
            for bx, (q0, q1, q2) in enumerate(zip(self.data[(fiber, 0)],
                                                  self.data[(fiber, 1)],
                                                  self.data[(fiber, 2)])):
                c0 = c1 = c2 = bx % 4
                block0, block1 = tpgFormat.blocks16(c0, c1, c2, q0, q1, q2)
                out += "%05x\n" % block0
                out += "%05x\n" % block1

        f = open("%s" % self.dataFileNameIn, "w")
        f.write(out)
        f.close()

    def setupClocks16(self):
        cmds = ["0", "clock", "setup", "1",
                "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="setupClocks.uhtr")

    def enablePattern(self):
        cmds = ["0", "link", "fe_rams", "setup",
                "1", "1", "0",
                "quit", "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="enablePattern.uhtr")

    def setSelfTriggerThreshold(self):
        cmds = ["0", "trig", "self",
                "1", "-1", str(self.selfTriggerThreshold), "-1",
                "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="setSelfTriggerThreshold.uhtr")

    def setIdentityLuts(self):
        cmds = ["0", "trig", "luts",
                "3", "0", "-1",
                "3", "1", "-1",
                "3", "2", "-1",
                "3", "3", "-1",
                "-1", "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="setLuts.uhtr")

    def loadDataIntoHtr(self):
        cmds = ["0", "link", "fe_rams", "load", self.dataFileNameIn,
                "quit", "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="loadData.uhtr")
 
    def dumpDataFromHtr(self):
        cmds = ["0", "link", "fe_rams", "dump", self.dataFileNameOut,
                "quit", "quit", "exit", "exit", ""]
        self.runHtr(cmds=cmds, script="dumpData.uhtr")

    def compareData(self):
        cmd = " ".join(["diff",
                        self.dataFileNameIn,
                        self.dataFileNameOut,
                        ])
        self.run(cmd)

    def dumpTpFromHtr(self):
        for triggerLink in self.triggerLinks:
            cmds = ["0", "trig", "spy",
                    str(self.orbitDelay),
                    str(triggerLink),
                    self.tpFileNameOut(triggerLink),
                    "quit", "exit", "exit", ""
                    ]
            self.runHtr(cmds=cmds, script="dumpTp_link%d.uhtr" % triggerLink)

    def dumpSfpFromHtr(self):
        for iSfp in range(2):
            cmds = ["0", "trig", "sfp",
                    "tx", str(iSfp), "0", 
                    "quit", "quit", "exit", "exit", ""
                    ]
            self.runHtr(cmds=cmds, script="dumpSfp%d.uhtr" % iSfp)

    def ormCmd(self, cmd=""):
        assert cmd
        cmds = ["0", "trig", "orm",
                cmd, "quit",
                "quit", "exit", "exit", ""
                ]
        self.runHtr(cmds=cmds, script="orm%s.uhtr" % cmd)

    def printTps(self, triggerLink=None, tps=[], offset=None):
        out = ["%s: link=%d (iByte0=%d)" % (self.name, triggerLink, offset)]
        for iBx, tp in enumerate(tps):
            if tp["K"] not in [188, 124]:
                print "ERROR   (%s): iBx=%d, K=%d" % (out[0], iBx, tp["K"])
            for key, value in sorted(tp.iteritems()):
                if key in ["K", "CRC"] or not value:
                    continue
                out.append("bx=%d, %s=%d" % (iBx, key, value))

        if len(out) >= 2:
            if self.oneLine:
                print " ".join(out)
            else:
                for item in out:
                    print item
                print


class qieData(object):
    def __init__(self, fibers={}, nBx=4096):
        self.nFiber = 24
        self.nFibCh = 3
        self.nBx = nBx
        self._fibers = fibers
        self.check()
        self._fill()

    def fibers(self):
        return self._fibers

    def check(self):
        assert type(self._fibers) is dict, type(self._fibers)
        for key, value in self._fibers.iteritems():
            # check key
            assert len(key) == 2, key
            fiber, fibCh = key
            assert fiber in range(self.nFiber), fiber
            assert fibCh in range(self.nFibCh), fiCh

            # check value
            assert type(value) is list, type(value)
            assert len(value) <= self.nBx, len(value)

    def _fill(self):
        for fiber in range(self.nFiber):
            for fibCh in range(self.nFibCh):
                key = (fiber, fibCh)
                if key in self._fibers:
                    nFill = self.nBx - len(self._fibers[key])
                    self._fibers[key] += [0]*nFill
                else:
                    self._fibers[key] = [0]*self.nBx


class tpData(object):
    def __init__(self, tps=[]):
        pass
