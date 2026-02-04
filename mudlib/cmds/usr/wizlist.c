// vim: set ts=4 sw=4 syntax=lpc

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string *list;
    string str;
    string *admin = ({}), *arch = ({}), *wizard = ({}),
        *apprentice = ({}), *immortal = ({}), *unknown = ({});
    int i;

    str = "  " + MUD_NAME + " 目前的巫師有﹕
=========================================================================\n";
    list = SECURITY_D->query_wizlist();

    for( i = 0; i < sizeof( list ); i++ ) {
        switch( SECURITY_D->get_status( list[i] ) ) {
        case "(admin)":
            admin += ({ list[i] });
            break;
        case "(arch)":
            arch += ({ list[i] });
            break;
        case "(wizard)":
            wizard += ({ list[i] });
            break;
        case "(apprentice)":
            apprentice += ({ list[i] });
            break;
        case "(immortal)":
            immortal += ({ list[i] });
            break;
        default:
            unknown += ({ list[i] });
        }
    }

    str += HIW
"【  大神  】【 adm 】   ";
    if( admin )
        for( i = 0; i < sizeof( admin ); i++ ) {
            str += sprintf("%-12s%s", admin[i],
                (i % 4 == 3 ) ? "\n\t\t\t" : "");
                }
    str += "\n" + HIY +
"【 大巫師 】【 arch 】  ";
    if( arch )
        for( i = 0; i < sizeof( arch ); i++ ) {
            str += sprintf("%-12s%s", arch[i],
                (i % 4 == 3 ) ? "\n\t\t\t" : "");
        }
    str += "\n" + HIG +
"【  巫師  】【 wiz 】   ";
    if( wizard )
        for( i = 0; i < sizeof( wizard ); i++ ) {
            str += sprintf("%-12s%s", wizard[i],
                (i % 4 == 3 ) ? "\n\t\t\t" : "");
        }
    str += "\n" + HIC +
"【實習巫師】【 app 】   ";
    if( apprentice )
        for( i = 0; i < sizeof( apprentice ); i++ ) {
            str += sprintf("%-12s%s", apprentice[i],
                (i % 4 == 3 ) ? "\n\t\t\t" : "");
        }
    str += "\n" + HIM +
"【見習巫師】【 imm 】   ";
    if( immortal )
        for( i = 0; i < sizeof( immortal ); i++ ) {
            str += sprintf("%-12s%s", immortal[i],
                (i % 4 == 3 ) ? "\n\t\t\t" : "");
        }
    str += "\n";
// vim: set ts=4 sw=4 syntax=lpc
    write( str + NOR + "\n" );
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : wizlist

用途: 列出目前所有的巫師名單.

參考: finger, who -w
HELP
     );
     return 1;
}

