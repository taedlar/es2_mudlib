// gender.c

string gender_self(mixed subject)
{
	switch(objectp(subject) ? subject->query("gender") : subject) {
		case "female": return "妳";	break;
		default: return "你";
	}
}

string gender_pronoun(string subject)
{
	switch(objectp(subject) ? subject->query("gender") : subject) {
		case "deity":	return "祂";	break;
		case "male":	return "他";	break;
		case "female":	return "她";	break;
		case "male-animal":
		case "female-animal":	return "牠";	break;
		default: return "它";
	}
}

