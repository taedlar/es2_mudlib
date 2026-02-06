

inherit F_CLEAN_UP;

void setup()
{
    seteuid(getuid());
}

string type()
{
    return "martial";
}



