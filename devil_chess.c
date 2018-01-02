#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

// WARNING: Do not define 'SIZE' to be any greater than 24
#define SIZE	4

struct node {
	bool	filled;
	uint	color;
	uint	links[SIZE];
};

struct node config[(1<<SIZE)];

void
print_bin(uint data) {
	for(char i = SIZE-1; i >= 0; i--)
		printf("%c", (data&(1<<i))? '1':'0');
	printf("\n");
}

void
fillConfig(uint index) {
	uint i;
	char j;
	for(i = 1, j = 0; j < SIZE; i <<= 1, j++) {
		config[index].links[j] = index^i;
		if(!config[index^i].filled) {
			config[index^i].filled = true;
			fillConfig(index^i);
		}
	}
}

void
colorConfig(uint index, uint depth) {
	if(config[index].color) return;
	uint this_color = 0;

	uint *links = &config[index].links[0];
	for(char i = 0; i < SIZE; i++) {
		this_color |= config[*links++].color;
	} this_color = ((1<<SIZE)-1) ^ this_color;

	config[index].color = this_color&-this_color;

	for(char i = 0; i < SIZE; i++) {
		colorConfig(config[index].links[i], depth+1);
	}
}

int
main(void) {

	config[0].filled = true;
	fillConfig(0);
	colorConfig(0, 0);

	for(char j = 0; j < SIZE; j++) {
		printf( "\n POS %2d\n--------\n", j);
		for(uint i = 0; i < (1<<SIZE); i++) {
			if(config[i].color == j) print_bin(i);
		}
	}

	return 0;
}
