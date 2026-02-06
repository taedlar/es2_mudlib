

inherit ITEM;
inherit F_STUDY;

void create()
{
    set_name("【幼學輯要】", "book of elementary", "book");
    set_weight(1000);
    if( !clonep() ) {
        set("unit", "本");
        set("value", 200);
        set("long",
            "這本書是民間一般私塾教導幼童啟蒙所用的教科書，內容\n"
            "十分淺顯，大致說一些尊親、忠君的基本道理。\n");
        set("required/skill", ([
	    "literate" : 1,
        ]));
        set("content", ([
	    "literate" : 10
        ]));
    }
    setup();
}


