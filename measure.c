/*
 * Benchmarking utilities extracted from SUPERCOP by Andres Erbsen
 * based on measure-anything.c version 20120328 and measure.c 
 * by D. J. Bernstein
 * Public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

// cpucycles_amd64cpuinfo
long long cpucycles(void)
{
  unsigned long long result;
  asm volatile(".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax"
    : "=a" (result) ::  "%rdx");
  return result;
}

// SUPERCOP randombytes
static uint32_t seed[32] = { 3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5 } ;
static uint32_t in[12];
static uint32_t out[8];
static int outleft = 0;
static void surf(void)
{
  #define ROTATE(x,b) (((x) << (b)) | ((x) >> (32 - (b))))
  #define MUSH(i,b) x = t[i] += (((x ^ seed[i]) + sum) ^ ROTATE(x,b));
  uint32_t t[12]; uint32_t x; uint32_t sum = 0;
  int r; int i; int loop;

  for (i = 0;i < 12;++i) t[i] = in[i] ^ seed[12 + i];
  for (i = 0;i < 8;++i) out[i] = seed[24 + i];
  x = t[11];
  for (loop = 0;loop < 2;++loop) {
    for (r = 0;r < 16;++r) {
      sum += 0x9e3779b9;
      MUSH(0,5) MUSH(1,7) MUSH(2,9) MUSH(3,13)
      MUSH(4,5) MUSH(5,7) MUSH(6,9) MUSH(7,13)
      MUSH(8,5) MUSH(9,7) MUSH(10,9) MUSH(11,13)
    }
    for (i = 0;i < 8;++i) out[i] ^= t[i + 4];
  }
  #undef ROTATE
  #undef MUSH
}
void randombytes(unsigned char *x,unsigned long long xlen)
{
  while (xlen > 0) {
    if (!outleft) {
      if (!++in[0]) if (!++in[1]) if (!++in[2]) ++in[3];
      surf();
      outleft = 8;
    }
    *x = out[--outleft];
    ++x;
    --xlen;
  }
}

// SUPERCOP limits
void limits()
{
#ifdef RLIM_INFINITY
  struct rlimit r;
  r.rlim_cur = 0;
  r.rlim_max = 0;
#ifdef RLIMIT_NOFILE
  setrlimit(RLIMIT_NOFILE,&r);
#endif
#ifdef RLIMIT_NPROC
  setrlimit(RLIMIT_NPROC,&r);
#endif
#ifdef RLIMIT_CORE
  setrlimit(RLIMIT_CORE,&r);
#endif
#endif
}

void UUT(unsigned char*);

void measure(void)
{
  unsigned char *buf = aligned_alloc(64, 1024);
  static long long cycles[TIMINGS + 1];

  for (int i = 0;i <= TIMINGS;++i) {
	  cycles[i] = cpucycles();
	  UUT(buf);
  }
  for (int i = 0;i < TIMINGS;++i) cycles[i] = cycles[i + 1] - cycles[i];
  for (int i = 0;i < TIMINGS;++i) printf("%lld\n", cycles[i]);

    __asm__ __volatile__("" :: "m" (buf)); // do not optimize buf away
}

int main()
{
  limits();
  measure();
  return 0;
}