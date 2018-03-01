#!/usr/bin/env python

from optparse import OptionParser
import sys
from tpgTest import tpgTest

def options():
    parser = OptionParser(usage="usage: %prog --ip=192.168.31.44")
    parser.add_option("--ip", action="store", type="string", dest="ip", default="")
    opts, args = parser.parse_args()
    if not opts.ip:
        parser.print_help()
        sys.exit()
    return opts

ip = options().ip
triggerLinks = [0, 1]


# test that zero ADC everywhere gives zero TP everywhere
tpgTest(name="allZero",
        ip=ip,
        triggerLinks=triggerLinks,
        data={},
        expectedTps={},
        execute=False,
        )

# square wave in each channel
pattern = {}
for fiber in range(24):
    for fibCh in range(3):
        pattern[(fiber, fibCh)] = [0, 100]*2048

tpgTest(name="square_wave",
        ip=ip,
        triggerLinks=triggerLinks,
        data=pattern,
        selfTriggerThreshold=5,
        execute=True,
        )

# determine the offset between ADC bx and TP bx (repeat 3x)
pattern = []
for iBx in range(4096):
    if iBx % 100:
        pattern.append(0)
    else:
        pattern.append(iBx/100)

for i in range(3):
    tpgTest(name="orbitSearch_%d" % i,
            ip=ip,
            triggerLinks=triggerLinks,
            data={(2, 0): pattern},
            execute=False,
            )

# test which channels contribute to which TPs
for fiber in range(24):
    for fibCh in range(3):
        tpgTest(name="test_%d_%d" % (fiber, fibCh),
                ip=ip,
                triggerLinks=triggerLinks,
                data={(fiber, fibCh): [0]*100 + [10]},
                #data={(fiber, fibCh): ([0, 10]*72 + [0]*500)*5},
                oneLine=True,
                execute=False,

                # speed-ups
                setupClocks=False,
                verifyData=False,
                setLuts=False,
                )
