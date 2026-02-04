// vim: set ts=4 sw=4 syntax=lpc

#include <ansi.h>
#include <dbase.h>

#define MAX_MSG_BUFFER 500

static string *msg_buffer = ({});

void
receive_message(string msgclass, string msg)
{
    object link;
    string subclass, *ch;

    if( ! msg ) return;

    if( ! interactive(this_object()) ) {
        this_object()->relay_message(msgclass, msg);
        return;
    }

    sscanf(msgclass, "%s:%s", subclass, msgclass);

    switch(subclass)
    {
    case "channel":
	if( !objectp(link = this_object()->link())
	||  !pointerp(ch = link->query("channels"))
	||  member_array(msgclass, ch)==-1 )
	    return;
	break;
    case "outdoor":
	if( !environment() || !environment()->query("outdoors") )
	    return;
	break;
    default:
	if( this_player()
	&&  (this_player() != this_object()) 
	&&  !this_player()->visible(this_object()) )
	    return;
    }

    if( query_temp("block_msg/all")
    ||	query_temp("block_msg/" + msgclass) )
	return;

    if( in_input(this_object()) || in_edit(this_object()) ) {
        if( sizeof(msg_buffer) < MAX_MSG_BUFFER )
            msg_buffer += ({ msg });
    } else
	receive( msg );
}

private void
write_prompt()
{
    if( sizeof(msg_buffer) )
    {
	receive(BOLD "[輸入時暫存訊息]\n" NOR);
	foreach(string msg in msg_buffer)
	    receive(msg);
	msg_buffer = ({});
    }
    receive("> ");
}

private void
receive_snoop(string msg)
{
    receive("※" + msg);
}

