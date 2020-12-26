#pragma once

#include <QObject>
#include <QTcpSocket>

namespace QtPiDeck::Network {
class DeckClient : public QObject {
    Q_OBJECT // NOLINT
public:
    explicit DeckClient(QObject *parent = nullptr);

    void connectToServer();
private:
    void sendPing();
    void readData();

    QTcpSocket m_socket{};
};
}
