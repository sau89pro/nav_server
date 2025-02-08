#pragma once

#include <QByteArray>
#include <QCoreApplication>
#include <QFutureWatcher>
#include <QVector>

class Application : public QCoreApplication
{
	Q_OBJECT

public:
	Application(int argc, char* argv[]);
	Application(const Application&) = default;
	Application(Application&&) = default;
	~Application();

	Application& operator=(const Application&) = default;
	Application& operator=(Application&&) = default;

	static Application* GetInstance();

signals:
	void ConsoleCommandReceived(QByteArray command);

private:
	void ListenInputAsync();

	QFutureWatcher<void> m_watcher;
	bool m_isListenInput = true;
};


