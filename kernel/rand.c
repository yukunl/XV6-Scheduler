#include "rand.h"


struct xorshift32_state {
  unsigned int a;
};
//static unsigned int x = 1 ;
struct xorshift32_state state;
/* 
    Return a random integer between 0 and XV6_RAND_MAX inclusive. 
    NOTE: If xv6_rand is called before any calls to xv6_srand have been made, the same 
    sequence shall be generated as when xv6_srand is first called with a seed value of 1.
*/
int xv6_rand (void){

  unsigned int x = state.a;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
  state.a = (x % XV6_RAND_MAX);
	return (x % XV6_RAND_MAX);
  //  usigned int t = 
}




/* 
    The xv6_srand function uses the argument as a seed for a new sequence of pseudo-random numbers to be returned by subsequent calls to rand.
    If xv6_srand is then called with the same seed value, the sequence of pseudo-random numbers shall be repeated.
    If xv6_rand is called before any calls to xv6_srand have been made, the same sequence shall be generated as when xv6_srand is first called with a seed value of 1.  
*/
void xv6_srand (unsigned int seed){
  state.a = seed;
}