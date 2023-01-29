#ifndef BLUETOOTH_WRAPPER_HPP
#define BLUETOOTH_WRAPPER_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth.h>
#include <rfcomm.h>
#include <iostream>
#include <string>
#include <l2cap.h>

class Bluetooth_Wrapper{

public:
    Bluetooth_Wrapper() { /* std::cout << "Neues Objekt von Bluetooth_Wrapper" << std::endl; */}
    int test_2(int &fd, struct sockaddr_l2 &addr);
    //int bt_connect(int fd, const sockaddr *addr, socklen_t len);
    //int bt_write(int fd, const void *buf, size_t n);
    int bt_write(int &fd, const void *buf, size_t n);
    int bt_connect(int fd);
    int bt_connect_l2cap(int &fd, std::string *bt_addr);
};



#endif // BLUETOOTH_WRAPPER_HPP
