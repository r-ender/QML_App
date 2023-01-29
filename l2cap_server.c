#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>
#include <l2cap.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>



int main(int argc, char **argv)
{
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[65534] = { 0 };
    //char buf[80] = { 0 };
    char bt_buf[80] = {0};
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

	uint32_t mtu;

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // bind socket to port 0x1001 of the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_psm = htobs(0x1001);
	

	//MTU erhöhen ?!
	struct l2cap_options {
		    uint16_t    omtu;
		    uint16_t    imtu;
		    uint16_t    flush_to;
		    uint8_t     mode;
		};

	struct l2cap_options opts;
	
	int optlen = sizeof(opts), err;
	err = getsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen );
	

	//uint32_t mtu = (21984*2);
	if(err == 0 ) {
		printf("opts.omtu = %d, opts.imtu = %d\n", opts.omtu, opts.imtu);
		mtu = 65534;
		opts.omtu = opts.imtu = mtu;
	} else
	{
		printf("Error for getsockoptions occured: %s!\n",strerror(errno)); 
	}
	


	err = setsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, optlen );
	if(err == 0)
	{ 
		getsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen );
		printf("opts.omtu = %d, opts.imtu = %d\n", opts.omtu, opts.imtu);
	}
	else
	{
		printf("Error for setsockoptions occured: %s!\n",strerror(errno)); 
	}

    if( fopen("/tmp/msg_recvd.txt", "w") == NULL) printf("Error opening/creating audio sample file!\n");
    int openfd = open("/tmp/msg_recvd.txt", O_WRONLY);
    if(openfd < 0) printf("Error opening audio sample file!\n");

    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);	//test with 2 parallel connections

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.l2_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);

    memset(buf, 0, sizeof(buf));

	/* read data from the client */


	//read(client, buf,sizeof(buf));
	//write(openfd, buf, sizeof(buf));

	while(read(client, buf,sizeof(buf)) > 0 )
        {
            write(openfd, buf, sizeof(buf));
        }
	
	/*
            while( (bytes_read = read(client, buf,sizeof(buf))) > 0 )
            {
                  write(openfd, buf, sizeof(buf));
			//write(openfd, buf, sizeof(buf));
            }

            if( bytes_read == 0 ) {
                puts("no bytes received\n");
            }	*/

    // read data from the client
    //bytes_read = read(client, buf, sizeof(buf));
/*
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }
*/
    // close connection
    close(client);
    close(openfd);
    close(s);
}
