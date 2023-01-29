#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>
#include <fcntl.h>
#include <stdbool.h>
#include <poll.h>
#include <string.h>


void bt_server()
{
	//puts("bt_server started!\n");

	    struct sockaddr_rc loc_addr = {0};	/* local bluetooth adapter's info */
            struct sockaddr_rc client_addr;	/* filled in with remote (client) bluetooth device's info */
            char buf[1024] = { 0 };
            char bt_buf[80] = {0};
	    bool bt_bound;
            int bytes_read,server_sock, client_sock;
            socklen_t opt = sizeof(client_addr);

	    struct pollfd btpoll;

            //Handy-test
            //client_addr.rc_bdaddr = (sockaddr_rc).rc_bdaddr "A4:6C:F1:08:96:B7";
            //client_addr.rc_bdaddr = (bdaddr_t){0xB7, 0x96, 0x08, 0xF1, 0x6C, 0xA4};

            /* allocate socket */
            server_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

            loc_addr.rc_family = AF_BLUETOOTH;		/* Addressing family, always AF_BLUETOOTH */
            //bacpy(&loc_addr.rc_bdaddr, BDADDR_ANY);		/* Bluetooth address of local bluetooth adapter */
            loc_addr.rc_bdaddr = *BDADDR_ANY;
            //loc_addr.rc_channel = RFCOMM_SERVER_PORT_NUM;	/* port number of local bluetooth adapter, here: 5 */
            loc_addr.rc_channel = (uint8_t) 1;


            if(bind(server_sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0) //accept incoming connections and reserve OS resources
            {
                perror("failed to bind");
                //exit(1);
            } else
            {
                bt_bound = true;
            }

            /* put socket into listening mode */
            listen(server_sock, 1);		/* backlog = 1, max length to which the queue pending connections for sockfd may grow */

            /* accept one connection */
            fcntl(server_sock, F_SETFL, O_NONBLOCK); //set socket listen and non-blocking
            client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &opt);	/* return new socket for connection with a client */

            ba2str( &client_addr.rc_bdaddr, buf );


		btpoll.fd = server_sock;
		btpoll.events = POLLIN;		//tell me when ready to read
		int num_events = poll(&btpoll, 1, -1); // wait forever, what means block?!

		if (num_events == 0) {
			printf("Poll timed out!\n");
			 } 
		else {
			 int pollin_happened = btpoll.revents & POLLIN;

			 if (pollin_happened) {
			 	//printf("File descriptor %d is ready to read\n", btpoll.fd);

				client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &opt);	/* return new socket for connection with a client */

					    ba2str( &client_addr.rc_bdaddr, buf );

				/* read data from the client */
				    memset(buf, 0, sizeof(buf));
				    //bytes_read = recv(client_sock, buf, sizeof(buf), 0);

				    if( fopen("/tmp/msg_recvd.txt", "w") == NULL) printf("Error opening/creating audio sample file!\n");
				    int openfd = open("/tmp/msg_recvd.txt", O_WRONLY);
				    if(openfd < 0) printf("Error opening audio sample file!\n");

				    //while(!(bytes_read = recv(client_sock, bt_buf, sizeof(bt_buf), 0)))
				    while( (bytes_read = read(client_sock, bt_buf,sizeof(bt_buf))) > 0 )
				    {
					  //qDebug() << "received "<< bt_buf << endl;
					  //printf("received: %c\n", bt_buf);

					  write(openfd, bt_buf, sizeof(bt_buf));

				    }
				    //bytes_read = recv(client_sock, bt_buf, sizeof(bt_buf), 0);
				    if( bytes_read == 0 ) {
					puts("no bytes received\n");
				    }

					close(openfd);

			 } 
		else {
			 	printf("Unexpected event occurred: %d\n", btpoll.revents);
			 }
		}


	//puts("bt_server finished!\n");

}


int main()
{


    std::thread t(bt_server);	//create and start bt_server
	std::string input;

	std::cout << "Please parse: ";
	std::cin >> input;

	std::cout << "The first letter of input was: " << input[0] << std::endl;

/*
    for (int j=0; j<15; j++)
    {
    	std::cout << "main thread" << std::endl;
	sleep(1);
    }
*/

	//t.detach();
	t.join();

    return 0;
}





