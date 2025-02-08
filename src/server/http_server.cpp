#include "http_server.hpp"

#include <QByteArray>
#include <QDirIterator>
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
		UpdateGUIData();
		connect(&m_server, &QTcpServer::newConnection, this, &HttpServer::OnNewConnection);

		if (auto* pApp = Application::GetInstance())
		{
			connect(pApp, &Application::ConsoleCommandReceived, this, &HttpServer::OnConsoleCommandReceived);
		}
	}
	else
	{
		qCritical().noquote() << QString("HttpServer: unable to start the server: %1").arg(m_server.errorString());
		m_server.close();
	}
}

void HttpServer::UpdateGUIData()
{
	QDirIterator it(QStringLiteral("gui/assets/_app/immutable"), { "*.css", "*.js" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		QString path = it.next();
		QFile file(path);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			const QString key = QStringLiteral("GET ") + path.replace(QStringLiteral("gui/assets"), "") + QStringLiteral(" HTTP/1.1\r\n");
			const QByteArray value = (g_responseTemplate + in.readAll()).toUtf8();
			m_gui.insert(key, value);
		}
		else
		{
			qWarning().noquote() << QStringLiteral("HttpServer::UpdateGUI: can't open file ") << path;
		}
	}

	QFile htmlFile(QStringLiteral("gui/index.html"));

	if (htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&htmlFile);
		m_htmlPage = (g_responseTemplate + in.readAll()).toUtf8();
	}
	else
	{
		qWarning().noquote() << QStringLiteral("HttpServer::UpdateGUI: can't open html");
	}

	QFile favFile(QStringLiteral("gui/assets/favicon.ico"));

	if (favFile.open(QIODevice::ReadOnly))
	{
		const QString key = QStringLiteral("GET /favicon.ico HTTP/1.1\r\n");
		const QByteArray value = g_responseTemplate.toUtf8() + favFile.readAll();
		m_assets.insert(key, value);
	}
	else
	{
		qWarning().noquote() << QStringLiteral("HttpServer::UpdateGUI: can't open asset ") << favFile.fileName();
	}
}

void HttpServer::OnConsoleCommandReceived(QByteArray command)
{
	if (!command.startsWith("server"))
	{
		return;
	}
	
	if (command == "server_update_gui")
	{
		UpdateGUIData();
	}
	else
	{
		qWarning() << "HttpServer::OnInput: unknown command = " << command;
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
	else if (m_gui.contains(request))
	{
		socket->write(m_gui[request]);
	}
	else if (m_assets.contains(request))
	{
		socket->write(m_assets[request]);
	}
	else
	{
		socket->write(g_responseErr);
		
		qWarning().noquote() << QStringLiteral("HttpServer::Response: can't handle request: ") << request;
	}

	socket->close();
}
