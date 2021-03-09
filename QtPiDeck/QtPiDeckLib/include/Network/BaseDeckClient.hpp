#pragma once

#include <QObject>
#include <QTcpSocket>

#include "Network/MessageHeader.hpp"
#include "Services/IClientSettingsStorage.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Network {
enum class SendError { NotConnected };

class BaseDeckClient : public QObject {
  Q_OBJECT // NOLINT
public:
  explicit BaseDeckClient(QObject* parent = nullptr);

  void connectToServer(const QString& address, int port) noexcept;
  auto send(MessageType messageType, const QString& requestId) noexcept -> std::optional<SendError>;

  [[nodiscard]] auto isConnected() const noexcept -> bool {
    return m_socket.state() == QAbstractSocket::ConnectedState;
  }

signals:
  void connected();
  void headerReceived(MessageHeader);

protected:
  [[nodiscard]] auto socket() noexcept -> QTcpSocket& { return m_socket; };
  [[nodiscard]] auto socket() const noexcept -> const QTcpSocket& { return m_socket; };

private:
  void readHeader();
  QTcpSocket m_socket{};
};
}
