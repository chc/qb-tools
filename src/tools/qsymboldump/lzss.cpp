/*----------------------------------------------------------------------------
LZSS.C -- A Data Compression Program
4/6/1989 Haruhiko Okumura
Use, distribute, and modify this program freely.
Please send me your improved versions.
	PC-VAN		SCIENCE
	NIFTY-Serve	PAF01022
	CompuServe	74050,1022

Some changes made June, 2003 by Chris Giese
	<geezer@execpc.com>, http://my.execpc.com/~geezer
- Changed F from 16 to 18, for compatability with Microsoft COMPRESS/EXPAND
  EXPAND.EXE is on the install disks for MS-DOS version 6 and Windows 3.1
  COMPRESS.EXE is in the Win 3.1 SDK, and comes with Borland C++ 3.1
- Changed compress/expand chars on command line from e/d to c/u
- Where possible and correct, changed ints to unsigned
- Made all functions static
- Changed formatting, indenting, global variable names, and function names
- Tried to simplify/clarify code in some areas
----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* size of ring buffer */
#define	N		4096
/* index for root of binary search trees */
#define NIL		N
#define	F		18
/* encode string into position and length
if match_length is greater than this: */
#define	THRESHOLD	2

/* these assume little-endian CPU like Intel x86
-- need byte-swap function for big endian CPU */

/* this assumes sizeof(long)==4 */
//typedef unsigned long	uint32_t;

/* text (input) size counter, code (output) size counter,
and counter for reporting progress every 1K bytes */
static unsigned long g_text_size, g_code_size;
/* ring buffer of size N, with extra F-1 bytes
to facilitate string comparison */
static unsigned char g_ring_buffer[N + F - 1];
/* position and length of longest match; set by insert_node() */
static unsigned g_match_pos, g_match_len;
/* left & right children & parent -- these constitute binary search tree */
static unsigned g_left_child[N + 1], g_right_child[N + 257], g_parent[N + 1];
/* input & output files */
//static FILE *g_infile, *g_outfile;
/*****************************************************************************
initialize trees
*****************************************************************************/
static void init_tree(void)
{
	unsigned i;

/* For i = 0 to N - 1, g_right_child[i] and g_left_child[i] will be the right and
   left children of node i.  These nodes need not be initialized.
   Also, g_parent[i] is the parent of node i.  These are initialized to
   NIL (= N), which stands for 'not used.'
   For i = 0 to 255, g_right_child[N + i + 1] is the root of the tree
   for strings that begin with character i.  These are initialized
   to NIL.  Note there are 256 trees. */
	for(i = N + 1; i <= N + 256; i++)
		g_right_child[i] = NIL;
	for(i = 0; i < N; i++)
		g_parent[i] = NIL;
}
/*****************************************************************************
Inserts string of length F, g_ring_buffer[r..r+F-1], into one of the
trees (g_ring_buffer[r]'th tree) and returns the longest-match position
and length via the global variables g_match_pos and g_match_len.
If g_match_len = F, then removes the old node in favor of the new
one, because the old one will be deleted sooner.
Note r plays double role, as tree node and position in buffer.
*****************************************************************************/
static void insert_node(int r)
{
	unsigned char *key;
	unsigned i, p;
	int cmp;

	cmp = 1;
	key = &g_ring_buffer[r];
	p = N + 1 + key[0];
	g_right_child[r] = g_left_child[r] = NIL;
	g_match_len = 0;
	while(1)
	{
		if(cmp >= 0)
		{
			if(g_right_child[p] != NIL)
				p = g_right_child[p];
			else
			{
				g_right_child[p] = r;
				g_parent[r] = p;
				return;
			}
		}
		else
		{
			if(g_left_child[p] != NIL)
				p = g_left_child[p];
			else
			{
				g_left_child[p] = r;
				g_parent[r] = p;
				return;
			}
		}
		for(i = 1; i < F; i++)
		{
			cmp = key[i] - g_ring_buffer[p + i];
			if(cmp != 0)
				break;
		}
		if(i > g_match_len)
		{
			g_match_pos = p;
			g_match_len = i;
			if(g_match_len >= F)
				break;
		}
	}
	g_parent[r] = g_parent[p];
	g_left_child[r] = g_left_child[p];
	g_right_child[r] = g_right_child[p];
	g_parent[g_left_child[p]] = r;
	g_parent[g_right_child[p]] = r;
	if(g_right_child[g_parent[p]] == p)
		g_right_child[g_parent[p]] = r;
	else
		g_left_child[g_parent[p]] = r;
	g_parent[p] = NIL;  /* remove p */
}
/*****************************************************************************
deletes node p from tree
*****************************************************************************/
static void delete_node(unsigned p)
{
	unsigned q;

	if(g_parent[p] == NIL)
		return; /* not in tree */
	if(g_right_child[p] == NIL)
		q = g_left_child[p];
	else if(g_left_child[p] == NIL)
		q = g_right_child[p];
	else
	{
		q = g_left_child[p];
		if(g_right_child[q] != NIL)
		{
			do q = g_right_child[q];
			while(g_right_child[q] != NIL);
			g_right_child[g_parent[q]] = g_left_child[q];
			g_parent[g_left_child[q]] = g_parent[q];
			g_left_child[q] = g_left_child[p];
			g_parent[g_left_child[p]] = q;
		}
		g_right_child[q] = g_right_child[p];
		g_parent[g_right_child[p]] = q;
	}
	g_parent[q] = g_parent[p];
	if(g_right_child[g_parent[p]] == p)
		g_right_child[g_parent[p]] = q;
	else
		g_left_child[g_parent[p]] = q;
	g_parent[p] = NIL;
}


#define xgetc(X)    ((g_infilen >= g_infilesz) ? (EOF) : (g_infile[g_infilen++]))
#define xputc(X,Y)  (g_outfile[g_outfilen++] = X)


