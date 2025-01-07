#pragma once

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>
#include <QTcpServer>

class QTcpSocket;

class HttpServer : public QObject
{
	Q_OBJECT
public:
	explicit HttpServer(QObject* parent = nullptr);

private:
	QTcpServer m_server;
	QByteArray m_htmlPage;
	QHash<QString, QByteArray> m_gui;

private slots:
	void OnNewConnection();
	void Response(QTcpSocket *socket);
};
