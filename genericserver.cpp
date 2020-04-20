#include "genericserver.h"

GenericServer::GenericServer(QObject *parent,quint16 port) : QObject(parent)
{

    // Create a server
    server = new QTcpServer(this);
    nconns = 0;

    // Emit signal when new user connects
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if (!server->listen(QHostAddress::Any,port))
    {
        QMessageBox msgBox;
        stemp.sprintf("Cannot start server on port %d",port);
        msgBox.setText(stemp);
        msgBox.exec();
        exit(-1);
    }

}


void GenericServer::newConnection()
{

    socket = server->nextPendingConnection();
    clients.append(socket);
    connect(socket,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
    ++nconns;

}


void GenericServer::sendText(QString string)
{

    qbatemp = string.toLatin1();

    QListIterator<QTcpSocket*> i(clients);
    while (i.hasNext())
    {
        socket = i.next();
        socket->write(qbatemp);
        socket->flush();
    }

}


void GenericServer::clientDisconnected()
{
    --nconns;
}


int GenericServer::numActiveConnections()
{
    return(nconns);
}
