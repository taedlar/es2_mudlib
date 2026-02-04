// vim: set ts=4 sw=4 syntax=lpc

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

// vim: set ts=4 sw=4 syntax=lpc

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

