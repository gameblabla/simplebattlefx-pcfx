/*
 SIMPLE BATTLE FX
 
 The code is messy but oh well.

Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

# This work is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

/*
I've done this a while ago and some of the things here are bad...
For example, the input code. (why i didn't used a struct here instead is beyond me)
*/

#include <eris/types.h>
#include <eris/std.h>
#include <eris/v810.h>
#include <eris/king.h>
#include <eris/tetsu.h>
#include <eris/romfont.h>
#include <eris/pad.h>
#include <eris/low/soundbox.h>
#include <eris/7up.h>

#include "main.h"

#include "functions.h"
#include "game.h"
#include "input.h"

#define SMALL 0
#define BIG 1

struct controllers JOY;
struct controllers JOY2;

struct thing
{
	short hp_max;
	short mp_max;
	short hp;
	short mp;
	short attack;
	short defense;
	short temp_attack;
	short temp_defense;
	unsigned char state;
	short temp;
	short strategy;
	short keep_guarding;
	short critic_rand;
	short critic;
	short turn;
	short choice;
} player[2];

unsigned char game_mode, text_y, text_y_P2, time_controls, choice;

char A_state, DOWN_state, UP_state;
char A_state_P2, DOWN_state_P2, UP_state_P2;
char t_time[4], t_time_P2[4];

struct snd
{
	short time;
	short play;
} sounds[6];

short snd[6][20]=
{
	{508,509,510,508,512,-1},
	{800,810,800,790,820,-1},
	{5000,5000,5400,5400,100,200,800,800,1000,1000,-1},
	{100,100,100,90,80,-1}
};

unsigned char multi = 0;
unsigned char survival = 0;
unsigned char survival_level = 0;

///
//	
//	MAIN LOOP
//
///

int main(void)
{
	A_state = 0;
	DOWN_state = 0;
	UP_state = 0;
	game_mode = 0;
	time_controls = 0;
	
	crapload();
	clear_bg0();
	set_to_text_mode();
	Init_controls();
	
	Init_sound();
		
	while(1)
	{
		Controls();
		A_input();
		UP_input();
		DOWN_input();
		
		Sound_effects();
	
		switch(game_mode)
		{
			case 0:
				Title_screen();
			break;
			case 1:
				Choose_screen();
			break;
			case 2:
				gameplay();
			break;
			case 3:
				Victory_screen();
			break;
			case 4:
				Dif_screen();
			break;
		}
	
	}

	return 0;
}



///
//	
//	SCREENS
//
///

void Title_screen()
{
	static char time = 0;
	
	eris_tetsu_set_palette(1, 0x0000);
	
	print("SIMPLE BATTLE FX", 8 , 1 , BIG);
	print("(C) 2015 Gameblabla", 6 , 160 , SMALL);
	
	print("MANY THANKS TO trap15", 6 , 175 , SMALL);
	print("FOR LIBERIS", 11 , 184 , SMALL);
	
	time++;
			
	if (time > 30)
	{
		print("PRESS START BUTTON", 7 , 130 , SMALL);
	}
	else
	{
		print("                  ", 7 , 130 , SMALL);
	}
			
	if (time > 60) time = 0;
			
	if (JOY.START)
	{
		clear_text();
		init_game();
		game_mode = 4;
	}
}