/*****************************************************************************
*****************************************************************************/
int compress_lzss(unsigned char *g_infile, int g_infilesz, unsigned char *g_outfile)
{
    int g_infilen = 0, g_outfilen = 0;
	unsigned i, len, r, s, last_match_length, code_buf_ptr;
	unsigned char code_buf[17], mask;
	int c;

	init_tree();  /* initialize trees */
/* code_buf[1..16] saves eight units of code, and code_buf[0] works as
eight flags, "1" representing that the unit is an unencoded letter (1 byte),
"0" a position-and-length pair (2 bytes). Thus, eight units require at most
16 bytes of code. */
	code_buf[0] = 0;
	code_buf_ptr = mask = 1;
	s = 0;
	r = N - F;
/* Clear the buffer with any character that will appear often. */
	memset(g_ring_buffer + s, ' ', r - s);
/* Read F bytes into the last F bytes of the buffer */
	for(len = 0; len < F; len++)
	{
		c = xgetc(g_infile);
		if(c == EOF)
			break;
		g_ring_buffer[r + len] = c;
	}
	g_text_size = len;
	if(g_text_size == 0) /* text of size zero */
		return(0);
/* Insert the F strings, each of which begins with one or more 'space'
characters. Note the order in which these strings are inserted.
This way, degenerate trees will be less likely to occur. */
	for(i = 1; i <= F; i++)
		insert_node(r - i);
/* Finally, insert the whole string just read. The global variables
g_match_len and g_match_pos are set. */
	insert_node(r);
	do
	{
/* g_match_len may be spuriously long near the end of text. */
		if(g_match_len > len)
			g_match_len = len;
/* Not long enough match.  Send one byte. */
		if(g_match_len <= THRESHOLD)
		{
			g_match_len = 1;
			code_buf[0] |= mask;  /* 'send one byte' flag */
			code_buf[code_buf_ptr] = g_ring_buffer[r];  /* Send uncoded. */
			code_buf_ptr++;
/* Send position and length pair. Note g_match_len > THRESHOLD. */
		}
		else
		{
			code_buf[code_buf_ptr] = (unsigned char)g_match_pos;
			code_buf_ptr++;
			code_buf[code_buf_ptr] = (unsigned char)
				(((g_match_pos >> 4) & 0xF0) |
				(g_match_len - (THRESHOLD + 1)));
			code_buf_ptr++;
		}
/* Shift mask left one bit. */
		mask <<= 1;
		if(mask == 0)
		{
/* Send at most 8 units of code together */
			for(i = 0; i < code_buf_ptr; i++)
				xputc(code_buf[i], g_outfile);
			g_code_size += code_buf_ptr;
			code_buf[0] = 0;
			code_buf_ptr = mask = 1;
		}
		last_match_length = g_match_len;
		for(i = 0; i < last_match_length; i++)
		{
			c = xgetc(g_infile);
			if(c == EOF)
				break;
/* Delete old strings and read new bytes */
			delete_node(s);
			g_ring_buffer[s] = c;
/* If the position is near the end of buffer, extend the buffer
to make string comparison easier. */
			if(s < F - 1)
				g_ring_buffer[s + N] = c;
/* Since this is a ring buffer, increment the position modulo N. */
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
/* Register the string in g_ring_buffer[r..r+F-1] */
			insert_node(r);
		}
/* Reports progress each time the g_text_size exceeds multiples of 1024. */
		g_text_size += i;
		while(i++ < last_match_length)/* After the end of text, */
		{
			delete_node(s);		/* no need to read, but */
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
			len--;
			if(len)
				insert_node(r);/* buffer may not be empty. */
		}
	} while(len > 0); /* until length of string to be processed is zero */
/* Send remaining code. */
	if(code_buf_ptr > 1)
	{
		for(i = 0; i < code_buf_ptr; i++)
			xputc(code_buf[i], g_outfile);
		g_code_size += code_buf_ptr;
	}

    return(g_outfilen);
}
/*****************************************************************************
Just the reverse of compress()
*****************************************************************************/
int decompress_lzss(unsigned char *g_infile, int g_infilesz, unsigned char *g_outfile)
{
    int g_infilen = 0, g_outfilen = 0;
	unsigned r, flags;
	int c, i, j, k;

	memset(g_ring_buffer, ' ', N - F);
	r = N - F;
	for(flags = 0; ; flags >>= 1)
	{
/* Get a byte. For each bit of this byte:
1=copy one byte literally, from input to output
0=get two more bytes describing length and position of previously-seen
  data, and copy that data from the ring buffer to output */
		if((flags & 0x100) == 0)
		{
			c = xgetc(g_infile);
			if(c == EOF)
				break;
/* uses higher byte cleverly to count eight */
			flags = c | 0xFF00;
		}
/* 1=copy one byte literally, from input to output */
		if(flags & 1)
		{
			c = xgetc(g_infile);
			if(c == EOF)
				break;
			xputc(c, g_outfile);
			g_ring_buffer[r] = c;
			r = (r + 1) & (N - 1);
		}
/* 0=get two more bytes describing length and position of previously-
seen data, and copy that data from the ring buffer to output */
		else
		{
			i = xgetc(g_infile);
			if(i == EOF)
				break;
			j = xgetc(g_infile);
			if(j == EOF)
				break;
			i |= ((j & 0xF0) << 4);
			j = (j & 0x0F) + THRESHOLD;
			for(k = 0; k <= j; k++)
			{
				c = g_ring_buffer[(i + k) & (N - 1)];
				xputc(c, g_outfile);
				g_ring_buffer[r] = c;
				r = (r + 1) & (N - 1);
			}
		}
	}
    return(g_outfilen);
}
