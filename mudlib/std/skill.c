// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

void setup()
{
    seteuid(getuid());
}

string type()
{
    return "martial";
}



