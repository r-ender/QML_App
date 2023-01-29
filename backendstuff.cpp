#include <backendstuff.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <bluetooth_wrapper.hpp>
//#include <bluetooth_recv.h>
//#include <time.h>
//#include <QThread>
//---test
#include <sys/socket.h>
#include <bluetooth.h>
#include <l2cap.h>
#include <fcntl.h>
#include <unistd.h>



//constructor
BackendStuff::BackendStuff(QObject *parent) : QObject(parent)
{

    tcp_status = false;
    tcpSocket = new QTcpSocket();

    host = "127.0.0.1";
    port = 1234;

    timeoutTimer = new QTimer();
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, &BackendStuff::connectionTimeout);

    //connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(gotError(QAbstractSocket::SocketError)));
    connect(tcpSocket, &QTcpSocket::disconnected, this, &BackendStuff::closeConnection);

    emit BluetoothClrChanged();
    BluetoothClr();

    //qDebug() << "Bluetooth msg recv true/false: " << bth_msg_recvd << "\n";

    emit Bth_msg_recvChanged();
    Bth_msg_recv();

    emit Bth_txtmsg_recvChanged();
    Bth_txtmsg_recv();

}

//call an api's verb with an argument
void BackendStuff::call(const QString &api, const QString &verb, const QJsonValue &arg, closure_t closure)
{

    QString callId = QString::number(m_nextCallId);
    m_closures[callId] = closure;

    static uint16_t upcount = 1;
    QJsonArray msg;
    msg.append(2);  //refers to Call
    //msg.append(callId);
    msg.append(upcount);
    upcount++;
    msg.append(api + "/" + verb);
    msg.append(arg);

    //m_nextCallId++;

    QJsonDocument value;
    value.setArray(msg);

    QByteArray AfbJson;
    AfbJson.append("\x81");             //Byte-Wörder für SET_OPCODE usw. und Array-Size
    AfbJson.append(value.toJson(QJsonDocument::Compact).size());
    AfbJson.append(value.toJson(QJsonDocument::Compact));
    tcpSocket->write(AfbJson);
}

void BackendStuff::gotError(QAbstractSocket::SocketError err)
{
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Connection was refused";
            break;
        case 1:
            strError = "Remote host closed the connection";
            break;
        case 2:
            strError = "Host address was not found";
            break;
        case 5:
            strError = "Connection timed out";
            break;
        default:
            strError = "Unknown error";
    }

    emit someError(strError);
}

void BackendStuff::connectClicked()
{
    connect2host();
}

void BackendStuff::sendClicked(QString msg)
{
    QByteArray arrBlock;
    /*
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << msg;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    */
    arrBlock = msg.toUtf8();
    tcpSocket->write(arrBlock);
}

//überladene Funktion von sendMessage mit mehreren möglichen Parametern
void BackendStuff::sendMessage(QString api, QString verb, QJsonValue arg)
{
    call(api,verb,QJsonValue(arg),nullptr);
}

void BackendStuff::sendMessage(QString api, QString verb)
{
    QJsonObject obj
    {
        {"index", m_UserInput}
    };

    call(api,verb,(QJsonValue)obj,nullptr);
}

void BackendStuff::bth_send()
{
    //qDebug() << "Output printmsg: " << printmsg << "   print_msg_recv: " << print_msg_recv();
    Bluetooth_Wrapper bt_wrapper;
    int s, status;

    s = socket(AF_BLUETOOTH,SOCK_SEQPACKET, BTPROTO_L2CAP); //L2CAP socket

    std::string bthadr = BluetoothAdr().toStdString();

    //std::cout << "User getter bluetooth addr: " << bthadr << std::endl;

    status = bt_wrapper.bt_connect_l2cap(s,&bthadr);

    std::string output = m_UserInput.toStdString(); //convert QString to std::string

    // send  message
    if( status == 0 ) {
        bt_wrapper.bt_write(s,&output[0],output.length()); //write-function requires void-pointer, hence the address of first element
    }

    if( status < 0 ){
        std::cout << "status: " << status << std::endl;
        std::perror("uh oh - bt connecting and sending not possible");
    }
    close(s);
}

