#include "ClientApplication.hpp"

int main(int argc, char *argv[])
{
    QtPiDeck::Client::ClientApplication app;
    return app.start(argc, argv);
}
