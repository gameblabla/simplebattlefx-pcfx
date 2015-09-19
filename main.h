/*
 SIMPLE BATTLE FX
 
 The code is messy but oh well.

Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

# This work is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

void init_game();
void init_game_survival();
void gameplay();

void Title_screen();
void Menu_screen();
void Dif_screen();
void Choose_screen();
void Victory_screen();

void show_stats();
void clear_cursor();
void clear_cursor_P2();
void clear_hud();
void show_hud();

void Wait(void);

void wait_input();
void clear_battle_text();

void define_ennemy_strategy();

void A_input();
void UP_input();
void DOWN_input();

void Init_sound();

void play_snd(unsigned char i);

void Reset_sound_effects(void);

void Sound_effects();