void BackendStuff::bt_voice_send()
{
    QByteArray ba;

    if( fopen("/tmp/jabra_capture.wav", "r") == NULL)
    {
        qDebug() << "Error parsing audio file!\n";
        return;
    }

    Bluetooth_Wrapper bt_wrapper;
    int s, status;

    //s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	//domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM
    s = socket(AF_BLUETOOTH,SOCK_SEQPACKET, BTPROTO_L2CAP); //l2cap bt-voice send

    std::string bthadr = btaddr.toStdString();

    //status = bt_wrapper.bt_connect(s, (struct sockaddr *)&addr2, sizeof(addr2));	//connect to server
    status = bt_wrapper.bt_connect_l2cap(s,&bthadr);

    QFile f("/tmp/jabra_capture.wav");
    if(f.open(QIODevice::ReadOnly))
    {
        ba = f.readAll();
        //qDebug() << "read " << ba.size() << "bytes";
        f.close();
    }
    else
    {
        qDebug() << "Parsing jabra_capture failed!\n";
    }

    uint32_t original_balength=(uint32_t)ba.length();
    //const uint32_t mtu = 65535;
    const uint32_t mtu = 65000;

    //uint32_t prev_sent = 0;
    // send  message
    if( status == 0 ) {

        /*
        status = bt_wrapper.bt_write(s,ba, (uint32_t)ba.length());

        if(status > 0 && (uint32_t) status < original_balength )
        {
            prev_sent = status;
            uint32_t sendloops = (original_balength - prev_sent)/mtu;
            for(uint8_t z=0; z < sendloops; z++)
            {
                //status = bt_wrapper.bt_write(s,ba.mid(z*mtu,mtu),mtu);
                status = bt_wrapper.bt_write(s,ba.mid(prev_sent+z*mtu,mtu),mtu);
                if(status < 0) perror("sending gone wrong!\n");
            }

        }
        */

        uint32_t sendloops = original_balength/mtu;

        for(uint8_t z=0; z < sendloops; z++)
        {
            status = bt_wrapper.bt_write(s,ba.mid(z*mtu,mtu),mtu);
            if(status < 0) perror("sending gone wrong!\n");
        }
        //status = bt_wrapper.bt_write(s, "hello!", 6);
        //bt_wrapper.bt_write(s,&output[0],output.length()); //write-function requires void-pointer, hence the address of first element
        //while ( (uint32_t) bt_wrapper.bt_write(s,ba,ba.length() != 0)

        /*
        uint32_t bytessent = 0;
        uint32_t total_bytessent = 0;
        uint32_t strsize = 5;
        uint32_t leftover_balength = 0;
        do{
            //bytessent = (uint32_t) bt_wrapper.bt_write(s,ba,ba.length());
            strsize = (uint32_t)ba.length() - total_bytessent;
            bytessent = (uint32_t) bt_wrapper.bt_write(s,ba.mid(bytessent),strsize);
            total_bytessent += bytessent;
            leftover_balength = original_balength - total_bytessent;
            qDebug() << total_bytessent << " Bytes have been transmitted and " << leftover_balength << " Bytes are left over\n";
        }while ((bytessent > 2) && leftover_balength > 0);
        */
    }


    if( status < 0 ){
        std::cout << "status: " << status << std::endl;
        std::perror("uh oh - bt connecting or sending not possible");
    }
    close(s);

}


