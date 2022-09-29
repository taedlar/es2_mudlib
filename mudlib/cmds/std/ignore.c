
inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main( object me, string name )
{
  string * ignore;
  int i;

  // add by dragoon
  if( name == "all" ) {
	me->set_temp("ignore/all", 1);
	tell_object(me, "你開始拒絕所有人對你說話。\n");
	return 1;
  }
  if( name == "cancel" ) {
	me->delete_temp("ignore/all");
	tell_object(me, "你開始接收所有人對你說話。\n");
	return 1;
  }

  seteuid(getuid(me));
  ignore = me->query( "ignore" );
  if( !pointerp( ignore ) ) ignore = ({ });
  if( !name )
  {
    if( !sizeof( ignore ) ) write( "你現在沒有拒絕別人跟你交談。\n" );
    else write( "你現在拒絕跟你交談的人有：" + implode( ignore, ", " ) + "\n");
    return 1;
  }
  if( -1 == member_array( name, ignore ) )
  {
    write( "你現在開始不讓  " + name + "  跟你交談。\n" );
    ignore += ({ name });
  } else {
    write( "你現在開始讓  " + name + "  跟你交談。\n" );
    ignore -= ({ name });
  }
  me->set( "ignore", ignore );
  return 1;
}

int help()
{
        write(@HELP
指令格式﹕ignore < player > | all | cancel

當你不想跟某人交談時就可以用這個指令。

範例﹕
    ignore cuteweca             拒絕聽到 cuteweca 的聲音。
    ignore cuteweca             讓 cuteweca 可以再跟您說話了。
    ignore all			拒絕聽到任何人對你講話。
    ignore cancel		取消 ignore all 的效用。

HELP
        );
  return 1;
}
