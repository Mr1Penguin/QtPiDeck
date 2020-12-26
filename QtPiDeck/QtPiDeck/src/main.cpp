#include "ClientApplication.hpp"

auto main(int argc, char *argv[]) -> int
{
    QtPiDeck::ClientApplication app;
    return app.start(argc, argv);
}
