#ifndef O_SERIALIZE_BASE_H
#define O_SERIALIZE_BASE_H

#include <type_traits>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <utility>
#include <memory>
#include <tuple>
#include <variant>
#include <deque>
#include <array>

#ifdef O_SERIALIZE_USE_QT
#include <QString>
#include <QStringList>
#include <QList>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QQueue>
#include <QStack>
#include <QMultiMap>
#include <QMultiHash>
#include <QVariant>
#include <QPair>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QPointer>
#include <QScopedPointer>
#endif

namespace OSerialize {

namespace Traits {

    // --- STL Traits ---

    // STL Container (vector, list, set, deque-like)
    template <typename T> struct is_stl_container : std::false_type {};
    template <typename T> struct is_stl_container<std::vector<T>> : std::true_type {};
    template <typename T> struct is_stl_container<std::list<T>> : std::true_type {};
    template <typename T> struct is_stl_container<std::deque<T>> : std::true_type {};
    template <typename T> struct is_stl_container<std::set<T>> : std::true_type {};

    // STL Map (map, unordered_map)
    template <typename T> struct is_stl_map : std::false_type {};
    template <typename K, typename V> struct is_stl_map<std::map<K, V>> : std::true_type {};
    template <typename K, typename V> struct is_stl_map<std::unordered_map<K, V>> : std::true_type {};
#ifdef O_SERIALIZE_USE_QT
    // 允许 Qt 环境下的特化，但上面的通用模板已经覆盖了
    // 我们可以保留这些以防万一，或者依赖通用模板
    // 为了保持 base.h 简洁，且通用模板 <K, V> 已经包含了 <QString, V>，这里不再需要特化
    // 除非需要区分处理，但 is_stl_map 只是 trait。
#endif

    // std::pair 序列化和反序列化时需要特殊处理，"first" 和 "second" 字段分别表示K和V
    template <typename T> struct is_pair : std::false_type {};
    template <typename K, typename V> struct is_pair<std::pair<K, V>> : std::true_type {};

    // std::shared_ptr / unique_ptr
    template <typename T> struct is_smart_ptr : std::false_type {};
    template <typename T> struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};
    template <typename T> struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};

    // std::variant
    template <typename T> struct is_variant : std::false_type {};
    template <typename... Args> struct is_variant<std::variant<Args...>> : std::true_type {};

    // std::tuple
    template <typename T> struct is_tuple : std::false_type {};
    template <typename... Args> struct is_tuple<std::tuple<Args...>> : std::true_type {};

    // --- Qt Traits ---

#ifdef O_SERIALIZE_USE_QT
    // Qt Container (QList, QVector, QSet, QQueue, QStack, QStringList)
    template <typename T> struct is_qt_container : std::false_type {};
    template <typename T> struct is_qt_container<QList<T>> : std::true_type {};
    template <typename T> struct is_qt_container<QVector<T>> : std::true_type {};
    template <typename T> struct is_qt_container<QSet<T>> : std::true_type {};
    template <typename T> struct is_qt_container<QQueue<T>> : std::true_type {};
    template <typename T> struct is_qt_container<QStack<T>> : std::true_type {};
    template <> struct is_qt_container<QStringList> : std::true_type {};

    // Qt Map (QMap, QHash, QMultiMap, QMultiHash)
    template <typename T> struct is_qt_map : std::false_type {};
    template <typename K, typename V> struct is_qt_map<QMap<K, V>> : std::true_type {};
    template <typename K, typename V> struct is_qt_map<QHash<K, V>> : std::true_type {};
    // QMultiMap/Hash derived from QMap/Hash usually, but treated as maps?
    // In Qt5 QMultiMap is a class, in Qt6 it's separate.
    template <typename K, typename V> struct is_qt_map<QMultiMap<K, V>> : std::true_type {};
    template <typename K, typename V> struct is_qt_map<QMultiHash<K, V>> : std::true_type {};
    
    // QVariant
    template <typename T> struct is_qvariant : std::false_type {};
    template <> struct is_qvariant<QVariant> : std::true_type {};

    // QPair
    template <typename T> struct is_qpair : std::false_type {};
    template <typename T1, typename T2> struct is_qpair<QPair<T1, T2>> : std::true_type {};

    // Qt Smart Pointers
    template <typename T> struct is_qt_smart_ptr : std::false_type {};
    template <typename T> struct is_qt_smart_ptr<QSharedPointer<T>> : std::true_type {};
    template <typename T> struct is_qt_smart_ptr<QWeakPointer<T>> : std::true_type {}; // Weak pointers might need special handling (lock/toStrongRef)
    template <typename T> struct is_qt_smart_ptr<QPointer<T>> : std::true_type {};
    template <typename T> struct is_qt_smart_ptr<QScopedPointer<T>> : std::true_type {};
#endif

} // namespace Traits

} // namespace OSerialize

#endif // O_SERIALIZE_BASE_H
