#pragma once

#include <QByteArray>
#include <QObject>
#include <QTcpServer>
#include <QVector>

class QTcpSocket;

class HttpServer : public QObject
{
	Q_OBJECT
public:
	explicit HttpServer(QObject* parent = nullptr);

private:
	QTcpServer m_server;
	QByteArray m_htmlPage;
	QByteArray m_css;

private slots:
	void OnNewConnection();
	void Response(QTcpSocket *socket);
};
