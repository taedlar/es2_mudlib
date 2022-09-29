/*  enhanced.c

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
#include <combat.h>

string enhance_weapon(object ob, int bonus, string weapon_type)
{
    int major = random(200), minor = random(1000);
    string new_name = "";
    string id = ob->query("id");
    mapping apply = ([]);

    if( major < 3 )
        switch( random(14) ) {
        case 0:
            new_name += CYN "青銅"; apply["defense"] = 10;
            id = "bronze " + id;
            break;
        case 1:
            new_name += RED "鑌鐵"; apply["damage"] = 3;
            id = "iron " + id;
            break;
        case 2:
            new_name += HIW "點鋼"; apply["attack"] = 5; apply["damage"] = 2;
            id = "steel " + id;
            break;
        case 3:
            new_name += HIB "青鋼"; apply["attack"] = 5; apply["defense"] = 5;
            id = "steel " + id;
            break;
        case 4:
            new_name += HIK "黑鐵"; apply["damage"] = 2; apply["defense"] = 5;
            id = "iron " + id;
            break;
        case 5:
            new_name += MAG "松紋"; apply["str"] = 1; apply["attack"] = 5;
            id = "fine " + id;
            break;
        case 6:
            new_name += HIM "雕花"; apply["dex"] = 1; apply["defense"] = 5;
            id = "fine " + id;
            break;
        case 7:
            new_name += HIK "纏布"; apply["cps"] = 1; apply["armor"] = 3;
            id = "wrapped-hilt " + id;
            break;
        case 8:
            new_name += HIK "鐵棘"; apply["str"] = 1; apply["damage"] = 2;
            id = "thorny " + id;
            break;
        case 9:
            new_name += HIW  "太極"; apply["wis"] = 1; apply["defense"] = 5;
            id = "taiji " + id;
            break;
        case 10:
            new_name += HIW  "陰陽"; apply["int"] = 1; apply["attack"] = 5;
            id = "yinyang " + id;
            break;
        case 11:
            new_name += HIR "硃砂"; apply["magic"] = 5; apply["spells"] = 5;
            id = "cinnabar " + id;
            break;
        case 12:
            new_name += HIY "日月"; apply["con"] = 1; apply["magic"] = 5;
            id = "glowing " + id;
            break;
        case 13:
            new_name += HIK "厭火"; apply["con"] = 1; apply["damage"] = 2;
            id = "yenholdish " + id;
            break;
    }
    else if( major < 6 )
        switch( random(22) ) {
        case 0:
            new_name += HIW "精鋼"; apply["attack"] = 10; apply["damage"] = 3;
            id = "highsteel " + id;
            break;
        case 1:
            new_name += HIW "白銀"; apply["defense"] = 15;
            id = "silver " + id;
            break;
        case 2:
            new_name += HIK "黑鋼"; apply["attack"] = 15;
            id = "darksteel " + id;
            break;
        case 3:
            new_name += YEL "古錠"; apply["defense"] = 10; apply["cps"] = 1;
            id = "darkmetal " + id;
            break;
        case 4:
            new_name += RED "雛鐵"; apply["attack"] = 10; apply["defense"] = 10;
            id = "iron" + id;
            break;
        case 5:
            new_name += HIR "彤雲"; apply["wis"] = 1; apply["cor"] = 1;
            id = "cloudy " + id;
            break;
        case 6:
            new_name += MAG "紫電"; apply["dex"] = 1; apply["str"] = 1;
            id = "thunder " + id;
            break;
        case 7:
            new_name += HIY "金鑲"; apply["con"] = 1; apply["int"] = 1;
            id = "golden " + id;
            break;
        case 8:
            new_name += HIW "太玄"; apply["spi"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 9:
            new_name += HIB "太清"; apply["wis"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 10:
            new_name += HIC "太乙"; apply["int"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 11:
            new_name += HIK "太陰"; apply["int"] = 2;
            id = "moon " + id;
            break;
        case 12:
            new_name += HIY "太陽"; apply["con"] = 2;
            id = "sun " + id;
            break;
        case 13:
            new_name += HIB "辟妖"; apply["magic"] = 15;
            id = "demonbane " + id;
            break;
        case 14:
            new_name += BLU "辟邪"; apply["spells"] = 15;
            id = "devilbane " + id;
            break;
        case 15:
            new_name += HIK "鎮鬼"; apply["magic"] = 10; apply["spells"] = 10;
            id = "nether " + id;
            break;
        case 16:
            new_name += RED "赤血"; apply["damage"] = 5;
            id = "blood " + id;
            break;
        case 17:
            new_name += HIR "碧血"; apply["armor"] = 5;
            id = "blood " + id;
            break;
        case 18:
            new_name += CYN "無腸"; apply["int"] = 3; apply["con"] = -1;
            id = "woochanian " + id;
            break;
        case 19:
            new_name += HIG "焦僥"; apply["dex"] = 3; apply["str"] = -1;
            id = "jiaojao " + id;
            break;
        case 20:
            new_name += HIM "紫陽"; apply["cps"] = 2;
            id = "dawn " + id;
            break;
        case 21:
            new_name += HIC "青陽"; apply["wis"] = 2;
            id = "shining " + id;
            break;
    }

    if( new_name=="" ) return ob->name();
    
    if( minor < 3 )
        switch( random(13) ) {
        case 4:
            new_name += HIR "火炎";
            id += " of flame";
            break;
        case 5:
            new_name += HIB "寒冰";
            id += " of freeze";
            break;
        case 6:
            new_name += HIC "風波";
            id += " of wind";
            break;
        case 7:
            new_name += HIB "震雷";
            id += " of lightning";
            break;
        case 8:
            new_name += YEL "羅漢";
            id += " of guardian";
            break;
        case 9:
            new_name += HIW "天龍";
            id += " of wyvern";
            break;
        case 10:
            new_name += HIR "赤龍";
            id += " of red dragon";
            break;
        case 11:
            new_name += HIY "金鷹";
            id += " of eagle";
            break;
        case 12:
            new_name += HIC "天鷹";
            id += " of falcon";
            break;
    }

    if( new_name != "" ) {
        mixed wield_as;
        string wield_skill;
        new_name += weapon_type + NOR;
        ob->set("name", new_name);
        ob->set("id", id);
        // Apply enchantment to the weapon.
        wield_as = ob->query("wield_as");
        if( arrayp(wield_as) ) 
            foreach(wield_skill in wield_as)
                ob->set("apply_weapon/" + wield_skill, apply);
        else
            ob->set("apply_weapon/" + wield_as, apply);
        return new_name;
    }

    return ob->name();
}
