/*  finance.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs int
pay_money(int amount, int check_only)
{
    int total;
    object gold, silver, coin;
    int gv=0, sv=0, cv=0;    // The 'value' of each money type we carry
    int gp=0, sp=0, cp=0;    // The 'amount' of each money type we'll pay

    if( amount < 1 ) return 1;

    gold = present("gold_money", this_object());
    silver = present("silver_money", this_object());
    coin = present("coin_money", this_object());

    total = 0;
    if( gold ) { gv = gold->value(); total += gv; }
    if( silver ) { sv = silver->value(); total += sv; }
    if( coin ) { cv = coin->value(); total += cv; }

    if( total < amount ) return 0;

    if( cv >= amount % 100 ) {
	cv -= amount % 100;
	cp = amount % 100;
	amount -= amount % 100;
    }
    if( amount % 100 ) return 2;

    if( (amount > 0) && (sv + cv >= amount % 10000) ) {
	sv -= amount % 10000;
	sp = (amount % 10000) / 100;
	if( sv < 0 ) { cv += sv; cp -= sv/1; sp += sv/100; sv = 0; }
	if( cv < 0 ) return 0;
	amount -= amount % 10000;
    }
    if( amount % 10000 ) return 2;

    if( (amount > 0) && (gv + sv + cv >= amount) ) {
	gv -= amount;
	gp = amount / 10000;
	if( gv < 0 ) { sv += gv; sp -= gv/100; gp += gv/10000; gv = 0; }
	if( sv < 0 ) { cv += sv; cp -= sv/1; sp += sv/100; sv = 0; }
	if( cv < 0 ) return 0;
	amount -= amount;
    }

    if( amount > 0 ) return 0;

    if( !check_only ) {
	if( coin ) coin->add_amount( - cp );
	if( silver ) silver->add_amount( - sp );
	if( gold ) gold->add_amount( - gp );
    }

    return 1;
}

// can_afford()
//
// This function checks if this_object() can afford a price, it returns:
// 0: cannot afford.
// 1: can afford with proper money(coins).
// 2: can afford, but don't have enough proper money(coins).

int can_afford(int amount) { return pay_money(amount, 1); }
