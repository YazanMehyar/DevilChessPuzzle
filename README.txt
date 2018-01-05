The Devil's Chessboard Puzzle:

You and a friend are in the company of the Devil.
Very shortly, He will lead you into a room in which there is a standard chessboard.
On each of the 64 squares is a coin with either heads or tails facing up,
in a way that the Devil has seen fit to arrange. Your friend must wait outside.
The Devil will then show you one of the coins and tell you that this is the Magic Coin.
Your friend will not see which coin the Devil has pointed to.
Moreover, the Magic Coin has no visible features by which it can be identified.
In a few moments, your friend will be led into the room, and he must identify the Magic Coin.
You will not be allowed to talk to him or give him any other information.
All he sees is the chessboard with the 64 coins arranged on it in some pattern---heads or tails.
However, before your friend enters, you may flip exactly one of the coins.
(Of course, your friend will not know which coin you have flipped.)
Work out a strategy, to be agreed with your friend before you enter the room,
which will allow him to identify the Magic Coin. 


A Basic Description of the Solution:

The solution is basically a hashing algorithm with certain properties.
The hash will reduce a 64 bit number representing the state of the board
into a 6 bit number indicating the position of a coin. The hash has the unique
property that all combinations of positions are accessed by any 64 bit pattern by the toggle
of a single bit (flipping the coin). The hash is basically an encoding scheme
that is the reverse of common memory decoding. Eg.

POSITION 011_010:
	011 -> row 3 of memory (board, counting from 0)
	010 -> column 2 of memory (board, counting from 0)

What makes up these position bits is the parity (in this program even parity)
of certain groups of coins on the board that correspond the decoded area pointed to by that bit.

Eg. 1st bit obtained by finding the parity of first 32 bits indicated by 'X':

	X X X X X X X X
	X X X X X X X X
	X X X X X X X X
	X X X X X X X X
	O O O O O O O O
	O O O O O O O O
	O O O O O O O O
	O O O O O O O O

    2nd bit obtained from a different arrangement of 32 bits:

	X X X X X X X X
	X X X X X X X X
	O O O O O O O O
	O O O O O O O O
	X X X X X X X X
	X X X X X X X X
	O O O O O O O O
	O O O O O O O O

This gives the hash its ability to point to any position by flipping
a certain coin regardless of the starting pattern.

The position of coin to be flipped in an 8x8 board is:

	63 - (hash(starting pattern) xor position of magic coin)

As to why it is so is left an exercise for the reader.
