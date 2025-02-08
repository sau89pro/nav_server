#include "app.hpp"

#include <iostream>
#include <string>
#include <QDebug>
#include <QFuture>
#include <QObject>
#include <QtConcurrent>

Application::Application(int argc, char* argv[]) : QCoreApplication(argc, argv)
{
	ListenInputAsync();
}

Application::~Application()
{
	m_isListenInput = false;
	
	qInfo().noquote() << QStringLiteral("Application::~Application: wait for ListenInputAsync finished");

	m_watcher.waitForFinished();
}

Application* Application::GetInstance()
{
	return qobject_cast<Application*>(QCoreApplication::instance());
}

void Application::ListenInputAsync()
{
	QFuture<void> future = QtConcurrent::run([this]() {
		while (m_isListenInput)
		{
			std::string str;
			std::cin >> str;

			if (str == "quit")
			{
				if (auto* pApp = instance())
				{
					m_isListenInput = false;
					pApp->quit();
				}
			}
			else
			{
				emit ConsoleCommandReceived(str.c_str());
			}
		}
	});

	m_watcher.setFuture(future);
}
