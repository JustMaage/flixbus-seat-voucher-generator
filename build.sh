if [ -f FlixbusSeatVoucher ]; then
    rm FlixbusSeatVoucher
fi

g++ -o FlixbusSeatVoucher FlixbusSeatVoucher.cpp -lcurl