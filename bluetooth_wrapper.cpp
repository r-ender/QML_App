#include <bluetooth_wrapper.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>
//#include <l2cap.h>

int Bluetooth_Wrapper::test_2(int &fd, struct sockaddr_l2 &addr)
{
    //struct sockaddr_l2 addr = { 0 };
        //int s, status;
        int status;

        // allocate a socket
        //s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        // set the connection parameters (who to connect to)
        //addr.l2_family = AF_BLUETOOTH;
        //addr.l2_psm = htobs(0x1001);
        //str2ba("CC:3D:82:39:9B:61", &addr.l2_bdaddr );

        uint32_t mtu = 65534; //Maximum transmission unit for L2CAP

        //erhöhen der MTU ?!
        struct l2cap_options {
            uint16_t omtu;
            uint16_t imtu;
            uint16_t flush_to;
            uint8_t mode;
        };
        struct l2cap_options opts;

        socklen_t optlen = sizeof(opts);
        int err;
        //int optlen = sizeof(opts), err;

        err = getsockopt(fd, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen);
        if(err == 0)
        {
            printf("omtu: %d, imtu: %d\n", opts.omtu, opts.imtu);
            opts.omtu = opts.imtu = mtu;
        }
        else
        {
            printf("getsocket error: %s!\n", strerror(errno));
        }

        err = setsockopt(fd, SOL_L2CAP, L2CAP_OPTIONS, &opts, optlen);
        if(err == 0)
        {
            getsockopt(fd, SOL_L2CAP, L2CAP_OPTIONS, &opts, &optlen);
            printf("omtu: %d, imtu: %d\n", opts.omtu, opts.imtu);
        }
        else
        {
            printf("setsocket error: %s!\n", strerror(errno));
        }


        // connect to server
        status = connect(fd, (struct sockaddr *)&addr, sizeof(addr));

        // send a message
        if( status == 0 ) {
            status = write(fd, "kalla fofm gort-pc", 18);
        }

        if( status < 0 ) perror("uh oh");

        close(fd);

        return 0;
}

/*
int Bluetooth_Wrapper::bt_connect(int fd, const sockaddr *addr, socklen_t len)
{
      ---test
    //return connect(fd, (struct sockaddr *)&addr, len);
    struct sockaddr_rc addr2;

        addr2.rc_family = AF_BLUETOOTH;
        addr2.rc_channel = (uint8_t) 1;
        str2ba("CC:3D:82:39:9B:61", &addr2.rc_bdaddr );	//für Cloud-Lab

        return (connect(fd, (struct sockaddr *)&addr2, sizeof(addr2)));	//connect to server
          ----test end

    return connect(fd, (struct sockaddr *)&addr, sizeof(addr));
} */

int Bluetooth_Wrapper::bt_connect(int fd)
{
    struct sockaddr_rc addr2;

    addr2.rc_family = AF_BLUETOOTH;
    addr2.rc_channel = (uint8_t) 1;
    str2ba("CC:3D:82:39:9B:61", &addr2.rc_bdaddr );	//für Cloud-Lab

    return (connect(fd, (struct sockaddr *)&addr2, sizeof(addr2)));	//connect to server
}

int Bluetooth_Wrapper::bt_connect_l2cap(int &fd, std::string *bt_addr)
{
    //L2CAP socket struct
    struct sockaddr_l2 addr;
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(0x1001);
    addr.l2_cid = 0x00;

    //convert std::string to char-array
    char btaddr[bt_addr->length()+1];
    strcpy(btaddr,bt_addr->c_str());

    //std::cout << "converted bt-Adr.: " << btaddr << std::endl;

    str2ba(btaddr, &addr.l2_bdaddr);

    //const uint32_t mtu = 65535;
    const uint32_t mtu = 65000;

    //erhöhen der MTU
    struct l2cap_options{
        uint16_t omtu;
        uint16_t imtu;
        uint16_t flush_to;
        uint8_t mode;
    };
    struct l2cap_options opts;

    socklen_t optlen = sizeof(opts);
    int err;

    opts.omtu = opts.imtu = mtu;
    opts.flush_to = 65535;
    opts.mode = 0;

    err = setsockopt(fd, SOL_L2CAP, L2CAP_OPTIONS, &opts, optlen);
    if(err != 0)
    {
        printf("setSocketError: %s!\n", strerror(errno));
    }

    return (connect(fd, (struct sockaddr *)&addr, sizeof(addr)));	//connect to server
}

int Bluetooth_Wrapper::bt_write(int &fd, const void *buf, size_t n)
{
    return (write(fd,buf, n));
}