void BackendStuff::bt_server(void)
{
        //struct sockaddr_rc loc_addr;	/* local bluetooth adapter's info for rfcomm */
        struct sockaddr_l2 loc_addr,rem_addr;  //local and remot structs bt-devices
        char buf[1024] = { 0 };
        //char bt_buf[65534] = {0}; //max MTU for L2CAP Bluetooth
        char bt_buf[65000] = {0};
        int server_sock, client_sock;
        socklen_t opt = sizeof(rem_addr);

        //uint16_t mtu = 65534;
        uint16_t mtu = 65000;
        // allocate socket
        server_sock = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        // bind socket to port 0x1001 of the first available
        // bluetooth adapter
        loc_addr.l2_family = AF_BLUETOOTH;
        loc_addr.l2_bdaddr.b[0] = 0x00; loc_addr.l2_bdaddr.b[1] = 0x00; loc_addr.l2_bdaddr.b[2] = 0x00;
        loc_addr.l2_bdaddr.b[3] = 0x00; loc_addr.l2_bdaddr.b[4] = 0x00; loc_addr.l2_bdaddr.b[5] = 0x00;
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
        opts.omtu = opts.imtu = mtu;

        err = setsockopt(server_sock, SOL_L2CAP, L2CAP_OPTIONS, &opts, optlen );
        if(err != 0)
        {
            printf("Error for setsockoptions occured: %s!\n",strerror(errno));
        }

        struct pollfd btpoll;

        /*
        //Handy-test
        //client_addr.rc_bdaddr = (sockaddr_rc).rc_bdaddr "A4:6C:F1:08:96:B7";
        //client_addr.rc_bdaddr = (bdaddr_t){0xB7, 0x96, 0x08, 0xF1, 0x6C, 0xA4};

        //bacpy(&loc_addr.rc_bdaddr, BDADDR_ANY);		// Bluetooth address of local bluetooth adapter
        loc_addr.rc_bdaddr = *BDADDR_ANY;
        //loc_addr.rc_channel = RFCOMM_SERVER_PORT_NUM;	// port number of local bluetooth adapter
        loc_addr.rc_channel = (uint8_t) 1;
    */

        if(bind(server_sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
        {
            perror("failed to bind");
            //exit(1);
        } else
        {
            bt_bound = true;
        }

    while(true) //ein Thread läuft normal nur einmal durch, darum die while-Schleife
    {

        /* put socket into listening mode */
        listen(server_sock, 1);		/* backlog = 1, max length to which the queue pending connections for sockfd may grow */

        /* accept one connection */
        fcntl(server_sock, F_SETFL, O_NONBLOCK); //set socket listen and non-blocking
        client_sock = accept(server_sock, (struct sockaddr *)&rem_addr, &opt); // return new socket for connection with a client

        ba2str( &rem_addr.l2_bdaddr, buf );

        btpoll.fd = server_sock;
        btpoll.events = POLLIN;		//tell me when ready to read
        int num_events = poll(&btpoll, 1, -1); // wait forever, what means block?!

        if (num_events == 0) {
            printf("Poll timed out!\n");
             }
        else {
             int pollin_happened = btpoll.revents & POLLIN;

             if (pollin_happened) {

                client_sock = accept(server_sock, (struct sockaddr *)&rem_addr, &opt);	/* return new socket for connection with a client */

                ba2str( &rem_addr.l2_bdaddr, buf );

                /* read data from the client */
                memset(bt_buf, 0, sizeof(bt_buf));

                if( fopen("/tmp/msg_recvd.wav", "w") == NULL) printf("Error opening/creating audio sample file!\n");
                int openfd = open("/tmp/msg_recvd.wav", O_WRONLY);
                if(openfd < 0) printf("Error opening audio sample file!\n");

                while(read(client_sock, bt_buf,sizeof(bt_buf)) > 0 )
                    {
                        write(openfd, bt_buf, sizeof(bt_buf));
                    }

                /*
                while( (bytes_read = read(client_sock, bt_buf,sizeof(bt_buf))) > 0 )
                {
                  //write(openfd, bt_buf, sizeof(bt_buf));
                    write(openfd, bt_buf, bytes_read); //test
                }
                //bytes_read = recv(client_sock, bt_buf, sizeof(bt_buf), 0);
                if( bytes_read == 0 ) {
                    puts("no bytes received\n");
                } */

                close(openfd);

                //check if parsed file is audio(binary) or text:
                FILE * pFile;
                uint32_t n= 0;
                pFile = fopen("/tmp/msg_recvd.wav", "rb");
                if(pFile == NULL) perror("Error opening file!\n");
                else
                {
                    while(fgetc(pFile) != EOF) ++n;
                }

                fclose(pFile);

                uint32_t unequal_zero = 0;
                int check;
                std::ifstream parsed("/tmp/msg_recvd.wav");
                for(uint32_t i = 0; i<n; i++)
                {
                    check = (int) parsed.get(); //ASCII text if file contains only the decimal bytes 9–13, 32–126
                    if((check >= 9 && check <= 13) || (check >= 32 && check <=126)) unequal_zero++;
                }

                if(unequal_zero > 4000)
                {
                    //qDebug() << "Received message is audio/binary!\n";
                    // < mache roten Punkt auf voice-option >
                    bth_msg_recvd = true;
                }
                else
                {
                    //qDebug() << "Received message is text!\n";
                    // < mache roten punkt auf type-option >
                    bth_txtmsg_recvd = true;

                    QString temp_printmsg;
                    QFile file("/tmp/msg_recvd.wav");
                    if(file.open(QIODevice::ReadOnly)) {
                         QTextStream in(&file);
                         while(!in.atEnd())
                             temp_printmsg = in.readAll();
                    }
                    file.close();

                    printmsg.clear();   //String clearen bevor neuer Inhalt angehängt wird
                    for(uint32_t x=0; x < (uint32_t)temp_printmsg.length(); x++)
                    {
                        if(temp_printmsg[x] != 0x00) printmsg.append(temp_printmsg[x]);
                    }
                }

             }
        else {
                printf("Unexpected event occurred: %d\n", btpoll.revents);
             }
        }
    }

}

void BackendStuff::bt_recv_onoff(bool bt_on)
{
    switch (bt_on)
    {
        case true:
            if (get_btstate())
            {
                std::cout << "Bluetooth already bound - turn off and on again" << std::endl;
            }
            else
            {
                std::cout << "bluetooth recv thread started" << std::endl;
                set_btstate(true);

                btrcv = std::thread{&BackendStuff::bt_server, this};
                //std::cout << "After create, get btrcv id? --> " << btrcv.get_id() << std::endl;
                btrcv.detach();
            }
            break;

        case false:
            if (get_btstate())
            {
                std::cout << "Close bluetooth-socket" << std::endl;

                //totally destory socket
                int enable = 1;
                setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
                setsockopt(client_sock,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));

                /* close connection */
                close(client_sock);
                close(server_sock);

                set_btstate(false);
            }
            else
            {
                std::cout << "bt is already off" << std::endl;
            }
            break;
        }

}

void BackendStuff::bluetoothStart()
{
    bluetoothStarted();
}

void BackendStuff::speechstart()
{
    speechStarted();
}

void BackendStuff::speechfinish()
{
    speechFinished();
}

void BackendStuff::listenFinish()
{
    listenFinished();
}

void BackendStuff::listenStart()
{
    listenStarted();
}

void BackendStuff::capture_voice()
{
      voice_flag = false;
      int rc, openfd, size, dir;
      long loops;
      snd_pcm_t *handle;
      snd_pcm_hw_params_t *params;
      unsigned int val;
      snd_pcm_uframes_t frames;
      char *buffer;

      rc = snd_pcm_open(&handle, "hw:1,0", SND_PCM_STREAM_CAPTURE, 0);

      //für CloudLab andere Headset-Schnittstelle
      //rc = snd_pcm_open(&handle, "hw:2,0", SND_PCM_STREAM_CAPTURE, 0);

      if (rc < 0) {
        fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
        //exit(1);
        return;
      }

      if( fopen("/tmp/jabra_capture.wav", "w") == NULL) printf("Error opening/creating audio sample file!\n");
      openfd = open("/tmp/jabra_capture.wav", O_WRONLY);
      if(openfd < 0) printf("Error opening audio sample file!\n");

        /* Allocate a hardware parameters object. */
        snd_pcm_hw_params_alloca(&params);

        /* Fill it in with default values. */
        snd_pcm_hw_params_any(handle, params);

        /* Set the desired hardware parameters. */
        /* Interleaved mode */
        snd_pcm_hw_params_set_access(handle, params,SND_PCM_ACCESS_RW_INTERLEAVED);

        /* Signed 16-bit little-endian format */
        snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

        /* Two channels (stereo) */
        //snd_pcm_hw_params_set_channels(handle, params, 2);
        //test mono, 1 channel:
        snd_pcm_hw_params_set_channels(handle, params, 1);

        /* 44100 bits/second sampling rate (CD quality) */
        val = 44100;
        snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

        /* Set period size to 32 frames. */
        //frames = 32;
        frames = 4410; // = 1 * period-size
        snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

        /* Write the parameters to the driver */
        rc = snd_pcm_hw_params(handle, params);
        if (rc < 0) {
           fprintf(stderr,"unable to set hw parameters: %s\n",
                   snd_strerror(rc));
           exit(1);
        }

        /* Use a buffer large enough to hold one period */
        snd_pcm_hw_params_get_period_size(params, &frames, &dir);
        //size = frames * 4; /* 2 bytes/sample, 2 channels */
        size = frames * 2;
        buffer = (char *) malloc(size);
        //qDebug() << "buffer-size: " << size << "\n";        //8820

        /* We want to loop for 5 seconds */
        snd_pcm_hw_params_get_period_time(params, &val, &dir);
        //loops = 5000000 / val; //5.000.000 / 44100 = ~120 loops
        loops = 4000000 / val; //5.000.000 / 44100 = ~120 loops


        while (loops > 0  && voice_flag == false) {
          loops--;
          rc = snd_pcm_readi(handle, buffer, frames);   //read interleaved frames from pcm
          if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
          }
          else if ( rc == -ESTRPIPE){
              fprintf(stderr, "a suspend event occurred (stream is suspended and waiting for an application recovery)\n");
          }
          else if ( rc == -EBADFD){
              fprintf(stderr, "PCM is not in the right state (SND_PCM_STATE_PREPARED or SND_PCM_STATE_RUNNING)\n");
          }
          else if (rc < 0) {
            fprintf(stderr,"error from read: %s\n",snd_strerror(rc));
            //rc = snd_pcm_open(&handle, "hw:2,0", SND_PCM_STREAM_CAPTURE, 0);

          } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
          }

          rc = write(openfd, buffer, size);
          if (rc != size)
            fprintf(stderr,"short write: wrote %d bytes\n", rc);
        }

        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(buffer);
        close(openfd);
}

