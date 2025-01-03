#include "http_server.hpp"

#include <QByteArray>
#include <QFile>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

const QString g_responseTemplate = QStringLiteral("HTTP/1.1 200 OK\r\n\r\n");
const QByteArray g_responseErr = "HTTP/1.1 404 Not Found";

HttpServer::HttpServer(QObject* parent) :
	QObject(parent)
{
	if (m_server.listen(QHostAddress::LocalHost, 8000))
	{
		connect(&m_server, &QTcpServer::newConnection, this, &HttpServer::OnNewConnection);

		QFile htmlFile(QStringLiteral("app.html"));

		if (htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&htmlFile);
			m_htmlPage = (g_responseTemplate + in.readAll()).toUtf8();
		}

		QFile cssFile(QStringLiteral("app.css"));

		if (cssFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&cssFile);
			m_css = (g_responseTemplate + in.readAll()).toUtf8();
		}

		QFile favFile(QStringLiteral("images/favicon.ico"));

		if (favFile.open(QIODevice::ReadOnly))
		{
			m_fav = g_responseTemplate.toUtf8() + favFile.readAll();
		}
	}
	else
	{
		qCritical().noquote() << QString("HttpServer: unable to start the server: %1").arg(m_server.errorString());
		m_server.close();
	}
}

void HttpServer::OnNewConnection()
{
	QTcpSocket* socket = m_server.nextPendingConnection();
	connect(socket, &QTcpSocket::readyRead, [this, socket]() { Response(socket); });
}

void HttpServer::Response(QTcpSocket *socket)
{
	//qDebug() << "HttpServer::Response: data size = " << socket->bytesAvailable();
	//qDebug() << "HttpServer::Response: data = " << socket->readAll();

	const QString request = socket->readLine();

	qDebug() << "HttpServer::Response: request = " << request;

	if (request.startsWith(QStringLiteral("GET / HTTP/1.1")))
	{
		socket->write(m_htmlPage);
	}
	else if (request.startsWith(QStringLiteral("GET /app.css HTTP/1.1")))
	{
		socket->write(m_css);
	}
	else if (request.startsWith(QStringLiteral("GET /favicon.ico HTTP/1.1")))
	{
		socket->write(m_fav);
	}
	else
	{
		socket->write(g_responseErr);
	}

	socket->close();
}
