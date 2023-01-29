#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QString>
#include <QDataStream>
#include <backendstuff.h>


uint8_t rfcomm_bth_portnr = 0;
bool bt_bound2 = false;
bool bth_msg_recvd = false;
bool bth_txtmsg_recvd = false;
QString btaddr, printmsg;

void set_btstate(bool x)
{
    bt_bound2 = x;
}

bool get_btstate()
{
    return bt_bound2;
}

QString translate_clr()
{
    qDebug() << "hallo from translate_clr\n";
    if (bt_bound2)
    {
        return "green";
    }
    else
    {
       return "red";
    }
}



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<BackendStuff>("io.qt.BackendStuff", 1, 0, "BackendStuff");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/startingpage.qml")));
    if (engine.rootObjects().isEmpty()) { return -1; }

    return app.exec();
}

//#include <main.moc>