void BackendStuff::listen_msg()
{
    long loops;
    int rc, openfd, size, dir;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    snd_pcm_uframes_t frames;
    char *buffer;

    QFile file("/tmp/msg_recvd.wav");
    QString content;
    if(file.open(QIODevice::ReadOnly)) {
         QTextStream in(&file);
         while(!in.atEnd())
             content = in.readAll();
    }
    file.close();

    if( fopen("/tmp/msg_recvd.wav", "r") == NULL) {
        qDebug() << "Error opening/creating audio sample file!\n";
        //exit(1);
        return;
    }

    openfd = open("/tmp/msg_recvd.wav", O_RDONLY);
    if(openfd < 0) printf("Error opening audio sample file!\n");

    //opens the default PCM device and set the access mode to PLAYBACK.
    //This function returns a handle in the first function argument that is used in subsequent calls to manipulate the PCM stream
    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
      fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
      exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */
    /* Interleaved mode = left and right channel information is stored alternately within a frame; this is called interleaved mode */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    /* One channel (mono) */
    snd_pcm_hw_params_set_channels(handle, params, 1);

    /* 44100 bits/second sampling rate (CD quality)
        note: sound hardware is not always able to support every sampling rate exactly.
        We use the function snd_pcm_hw_params_set_rate_near to request the nearest supported sampling rate. */
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params,&val, &dir);

    //The buffer can be quite large, and transferring it at once could cause latency. Hence ALSA splits the buffer up into a series of periods and transfers the data in units of a period.
    //A period stores frames, each of which contains the samples captured at one point in time. For a stereo device, the frame would contain samples for two channels.
    /* Set period size to 4410 frames. */
    frames = 4410; // = 1 period-size

    /* Params not active until we call the function snd_pcm_hw_params!! Write the parameters to the driver! */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
      fprintf(stderr,"unable to set hw parameters: %s\n", snd_strerror(rc));
      exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    //size = frames * 2 --> 2 bytes/sample, 1 channel
    size = frames * 2;
    buffer = (char *) malloc(size);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    /* 5 seconds in microseconds divided by period time */
    loops = 4000000 / val;

    while (loops > 0) {
      loops--;

      rc = read(openfd, buffer, size);      //falls openfd=0 --> read from keyboard/micophone
      if (rc == 0) {
        fprintf(stderr, "end of file on input\n");
        break;
      } else if (rc != size) {
        fprintf(stderr, "short read: read %d bytes\n", rc);
      }
      rc = snd_pcm_writei(handle, buffer, frames);
      if (rc == -EPIPE) {
        /* EPIPE means underrun */
        fprintf(stderr, "underrun occurred\n");
        snd_pcm_prepare(handle);
      } else if (rc < 0) {
        fprintf(stderr,"error from write: %s\n", snd_strerror(rc));
      }  else if (rc != (int)frames) {
        fprintf(stderr, "short write, write %d frames\n", rc);
      }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
    close(openfd);

    bth_msg_recvd = false;
}

