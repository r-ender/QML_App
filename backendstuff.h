#ifndef BACKENDSTUFF_H
#define BACKENDSTUFF_H

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
#include <QQmlProperty>
#include <QQuickItem>
//#include <QThread>

#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API

extern bool bth_msg_recvd;
extern bool bth_txtmsg_recvd;
extern QString btaddr;
extern QString printmsg;
extern bool get_btstate(void);
extern void set_btstate(bool);
extern uint8_t rfcomm_bth_portnr;
//extern QString translate_clr(void);

/*
class bluetooth_recv
{
    public:
        //explicit bluetooth_recv(QObject *parent = 0);   //explicit avoids implicit conversion of type
        bluetooth_recv();
        //void run();
        //void abort();
        void bt_testloop();

        int client_sock, server_sock;
        //bluetooth_recv();

    public slots:
        //void bt_doWork();

    signals:
        //void start_bt_testloop();
        //void bt_finished();

    private:
        bool _abort;    //Process is aborted when _abort true
        bool _working;  //true when Worker is doing work
        //QMutex mutex;   //Protects access to _abort
};
*/

class BackendStuff : public QObject //, public bluetooth_recv
{
    Q_OBJECT
    Q_PROPERTY(QString receiveBuffer READ getBuffer NOTIFY bufferChanged)
    Q_PROPERTY(QString UserInput READ UserInput WRITE setUserInput NOTIFY UserInputChanged)
    Q_PROPERTY(QString BluetoothAdr READ BluetoothAdr WRITE setBluetoothAdr NOTIFY BluetoothAdrChanged)
    Q_PROPERTY(QString print_msg_recv READ print_msg_recv)
    Q_PROPERTY(QString BluetoothClr READ BluetoothClr NOTIFY BluetoothClrChanged)
    Q_PROPERTY(bool Bth_msg_recv READ Bth_msg_recv NOTIFY Bth_msg_recvChanged)
    Q_PROPERTY(bool Bth_txtmsg_recv READ Bth_txtmsg_recv NOTIFY Bth_txtmsg_recvChanged)
    Q_PROPERTY(bool Bth_txtmsg_clear READ Bth_txtmsg_clear)

public:
    BackendStuff(QObject *parent = nullptr);
    //~BackendStuff();      //keinen Destruktor benutzen, sonst stürzt App ab, wenn ich page zurückgehe!
    QTcpSocket *tcpSocket;

    void bt_server(void);

    QString getBuffer(){
        //bytesAvailable(): Returns the number of incoming bytes that are waiting to be read.
        //'Sec-WebSocket-Accept' = 0x 5365 632d 5765 6253 6f63 6b65 742d 4163 6365 7074
        if (tcpSocket->bytesAvailable() > (quint16)sizeof(quint16))
            {
                 receive_dump.clear();
                 receive_dump.append(tcpSocket->readAll());

                 //checks whether websocket-connection is established
                 if(ws_status != true)
                 {
                     QRegExp cmp("\\b(Sec-WebSocket-Accept)\\b");
                     if(cmp.indexIn((QString)receive_dump) != -1)
                         ws_status = true;
                 }
                 else
                 {
                     int pos=0;
                     while(receive_dump[pos] != '[') pos++;     //find first match of '[' in ByteArray
                     receive_dump.remove(0,pos);    //removes unused preamble
                 }
        }

        m_receivebuffer = (QString)receive_dump;
        return m_receivebuffer;
    }

    void setUserInput(const QString &a)
    {
        if (a != NULL)
        {
            if (a != m_UserInput)
            {
                m_UserInput = a;
                emit UserInputChanged();
            }
            //qDebug() << "setUserInput: " << m_UserInput;

            QString path = QString("/tmp/motalkfile.txt");  //set File-Path
            QFile file(path);

            //write QString m_UserInput into file in overwrite mode
            if(!file.open(QIODevice::WriteOnly)){
                    file.close();
                } else {
                    QTextStream out(&file); out << m_UserInput;
                    file.close();
                }
        }
    }

    QString UserInput() const {
            qDebug() << "UserInput getter function";
            return m_UserInput;
    }

    void setBluetoothAdr(const QString &a)
    {
        if (a != NULL)
        {
            if (a != m_BluetoothAdr)
            {
                m_BluetoothAdr = a;
                emit BluetoothAdrChanged();
            }
            qDebug() << "setBluetoothAdr: " << m_BluetoothAdr;

            //save Bluetooth-Address in global variable
            btaddr = m_BluetoothAdr;

            /*
            //write Bluetooth-Adr in file to make it persistent
            QString path = QString("/tmp/motalk_bt-address.txt"); //set filepath
            QFile file(path);

            //write m_BluetoothAdr into file with overwrite-mode
            if(!file.open(QIODevice::WriteOnly)){
                file.close();
            } else{
                QTextStream out(&file); out << m_BluetoothAdr;
                file.close();
            } */
        }
    }

    bool Bth_txtmsg_recv() const {
        if (bth_txtmsg_recvd == true)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool Bth_txtmsg_clear() const{
        bth_txtmsg_recvd = false;
        return (false);
    }

    bool Bth_msg_recv() const {
        if( bth_msg_recvd == true)
        {
            return(true);
        }
        else
        {
            return (false);
        }
    }

    QString BluetoothClr() const {
        if(get_btstate() == true)
        {
            return "green";
        }
        else
        {
            return "red";
        }
    }

    QString print_msg_recv() const {
        return printmsg;
    }

    QString BluetoothAdr() const {
        return btaddr;
    }

    //from QAfbWebSocketClient
    using closure_t = std::function<void(bool, const QJsonValue&)>;
    void call(const QString& api, const QString& verb, const QJsonValue& arg = QJsonValue(), closure_t closure = nullptr);

    //signals can trigger qml from c++
signals:
    void someError(QString err);
    void statusChanged2(bool);
    void bufferChanged();
    void UserInputChanged();
    void BluetoothAdrChanged();
    void BluetoothClrChanged();
    void speechFinished();
    void speechStarted();
    void listenFinished();
    void listenStarted();
    void bluetoothStarted();
    void Bth_msg_recvChanged();
    void Bth_txtmsg_recvChanged();

    //slots make c++ methods callable from qml
public slots:
    void gotError(QAbstractSocket::SocketError err);
    void sendClicked(QString msg);
    //überladene Funktionen von sendMessage mit mehreren möglichen Parametern
    void sendMessage(QString api, QString verb);
    void sendMessage(QString api, QString verb, QJsonValue arg);
    void connectClicked();
    void disconnectClicked();
    void closeConnection();
    void connect2host();
    void bth_send();   // former: bluetooth_test
    void bt_voice_send();
    void capture_voice();
    void listen_msg();
    //void print_msg();
    void speechstart();   // --> former teststart
    void speechfinish();  // --> former testfinish
    void listenFinish();
    void listenStart();
    void bt_recv_onoff(bool);
    void bluetoothStart();

private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

private:    //note: private variablen könne nicht vererbt werden
    //from QAfbWebSocketClient
    int m_nextCallId, port;
    bool tcp_status, ws_status = false;
    bool voice_flag, bt_bound = false, bt_bound2 = false;
    QMap<QString, closure_t> m_closures;
    QString host, m_receivebuffer, m_UserInput, m_BluetoothAdr;
    QTimer *timeoutTimer;
    QByteArray receive_dump;
    int server_sock, client_sock;
    std::thread btrcv;
};


#endif // BACKENDSTUFF_H
