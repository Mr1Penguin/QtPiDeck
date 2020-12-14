#include "ClientApplication.hpp"

auto main(int argc, char *argv[]) -> int
{
    QtPiDeck::Client::ClientApplication app;
    return app.start(argc, argv);
}
