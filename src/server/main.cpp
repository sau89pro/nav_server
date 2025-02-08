#include "app.hpp"
#include "http_server.hpp"

int main(int argc, char* argv[])
{
    Application app(argc, argv);

    HttpServer server;

    return app.exec();
}


