// vim: set ts=4 sw=4 syntax=lpc

string
wizhood(mixed ob)
{
    return SECURITY_D->get_status(ob);
}

int
wiz_level(mixed ob)
{
    return SECURITY_D->get_wiz_level(ob);
}

