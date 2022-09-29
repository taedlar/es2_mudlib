/*  corpse.c - the coprse object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ITEM;

static int decayed = 0;
static int decay_interval = 120;

void create()
{
    set_name("無名屍體", ({ "corpse" }) );
    set("long", "這是一具無名屍體。\n");
    set("unit", "具" );
    setup();
}

void
start_decay(int interval)
{
    if( interval < 1 ) interval = 0;

    decay_interval = interval;

    remove_call_out("decay");
    decayed = 0;
    call_out("decay", decay_interval * 2);
}

int is_corpse() { return decayed < 2; }
int is_character() { return decayed < 1; }

string short() { return name() + "(" + capitalize(query("id")) + ")"; }

void decay()
{
    switch(++decayed) {
        case 1:
            say( query("name") + "開始腐爛了﹐發出一股難聞的惡臭。\n" );
            switch(query("gender")) {
                case "male":
                    set_name("腐爛的男屍", ({ "corpse" }) );
                    break;
                case "female":
                    set_name("腐爛的女屍", ({ "corpse" }) );
                    break;
                default:
                    set_name("腐爛的屍體", ({ "corpse" }) );
                    break;
            }
            set("long", "這具屍體顯然已經躺在這裡有一段時間了﹐正散發著一股腐屍的味道。\n");
            call_out("decay", decay_interval);
            break;
        case 2:
            say( query("name") + "被風吹乾了﹐變成一具骸骨。\n" );
            set_name("枯\乾的骸骨", ({ "skeleton" }) );
            set("long", "這副骸骨已經躺在這裡很久了。\n");
            call_out("decay", decay_interval);
            break;
        case 3:
            say( "一陣風吹過﹐把" + query("name") + "化成骨灰吹散了。\n" );
            destruct(this_object());
            break;
    }
}

int receive_object(object oob, int from_inventory) { return 1; }
