# Disclaimer:

This program is provided for educational purposes only. The use of this code for generating FlixBus seat vouchers may violate FlixBus' terms of service. The developer of this code is not responsible for any misuse or illegal activities conducted with this program.

***

This program allows you to generate FlixBus seat vouchers by providing your email address.

### Environment Variable:

Make sure to set the `VOUCHER_URL environment` variable before running the program. This variable should contain the URL to the FlixBus voucher redemption page. On linux, this can be done using `export VOUCHER_URL='URL'` in your terminal.

### Dependencies:

- libcurl: This program uses libcurl for making HTTP requests. Make sure to have libcurl installed on your system: `sudo apt-get install libcurl4-openssl-dev`

### Instructions:

1. Clone this repository or download the source code.
2. Execute the `build.sh` script to build the program. This script compiles the code using g++.
3. Once the build is successful, you can run the program by providing your email address as an argument.

### Usage:

```bash
./FlixbusSeatVoucher <email>
```

Replace `<email>` with your valid email address.

**Note:** Ensure that you use this program responsibly and adhere to all relevant terms of service and legal regulations.
