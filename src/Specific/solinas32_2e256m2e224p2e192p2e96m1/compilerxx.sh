#!/bin/sh
set -eu

g++ -march=native -mtune=native -std=gnu++11 -O3 -flto -fomit-frame-pointer -fwrapv -Wno-attributes -Da_minus_two_over_four_array='{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x39}' -Dmodulus_limbs='12' -Da24_val='12345' -Dbitwidth='32' -Dq_mpz='(1_mpz<<256) - (1_mpz<<224) + (1_mpz<<192) + (1_mpz<<96) - 1 ' -Dlimb_weight_gaps_array='{22,21,21,22,21,21,22,21,21,22,21,21}' -Da24_hex='0x3039' -Dmodulus_bytes_val='32' -Dmodulus_array='{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}' "$@"
