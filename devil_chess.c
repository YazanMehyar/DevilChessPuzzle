#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint;

// WARNING: Do not define 'lSIZE' to be greater than 4
#define lSIZE		4
#define SIZE		(1<<lSIZE)
#define PSIZE		(1<<SIZE)

uint COLOR[PSIZE];

static inline uint cnum(uint i, uint j, uint inc) {
	return i & ~(0x3<<j) | (0x3&(0x3&i>>j)+inc)<<j;
}

static inline uint getCset(uint i, uint j, uint k) {
	uint j_inc = j&0x1? j&=~0x1, 1 : -1;
	uint k_inc = k&0x1? k&=~0x1, 1 : -1;

	return cnum(cnum(i,j,j_inc),k,k_inc);
}

static inline uint getKnum(uint i) {
	uint m = 0xAAAAAAAA;

	return i&m | (i&m)>>1^i&m>>1;
}

void
print_bin(uint data) {
	for(char i = SIZE-1; i >= 0; i--)
		printf("%c", data&(1<<i)? '1':'0');
}

uint
colorKmap(uint i) {
	if( i == PSIZE ) return 1;

	uint colors = 0;

	for(uint j = 0; j < SIZE; j++)
		for(uint k = 0; k < SIZE; k++)
			colors |= COLOR[getCset(i, j, k)];

	colors = ~colors & (PSIZE-1);
	if(!colors) return 0;

	uint found = 0;
	for(uint j = 0; !found && j < SIZE; j++) {
		if(1<<j & colors) {
			COLOR[i] = 1<<j;
			found = colorKmap(i+1);
		}
	} if(!found) COLOR[i] = 0;
	return COLOR[i];
}

int
main(void) {

	colorKmap(0);

	for(uint i = 0; i < SIZE; i++) {
		for(uint j = 0; j < PSIZE; j++) {
			if(COLOR[j] == 1 << i) {
				printf("%02d: ", i); print_bin(getKnum(j)); putchar('\n');
			}
		}
	} return 0;
}
