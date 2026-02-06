

int save()
{
    string file;

    seteuid(getuid());
    if( stringp(file = this_object()->query_save_file()) ) {
        assure_file(file);
        return save_object(file);
    }
    return 0;
}

int restore()
{
    string file;

    seteuid(getuid());
    if( stringp(file = this_object()->query_save_file()) )
        return restore_object(file, 1);
    return 0;
}

