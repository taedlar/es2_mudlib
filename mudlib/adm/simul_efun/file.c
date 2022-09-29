// file.c

void cat(string file)
{
    write(read_file(file));
}

void log_file(string file, string text)
{
    write_file(LOG_DIR + file, text);
}

void
assure_file(string file)
{
    string path, dir, *dirs;

    if( file_size(file)!=-1 ) return;

    dirs = explode(file, "/");
    path = "";
    seteuid(ROOT_UID);
    foreach(dir in dirs[0..<2]) {
	if( dir=="" ) continue;
	path += "/" + dir;
	switch( file_size(path) ) {
	case -1: mkdir(path); break;
	case -2: continue;
	default: return;
	}
    }
}

string
base_name(object ob)
{
    string file;

    if( sscanf(file_name(ob), "%s#%*d", file)==2 )
        return file;
    else
        return file_name(ob);
}
