// vim: syntax=lpc

inherit BULLETIN_BOARD;

private void
create()
{
  set_name("問題回報留言板", ({"bug board", "board"}));
  set("location", "/d/snow/bug_room");
  set("board_id", "bugs");
  set("capacity", 30);
  setup();
  set("long", "這是一個提供玩家回報問題的留言板。\n");
  replace_program(BULLETIN_BOARD);
}

