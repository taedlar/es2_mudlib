// socket.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	write(dump_socket_status());
	return 1;
}
