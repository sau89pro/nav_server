#pragma once

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
	QVector<QTcpSocket*> m_pSockets;

private slots:
	void OnNewConnection();
	void OnNewData();
};
