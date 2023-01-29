/*
establishing and using RFCOMM connections analog to TCP/IP programming. The only difference is that the socket addressing structures are different, and we use different functions for byte ordering of
multibyte integers. rfcomm_server and rfcomm_client show how to establish a connection using an RFCOMM socket, transfer some data, and disconnect. 
For simplicity, the client is hard-coded to connect to ``01:23:45:67:89:AB". 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 };
    struct sockaddr_rc rem_addr = { 0 };
    char buf[1009] = { 0 };
    char bt_buf[8] = {0};
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	//domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM

    // bind socket to port 1 of the first available local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH; 	//specifies the addressing family of the socket
    loc_addr.rc_bdaddr = *BDADDR_ANY;		//indicates that any local Bluetooth adapter is acceptable.
    loc_addr.rc_channel = (uint8_t) 1;	//specify port number to connect to
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));	// accept incoming connections with a socket, use bind to reserve operating system resource

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);	//block and accept an incoming connection

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    //write buffer in file
    int openfd;
    uint32_t gesamt_bytesread = 0;
    if (fopen("/tmp/jabra_cap.wav", "w") == NULL) printf("Error creating/opening receive-File!\n");
    openfd = open("/tmp/jabra_cap.wav", O_WRONLY);
    if (openfd < 0) printf("Error opening openfd!\n");
    
    //int x=0;
    while( (bytes_read = read(client, bt_buf, sizeof(bt_buf))) > 0)
    {
	//printf("Im %d.Durchgang bytes_read: %d  -  insgesamt bytes_read: %d\n",x, bytes_read, (gesamt_bytesread += bytes_read));
    	write(openfd, bt_buf, sizeof(bt_buf));
    	//x++;
    }

    // close connection
    close(client);
    close(s);
    return 0;
}
