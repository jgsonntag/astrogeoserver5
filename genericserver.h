#ifndef GENERICSERVER_H
#define GENERICSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QMessageBox>
#include <QString>


class GenericServer : public QObject
{
    Q_OBJECT

public:
    explicit GenericServer(QObject *parent = 0,quint16 port=0);
    void sendText(QString string);
    int numActiveConnections();

signals:

public slots:
    void newConnection();
    void clientDisconnected();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket*> clients;
    QByteArray qbatemp;
    int nconns;
    QString stemp;

};

#endif // GENERICSERVER_H
