// flag.c

string __flag = "";

int recognize(int flag, int set_if_zero)
{
	if( stringp(__flag) && test_bit(__flag, flag) ) return 1;
	if( set_if_zero ) __flag = set_bit(__flag, flag);
	return 0;
}
