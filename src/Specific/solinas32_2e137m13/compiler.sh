#!/bin/sh
set -eu

gcc -march=native -mtune=native -std=gnu11 -O3 -flto -fomit-frame-pointer -fwrapv -Wno-attributes -Da_minus_two_over_four_array='{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x39}' -Dmodulus_limbs='8' -Da24_val='12345' -Dbitwidth='32' -Dq_mpz='(1_mpz<<137) - 13' -Dlimb_weight_gaps_array='{18,17,17,17,17,17,17,17}' -Da24_hex='0x3039' -Dmodulus_bytes_val='18' -Dmodulus_array='{0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3}' "$@"
