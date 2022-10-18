#include <armor.h>
#include <ansi.h>
void do_clean(object me);


void create()
{
	set_name(GRN"七彩石"NOR, ({ "stone" }));
	set_weight(100000000);
	set("no_get", 1);
        if( !clonep() ) {
                set("unit", "個");
		set("value", 1);
                set("long",
"一顆七彩絢爛的大石, 可以消去你身上錯亂的記錄
若是你在讀取留言板有問題, 可以試著摸摸這塊大
石(touch stone)。\n");
        }
	setup();
}

void init()
{
	add_action("do_touch", "touch");
}

int do_touch(string arg)
{
	if( arg != "stone" ) return notify_fail("你想摸什麼?\n");

	if( this_player()->query_temp("stone/clean") ) {
		tell_object(this_player(),
		"系統清除記錄中, 請稍待。\n\n");
		call_out( (: do_clean, this_player() :), 1);
		this_player()->delete_temp("stone/clean");
		return 1; 
	}

	tell_object(this_player(),
	"一股奇妙的聲音在你腦中響起:
讀留言發生困難嗎??你確定要清除你所有的記錄話, 請再摸一次。\n");
	this_player()->set_temp("stone/clean", 1);
	return 1;
}

void do_clean(object me)
{
	object link_ob;
        seteuid(getuid());
	link_ob = me->link();
	if( link_ob->query("board_last_read/announce") ) {
		link_ob->set("board_last_read/announce", 000000001);}
	if( link_ob->query("board_last_read/board_tree") ) {
		link_ob->set("board_last_read/board_tree", 000000001);}
	if( link_ob->query("board_last_read/bridge_b") ) {
		link_ob->set("board_last_read/bridge_b", 000000001);}
	if( link_ob->query("board_last_read/bug") ) {
		link_ob->set("board_last_read/bug", 000000001);}
	if( link_ob->query("board_last_read/chin_palace_b") ) {
		link_ob->set("board_last_read/chin_palace_b", 000000001);}
	if( link_ob->query("board_last_read/choyin_b") ) {
		link_ob->set("board_last_read/choyin_b", 000000001);}
	if( link_ob->query("board_last_read/common") ) {
		link_ob->set("board_last_read/common", 000000001);}
	if( link_ob->query("board_last_read/fighter") ) {
		link_ob->set("board_last_read/fighter", 000000001);}
	if( link_ob->query("board_last_read/kangpin_b") ) {
		link_ob->set("board_last_read/kangpin_b", 000000001);}
	if( link_ob->query("board_last_read/lee_b") ) {
		link_ob->set("board_last_read/lee_b", 000000001);}
	if( link_ob->query("board_last_read/market_b") ) {
		link_ob->set("board_last_read/market_b", 000000001);}
	if( link_ob->query("board_last_read/purple_b") ) {
		link_ob->set("board_last_read/purple_b", 000000001);}
	if( link_ob->query("board_last_read/swaylan_b") ) {
		link_ob->set("board_last_read/swaylan_b", 000000001);}
	if( link_ob->query("board_last_read/wutan_b") ) {
		link_ob->set("board_last_read/wutan_b", 000000001); }

	tell_object(me, "記錄已消除。\n");
	me->save();
	return;
}
