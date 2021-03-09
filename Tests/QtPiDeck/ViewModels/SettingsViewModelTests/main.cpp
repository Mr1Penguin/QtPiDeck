#include <QtTest>

#include "Application.hpp"
#include "ViewModels/SettingsViewModel.hpp"

namespace QtPiDeck::ViewModels::Tests {
class SettingsViewModelTests : public QObject
{
    Q_OBJECT // NOLINT

private slots:
    void addressIsDefaultWithoutService();
    void portIsDefaultWithoutService();
    void saveSettingsDoNothingWithoutService();
    void addressIsInitedWithService();
    void portIsInitedWithService();
    void saveSettingsSetsStorageValuesWithService();

private: // NOLINT(readability-redundant-access-specifiers)
    std::unique_ptr<SettingsViewModel> m_viewModel;
};

void SettingsViewModelTests::addressIsDefaultWithoutService() {
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
    QVERIFY(m_viewModel->deckServerAddress().isEmpty());
}

void SettingsViewModelTests::portIsDefaultWithoutService() {
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
    QVERIFY(m_viewModel->deckServerPort().isEmpty());
}

void SettingsViewModelTests::saveSettingsDoNothingWithoutService() {
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
    m_viewModel->saveSettings();
}

class DummySettingsStorage : public Services::IClientSettingsStorage {
public:
    [[nodiscard]] auto read(const QString& /*key*/, const QVariant& /*defaultValue*/ = QVariant{}) const noexcept -> QVariant final { return {}; }
    void store(const QString& /*key*/, const QVariant& /*value*/) noexcept final { }
    [[nodiscard]] auto deckServerAddress() const noexcept -> QString final { return address; }
    void setDeckServerAddress(const QString& deckServerAddress) noexcept final { m_newAddress = deckServerAddress; }
    [[nodiscard]] auto deckServerPort() const noexcept -> QString final { return port; }
    void setDeckServerPort(const QString& deckServerPort) noexcept final { m_newPort = deckServerPort; }

    [[nodiscard]] auto newAddress() const noexcept -> QString { return m_newAddress; }
    [[nodiscard]] auto newPort() const noexcept -> QString { return m_newPort; }

    inline static const QString address = "123.123.123.123";
    inline static const QString port = "13999";

private:
    QString m_newAddress{};
    QString m_newPort{};
};

void SettingsViewModelTests::addressIsInitedWithService() {
    auto settingsStorage = std::make_shared<DummySettingsStorage>();
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
    QCOMPARE(m_viewModel->deckServerAddress(), DummySettingsStorage::address);
}

void SettingsViewModelTests::portIsInitedWithService() {
    auto settingsStorage = std::make_shared<DummySettingsStorage>();
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
    QCOMPARE(m_viewModel->deckServerPort(), DummySettingsStorage::port);
}

void SettingsViewModelTests::saveSettingsSetsStorageValuesWithService() {
    auto settingsStorage = std::make_shared<DummySettingsStorage>();
    m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
    const QString newAddress{"52.52.52.52"};
    const QString newPort{"5252"};
    m_viewModel->setDeckServerAddress(newAddress);
    m_viewModel->setDeckServerPort(newPort);
    QVERIFY(settingsStorage->deckServerAddress() != newAddress);
    QVERIFY(settingsStorage->deckServerPort() != newPort);
    m_viewModel->saveSettings();
    QCOMPARE(settingsStorage->newAddress(), newAddress);
    QCOMPARE(settingsStorage->newPort(), newPort);
}
}

QTEST_APPLESS_MAIN(QtPiDeck::ViewModels::Tests::SettingsViewModelTests) // NOLINT

#include "main.moc"