void Choose_screen()
{
	show_stats();
	show_hud();
	
		if (multi == 0) player[1].turn = 1;
		
		if (player[0].turn == 0)
		{
		
			if (DOWN_state == 1 && player[0].choice < 2)
			{
				player[0].choice++;
				if (player[0].choice > 2) player[0].choice = 2;
				clear_cursor();
				DOWN_state = 2;
				
				play_snd(0);
			}
			else if (UP_state == 1 && player[0].choice > 0)
			{
				player[0].choice--;
				if (player[0].choice < 1) player[0].choice = 0;
				clear_cursor();
				UP_state = 2;
				
				play_snd(0);
			}
						
			if (A_state == 1)
			{
				if (player[0].choice != 2)
				{
					play_snd(2);
					clear_cursor();
					player[0].turn = 1;
				}
				else if (player[0].choice == 2 && player[0].mp > 19)
				{
					play_snd(2);
					clear_cursor();
					player[0].turn = 1;
				}
			}

		}
		
		if (player[1].turn == 0)
		{
		
			if (JOY2.DOWN == 1 && player[1].choice < 2)
			{
				player[1].choice++;
				if (player[1].choice > 2) player[1].choice = 2;
				clear_cursor();
				DOWN_state_P2 = 2;
				
				play_snd(0);
			}
			else if (JOY2.UP == 1 && player[1].choice > 0)
			{
				player[1].choice--;
				if (player[1].choice < 1) player[1].choice = 0;
				clear_cursor();
				UP_state_P2 = 2;
				
				play_snd(0);
			}
						
			if (A_state_P2 == 1)
			{
				if (player[1].choice != 2)
				{
					play_snd(2);
					clear_cursor_P2();
					player[1].turn = 1;
				}
				else if (player[1].choice == 2 && player[1].mp > 19)
				{
					play_snd(2);
					clear_cursor_P2();
					player[1].turn = 1;
				}
			}

		}
		
		
		if (player[0].turn == 1 && player[1].turn == 1)
		{
			clear_hud();
			game_mode = 2;
		}
		
}

void Victory_screen()
{
	if (survival == 1) 
	{
		print("Survival level :", 1 , 60 , SMALL);
		if (survival_level > 0) print(itoa(survival_level), 18 , 60 , SMALL);
		else print("0", 18 , 60 , SMALL);
	}

	if (player[0].hp > 0 && player[1].hp < 1)
	{
		print("You are victorious !", 1 , 20 , SMALL);
		if (survival == 1) print("Prepare for the next battle !", 1 ,40 , SMALL);
	}
	else if (player[1].hp > 0 && player[0].hp < 1)
	{
		print("You are DEAD !", 1 , 20 , SMALL);
		
		if (survival == 1) 
		{
			print("COMMENT:", 1 , 120 , SMALL);
			if (survival_level == 0)
			{
				print("YOU SUCK", 1 , 130 , SMALL);
			}
			else if (survival_level < 2)
			{
				print("YOU CAN DO BETTER", 1 , 130 , SMALL);
			}
			else if (survival_level < 5)
			{
				print("NOT BAD", 1 , 130 , SMALL);
			}
			else if (survival_level < 8)
			{
				print("KEEP GOING", 1 , 130 , SMALL);
			}
			else if (survival_level < 10)
			{
				print("YOU'RE DOING GREAT", 1 , 130 , SMALL);
			}
			else if (survival_level == 42)
			{
				print("YOU ARE LIFE", 1 , 130 , SMALL);
			}
			else
			{
				print("OUTSTANDING", 1 , 130 , SMALL);
			}
		}

	}
	else if (player[1].hp < 1 && player[0].hp < 1)
	{
		print("Archaeologists from the future", 1 , 20 , SMALL);
		print("found two dead bodies...", 1 , 40 , SMALL);
	}

	print("PRESS A TO CONTINUE", 1 , 180 , SMALL);
			
	if (A_state == 1)
	{
		clear_text();
		A_state = 0;
		DOWN_state = 0;
		UP_state = 0;
		
		if (survival == 1 && player[0].hp > 0) 
		{
			init_game_survival();
			game_mode = 1;
		}
		else
		{
			game_mode = 0;
		}
		
		
	}
			
}

