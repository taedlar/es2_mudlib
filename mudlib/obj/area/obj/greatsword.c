/*  greatsword.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <weapon.h>

inherit F_SWORD;

void create()
{
    set_name("大劍", "great sword", "sword" );
    set_weight(24000);
    setup_sword(3, 13, 100, 5);

    if( !clonep() ) {
        set("wield_as", "twohanded sword");
        set("unit", "把");
        set("value", 11000);
        set("rigidity", 25);
        set("long",
            "一把約五尺長的重劍﹐像這麼巨大的重劍多半是背在背後，只能由膂力\n"
            "過人的壯漢使用。\n");
        set("wield_msg", "$N將背後的劍囊一扯，「呼」地一聲抽出一把大劍。\n");
    }
    setup();
}

void varify_template(object owner)
{
    int i = owner->query_level();
    
    ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "劍");
}
