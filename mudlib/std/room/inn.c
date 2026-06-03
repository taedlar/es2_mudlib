/*---
description: 定義旅館房間相關功能。
author: Annihilator <taedlar@gmail.com>
---*/
#include <daemon.h>

inherit ROOM;

void
setup()
{
  ::setup();
  set("valid_startroom", 1);
  set("no_fight", 1);

  TRAVELER_D->register_travel_site();
}
