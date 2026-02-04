// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int
restore_mailbox(object ob)
{
    string owner;

    if( getuid(ob) != ROOT_UID || geteuid(ob) != 0)
	error("Permission denied.\n");

    owner = geteuid(this_player());
    if( !owner ) return 0;

    seteuid(owner);
    export_uid(ob);
    seteuid(getuid());

    ob->restore();
}

int
send_mail(string from, string to, mapping mail)
{
    object rcpt, mbox;
    int keep_mbox = 0;

    // vim: set ts=4 sw=4 syntax=lpc
    if( objectp(rcpt = find_player(to))
    &&	objectp(mbox = rcpt->query_temp("mbox_ob")) )
	keep_mbox = 1;
    else {
	mbox = new( MAILBOX_OB );
	seteuid(to);
	export_uid(mbox);
	seteuid(getuid());
	if( !mbox->restore() ) {
	    destruct(mbox);
	    return 0;
	}
    }

    mbox->receive_mail(mail);
    mbox->save();
    if( ! keep_mbox ) destruct(mbox);
    log_file("MAIL", sprintf("[%s] %s mailed to %s, size = %d\n",
	ctime(time()), from, to, strlen(mail["text"])) );
    return 1;
}

