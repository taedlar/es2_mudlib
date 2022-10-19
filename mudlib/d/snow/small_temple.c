// vim: syntax=lpc
#include <room.h>

inherit TEMPLE;

private void
create()
{
  set("short", "土地公廟");
  set("long", @LONG
這裡是一處小而整潔的土地公廟，中央一座紅磚砌成的神龕裡
供奉著一尊黑色臉龐的土地公神像。神龕前有一個黃銅香爐，幾柱
線香伴隨著輕煙散發出一種安詳的氛圍。
LONG
  );
  set("detail", ([
	"神龕": "這個神龕大約五尺見方，用紅磚砌成，沒有一般寺廟常見的雕刻裝飾。",
	"神像": "一尊黑色臉龐的木質土地公神像，看起來頗有年代了。",
	"香爐": "一個黃銅材質的香爐，雖然看上去有點老舊，但似乎經常有人細心擦拭。",
  ]));
  set("exits", ([
	"west" : __DIR__"nstreet1",
  ]));

  setup();
  replace_program(TEMPLE);
}
