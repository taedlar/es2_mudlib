/*  woodblade.c

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

inherit F_BLADE;

void create()
{
    set_name("木刀", ({ "wooden blade", "blade" }) );
    set_weight(2000);
    setup_blade(1, 14, 40, 0);

    if( !clonep() ) {
        set("wield_as", "blade");
        set("unit", "把");
        set("value", 400);
        set("rigidity", 10);
        set("long", "一把用硬木削成的單刀﹐被炭火燻得黑黑的。\n");
    }
    setup();
}
