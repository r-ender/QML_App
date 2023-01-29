CC = gcc
LNK = gcc

INC=-I/home/cloud/agl/sdk/sysroots/corei7-64-agl-linux/usr/include/bluetooth -I/home/cloud/agl/sdk/sysroots/corei7-64-agl-linux/usr/include
CFLAGS = -Wall -g3 -c $(INC)
SOUNDCFLAGS = -Wall -g3 -c --sysroot=$(SYSROOT) $(INC)
LIBDIR = -L/usr/lib/x86_64-linux-gnu/
SOUNDLIBDIR = -L/home/cloud/agl/sdk/sysroots/corei7-64-agl-linux/usr/lib/
LIB = -lbluetooth
SOUNDLIB = -lasound

sendvoicemsg: sendvoicemsg.o
	$(LNK) $(LIBDIR) $(SOUNDLIBDIR) sendvoicemsg.o -o sendvoicemsg $(LIB) $(SOUNDLIB) $(INC)

sendvoicemsg.o: sendvoicemsg.c
	$(CC) ${SOUNDCFLAGS} sendvoicemsg.c


listenmsg: listenmsg.o
	$(LNK) $(SOUNDLIBDIR) listenmsg.o -o listenmsg $(SOUNDLIB) $(INC)

listenmsg.o: listenmsg.c
	$(CC) ${SOUNDCFLAGS} listenmsg.c

min_sendcode: min_sendcode.o
	$(LNK) $(LIBDIR) min_sendcode.o -o min_sendcode $(LIB) $(INC)

min_sendcode_raspi: min_sendcode_raspi.o
	$(LNK) $(LIBDIR) min_sendcode_raspi.o -o min_sendcode_raspi $(LIB) $(INC)

compiled_bt: compiled_bt.o
	$(LNK) $(LIBDIR) compiled_bt.o -o compiled_bt $(LIB) $(INC)

service_discovery: service_discovery.o
	$(LNK) $(LIBDIR) service_discovery.o -o service_discovery $(LIB) $(INC)
	
service_discovery.o: service_discovery.c
	$(CC) ${CFLAGS} service_discovery.c

l2cap: l2cap_server l2cap_client

l2cap_server: l2cap_server.o
	$(LNK) $(LIBDIR) l2cap_server.o -o l2cap_server $(LIB) $(INC)
	
l2cap_server.o: l2cap_server.c
	$(CC) ${CFLAGS} l2cap_server.c
	
l2cap_client: l2cap_client.o
	$(LNK) $(LIBDIR) l2cap_client.o -o l2cap_client $(LIB) $(INC)
	
l2cap_client.o: l2cap_client.c
	$(CC) ${CFLAGS} l2cap_client.c
		

rfcomm: rfcomm_server rfcomm_client

simplescan: simplescan.o
	$(LNK) $(LIBDIR) simplescan.o -o simplescan $(LIB) $(INC)

rfcomm_server: rfcomm_server.o
	$(LNK) $(LIBDIR) rfcomm_server.o -o rfcomm_server $(LIB) $(INC)
	
rfcomm_client : rfcomm_client.o
	$(LNK) $(LIBDIR) rfcomm_client.o -o rfcomm_client $(LIB) $(INC)

simplescan.o: simplescan.c
	$(CC) ${CFLAGS} simplescan.c
	
rfcomm_server.o: rfcomm_server.c
	$(CC) ${CFLAGS} rfcomm_server.c
	
rfcomm_client.o: rfcomm_client.c
	$(CC) ${CFLAGS} rfcomm_client.c 
	
compiled_bt.o: compiled_bt.c
	$(CC) ${CFLAGS} compiled_bt.c	
	
min_sendcode.o: min_sendcode.c
	$(CC) ${CFLAGS} min_sendcode.c
	
min_sendcode_raspi.o: min_sendcode_raspi.o
	$(CC) ${CFLAGS} min_sendcode_raspi.c	
clean:
	rm -rf simplescan simplescan.o rfcomm_server rfcomm_client rfcomm_server.o rfcomm_client.o compiled_bt.o compiled_bt

#RFCOMM:

#L2CAP:
