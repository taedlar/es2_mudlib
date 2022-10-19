// vim: syntax=lpc
#include <ansi.h>

inherit ROOM;

string temple_long(object r);

void
setup()
{
  string long_desc = query("long");
  set("long_desc", long_desc);
  set("long", (: temple_long :));
}

private void
init()
{
  add_action("do_enter", "enter");
}

private string
temple_long(object r)
{
  string long_desc = r->query("long_desc");
  if (this_player() && this_player()->query("life_form")=="ghost")
    long_desc += "    你看見地面上有一個" RED "紅色的漩渦(swirl)" NOR "在不斷旋轉 ...\n";

  return long_desc;
}

int
do_enter(string arg)
{
  if (!this_player() || arg!="swirl")
    return 0;

  if (this_player()->query("life_form") != "ghost")
    return 0;

  write("你覺得一陣暈眩 ...\n");
  CHAR_D->make_living(this_player());
  this_player()->start_busy(3);

  return 1;
}

varargs int
do_look(object me, string arg)
{
  if (me->query("life_form")!="ghost" || (arg!="swirl" && arg!="漩渦"))
    return ::do_look(me, arg);

  write("這個漩渦散發著詭異的紅光，你不確定是否應該鼓起勇氣走進去(" UNDL "enter" NOR ") ...\n");
  return 1;
}

