inherit F_DBASE;

static void
create()
{
}

string
query_rank (object obj, string politness)
{
	if (!politness)
		return "武者";

	switch (politness) {
		case "self":
			return "在下";
		case "respectful":
			return "英雄";
		case "rude":
		default:
			return "傢伙";
	}
}

