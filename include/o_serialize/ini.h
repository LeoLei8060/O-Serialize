#ifndef O_SERIALIZE_INI_H
#define O_SERIALIZE_INI_H

#include "o_serialize/base.h"
#include "o_serialize/o_serialize.h"
#include "inipp/inipp.h"
#include <string>
#include <iostream>
#include <sstream>

#ifdef O_SERIALIZE_USE_QT
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QColor>
#endif

namespace OSerialize {

class INI {
public:
    template <typename T>
    static std::string stringify(const T& obj, const std::string& sectionName = "default") {
        inipp::Ini<char> ini;
        to_ini(obj, ini.sections[sectionName]);
        
        std::stringstream ss;
        ini.generate(ss);
        return ss.str();
    }

    template <typename T>
    static T parse(const std::string& iniStr, const std::string& sectionName = "default") {
        inipp::Ini<char> ini;
        std::istringstream is(iniStr);
        ini.parse(is);
        
        T obj;
        if (ini.sections.find(sectionName) == ini.sections.end()) {
             return obj;
        }

        from_ini(ini.sections[sectionName], obj);
        return obj;
    }

private:
    // --- Helper for container insert ---
    template <typename C, typename V>
    static auto add_item(C& c, const V& v) -> decltype(c.push_back(v)) { c.push_back(v); }

    template <typename C, typename V>
    static auto add_item(C& c, const V& v) -> decltype(c.insert(v)) { c.insert(v); }

    // --- to_ini implementations ---

    // Reflected Types
    template <typename T>
    static typename std::enable_if<Meta::has_reflection<T>::value, void>::type
    to_ini(const T& obj, inipp::Ini<char>::Section& section) {
        Meta::visit_members(obj, [&](const char* name, const auto& member) {
            section[name] = val_to_string(member);
        });
    }

    // Basic Types
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, void>::type
    to_ini(const T& val, inipp::Ini<char>::Section& section) {
        section["value"] = val_to_string(val);
    }
    
    static void to_ini(const std::string& val, inipp::Ini<char>::Section& section) {
        section["value"] = val;
    }

#ifdef O_SERIALIZE_USE_QT
    static void to_ini(const QString& val, inipp::Ini<char>::Section& section) {
        section["value"] = val.toStdString();
    }

    // Qt Smart Pointers (flattened if possible, but INI doesn't support nesting well)
    // We treat smart pointer content as the value.
    template <typename T>
    static typename std::enable_if<Traits::is_qt_smart_ptr<T>::value, void>::type
    to_ini(const T& ptr, inipp::Ini<char>::Section& section) {
        if (ptr) {
            to_ini(*ptr, section);
        }
    }

    // QPair (Key-Value style if possible? No, QPair is just 2 values. "first=val1, second=val2")
    template <typename T>
    static typename std::enable_if<Traits::is_qpair<T>::value, void>::type
    to_ini(const T& pair, inipp::Ini<char>::Section& section) {
        section["first"] = val_to_string(pair.first);
        section["second"] = val_to_string(pair.second);
    }
#endif

    // STL Map (Key must be string-like)
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value, void>::type
    to_ini(const T& map, inipp::Ini<char>::Section& section) {
        for (const auto& pair : map) {
            section[pair.first] = val_to_string(pair.second);
        }
    }

#ifdef O_SERIALIZE_USE_QT
    // Qt Map
    template <typename T>
    static typename std::enable_if<Traits::is_qt_map<T>::value, void>::type
    to_ini(const T& map, inipp::Ini<char>::Section& section) {
        auto it = map.begin();
        while (it != map.end()) {
            section[val_to_string(it.key())] = val_to_string(it.value());
            ++it;
        }
    }
#endif

    // STL/Qt Container (Vector/List) - Not standard INI.
    // We can do: item1=val, item2=val...
    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value, void>::type
    to_ini(const T& container, inipp::Ini<char>::Section& section) {
        int i = 0;
        for (const auto& item : container) {
            section["item" + std::to_string(i++)] = val_to_string(item);
        }
    }
    
    // --- val_to_string helpers ---
    template <typename T>
    static typename std::enable_if<!Traits::is_stl_container<T>::value && !Traits::is_qt_container<T>::value && !Traits::is_stl_map<T>::value && !Traits::is_qt_map<T>::value, std::string>::type
    val_to_string(const T& val) {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }

    // Map to String (JSON-like representation for nested maps in INI values)
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value || Traits::is_qt_map<T>::value, std::string>::type
    val_to_string(const T& map) {
        // Fallback: Serialize map as JSON string
        return "Map(...)"; // Simplified or use JSON::stringify if available, but avoiding circular dependency is better.
        // Or implement simple string format
    }

    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value, std::string>::type
    val_to_string(const T& container) {
        std::stringstream ss;
        bool first = true;
        for (const auto& item : container) {
            if (!first) ss << ",";
            ss << val_to_string(item);
            first = false;
        }
        return ss.str();
    }

    static std::string val_to_string(const std::string& val) { return val; }
    
#ifdef O_SERIALIZE_USE_QT
    static std::string val_to_string(const QString& val) { return val.toStdString(); }
    
    static std::string val_to_string(const QDate& val) { return val.toString(Qt::ISODate).toStdString(); }
    static std::string val_to_string(const QTime& val) { return val.toString(Qt::ISODate).toStdString(); }
    static std::string val_to_string(const QDateTime& val) { return val.toString(Qt::ISODate).toStdString(); }
    static std::string val_to_string(const QColor& val) { return val.name().toStdString(); }
#endif

    // --- from_ini implementations ---

