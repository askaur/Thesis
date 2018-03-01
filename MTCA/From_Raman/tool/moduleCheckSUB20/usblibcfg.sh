if [[ ! ${LD_LIBRARY_PATH} == *"/home/ridhi/hcalUHTR/tool/moduleCheckSUB20/libusb-1.0.9/lib"* ]]; then
    export LD_LIBRARY_PATH=/home/ridhi/hcalUHTR/tool/moduleCheckSUB20/libusb-1.0.9/lib:${LD_LIBRARY_PATH}
fi

if [[ ! ${LD_LIBRARY_PATH} == *"/home/ridhi/hcalUHTR/tool/moduleCheckSUB20/SUB-20-snap-110319/lib"* ]]; then
    export LD_LIBRARY_PATH=/home/ridhi/hcalUHTR/tool/moduleCheckSUB20/SUB-20-snap-110319/lib:${LD_LIBRARY_PATH}
fi
