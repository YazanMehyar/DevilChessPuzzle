/**
 * @Author: Yazan Mehyar <zen>
 * @Email:  stcyazanerror@gmail.com
 * @Filename: devil_chess.c
 * @Last modified time: 04-Jan-2018
 */


#include <stdio.h>
#include <ctype.h>

typedef unsigned int  uint;
typedef unsigned char byte_t;

#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"

#define SIZE	8u
#define ERROR(fmt, ...)	fprintf(stderr,"\n"RED"ERROR: "fmt NRM"\n", ##__VA_ARGS__)

static byte_t devil_board[SIZE*SIZE];
static const uint BOARD_SIZE = sizeof(devil_board)/sizeof(devil_board[0]);

/****************************************************************************************/

static void printUsage(char *p) {
	fprintf(stderr, "USAGE: %s <FILE-NAME>\n"
					"The file specified should contain a valid pattern\n"
					"PATTERN:       64<UNIT>\n"
					"UNIT:          <WHITE_SPACE>*(<HEAD> OR <TAIL>)<WHITE_SPACE>*\n"
					"HEAD:          (H OR h OR 0 OR o OR O)\n"
					"TAIL:          (T or t or 1 or x or X)\n"
					"WHITE_SPACE:   (TAB OR SPACE OR NEWLINE OR FORMFEED"
									" OR CARRIAGE RETURN OR VERTICAL TAB)\n"
					"NOTE: WHITE_SPACE is optional & you can have a string of WHITE_SPACE\n"
					,p);
}

static void printBoard(void) {
	for(uint i = 0; i < SIZE; i++) {
		for(uint j = 0; j < SIZE; j++) {
			printf("%c ", devil_board[i*SIZE+j]? 'X':'O');
		} putchar('\n');
	}
}

static uint parseFile(FILE *f) {
	char c;
	uint idx = 0;
	while (idx < BOARD_SIZE) {
		c = fgetc(f);
		if(isspace(c)) continue;
		switch (c) {
			case '0':
			case 'H':
			case 'h':
			case 'O':
			case 'o':	devil_board[idx] = 0;
			break;

			case '1':
			case 'T':
			case 't':
			case 'X':
			case 'x':	devil_board[idx] = 1;
			break;

			case EOF:
				ERROR("Incomplete pattern, last index: %u", idx);
				return 1;
			default:
				ERROR("Parsing error, illegal character: row %u, column %u", idx/SIZE, idx%SIZE);
				return 1;
		} idx++;
	} return 0;
}

// Figure out the position indicated by the pattern
static byte_t dhash(void) {
	uint pos, i, size;
	byte_t p;

	size = i = BOARD_SIZE;
	pos  = 0;
	byte_t b[BOARD_SIZE];
	while(i--) b[i] = devil_board[i];

	while(size>>=1) {
		for(i = p = 0; i < size; i++) {
			p ^= b[i];
			b[i] ^= b[i+size];
		} pos = p | pos<<1;
	} return pos;
}

/****************************************************************************************/

int main(uint argc, char **argv) {
	if(argc < 2) {
		printUsage(argv[0]);
		return 1;
	}

	FILE* f;
	if( (f = fopen(argv[1], "r")) == NULL ) {
		ERROR("Cannot read file: %s", argv[1]);
		return 1;
	} if( parseFile(f) ) {
		fclose(f);
		return 1;
	} fclose(f);

	uint dim, idx;

SCAN_ROW:
	fprintf(stderr,"Enter row of magic coin: ");
	if(scanf("%u", &dim) != 1 || dim > 7){
		ERROR("Value must be a digit from 0 to 7 inclusive\n");
		goto SCAN_ROW;
	} idx = dim*SIZE;

SCAN_COL:
	fprintf(stderr,"Enter column of magic coin: ");
	if(scanf("%u", &dim) != 1 || dim > 7){
		ERROR("Value must be a digit from 0 to 7 inclusive\n");
		goto SCAN_COL;
	} idx += dim;

	dim = dhash();
	fprintf(stderr, "Starting pattern pointed to row %u column %u\n",dim/SIZE,dim%SIZE);

	idx = (BOARD_SIZE-1) - (idx^dim);
	devil_board[idx] ^= 1;

	fprintf(stderr, "Flip coin at row %u column %u\n", idx/SIZE, idx%SIZE);
	printBoard();
	dim = dhash();
	fprintf(stderr, "Finishing pattern now pointing to row %u column %u\n",dim/SIZE,dim%SIZE);
	return 0;
}
