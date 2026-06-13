/*---
description: 定義寺廟房間(復活點)相關功能。
author: Annihilator <taedlar@gmail.com>
---*/
#include <ansi.h>

inherit ROOM;

void setup() {
}

void init() {
  add_action ("do_enter", "enter");
}

string extra_room_desc() {
  if (this_player() && (wizardp(this_player()) || this_player()->query("life_form") == "ghost"))
    return HIR "    你看見地面上有一個紅色的漩渦(swirl)在不斷旋轉 ...\n" NOR;
  return ::extra_room_desc();
}

int do_enter (string arg) {
  if (!this_player() || this_player()->query("life_form") != "ghost") {
    write ("你走進漩渦的中央，但甚麼事都沒有發生。\n");
    return 1;
  }

  write (HIW "\n你覺得一陣暈眩 ...\n\n" NOR);
  CHAR_D->make_living (this_player());
  this_player()->start_busy(3);
  return 1;
}

varargs int do_look (object me, string arg) {
  if (arg == "swirl" || arg == "漩渦") {
    if (!wizardp(me) && me->query("life_form")!="ghost")
      return ::do_look (me, arg);
    write ("這個漩渦散發著詭異的紅光，你不確定是否應該鼓起勇氣走進去(enter) ...\n");
    return 1;
  }

  return ::do_look(me, arg);
}