void Menu_screen()
{

	print("=>", 8 , text_y , BIG);
	
	print("SOLO MODE", 12 , 60 , BIG);
	print("2 PLAYERS VS", 12 , 80 , BIG);
	
	if (UP_state == 1)
	{
		print("  ", 8 , text_y , BIG);	
		text_y = 60;
	}
	else if (DOWN_state == 1)
	{
		print("  ", 8 , text_y , BIG);	
		text_y = 80;
	}
	
	if (A_state == 1)
	{
		clear_text();
		init_game();
		game_mode = 1;
		
		if (text_y == 60)
		{
			multi = 0;
		}
		else if (text_y == 80)
		{
			multi = 1;
		}
		
		A_state = 2;
	}
}



void Dif_screen()
{

	print("=>", 8 , text_y , BIG);
	
	print("DUEL", 12 , 60 , BIG);
	print("SURVIVAL", 12 , 80 , BIG);
	
	if (UP_state == 1)
	{
		if (text_y != 60) play_snd(0);
		print("  ", 8 , text_y , BIG);	
		text_y = 60;
	}
	else if (DOWN_state == 1)
	{
		if (text_y != 80) play_snd(0);
		print("  ", 8 , text_y , BIG);	
		text_y = 80;
	}
	
	if (A_state == 1)
	{
		clear_text();
		init_game();
	
		if (text_y == 60)
		{
			game_mode = 1;
			survival = 0;
		}
		else if (text_y == 80)
		{
			game_mode = 1;
			survival = 1;
		}
		
		A_state = 2;
	}
}





///
//	
//	LOGIC
//
///


void init_game()
{
	unsigned char i;
	
	for (i=0;i<2;i++)
	{
		player[i].hp = rand_a_b(90,120);
		player[i].mp = rand_a_b(20,41);
		player[i].attack = rand_a_b(12,20);
		player[i].defense = rand_a_b(5,10);
		player[i].temp_attack = 0;
		player[i].temp_defense = 0;
		player[i].hp_max = player[i].hp;
		player[i].mp_max = player[i].mp;
		player[i].turn = 0;
	}
	
	text_y = 60;
	text_y_P2 = 120;
	time_controls = 0;
	
	eris_tetsu_set_palette(1, 0xE088);
	
}

void init_game_survival()
{

	survival_level = survival_level + 1;

	if (survival_level > 3)
	{
		player[1].hp = rand_a_b(150,220);
		player[1].mp = rand_a_b(50,100);
		player[1].attack = rand_a_b(15,25);
		player[1].defense = rand_a_b(8,10);
	}
	else if (survival_level > 1)
	{
		player[1].hp = rand_a_b(120,200);
		player[1].mp = rand_a_b(30,80);
		player[1].attack = rand_a_b(15,30);
		player[1].defense = rand_a_b(10,15);
	}
	else
	{
		player[1].hp = rand_a_b(90,120);
		player[1].mp = rand_a_b(20,41);
		player[1].attack = rand_a_b(12,20);
		player[1].defense = rand_a_b(5,10);
	}


	player[0].temp_attack = 0;
	player[0].temp_defense = 0;
	player[0].turn = 0;
	player[1].temp_attack = 0;
	player[1].temp_defense = 0;
	player[1].turn = 0;
	player[1].hp_max = player[1].hp;
	player[1].mp_max = player[1].mp;
	
	
	text_y = 60;
	text_y_P2 = 120;
	time_controls = 0;
	
	eris_tetsu_set_palette(1, 0xE088);
	
}

