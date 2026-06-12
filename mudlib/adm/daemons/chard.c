/*---
description: Character setup daemon, used to setup characters and create corpse/mist/ghost.
author: Annihilator <taedlar@gmail.com>
---*/
#pragma save_binary

#include <login.h>
#include <statistic.h>

inherit F_CLEAN_UP;

private void create() {
  seteuid (getuid());
}

/** Setup character's basic attributes, statistics, and hit-points according to
 *  character's race, class, and life form. This function is called by character object during setup.
 */
void setup_char (object ob) {
    mapping apply;

    // assign default race, class, level if not set.
    if (!ob->query_race())
        ob->set_race ("human");
    if (!ob->query_class())
        ob->set_class ("commoner");
    if (!ob->query_level())
        ob->set_level (1);

    // assign statistics and attributes if not set.
    ob->init_statistic();
    ob->init_attribute();

    // initialize apply, which is used to temporarily modify character's attributes and statistics.
    if (mapp (apply = ob->query("perm_apply")))
        ob->set_temp ("apply", copy(apply));
    else
        ob->set_temp ("apply", ([]));

    // setup hit-points, which regenerate according to constitution.
    ob->set_stat_maximum ("HP", ob->query_attr("con") * 5);
    ob->set_stat_regenerate ("HP", TYPE_HEALTH);

    // setup fatigue, which decays according to character's life form.
    ob->set_stat_maximum ("fatigue", 100);
    ob->set_stat_effective ("fatigue", 100);
    if (!ob->query_stat_current("fatigue"))
        ob->set_stat_current ("fatigue", 0);
    ob->set_stat_regenerate ("fatigue", TYPE_WASTING);

    // setup statistics and regenerate type according to character's life form.
    switch (ob->query("life_form"))
    {
    case "ghost":
        ob->set_temp ("apply/vision_of_ghost", 1);
        ob->delete_stat ("kee");
        ob->delete_stat ("food");
        ob->delete_stat ("water");
        ob->set_stat_regenerate ("gin", TYPE_WASTING);
        ob->set_stat_regenerate ("sen", TYPE_WASTING);
        break;
    case "undead":
        ob->set_temp ("apply/vision_of_ghost", 1);
        ob->delete_stat ("sen");
        ob->set_stat_regenerate ("gin", TYPE_HEALTH);
        ob->set_stat_regenerate ("kee", TYPE_HEALTH);
        ob->set_stat_regenerate ("food", TYPE_WASTING);
        ob->set_stat_regenerate ("water", TYPE_WASTING);
        break;
    case "living":
    default:
        ob->set_stat_regenerate ("gin", TYPE_HEALTH);
        ob->set_stat_regenerate ("kee", TYPE_HEALTH);
        ob->set_stat_regenerate ("sen", TYPE_HEALTH);
        ob->set_stat_regenerate ("food", TYPE_WASTING);
        ob->set_stat_regenerate ("water", TYPE_WASTING);
        break;
    }

    // wizards are immortal, no need for food and water.
    if (wizardp(ob)) {
        ob->set_stat_regenerate ("food", TYPE_STATIC);
        ob->set_stat_regenerate ("water", TYPE_STATIC);
    }

    // delegate to race/class daemon for further setup.
    RACE_D(ob->query_race())->setup (ob);
    CLASS_D(ob->query_class())->setup (ob);
    ob->add_path ("/daemon/race/" + ob->query_race() + "/");
    ob->add_path ("/daemon/class/" + ob->query_class() + "/");
}