    // Reflected Types
    template <typename T>
    static typename std::enable_if<Meta::has_reflection<T>::value>::type
    from_ini(const inipp::Ini<char>::Section& section, T& obj) {
        Meta::visit_members(obj, [&](const char* name, auto& member) {
            auto it = section.find(name);
            if (it != section.end()) {
                string_to_val(it->second, member);
            }
        });
    }

    // Helper to extract value
    template <typename T>
    static typename std::enable_if<!Traits::is_stl_container<T>::value && !Traits::is_qt_container<T>::value && !Traits::is_stl_map<T>::value && !Traits::is_qt_map<T>::value, void>::type
    string_to_val(const std::string& s, T& val) {
        std::stringstream ss(s);
        ss >> val;
    }

    // Map from String (Not supported fully, clear map)
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value || Traits::is_qt_map<T>::value, void>::type
    string_to_val(const std::string&, T& map) {
        map.clear();
    }
    
    // Specialization for vector<string> / set / list
    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value, void>::type
    string_to_val(const std::string& s, T& container) {
        container.clear();
        std::stringstream ss(s);
        std::string item_str;
        while (std::getline(ss, item_str, ',')) {
             typename T::value_type val;
             string_to_val(item_str, val);
             add_item(container, val);
        }
    }

    static void string_to_val(const std::string& s, std::string& val) { val = s; }

#ifdef O_SERIALIZE_USE_QT
    static void string_to_val(const std::string& s, QString& val) { val = QString::fromStdString(s); }
    static void string_to_val(const std::string& s, QDate& val) { val = QDate::fromString(QString::fromStdString(s), Qt::ISODate); }
    static void string_to_val(const std::string& s, QTime& val) { val = QTime::fromString(QString::fromStdString(s), Qt::ISODate); }
    static void string_to_val(const std::string& s, QDateTime& val) { val = QDateTime::fromString(QString::fromStdString(s), Qt::ISODate); }
    static void string_to_val(const std::string& s, QColor& val) { val.setNamedColor(QString::fromStdString(s)); }
#endif

    // Basic Types
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, void>::type
    from_ini(const inipp::Ini<char>::Section& section, T& val) {
        auto it = section.find("value");
        if (it != section.end()) string_to_val(it->second, val);
    }

    static void from_ini(const inipp::Ini<char>::Section& section, std::string& val) {
        auto it = section.find("value");
        if (it != section.end()) val = it->second;
    }

#ifdef O_SERIALIZE_USE_QT
    static void from_ini(const inipp::Ini<char>::Section& section, QString& val) {
        auto it = section.find("value");
        if (it != section.end()) val = QString::fromStdString(it->second);
    }

    // Qt Smart Pointers
    template <typename T>
    static typename std::enable_if<Traits::is_qt_smart_ptr<T>::value>::type
    from_ini(const inipp::Ini<char>::Section& section, T& ptr) {
        // Assume QSharedPointer/QScopedPointer
        create_qt_smart_ptr_ini(ptr, section);
    }

    // Helpers for creating Qt smart pointers (INI)
    template <typename T>
    static auto create_qt_smart_ptr_ini(QSharedPointer<T>& ptr, const inipp::Ini<char>::Section& section) -> void {
        ptr = QSharedPointer<T>::create();
        from_ini(section, *ptr);
    }
    
    template <typename T>
    static auto create_qt_smart_ptr_ini(QScopedPointer<T>& ptr, const inipp::Ini<char>::Section& section) -> void {
        ptr.reset(new T());
        from_ini(section, *ptr);
    }
    
    template <typename T>
    static auto create_qt_smart_ptr_ini(QPointer<T>& ptr, const inipp::Ini<char>::Section&) -> void {
        ptr = nullptr;
    }

    // QPair
    template <typename T>
    static typename std::enable_if<Traits::is_qpair<T>::value>::type
    from_ini(const inipp::Ini<char>::Section& section, T& pair) {
        auto it = section.find("first");
        if (it != section.end()) string_to_val(it->second, pair.first);
        
        it = section.find("second");
        if (it != section.end()) string_to_val(it->second, pair.second);
    }
#endif

    // STL/Qt Map
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value || Traits::is_qt_map<T>::value>::type
    from_ini(const inipp::Ini<char>::Section& section, T& map) {
        map.clear();
        for (const auto& pair : section) {
            typename T::mapped_type val;
            string_to_val(pair.second, val);
            
            // Handle Key Conversion: INI keys are always std::string.
            // For std::map<string, ...> -> direct assignment works.
            // For QMap<QString, ...> -> pair.first (std::string) needs conversion to QString.
            // For std::map<int, ...> -> string needs conversion to int.
            
            insert_map_item(map, pair.first, val);
        }
    }
    
    // Helper to insert into map (with key conversion)
    template <typename Map, typename Val>
    static void insert_map_item(Map& map, const std::string& key, const Val& val) {
        map[key] = val; // Works for std::map<string, ...>
    }
    
#ifdef O_SERIALIZE_USE_QT
    template <typename Val>
    static void insert_map_item(QMap<QString, Val>& map, const std::string& key, const Val& val) {
        map[QString::fromStdString(key)] = val;
    }
    template <typename Val>
    static void insert_map_item(QHash<QString, Val>& map, const std::string& key, const Val& val) {
        map[QString::fromStdString(key)] = val;
    }
#endif

    // STL/Qt Container
    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value>::type
    from_ini(const inipp::Ini<char>::Section& section, T& container) {
        container.clear();
        int i = 0;
        while (true) {
            std::string key = "item" + std::to_string(i);
            auto it = section.find(key);
            if (it == section.end()) break;
            
            typename T::value_type val;
            string_to_val(it->second, val);
            add_item(container, val);
            i++;
        }
    }

};

} // namespace OSerialize

#endif // O_SERIALIZE_INI_H
