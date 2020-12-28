#pragma once

#include <memory>

#include <QSettings>

#include "Services/ISettingsStorage.hpp"

namespace QtPiDeck::Services {
class SettingsStorage : public ISettingsStorage {
public:
    SettingsStorage();
    SettingsStorage(const SettingsStorage&) = delete;
    SettingsStorage(SettingsStorage&&) = default;
    ~SettingsStorage() final = default;

    auto operator=(const SettingsStorage&) -> SettingsStorage& = delete;
    auto operator=(SettingsStorage&&) -> SettingsStorage& = default;

    [[nodiscard]] auto read(const QString& key, const QVariant& defaultValue = QVariant{}) const noexcept -> QVariant final;
    void store(const QString& key, const QVariant& value) noexcept final;
private:
    std::unique_ptr<QSettings> m_settings;
};
}
