/*  userid.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// Created by Elon (5/10/1997)

#include <intermud.h>

//object ob;
mapping requests;

void create()
{
    seteuid(getuid(this_object()));
    //ob = find_player("elon");
    requests = ([]);
}

void query_userid(string str) {
    string socket, address;
    int port,fd,err;
    object ob;

    if(!str) return 0;
    ob = find_player(str);

    // for debug
    if(!ob) return notify_fail("找不到這個人.\n");

    socket = socket_address(ob);
    //printf("Address: %s\n", socket);
    
    sscanf(socket, "%s %d", address, port);
    //printf("telnet %s 113\n", address);
     //printf("Ident syntax: %d, 4000\n", port);

    fd = socket_create(STREAM, "socket_shutdown");
    if(fd<0) return;

    err = socket_connect(fd, address+" 113","receive_data", "write_data");
    //printf("error: %d", err);
    if(err<0) {
        socket_close(fd);                                             
        return;
    }
    call_out("timeout",60,fd);
    requests[fd] = ({ob, port});
    return;
}

/* 
void test_socket(string str)
{
    int fd;
    
    fd = socket_create(STREAM, "socket_shutdown");
    printf("fd: %d\n",fd);
    if(fd<0) return;
    if(socket_connect(fd, str+" 113","receive_data", "write_data")<0) {
        socket_close(fd);
        write("not connected\n",);
        return;
    } 
}
*/

void write_data(int fd)
{
    int t;
    t = socket_write(fd, requests[fd][1]+", 4000\n");
    if(t<0) {
        socket_close(fd);
        requests[fd][0]->set("userid","no identd");
        map_delete(requests, fd);
        return;
    }
}

void receive_data(int fd, string str)
{
    string tmp, machine, name, userid;

    if(!requests[fd][0]) {
        socket_close(fd);
        map_delete(requests, fd);
        return;
    }
    //requests[fd][0]->set("userid",str);
    //log_file("ident",sprintf("[%s]: %s login from %s@%s.\n",
//                            ctime(time()),
//                            requests[fd][0]->query("id"),
//                            str,
//                            query_ip_name(requests[fd][0]))
//            );
    //return;

    if(sscanf(str, "%s : USERID : %s : %s\r\n", tmp, machine, name)!=3) {
        socket_close(fd);
        requests[fd][0]->set("userid","error");
        map_delete(requests, fd);
        return;
    }
    requests[fd][0]->set("userid",name+"@"+query_ip_name(requests[fd][0]));

    socket_close(fd);
    map_delete(requests, fd);
    return;

}

mapping query_requests() { return requests; }

void timeout(int fd)
{
    if(requests[fd]) { 
    if(!requests[fd][0]) {
        socket_close(fd);
        map_delete(requests, fd);
        return;
    }
    //log_file("ident.timeout",sprintf("[%s]: Timeout for %s.\n",
//                            ctime(time()),requests[fd][0]->query("id")));
    }
    socket_close(fd);
    map_delete(requests, fd);
}
