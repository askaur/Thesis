---------
| Usage |
---------
cd hcalUHTR/scripts
# edit runTpgTests.py to specify the IP address
./runTpgTests.py

# this will:
# - write various files to /tmp/$USER (including a log file for each test)
# - print non-zero TPs to stdout

---------------
| Data format |
---------------
{(fiber, fiber-channel): [adc1, adc2, adc3, ..., adc3564],
 (fiber, fiber-channel): [adc1, adc2, adc3, ..., adc3564],
 ...,
}

* TP format to be chosen


-------------------
| Time dependence |
-------------------
(independent of front f/w and HCAL DAQ release)
- Before and on May 9: clean baseline; FE BX 2001 --> TP BX 137
- On and after May 10: polluted baseline; FE BX 100 --> TP BX 5 or 6
- May 21 (using f/w 6.21): clean baseline; FE BX 100 --> TP BX 5 or 6
- May 22 (using f/w 6.21): clean baseline; FE BX 100 --> TP BX 116 or 117
- May 23 (using f/w 6.30): clean baseline; FE BX 100 --> TP BX 6 or 7

--------------------------------
| Front f/w version dependence |
--------------------------------
0.6.00: tpByteOffset = -1
0.6.10: tpByteOffset = -4
0.6.21: tpByteOffset = -3
(this value is now determined automatically)
