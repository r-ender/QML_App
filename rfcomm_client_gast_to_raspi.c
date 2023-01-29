/*
establishing and using RFCOMM connections analog to TCP/IP programming. The only difference is that the socket addressing structures are different, and we use different functions for byte ordering of
multibyte integers. rfcomm_server and rfcomm_client show how to establish a connection using an RFCOMM socket, transfer some data, and disconnect. 
For simplicity, the client is hard-coded to connect to ``01:23:45:67:89:AB". 
*/

//#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>


int main(int argc, char **argv)
{
/*
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "A8:6D:AA:B0:2D:49"; //"CC:3D:82:39:9B:61";	// "01:23:45:67:89:AB";

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	////domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM
	printf("socket set up - return value: %d\n", s);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );	//convert string to bt-adress
    puts("convert str 2 ba\n");

    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));	//connect to server
	puts("connect\n");

    // send  message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0; */
    
    /*
    	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	int i;
	char addr[19] = { 0 };
	char name[248] = {0};


    uint8_t svc_uuid_int[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xab, 0xcd };	//represents the 128-bit UUID that identifies the desired service
    uuid_t svc_uuid;
    int err;
    bdaddr_t target;
    sdp_list_t *response_list = NULL, *search_list, *attrid_list;
    sdp_session_t *session = 0;	//a session with a SDP server
	*/
    struct sockaddr_rc addr2 = { 0 };
    int s, status;
    //char dest[18] = "78:00:9E:8A:A6:0F"; //"CC:3D:82:39:9B:61";	// "01:23:45:67:89:AB";

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	////domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM
	printf("socket set up - return value: %d\n", s);

    addr2.rc_family = AF_BLUETOOTH;
    addr2.rc_channel = (uint8_t) 1;
    //str2ba( addr, &addr2.rc_bdaddr );	//convert string to bt-adress
    str2ba("CC:3D:82:39:9B:61", &addr2.rc_bdaddr );

    status = connect(s, (struct sockaddr *)&addr2, sizeof(addr2));	//connect to server
    puts("connect\n");

    // send  message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
    
}

