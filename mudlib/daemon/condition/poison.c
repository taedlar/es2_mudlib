/*  poison.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("poison");
}

/*
    這個中毒症狀是一般性的，用金木水火土來定義毒的性質，以後會醫術的人可以診斷
    出毒的性質，開藥解毒。

    一般而言你可以自由定義毒物的五行屬性，以下分類只供參考：

    金 - 礦物之毒，每次發作會損傷神的狀態值。
    木 - 草木之毒，中這類毒之人的悟性會降低。
    水 - 陰性之毒，每次發作會損傷精的狀態值。
    火 - 陽性之毒，每次發作會損傷氣的狀態值。
    土 - 蟲蛇之毒，中這類毒之人的膂力會降低。

    一種毒物通常由五行中的兩三種性質構成，如我們定義某種蛇毒是[土 10, 水 10]，
    用藥物治療這種蛇毒就需要用剋制土和水的組合來對付，因此需要一種性質為[木 10,
    土 10]的藥物才能解毒。

    中毒症狀另外可以定義幾個參數：

    duration	- 持續時間，每發作一次就減一，減到 0 毒性就消失。
    damage	- 毒性發作所扣生命點數。
		  傷害力是在 update_condition 中作用，屬慢性傷害。
*/

void
condition_update(object me, string cnd, mixed cnd_data)
{
    // 只有 living 的生命型態會受到毒的影響。
    if( me->query("life_form") != "living"
    ||	!mapp(cnd_data)
    ||	(cnd_data["duration"]--) < 1 ) {
	me->delete_condition(cnd);
	return;
    }

    if( cnd_data["damage"] )
    {
	int dam = cnd_data["damage"];

	tell_object(me, "你中的毒發作了。\n");

	dam -= random( me->query_temp("apply/resist_poison") );
	if( dam < 0 ) dam = 0;

	me->consume_stat("HP", dam);
	if( cnd_data["水"] > 0 ) me->damage_stat("gin", cnd_data["水"] );
	if( cnd_data["火"] > 0 ) me->damage_stat("kee", cnd_data["火"] );
        if( cnd_data["金"] > 0 ) me->damage_stat("sen", cnd_data["金"] );
    }
    me->set_condition("poison", cnd_data);
}
/*
void
condition_apply(object me, string cnd, mapping cnd_data)
{
    if( ! mapp(cnd_data) ) return;
    if( cnd_data["damage"] && cnd_data["土"] )
	me->add_temp("apply/str", - cnd_data["土"]);
    if( cnd_data["damage"] && cnd_data["木"] )
	me->add_temp("apply/int", - cnd_data["木"]);
}

void
condition_unapply(object me, string cnd, mapping cnd_data)
{
    if( ! mapp(cnd_data) ) return;
    if( cnd_data["damage"] && cnd_data["土"] )
	me->add_temp("apply/str", cnd_data["damage"]);
    if( cnd_data["damage"] && cnd_data["木"] )
	me->add_temp("apply/int", cnd_data["damage"]);
}
*/