varargs object make_corpse (object victim, object killer) {
    int i, k, negative_fame, killer_rep, killer_fame;
    object corpse, inv, police1, police2;
    string corpse_ob;
    mapping bounty;
    string score;
    int amount;

    // wizards never leave a corpse.
    if (wizardp (victim))
        return 0;

    // notify inventory item to save data if nessessary.
    all_inventory(victim)->owner_is_killed (killer);

    // check life form.
    switch (victim->query("life_form"))
    {
    case "ghost":
        message ("vision", victim->name() + "化成一陣煙，消失了 ...\n",
            environment(victim), victim);
        return 0;
    case "living":
    default:
        all_inventory()->add ("sinister", 1);
    }

    // check if using custom corpse object.
    corpse_ob = victim->query("corpse_ob");
    if (!corpse_ob)
        corpse_ob = CORPSE_OB;

    // create corpse object and set it up.
    seteuid (MUDLIB_UID);
    catch (corpse = new(corpse_ob));
    seteuid (getuid());

    corpse->set_name (victim->name(1) + "的屍體",
        "corpse of " + capitalize (victim->query("id")), "corpse" );
    corpse->set ("long", victim->long()
        + "然而﹐" + gender_pronoun(victim->query("gender")) 
        + "已經死了﹐只剩下一具屍體靜靜地躺在這裡。\n");
    corpse->set ("age", victim->query("age"));
    corpse->set ("gender", victim->query("gender"));
    corpse->set ("victim_name", victim->name(1));

    // relate victim with hir corpse.
    victim->set_temp ("corpse", corpse);

    // move the corpse to the environment of the victim, and start decaying.
    corpse->set_weight (victim->query_weight());
    corpse->set_max_encumbrance (victim->query_max_encumbrance());
    corpse->move (environment(victim));
    corpse->start_decay (userp(victim) ? 90 : 60);

    // transfer victim's inventory into corpse.
    foreach (inv in all_inventory(victim))
        inv->move (corpse);
        
    // handle the killer.
    if (!killer)
        killer = victim->last_damage_giver();

    if (objectp (killer)) {
        // Player-killing
        if (userp (victim) && userp (killer) && (victim != killer) && (victim->query_level() > 1) ) {
            killer->gain_score ("mortal sin",
                victim->query_level() * victim->query_level() * 10);
            // 如果被害人素行良好, 殺人者折損自身聲望及武林聲望, 殺人者
            // 若是小偷, 則略增黑道聲望 -dragoon
            negative_fame = victim->query_score("reputation") + 
                victim->query_level() * victim->query_level() -
                victim->query_score("mortal sin") / 15;

            if( negative_fame > 0 ) {
                killer_rep = killer->query_score("reputation");
                killer_fame = killer->query_score("martial fame");
                k = random(negative_fame);
                // 扣聲望
                if( killer_rep > k ) 
                    killer->gain_score("reputation", -k);
                else killer->gain_score("reputation", -killer_rep);
                // 扣武林聲望
                if( killer_fame > 0 ) {
                    if( killer_fame > k/10 )
                        killer->gain_score("martial fame", -k/10);
                    else killer->gain_score("martial fame", -killer_fame);
                }
                // 小偷略增黑道聲望
                if( killer->query("class") == "thief" )
                    killer->gain_score("negative fame", k/10);
            }
        }

        // if pker die, delete mark about pker -dragoon
        if( victim->query("last_pk_time")
        || victim->query("pker")
        || victim->query("pk_record") ) {
            victim->delete("last_pk_time");
            victim->delete("pker");
            victim->delete("pk_record");
        }
        // set marks on PKer -dragoon
        if( userp(killer) && userp(victim) ) {
            // 若是此人曾殺過人, 且時間少於某段時間, 則記錄上加一筆, 否則, 
            // 便重設時間記錄
            // 加上設限: 必需lv>1才會增加, 以防神風特攻隊以另類方式 pk
            if( killer->query("last_pk_time")
            && time() - (int)killer->query("last_pk_time") < 60 * 60 ) {
                if( victim->query("level") > 1 )
                    killer->add("pk_record", 1); }
            else {  killer->set("pk_record", 1); }
            // 設上最近一次殺人的時間
            killer->set("last_pk_time", time());
            // 殺人數高於3開始有機會出現捕頭, 另外加上治安指數
            // 目前設定: 治安普通, 命案發生有三成機率出現捕頭
            if( killer->query("pk_record") > 3 + random(12) 
            && random(10) > 6 ) {
                if( killer->query("pk_record") > 15 + random(10) ) {
                    write("兩個捕頭遠遠的聽見慘叫，迅速的奔了過來!!\n");
                    police1 = new("/d/domain/npc/police_leader");
                    police1->move(environment(killer)); }
                else write("捕頭遠遠的聽見慘叫，迅速的奔了過來!!\n");
                police2 = new("/d/domain/npc/police");
                police2->move(environment(killer));
            }		
        }

        // Register vendetta marks if any.
        if( victim->query("vendetta_mark") )
            killer->add("vendetta/" + victim->query("vendetta_mark"), 1);

        // Killing humanoid creatures adds kills statistic
        if( victim->query("humanoid") )
            killer->gain_score("kills", 1);

        // Gain other bounty if any.
        if( mapp(bounty = victim->query("bounty")) )
            foreach(score, amount in bounty)
                killer->gain_score(score, amount);

        // Finally, let class daemon check this kill.
        CLASS_D(killer->query_class())->kill_check(killer, victim, corpse);
    }        

    return corpse;
}

int make_mist (object victim) {
    LOGIN_D->reincarnate (victim);
    return 1;
}

/*  生命型態轉換：生物 -> 鬼魂
 */
int make_ghost (object victim) {
    object corpse;

    if( victim->query("life_form") != "living" )
        error ("鬼魂只能由生物轉換而成。\n");

    corpse = victim->query_temp("corpse");
    victim->set ("life_form", "ghost");
    victim->set ("living", ([
        "kee" : victim->query_stat_maximum("kee"),
    ]));

    victim->supplement_stat("HP", 9999);
    victim->supplement_stat("gin", 9999);
    victim->supplement_stat("sen", 9999);
    victim->clear_statistic_flags();

    victim->clear_temp_dbase();
    victim->setup();

    /* 重新設定屍體的連結，因為 clear_temp_dbase() 會清掉 */
    if (objectp(corpse))
        victim->set_temp ("corpse", corpse);
    return 1;
}

int make_living (object ob) {
    object corpse;
    mapping living_stat;
    string stat;
    int max;

    living_stat = ob->query ("living");
    corpse = ob->query_temp ("corpse");

    /* 恢復生物的基本數值 */
    if (mapp (living_stat))
        foreach (stat, max in living_stat) {
            if (!corpse)
                max -= max/10;
            ob->set_stat_maximum (stat, max);
            ob->set_stat_effective (stat, max);
            ob->set_stat_current (stat, max);
        }

    ob->set ("life_form", "living");
    ob->delete ("living");

    ob->clear_temp_dbase();
    ob->setup();
    return 1;
}
