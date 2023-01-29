
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <l2cap.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    struct sockaddr_l2 addr = { 0 }, addr2 = {0};
    int s,s2, status;
    //char *message = "hello!";
    char dest[18] = "01:23:45:67:89:AB";

    if(argc < 2)
    {
        fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
        exit(2);
    }

    strncpy(dest, argv[1], 18);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    
    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(0x1001);
    str2ba( dest, &addr.l2_bdaddr );

//file-size ermitteln
        long numbytes;
        
	off_t file_size;
	char *voice_buffer;
	struct stat stbuf;
	int fd2;
	
	uint32_t mtu = 65534; //Maximum transmission unit for L2CAP
	  
	fd2 = open("/home/cloud/bltest.c", O_RDONLY);
	//fd2 = open("/home/cloud/jabra_cap.wav", O_RDONLY);
	if (fd2 == -1) {
	  /* Handle error */
	}
	  
	if ((fstat(fd2, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) {
	  /* Handle error */
	}
	  
	file_size = stbuf.st_size;
	  
	voice_buffer = (char*)malloc(file_size);
	if (voice_buffer == NULL) {
	  /* Handle error */
	}  
	    
	    
	    //puts("chapter 1a\n");
	    //if (numbytes = ftell(fjab) == 0) puts("error reading ftell\n");
      
      numbytes = (long)file_size;
        
      //size_t fread(void * buffer, size_t size, size_t count, FILE * stream)  
      //fread(voice_buffer,sizeof(char), numbytes, fopen);
      int rc = read(fd2, voice_buffer, (int)numbytes);
      
      if (voice_buffer == NULL) {
	  printf("rc: %d - Couldn't read in voice-buffer\n", rc);
	}    

	//erhöhen der MTU ?!
	struct l2cap_options {
		uint16_t omtu;
		uint16_t imtu;
		uint16_t flush_to;
		uint8_t mode;
	};
	struct l2cap_options opts;

	int optlen = sizeof(opts), err;

	err = getsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen);
	if(err == 0)
	{
		printf("omtu: %d, imtu: %d\n", opts.omtu, opts.imtu);
		opts.omtu = opts.imtu = mtu;
	}
	else
	{
		printf("getsocket error: %s!\n", strerror(errno));
	}



	err = setsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, optlen);
	if(err == 0)
	{
		getsockopt(s, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen);
		printf("omtu: %d, imtu: %d\n", opts.omtu, opts.imtu);
	}
	else
	{
		printf("setsocket error: %s!\n", strerror(errno));
	}

	
    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        //status = write(s, "hello from cloud-lab", 20);
        
        if(numbytes < mtu) 
        {
        	status = write(s, &voice_buffer[0],numbytes);
        }
        else
        {
        	uint32_t sendloops = numbytes/mtu ;
		if( (numbytes % mtu) > 20000 ) sendloops++;	//falls verbleibender Rest größer als 20000Bytes ist, führe eine weitere sendloop durch
	
		for(uint8_t z = 0; z < sendloops; z++)
		{
			//printf("%d.Runde", z+1);
			status = write(s, &voice_buffer[z*mtu], mtu);
			if(status < 0) perror("send gone wrong");	
		}
    	}
    }

    if( status < 0 ) perror("uh oh");

    close(fd2);
    close(s);
}


