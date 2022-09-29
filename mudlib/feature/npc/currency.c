private string price_string(int v)
{
	if( v%10000 == 0 )
		return chinese_number(v/10000) + "兩黃金";
	if( v%100 == 0 )
		return chinese_number(v/100) + "兩銀子";
	return chinese_number(v) + "文錢";
}

