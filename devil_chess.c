#include <stdio.h>

typedef unsigned long long int uint;
typedef unsigned char byte_t;

// WARNING: Do not define 'lSIZE' to be greater than 4
#define lSIZE		3
#define SIZE		(1u<<lSIZE)
#define PSIZE		(1ull<<SIZE)

byte_t COLOR[PSIZE];

/****************************************************************************************/

static inline uint cnum(uint i, uint j, uint inc) {
	return i & ~(0x3<<j) | (0x3&(0x3&i>>j)+inc)<<j;
}

static inline uint getCset(uint i, uint j, uint k) {
	uint j_inc = j&0x1? j&=~0x1, 1 : -1;
	uint k_inc = k&0x1? k&=~0x1, 1 : -1;
	return cnum(cnum(i,j,j_inc),k,k_inc);
}

static inline uint knum(uint i) {
	uint m = 0xAAAAAAAAAAAAAAAAull;
	return i&m | (i&m)>>1^i&m>>1;
}

static inline byte_t bpos(uint i) {
	byte_t m = 0;
	i &= -i;
	while(i>>=1) m++;
	return m;
}

static inline uint eparity(uint d) {
	byte_t m = SIZE;
	while(m>>=1) d ^= d>>m;
	return ~d & 1;
}

static inline byte_t dhash(uint d) {
	d = knum(d);
	d ^= eparity(d);
	return d >> 1;
}

/****************************************************************************************/

int main(void) {
	for(uint i = 0; i < PSIZE; i++) {
		uint colors = 0;
		for(uint j = 0; j < SIZE; j++)
			for(uint k = 0; k < SIZE; k++)
				if(i > getCset(i,j,k)) colors |= 1 << COLOR[getCset(i,j,k)];

		COLOR[i] = bpos(colors+1);
	}

	for(uint j = 0; j < SIZE; j++){
		printf("%02X: ",j);
		for(uint i = 0; i < PSIZE; i+=2) {
				if(COLOR[i]==j) printf("%02X ", dhash(i));
		} putchar('\n');
	} return 0;
}
