#pragma once

#include <QQmlEngine>

namespace QtPiDeck {
template<class T>
T* qmlSingletonProvider(QQmlEngine *, QJSEngine*) {
    struct Enablr : public T { explicit Enablr(QObject * parent = nullptr) : T(parent) {}};
    static std::unique_ptr<T> instance = std::make_unique<Enablr>();
    return instance.get();
}
}
