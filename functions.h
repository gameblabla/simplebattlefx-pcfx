/*
 SIMPLE BATTLE FX
 
 The code is messy but oh well.

Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

# This work is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

void crapload();
void clear_bg0();
void set_to_text_mode();
void printch(u32 sjis, u32 kram, int tall);
void printstr(u32* str, int x, int y, int tall);
void chartou32(char* str, u32* o);
void Init_controls();

void clear_text();