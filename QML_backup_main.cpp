#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <fstream>
#include <iostream>

using namespace std;

class BackendStuff : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString receiveBuffer READ getBuffer NOTIFY bufferChanged)
    Q_PROPERTY(QString UserInput READ UserInput WRITE setUserInput NOTIFY UserInputChanged)

public:
    explicit BackendStuff(QObject *parent = nullptr);
    QTcpSocket *tcpSocket;


    QString getBuffer(){
        qDebug() << "ws_status: " << ws_status << endl;
        //bytesAvailable(): Returns the number of incoming bytes that are waiting to be read.
        //'Sec-WebSocket-Accept' = 0x 5365 632d 5765 6253 6f63 6b65 742d 4163 6365 7074
        if (tcpSocket->bytesAvailable() > (quint16)sizeof(quint16))
            {
                 receive_dump.clear();
                 receive_dump.append(tcpSocket->readAll());
                 qDebug() << "receive_dump after tcp.append: " << receive_dump << endl;

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
                     qDebug() << "first [ - match at position: " << pos << endl;
                     receive_dump.remove(0,pos);    //removes unused preamble
                 }

                //m_receivebuffer = ((QString)receive_dump).remove(0,2);
                qDebug() << "receive_dump: " << receive_dump << endl;
        }

        m_receivebuffer = (QString)receive_dump;
        qDebug() << "m_receivebuffer " << m_receivebuffer << endl;
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
            qDebug() << "setUserInput: " << m_UserInput;

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
            qDebug() << "UserInput";
            return m_UserInput;
    }

    //from QAfbWebSocketClient
    using closure_t = std::function<void(bool, const QJsonValue&)>;
    void call(const QString& api, const QString& verb, const QJsonValue& arg = QJsonValue(), closure_t closure = nullptr);

signals:
    void someError(QString err);
    void statusChanged2(bool);
    void hasReadSome(QString msg);
    void bufferChanged();
    void UserInputChanged();

public slots:
    void gotError(QAbstractSocket::SocketError err);
    void sendClicked(QString msg);
    //überladene Funktion von sendMessage mit mehreren möglichen Parametern
    void sendMessage(QString api, QString verb);
    void sendMessage(QString api, QString verb, QJsonValue arg);
    void connectClicked();
    void disconnectClicked();
    void closeConnection();
    void connect2host();

private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

private:
    //from QAfbWebSocketClient
    int m_nextCallId, port;
    QMap<QString, closure_t> m_closures;
    QString host, m_receivebuffer, m_UserInput;
    bool tcp_status, ws_status = false;
    QTimer *timeoutTimer;
    QByteArray receive_dump;
};

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

    //connect(client, &BackendStuff::statusChanged, this, &Backend::setStatus);
    //connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(gotError(QAbstractSocket::SocketError)));
    connect(tcpSocket, &QTcpSocket::disconnected, this, &BackendStuff::closeConnection);
}

//call an api's verb with an argument
void BackendStuff::call(const QString &api, const QString &verb, const QJsonValue &arg, closure_t closure)
{

    QString callId = QString::number(m_nextCallId);
    m_closures[callId] = closure;

    QJsonArray msg;
    msg.append(2);  //refers to Call
    //msg.append(callId);
    msg.append("999999");
    msg.append(api + "/" + verb);
    msg.append(arg);

    m_nextCallId++;

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
    qDebug << "arg: " << arg << " QJsonValue(arg): " << QJsonValue(arg) << endl;
    call(api,verb,QJsonValue(arg),nullptr);
}

void BackendStuff::sendMessage(QString api, QString verb)
{

    QString key = "\"index\"";  //key
    QString value = m_UserInput;    //value
    //QString user_arg = "{\"index\":\"" + m_UserInput + "\"";
    //qDebug() << "user_arg: " << user_arg << endl;
    QJsonValue arg = key + ":" + value;
    call(api,verb,arg,nullptr);
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
    //qDebug() << tcpSocket->state();
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


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<BackendStuff>("io.qt.BackendStuff", 1, 0, "BackendStuff");

    QQmlApplicationEngine engine;


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) { return -1; }

    return app.exec();
}

#include <main.moc>
