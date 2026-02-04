// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me)
{
    me->delete("long");
    write("Ok。\n");
    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕undescribe

這個指令讓你拿掉你自己的描述。
TEXT
    );
    return 1;
}

