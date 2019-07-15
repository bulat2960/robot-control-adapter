﻿#include "planner.h"

Planner::Planner(QString unitName, QString rcaIp, quint16 rcaPort)
{
    // Set RCA ip and port
    this->rcaIp = rcaIp;
    this->rcaPort = rcaPort;

    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, &QTcpSocket::connected, this, &Planner::sendName);
}

bool Planner::connectToServer()
{
    // Try to connect to server
    socket->connectToHost(rcaIp, rcaPort);
    if (socket->waitForConnected())
    {
        return true;
    }
    return false;
}

void Planner::sendName()
{
    // Check the connection and send our name
    qDebug() << "Planner - connection established";
    socket->write("p");
    socket->waitForBytesWritten();
}

void Planner::sendMsg(QString msg)
{
    QByteArray arr;
    arr.append(msg);
    socket->write(arr);
    socket->waitForBytesWritten();
}

bool Planner::disconnectFromServer()
{
    qDebug() << "Planner - disconnect";
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected())
    {
        return true;
    }
    return false;
}

bool Planner::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

bool Planner::isDisconnected() const
{
    return socket->state() == QAbstractSocket::UnconnectedState;
}
