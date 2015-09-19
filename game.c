/*
 SIMPLE BATTLE FX
 
 The code is messy but oh well.

Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

# This work is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

#include <eris/types.h>
#include <eris/std.h>
#include <eris/v810.h>
#include <eris/king.h>
#include <eris/tetsu.h>
#include <eris/romfont.h>
#include <eris/pad.h>

#include "functions.h"
#include "input.h"

struct controllers JOY;
struct controllers JOY2;

void print(char* string, int x , int y , int size)
{	
	u32 str[256];

	chartou32(string, str);
	printstr(str, x, y+20, size);
}

void Controls()
{
	u32 padtype, paddata;
	u32 padtype2, paddata2;
	
		eris_pad_init(1);
	
		padtype = eris_pad_type(0);
		paddata = eris_pad_read(0);
		
		padtype2 = eris_pad_type(1);
		paddata2 = eris_pad_read(1);
		
		if(paddata & (1 << 0)) JOY.A = 1;
		else JOY.A = 0;

		if(paddata & (1 << 1)) JOY.B = 1;
		else JOY.B = 0;

		if(paddata & (1 << 2)) JOY.C = 1;
		else JOY.C = 0;
		
		if(paddata & (1 << 3)) JOY.D = 1;
		else JOY.D = 0;
		
		if(paddata & (1 << 4)) JOY.E = 1;
		else JOY.E = 0;
		
		if(paddata & (1 << 5)) JOY.F = 1;
		else JOY.F = 0;
		
		
		if(paddata & (1 << 6)) JOY.SELECT = 1;
		else JOY.SELECT = 0;
		
		if(paddata & (1 << 7)) JOY.START = 1;
		else JOY.START = 0;
		
		
		if(paddata & (1 << 8)) JOY.UP = 1;
		else JOY.UP = 0;
		
		if(paddata & (1 << 9)) JOY.RIGHT = 1;
		else JOY.RIGHT = 0;
		
		if(paddata & (1 << 10)) JOY.DOWN = 1;
		else JOY.DOWN = 0;

		
		if(paddata & (1 << 11)) JOY.LEFT = 1;
		else JOY.LEFT = 0;
		
		if(padtype2 & (1 << 0)) JOY2.A = 1;
		else JOY2.A = 0;

		if(padtype2 & (1 << 1)) JOY2.B = 1;
		else JOY2.B = 0;

		if(padtype2 & (1 << 2)) JOY2.C = 1;
		else JOY2.C = 0;
		
		if(padtype2 & (1 << 3)) JOY2.D = 1;
		else JOY2.D = 0;
		
		if(padtype2 & (1 << 4)) JOY2.E = 1;
		else JOY2.E = 0;
		
		if(padtype2 & (1 << 5)) JOY2.F = 1;
		else JOY2.F = 0;
		
		
		if(padtype2 & (1 << 6)) JOY2.SELECT = 1;
		else JOY2.SELECT = 0;
		
		if(padtype2 & (1 << 7)) JOY2.START = 1;
		else JOY2.START = 0;
		
		
		if(padtype2 & (1 << 8)) JOY2.UP = 1;
		else JOY2.UP = 0;
		
		if(padtype2 & (1 << 9)) JOY2.RIGHT = 1;
		else JOY2.RIGHT = 0;
		
		if(padtype2 & (1 << 10)) JOY2.DOWN = 1;
		else JOY2.DOWN = 0;

		if(padtype2 & (1 << 11)) JOY2.LEFT = 1;
		else JOY2.LEFT = 0;
		
		
}



static unsigned long int next = 1;

// This LRNG obviously suck
// Need a better replacement

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}


char* itoa(int val)
{
	
	static char buf[32] = {0};
	int base = 10;
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