void gameplay()
{
	int i;
	
	Reset_sound_effects();

	for(i=0;i<2;i++)
	{
		player[i].temp_attack = 0;
		player[i].temp_defense = 0;
		player[i].critic = 1;
		player[i].turn = 0;
	}
	
	if (multi == 0) define_ennemy_strategy();

	if (player[0].choice == 0)
	{
	// If user choosed to attack
		print("You attack the ENEMY", 1 , 60 , SMALL);
		
		Wait();
		
		player[0].critic_rand = rand_a_b(0,10);
		if (player[0].critic_rand == 5) player[0].critic = 2;
		else player[0].critic = 1;
		
		player[0].keep_guarding = 0;
		
		player[0].attack = rand_a_b(10,20);
		player[0].temp = ( (player[0].attack*player[0].critic) -  (player[1].defense + player[1].temp_defense) ) + rand_a_b(-3,3);
		
		if (player[0].temp < 1) 
		{
			player[0].temp = 1;
		}
		
		player[1].hp = player[1].hp - player[0].temp;
		print("ENEMY lost", 1 , 90 , SMALL);
		print(itoa(player[0].temp), 13 , 90 , SMALL);
		print("HP", 17 , 90 , SMALL);
		
		if (player[0].critic == 2) print("CRITICAL HIT !", 1 , 80 , SMALL);
		
	}
	else if (player[0].choice == 1)
	{
	// If user choosed to Guard
		print("You are GUARDING", 1 , 60 , SMALL);
		
		Wait();
		
		player[0].keep_guarding = player[0].keep_guarding + 1;
		
		player[0].temp_attack = rand_a_b(5,8);
		player[0].temp_defense = rand_a_b(5,10);
		
		player[0].mp = player[0].mp + player[0].temp_attack;
		player[0].hp = player[0].hp + player[0].temp_defense;
		
		if (player[0].mp > player[0].mp_max) player[0].mp = player[0].mp_max;
		if (player[0].hp > player[0].hp_max) player[0].hp = player[0].hp_max;
		
		print("DEFENSE +", 1 , 80 , SMALL);
		print(itoa(player[0].temp_defense), 10 , 80 , SMALL);
		print("MP,HP   +", 1 , 90 , SMALL);
		print(itoa(player[0].temp_attack), 10 , 90 , SMALL);
	}
	else if (player[0].choice == 2)
	{
	// If user choosed to attack
		print("You use SuperPunch", 1 , 60 , SMALL);
		
		Wait();
		
		player[0].critic_rand = rand_a_b(0,10);
		if (player[0].critic_rand == 5) player[0].critic = 2;
		else player[0].critic = 1;
		
		player[0].keep_guarding = 0;
		
		player[0].mp -= 20;
		
		if (player[0].mp < 0) player[0].mp = 0;
		
		player[0].temp_attack = rand_a_b(15,40);
		player[0].attack = rand_a_b(10,20) + player[0].temp_attack;
		
		player[0].temp = ( (player[0].attack*player[0].critic) -  (player[1].defense) ) + rand_a_b(-3,3);
		
		if (player[0].temp < 1) 
		{
			player[0].temp = 1;
		}
		
		player[1].hp = player[1].hp - player[0].temp;
		
		print("ENEMY lost", 1 , 90 , SMALL);
		print(itoa(player[0].temp), 13 , 90 , SMALL);
		print("HP", 17 , 90 , SMALL);
		
		if (player[0].critic == 2) print("CRITICAL HIT !", 1 , 80 , SMALL);
	}
	
	Wait();
	
	if (player[1].hp > 0)
	{
	
		if (player[1].choice == 0)
		{
			print("ENEMY attacks YOU", 1 , 120 , SMALL);
			
			Wait();
			
			player[1].critic_rand = rand_a_b(0,10);
			if (player[1].critic_rand == 5) player[0].critic = 2;
			else player[1].critic = 1;
			
			player[1].attack = rand_a_b(10,20);
			
			player[1].temp = ( (player[1].attack*player[1].critic) -  (player[0].defense + player[0].temp_defense)) + rand_a_b(-3,3);
			
				if (player[1].temp < 1) 
				{
					player[1].temp = 1;
				}
				
			player[0].hp = player[0].hp - player[1].temp;
			
			print("YOU lost", 1 , 150 , SMALL);
			print(itoa(player[1].temp), 10 , 150 , SMALL);
			print("HP", 14 , 150 , SMALL);
			
			if (player[1].critic == 2) print("CRITICAL HIT !", 1 , 140 , SMALL);
		}
		else if (player[1].choice == 1)
		{
			print("ENEMY is guarding", 1 , 120 , SMALL);
			Wait();
			
			player[1].temp_attack = rand_a_b(5,8);
			player[1].temp_defense = rand_a_b(5,12);
			
			player[1].mp = player[1].mp + player[1].temp_attack;
			player[1].hp = player[1].hp + player[1].temp_defense;
			
			if (player[1].mp > player[1].mp_max) player[1].mp = player[1].mp_max;
			if (player[1].hp > player[1].hp_max) player[1].hp = player[1].hp_max;
			
			print("DEFENSE +", 1 , 140 , SMALL);
			print(itoa(player[1].temp_defense), 10 , 140 , SMALL);
			print("MP,HP   +", 1 , 150 , SMALL);
			print(itoa(player[1].temp_attack), 10 , 150 , SMALL);
		}
		else if (player[1].choice == 2)
		{
			print("ENEMY use Burning Piss", 1 , 120 , SMALL);
			
			Wait();
			
			player[1].critic_rand = rand_a_b(0,10);
			if (player[1].critic_rand == 5) player[0].critic = 2;
			else player[1].critic = 1;
			
			player[1].mp = player[1].mp - 10;
			if (player[1].mp < 0) player[1].mp = 0;
			
			player[1].temp_attack = rand_a_b(20,40);
			player[1].attack = rand_a_b(10,20) + player[0].temp_attack;
			
			player[1].temp = ( (player[1].attack*player[1].critic) -  (player[0].defense) ) + rand_a_b(-3,3);
			
			if (player[1].temp < 1) 
			{
				player[1].temp = 1;
			}
			
			player[0].hp = player[0].hp - player[1].temp;
			
			print("YOU lost", 1 , 150 , SMALL);
			print(itoa(player[1].temp), 10 , 150 , SMALL);
			print("HP", 14 , 150 , SMALL);
			
			if (player[1].critic == 2) print("CRITICAL HIT !", 1 , 140 , SMALL);
			
		}
		
	}
	else
	{
		print("ENEMY is DEAD", 1 , 160 , SMALL);
	}
	
	if (player[1].hp < 1)
	{
		print("ENEMY is DEAD", 1 , 160 , SMALL);
	}
		
	print("      ", 6, 18 , SMALL);
	print("      ", 6, 27 , SMALL);
	print("      ", 25, 18 , SMALL);
	print("      ", 25, 27 , SMALL);
	show_stats();
		
	wait_input();

	choice = 0;
	time_controls = 0;
	
	if (player[1].hp < 1 || player[0].hp < 1) 
	{
		play_snd(2);
		clear_text();
		A_state = 2;
		game_mode = 3;
	}
	else if (player[1].hp > 0)
	{	
		clear_battle_text();
		A_state = 2;
		game_mode = 1;
	}
}


