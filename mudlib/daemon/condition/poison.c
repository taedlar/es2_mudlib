

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("poison");
}



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