void BackendStuff::disconnectClicked()
{
    closeConnection();
}

//here the connection is executed via QAbstractSocket
void BackendStuff::connect2host()
{
    timeoutTimer->start(3000);
    tcpSocket->connectToHost(host, port);
    connect(tcpSocket, &QTcpSocket::connected, this, &BackendStuff::connected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &BackendStuff::readyRead);
}

void BackendStuff::connectionTimeout()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        tcpSocket->abort();
        emit tcpSocket->error(QAbstractSocket::SocketTimeoutError);
    }
}

void BackendStuff::connected()
{
    tcp_status = true;
    emit statusChanged2(tcp_status);
}

void BackendStuff::readyRead()
{
   if(getBuffer() != 0) emit bufferChanged();
}

void BackendStuff::closeConnection()
{
    timeoutTimer->stop();

    disconnect(tcpSocket, &QTcpSocket::connected, 0, 0);
    disconnect(tcpSocket, &QTcpSocket::readyRead, 0, 0);

    bool shouldEmit = false;
    switch (tcpSocket->state())
    {
        case 0:
            tcpSocket->disconnectFromHost();
            shouldEmit = true;
            break;
        case 2:
            tcpSocket->abort();
            shouldEmit = true;
            break;
        default:
            tcpSocket->abort();
    }

    if (shouldEmit)
    {
        tcp_status = false;
        ws_status = false;
        emit statusChanged2(tcp_status);
    }
}