void define_ennemy_strategy()
{
	
	// Conditions to choose what the ENNEMY is doing
	if (player[0].keep_guarding > 2  && player[1].mp > 10)
	{
		player[1].choice = 2;
	}
	else if (player[1].hp < (player[1].hp_max/2))
	{
		player[1].choice = 1;
	}
	else if (player[0].hp < (player[0].hp_max/2) && player[1].mp > 10)
	{
		player[1].choice = 2;
	}
	else
	{
		player[1].choice = 0;
	}
	
	
}


///
//	
//	DISPLAY
//
///


void show_stats()
{
	print("YOU", 1 , 1 , BIG);
	print("HP :", 1 , 18 , SMALL);
	print(itoa(player[0].hp) , 6, 18 , SMALL);
			
	print("MP :", 1 , 27 , SMALL);
	print(itoa(player[0].mp) , 6, 27 , SMALL);
			
	print("ENEMY", 20 , 1 , BIG);
	print("HP :", 20 , 18 , SMALL);
	print(itoa(player[1].hp) , 25, 18 , SMALL);
			
	print("MP :", 20 , 27 , SMALL);
	print(itoa(player[1].mp) , 25, 27 , SMALL);
	
	print("--------------------------------", 0 , 40 , SMALL);
}

void clear_cursor()
{
	unsigned char i;
	
	for (i=0;i<4;i++) 
	{
		print("  ", 2 , 60+(i*10)-1 , BIG);
	}
}

