#include "http_server.hpp"

#include <QByteArray>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>

HttpServer::HttpServer(QObject* parent) :
	QObject(parent)
{
	if (m_server.listen(QHostAddress::LocalHost, 8000))
	{
		connect(&m_server, &QTcpServer::newConnection, this, &HttpServer::OnNewConnection);
	}
	else
	{
		qCritical().noquote() << QString("HttpServer: unable to start the server: %1").arg(m_server.errorString());
		m_server.close();
	}
}

void HttpServer::OnNewConnection()
{
	m_pSockets.append(m_server.nextPendingConnection());
	connect(m_pSockets.back(), &QTcpSocket::readyRead, this, &HttpServer::OnNewData);
}

void HttpServer::OnNewData()
{
	for (QTcpSocket* socket : m_pSockets)
	{
		qDebug() << "HttpServer::OnNewData: data size = " << socket->bytesAvailable();
		qDebug() << "HttpServer::OnNewData: data = " << socket->readAll();
	}
}
