#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>


int main(int argc, char **argv)
{
    struct sockaddr_rc addr2 = { 0 };
    int s, status;

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	////domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM
	printf("socket set up - return value: %d\n", s);

    addr2.rc_family = AF_BLUETOOTH;
    addr2.rc_channel = (uint8_t) 1;
    str2ba("B8:27:EB:DC:8C:D1", &addr2.rc_bdaddr );

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