void clear_cursor_P2()
{
	unsigned char i;
	
	for (i=0;i<4;i++) 
	{
		print("  ", 2 , 120+(i*10)-1 , BIG);
	}
}

void clear_hud()
{
	print("      ", 5 , 60 , SMALL);
	print("      ", 5 , 70 , SMALL);
	print("                    ", 5 , 80 , SMALL);
}

void show_hud()
{
	print("=>", 2 , (text_y+(10*player[0].choice))-1 , SMALL);
	print("Attack", 5 , 60 , SMALL);
	print("Guard", 5 , 70 , SMALL);
	print("Super Punch - MP 20", 5 , 80 , SMALL);
	
	if (multi == 1)
	{
		print("=>", 2 , (text_y_P2+(10*player[1].choice))-1 , SMALL);
		print("Attack", 5 , 120 , SMALL);
		print("Guard", 5 , 130 , SMALL);
		print("Burning Piss - MP 20", 5 , 140 , SMALL);
	}
}

void clear_battle_text()
{
	int i;
	for (i=60;i<180;i++) 
	{
		print("                                   ",0,i,0);
	}
}

void Wait(void)
{
	int i;
	for(i = 0; i < 0x80000; i++) {
		asm("mov r0, r0");
	}
}


///
//	
//	CONTROLS
//
///

void A_input()
{	

	if (A_state_P2 == 0)
	{
		if (JOY2.A) 
		{
			t_time_P2[0] = 0;
			A_state_P2 = 1;
		}
	}
	else if (A_state_P2 == 1)
	{
		if (t_time_P2[0] > 0)
		{
			t_time_P2[0] = 0;
			A_state_P2 = 2;
		}
		t_time_P2[0]++;
	}
	else if (A_state_P2 == 2)
	{
		if (JOY2.A == 0) 
		{
			t_time_P2[0] = 0;
			A_state_P2 = 3;
		}
	}
	else if (A_state_P2 == 3)
	{
		t_time_P2[0]++;
		if (t_time_P2[0] > 5)
		{
			A_state_P2 = 0;
		}
	}

	if (A_state == 0)
	{
		if (JOY.A) 
		{
			t_time[0] = 0;
			A_state = 1;
		}
	}
	else if (A_state == 1)
	{
		if (t_time[0] > 0)
		{
			t_time[0] = 0;
			A_state = 2;
		}
		t_time[0]++;
	}
	else if (A_state == 2)
	{
		if (JOY.A == 0) 
		{
			t_time[0] = 0;
			A_state = 3;
		}
	}
	else if (A_state == 3)
	{
		t_time[0]++;
		if (t_time[0] > 5)
		{
			A_state = 0;
		}
	}
	
}

void UP_input()
{	
	switch(UP_state_P2)
	{
		case 0:
		if (JOY2.UP) 
		{
			t_time_P2[1] = 0;
			UP_state_P2 = 1;
		}
		break;
		case 1:
		if (t_time_P2[1] > 0)
		{
			t_time_P2[1] = 0;
			UP_state_P2 = 2;
		}
		t_time_P2[1]++;
		break;
		case 2:
		if (JOY2.UP == 0) 
		{
			t_time_P2[1] = 0;
			UP_state_P2 = 3;
		}
		break;
		case 3:
		t_time_P2[1]++;
		if (t_time_P2[1] > 5)
		{
			UP_state_P2 = 0;
		}
		break;
	}

	switch(UP_state)
	{
		case 0:
		if (JOY.UP) 
		{
			t_time[1] = 0;
			UP_state = 1;
		}
		break;
		case 1:
		if (t_time[1] > 0)
		{
			t_time[1] = 0;
			UP_state = 2;
		}
		t_time[1]++;
		break;
		case 2:
		if (JOY.UP == 0) 
		{
			t_time[1] = 0;
			UP_state = 3;
		}
		break;
		case 3:
		t_time[1]++;
		if (t_time[1] > 5)
		{
			UP_state = 0;
		}
		break;
	}

}

