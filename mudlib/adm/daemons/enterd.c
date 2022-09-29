// enterd.c
//
//      預定將所有進 mud 的檢查寫在這邊, 雖然 logind.c 中已經
//      有 enter_world 了, 但是還是寫一個出來, 我比較習慣
//
//                                                      Arthur (1999/1/17)
//
// extract()                            By Annihilator (1-8-94)
// check_okip() & compare_okip()        By Arthur (1999/1/17)

varargs string extract( string str, int from, int to )
{
        if( nullp(to) ) return str[from..sizeof(str)-1];
        return str[from..to];
}

int compare_okip(object me)
{
	mixed ip_list;
        string  cur_ip, cur_ip_num, ip_part, num_part, be_checked;
        int     i, gate, check = 1, len, ed;

	seteuid(geteuid(me));
        if( !me ) return 0;
        if( !me->query("okip") ) {
                tell_object(me, "你沒有任何 ip 限制, 請用 okip 設定。\n");
                return 1;
        }
        
	ip_list = ({ me->query("okip/1") , me->query("okip/2"), 
			me->query("okip/3")});
        cur_ip  = query_ip_name(me);
        cur_ip_num = query_ip_number(me);
/*        i       = sizeof(ip_list);
	i = 3;
	log_file("OKIP_NUM", i);
        while(i--) {
		log_file("OKIP1","test \n");
                if( ip_list[i] == "" ) continue;
                check = 0;
                if( sscanf(ip_list[i], "%s*", be_checked) == 1 
                && be_checked != "" ) {
			log_file("OKIP2","test \n");
                        len      = strlen(be_checked);
                        ip_part  = extract(cur_ip, 0, len-1);
                        num_part = extract(cur_ip_num, 0, len-1);
                } else if( sscanf(ip_list[i], "*%s", be_checked) == 1 
                && be_checked != "" ) {
			log_file("OKIP3","test \n");
                        len      = strlen(be_checked);
                        ed       = strlen(cur_ip);
                        ip_part  = extract(cur_ip, ed - len);
                        num_part = extract(cur_ip_num, ed - len);
                } else {
			log_file("OKIP4","test \n");
                        be_checked = ip_list[i];
                        ip_part    = cur_ip;
                        num_part   = cur_ip_num;
                }
                if( lower_case(be_checked) == lower_case(ip_part) 
                ||  lower_case(be_checked) == lower_case(num_part) ) {
                        tell_object(me, " IP checked and ok.\n");
                        return 1;
                }
        }  */
        for(i=0; i<4; i++) {
	log_file("OKIP_NUM", i);
      if(ip_list[i] == "") continue;
      check = 0;
          if(sscanf(ip_list[i],"%s*", be_checked)==1 && be_checked != "" ) {
                len = strlen(be_checked);
                ip_part = extract(cur_ip, 0, len-1);
                num_part = extract(cur_ip_num, 0, len-1);
          } else if(sscanf(ip_list[i],"*%s", be_checked)==1 && be_checked != "" ) {
                len = strlen(be_checked);
                ed = strlen(cur_ip);
                ip_part = extract(cur_ip, ed-len); /* Elon fix this line */
                num_part = extract(cur_ip_num, ed-len);
          } else {
            be_checked = ip_list[i];
            ip_part = cur_ip;
            num_part = cur_ip_num;
          }
          if( lower_case(be_checked) == lower_case(ip_part) ||
              lower_case(be_checked) == lower_case(num_part) ) {
                log_file("okipok", " IP checked and ok.\n");
                return 1;
          }
        }

        if( check ) {
                tell_object(me,
                        "無 IP Address 限制。\n");
                return 1;
        } else return 0;
}               
 
void check_okip(object me)
{
	log_file("OKIP", "test1");
        if( !me ) return;
        if( !compare_okip(me) ) {
                tell_object(me, "警告: 你無法從這個位址上來。\n" );
		command("quit");
		log_file("OKIP", "test2");
                return;
        }
}
