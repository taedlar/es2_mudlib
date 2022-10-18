/*  slasher_sword.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <weapon.h>

inherit F_UNIQUE;
inherit F_SWORD;

void create()
{
    set_name( HIW "繞指柔劍" NOR, "slasher sword", "sword" );
    set_weight(2000);
    setup_sword(5, 7, 60, 10);

    if( !clonep() ) {
	set("replica_ob", STOCK_WEAPON("longsword"));
        set("wield_as", "sword");
        set("unit", "把");
        set("value", 26000);
        set("long",
            "一把又細又長的劍﹐拿在手上劍刃就不斷抖動﹐看來要使用這把劍得要\n"
            "相當敏捷的身手才不會誤傷自己。\n");
        set("rigidity", 70);
        set("apply_weapon/sword", ([
            "dex":		2,
            "attack":		20,
            "intimidate":	20,
            "parry":		-10,
        ]) );
    }
    setup();
}