void DOWN_input()
{	
	

	if (DOWN_state_P2 == 0)
	{
		if (JOY2.DOWN) 
		{
			t_time_P2[2] = 0;
			DOWN_state_P2 = 1;
		}
	}
	else if (DOWN_state_P2 == 1)
	{
		if (t_time_P2[2] > 0)
		{
			t_time_P2[2] = 0;
			DOWN_state_P2 = 2;
		}
		t_time_P2[2]++;
	}
	else if (DOWN_state_P2 == 2)
	{
		if (JOY2.DOWN == 0) 
		{
			t_time_P2[2] = 0;
			DOWN_state_P2 = 3;
		}
	}
	else if (DOWN_state_P2 == 3)
	{
		t_time_P2[2]++;
		if (t_time_P2[2] > 5)
		{
			DOWN_state_P2 = 0;
		}
	}
	
	if (DOWN_state == 0)
	{
		if (JOY.DOWN) 
		{
			t_time[2] = 0;
			DOWN_state = 1;
		}
	}
	else if (DOWN_state == 1)
	{
		if (t_time[2] > 0)
		{
			t_time[2] = 0;
			DOWN_state = 2;
		}
		t_time[2]++;
	}
	else if (DOWN_state == 2)
	{
		if (JOY.DOWN == 0) 
		{
			t_time[2] = 0;
			DOWN_state = 3;
		}
	}
	else if (DOWN_state == 3)
	{
		t_time[2]++;
		if (t_time[2] > 5)
		{
			DOWN_state = 0;
		}
	}
	
}

void wait_input() 
{
	unsigned char done;
	done = 1;
	
	Init_controls();
	A_state = 2;
	
	while(done)
	{
		Controls();
		A_input();
		
		if (A_state==1)
		{
			done = 0;
		}
	}
	
	Init_controls();
}

///
//	
//	Play sound effects
//
///

void Init_sound()
{
	eris_low_psg_set_main_volume(0, 0);
	eris_low_psg_set_channel(0);
}

void play_snd(unsigned char i)
{
	sounds[i].play = 1;
	sounds[i].time = 0;
}

void Sound_effects()
{
	unsigned char i;
	
	for (i=0;i<6;i++)
	{
		if (sounds[i].play == 1)
		{
			eris_low_psg_set_main_volume(15, 15);
			eris_low_psg_set_channel(0);
			eris_low_psg_set_volume(31, 1, 0);
			eris_low_psg_set_freq(snd[i][sounds[i].time]);
			eris_low_psg_set_balance(0xF, 0xF);
			eris_low_psg_waveform_data(0x1F);
		
			if (snd[i][sounds[i].time] == -1)
			{
				eris_low_psg_set_main_volume(0, 0);
				eris_low_psg_set_channel(0);
				eris_low_psg_set_volume(0, 1, 0);
				eris_low_psg_set_freq(0);
				sounds[i].time = 0;
				sounds[i].play = 0;
			}
			
			sounds[i].time++;
		}
	}
}



void Reset_sound_effects(void)
{
	unsigned char i;

	eris_low_psg_set_main_volume(0, 0);
	eris_low_psg_set_channel(0);
	eris_low_psg_set_volume(0, 1, 0);
	eris_low_psg_set_freq(0);
	
	for(i=0;i<6;i++)
	{
		sounds[i].time = 0;
		sounds[i].play = 0;
	}
}

