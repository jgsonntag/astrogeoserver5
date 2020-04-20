#ifndef GENERICCLIENT_H
#define GENERICCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QString>


class GenericClient : public QObject
{
    Q_OBJECT

public:
    explicit GenericClient(QObject *parent=0,const QString &host=0,quint16 port=0);

signals:
    void newLine(QString);

private:
    QTcpSocket *myTcpSocket;
    char line[200];
    QString lineQstring;
    bool connected;

public slots:
    bool slotConnectStatus();

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadLine();

};

#endif // GENERICCLIENT_H
