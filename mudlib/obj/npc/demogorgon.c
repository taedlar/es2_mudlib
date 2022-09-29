// demongorgon.c

#include <ansi.h>

inherit NPC;

void create()
{
        set_name( HIW "死神" NOR, ({ "demogorgon" }) );
        set_level(100);
        set_race("human"); 
       set("long",
                "    死神正冷酷地站在你面前，他高大的身軀跟一股令人窒息的\n"
                "可怕殺氣正壓得你幾乎喘不過氣來，你的心裡似乎有一個聲音在告\n"
                "訴你﹕「 快離開 ！！」\n");

        set("age", 500);

        advance_stat("kee",900);
        advance_stat("gin",900);

        set_attr("str",35);
        set_attr("dex",42);
        set_attr("con",35);
         set_attr("spi",45);
        set_attr("cor",45);

        setup();

}

void start_shutdown()
{
        if( geteuid(previous_object()) != ROOT_UID ) return;

        message("system",
                HIR "\n你聽到天空中傳來一陣可怕的咆哮 ....\n\n"
                HIW     "死神" HIR "冷酷低沉的聲音傳入你的耳內：\n\n"
                        "\t\t想要命的，你們還有五分鐘可以趕快離開吧！！\n\n"
                        "\t\t不然的話，後果自行負責～～\n\n" NOR,
                users() );
        call_out("countdown", 240, 61);
}

private void countdown(int sec)
{
        sec--;
        if( sec > 59) {
                message("system",
                        HIW "\n死神" HIR "冷酷低沉的聲音傳進你的耳內：\n\n"
                "\t\t你們還有" + chinese_number(sec) + "秒鐘可以離開。\n\n"
                    "\t\t不要命的你就試試看吧！！\n\n" NOR,
                        users() );
                call_out("countdown", 49, 11);
        }else if( sec > 0) {
                message("system",
                        HIW "\n死神" HIR "冷酷低沉的聲音傳進你的耳內：\n\n"
                                "\t\t你們還有" + chinese_number(sec) + "秒鐘的時間可以逃！！\n\n"
                                "\t\t不想死的就趕快逃吧！！！\n\n" NOR,
                        users() );
                call_out("countdown", 1, sec);
        }
        else {
                message("system",
                        HIW "\n死神" HIR "用憤怒的聲音咆哮道：\n\n"
                            "\t\t天啊～ 地啊～  神啊～ \n\n" NOR,
                        users() );
                call_out("do_shutdown", 2);
        }
}

private void do_shutdown()
{
        message("system",
                HIW "\n\n死神" HIR "用憤怒的聲音喝道﹕\n\n"
                HIW "\t\t通 通 和 我 一 起 下 地 獄 吧 ～～～～ ﹗\n\n"
                HIR "\t\t聲音一畢，天空立刻變得血紅，不....你眼前的一切都變得血紅...\n\n"
                        "\t\t所有的一切都在瞬間陷入火海....\n\n"
                        "\t\t然後你的眼前是一片黑暗....無止盡的黑暗....\n\n" NOR,
                users() );
        shutdown(0);
}

void die()
{
        object ob;

        if( !ob = query_temp("last_damage_from") )
                ob = this_player(1);

        if( !ob ) return;

        message("system",
                HIR "\n\n你聽到一聲帶著憤恨、驚恐、與野獸般的咆哮聲響徹整個天空。\n\n"
                HIW "死神" HIR "嘶啞地吼著﹕可惡的" + ob->short() + "﹐我一定會報仇的～～～\n\n"
                        "然後一道黑色火焰呼嘯著衝上雲端﹐大地又恢復了寧靜。\n\n" NOR,
                users() );
        
        destruct(this_object());
}
