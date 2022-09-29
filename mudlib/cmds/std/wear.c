// wear.c

#include <ansi.h>

inherit F_CLEAN_UP;

int do_wear(object me, object ob);

int main(object me, string arg)
{
	object ob, *inv;
	int i, count;


	if( me->is_busy() )	return notify_fail("你現在沒有空﹗\n");

	if( !arg ) return notify_fail("你要穿戴什麼﹖\n");

	if(arg=="all") {
		inv = all_inventory(me);
		for(count=0, i=0; i<sizeof(inv); i++) {
			if( inv[i]->query("equipped") ) continue;
			if(do_wear(me, inv[i]) ) count ++;
		}
		write("Ok.\n");
		return 1;
	}

	if( !objectp(ob = present(arg, me)) )
		return notify_fail("你身上沒有這樣東西。\n");

	if( ob->query("equipped") )
		return notify_fail("你已經裝備著了。\n");

	return do_wear(me, ob);
}

int do_wear(object me, object ob)
{
	string str;

	if( ob->query("female_only")
	&&	(string)me->query("gender") != "女性" )
		return notify_fail("這是女人的衣衫﹐你一個大男人也想穿﹐羞也不羞﹖\n");

	if( ob->wear() ) {
		if( !stringp(str = ob->query("wear_msg")) ) {
			// Get equipped info "armor/xxxxx"
			switch( ob->query("equipped")[6..<1] ) {
				case "cloth":
				case "armor":
				case "leg_eq":
				case "feet_eq":
					str = YEL "$N穿上一" + ob->query("unit") + "$n。\n" NOR;
					break;
				case "head_eq":
				case "neck_eq":
				case "finger_eq":
				case "hand_eq":
					str = YEL "$N戴上一" + ob->query("unit") + "$n。\n" NOR;
					break;
				case "waist_eq":
					str = YEL "$N將一" + ob->query("unit") + "$n繫在腰間。\n" NOR;
					break;
				default:
					str = YEL "$N裝備$n。\n" NOR;
			}
		}
		message_vision(str, me, ob);
		return 1;
	} else
		return 0;
}

int help(object me)
{
	write(@HELP
指令格式﹕wear <裝備名稱>
 
這個指令讓你裝備某件防具。
 
HELP
    );
    return 1;
}
