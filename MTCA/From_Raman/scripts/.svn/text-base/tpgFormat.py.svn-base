# https://cms-docdb.cern.ch/cgi-bin/PublicDocDB/ShowDocument?docid=3327
def blocks16(c0, c1, c2, q0, q1, q2):
    first = 0x3
    first |= (c2 & 0x3) << 3
    first |= (c1 & 0x3) << 5
    first |= (c0 & 0x3) << 7
    first |= (q2 & 0x7F) << 9
    second = (q0 & 0x7F) << 9
    second |= (q1 & 0x7F) << 1
    return first, second


# HCAL Upgrade TDR Table 6.6 (http://cdsweb.cern.ch/record/1481837)
def decodeTpByte(d={}, iByte=None, byte=None):
    if iByte == 0:
        d["K"] = byte

    if 1 <= iByte <= 11:
        d["TP%d" % (29+iByte)] = byte

    if 12 <= iByte <= 14:
        for j in range(4):
            d["FB%d" % (4*iByte+j-18)] = (byte >> (2*j)) & 0x3

    if iByte == 14:
        del d["FB41"]

    if iByte == 15:
        d["CRC"] = byte


def tpsFromFile(fileName="", useK=False):
    k = [188, 124]

    tps = []
    d = {}

    iByte0 = None
    f = open(fileName)
    for line in f:
        fields = line.split(" ")
        iWord16 = int(fields[-2])
        digits = fields[-1]
        # take final 4 digits, excluding trailing newline
        word16 = int(digits[-5:-1], base=16)

        # take fourth digit
        flag = int(digits[3], base=16) & 0x1

        byte1 = word16 & 0xff
        byte2 = word16 >> 8
        iByte = 2*iWord16

        # determine offset
        if iByte0 is None:
            if useK:
                if byte1 in k:
                    iByte0 = iByte
                if byte2 in k:
                    iByte0 = iByte + 1
            elif flag:
                iByte0 = iByte

        if iByte0 is not None:
            if (iByte-iByte0) and not (iByte-iByte0) % 16:
                tps.append(d)
                d = {}
                
            decodeTpByte(d, (iByte-iByte0)   % 16, byte1)
            decodeTpByte(d, (iByte-iByte0+1) % 16, byte2)

    f.close()
    return tps, iByte0
