#include "http_server.hpp"

#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
