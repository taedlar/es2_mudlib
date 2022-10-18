inherit F_DBASE;

static void
create()
{
}

string
query_rank (object obj, string politness)
{
	if (!politness)
		return "平民";

	switch (politness) {
		case "respectful":
			return "朋友";
		case "rude":
		default:
			return "傢伙";
	}
}

