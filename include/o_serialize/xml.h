#ifndef O_SERIALIZE_XML_H
#define O_SERIALIZE_XML_H

#include "o_serialize/base.h"
#include "o_serialize/o_serialize.h"
#include "tinyxml/tinyxml2.h"
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
#include <QLine>
#include <QLineF>
#include <QColor>
#include <QByteArray>
#endif

namespace OSerialize {

class XML {
public:
    template <typename T>
    static std::string stringify(const T& obj, const std::string& rootName = "root") {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement* root = doc.NewElement(rootName.c_str());
        doc.InsertEndChild(root);

        to_xml(obj, root, doc);

        tinyxml2::XMLPrinter printer;
        doc.Print(&printer);
        return printer.CStr();
    }

    template <typename T>
    static T parse(const std::string& xml, const std::string& rootName = "root") {
        tinyxml2::XMLDocument doc;
        doc.Parse(xml.c_str());
        
        T obj;
        if (doc.Error()) {
            std::cerr << "XML Parse Error: " << doc.ErrorStr() << std::endl;
            return obj;
        }

        tinyxml2::XMLElement* root = doc.FirstChildElement(rootName.c_str());
        if (!root) {
             std::cerr << "XML Parse Error: Root element '" << rootName << "' not found." << std::endl;
             return obj;
        }

        from_xml(root, obj);
        return obj;
    }

private:
    // --- Helper for container insert ---
    template <typename C, typename V>
    static auto add_item(C& c, const V& v) -> decltype(c.push_back(v)) { c.push_back(v); }

    template <typename C, typename V>
    static auto add_item(C& c, const V& v) -> decltype(c.insert(v)) { c.insert(v); }

    // --- to_xml implementations ---

    // Reflected Types
    template <typename T>
    static typename std::enable_if<Meta::has_reflection<T>::value, void>::type
    to_xml(const T& obj, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        Meta::visit_members(obj, [&](const char* name, const auto& member) {
            tinyxml2::XMLElement* child = doc.NewElement(name);
            element->InsertEndChild(child);
            to_xml(member, child, doc);
        });
    }

    // Basic Types: stored as text content of the element
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    to_xml(const T& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val);
    }
    
    // Enum Types
    template <typename T>
    static typename std::enable_if<std::is_enum<T>::value, void>::type
    to_xml(T val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText((int)val);
    }

    static void to_xml(const std::string& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.c_str());
    }

    static void to_xml(const char* val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val);
    }

    // Smart Pointers
    template <typename T>
    static typename std::enable_if<Traits::is_smart_ptr<T>::value, void>::type
    to_xml(const T& ptr, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        if (ptr) {
            to_xml(*ptr, element, doc);
        }
        // If null, we leave empty or add explicit null attribute? XML usually just empty or missing.
    }

    // Variant
    template <typename... Args>
    static void to_xml(const std::variant<Args...>& v, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        std::visit([&](const auto& val) {
            to_xml(val, element, doc);
        }, v);
    }

#ifdef O_SERIALIZE_USE_QT
    static void to_xml(const QString& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.toUtf8().constData());
    }

    // Qt Date/Time
    static void to_xml(const QDate& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.toString(Qt::ISODate).toUtf8().constData());
    }
    static void to_xml(const QTime& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.toString(Qt::ISODate).toUtf8().constData());
    }
    static void to_xml(const QDateTime& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.toString(Qt::ISODate).toUtf8().constData());
    }

    // Qt Geometry
    static void to_xml(const QPoint& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* x = doc.NewElement("x"); x->SetText(val.x()); element->InsertEndChild(x);
        tinyxml2::XMLElement* y = doc.NewElement("y"); y->SetText(val.y()); element->InsertEndChild(y);
    }
    static void to_xml(const QPointF& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* x = doc.NewElement("x"); x->SetText(val.x()); element->InsertEndChild(x);
        tinyxml2::XMLElement* y = doc.NewElement("y"); y->SetText(val.y()); element->InsertEndChild(y);
    }
    static void to_xml(const QSize& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* w = doc.NewElement("width"); w->SetText(val.width()); element->InsertEndChild(w);
        tinyxml2::XMLElement* h = doc.NewElement("height"); h->SetText(val.height()); element->InsertEndChild(h);
    }
    static void to_xml(const QSizeF& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* w = doc.NewElement("width"); w->SetText(val.width()); element->InsertEndChild(w);
        tinyxml2::XMLElement* h = doc.NewElement("height"); h->SetText(val.height()); element->InsertEndChild(h);
    }
    static void to_xml(const QRect& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* x = doc.NewElement("x"); x->SetText(val.x()); element->InsertEndChild(x);
        tinyxml2::XMLElement* y = doc.NewElement("y"); y->SetText(val.y()); element->InsertEndChild(y);
        tinyxml2::XMLElement* w = doc.NewElement("width"); w->SetText(val.width()); element->InsertEndChild(w);
        tinyxml2::XMLElement* h = doc.NewElement("height"); h->SetText(val.height()); element->InsertEndChild(h);
    }
    static void to_xml(const QRectF& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* x = doc.NewElement("x"); x->SetText(val.x()); element->InsertEndChild(x);
        tinyxml2::XMLElement* y = doc.NewElement("y"); y->SetText(val.y()); element->InsertEndChild(y);
        tinyxml2::XMLElement* w = doc.NewElement("width"); w->SetText(val.width()); element->InsertEndChild(w);
        tinyxml2::XMLElement* h = doc.NewElement("height"); h->SetText(val.height()); element->InsertEndChild(h);
    }
    
    static void to_xml(const QColor& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.name().toUtf8().constData());
    }
    
    static void to_xml(const QByteArray& val, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        element->SetText(val.toStdString().c_str());
    }

    // Qt Smart Pointers
    template <typename T>
    static typename std::enable_if<Traits::is_qt_smart_ptr<T>::value, void>::type
    to_xml(const T& ptr, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        if (ptr) {
            to_xml(*ptr, element, doc);
        }
    }

    // QPair
    template <typename T>
    static typename std::enable_if<Traits::is_qpair<T>::value, void>::type
    to_xml(const T& pair, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        tinyxml2::XMLElement* first = doc.NewElement("first");
        element->InsertEndChild(first);
        to_xml(pair.first, first, doc);

        tinyxml2::XMLElement* second = doc.NewElement("second");
        element->InsertEndChild(second);
        to_xml(pair.second, second, doc);
    }
#endif

    // STL/Qt Container (Vector, List, Set)
    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value, void>::type
    to_xml(const T& container, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        for (const auto& item : container) {
            tinyxml2::XMLElement* child = doc.NewElement("item");
            element->InsertEndChild(child);
            to_xml(item, child, doc);
        }
    }

    // STL Map (Key must be string-like)
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value, void>::type
    to_xml(const T& map, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        for (const auto& pair : map) {
            tinyxml2::XMLElement* child = doc.NewElement(pair.first.c_str());
            element->InsertEndChild(child);
            to_xml(pair.second, child, doc);
        }
    }

#ifdef O_SERIALIZE_USE_QT
    // Qt Map
    template <typename T>
    static typename std::enable_if<Traits::is_qt_map<T>::value, void>::type
    to_xml(const T& map, tinyxml2::XMLElement* element, tinyxml2::XMLDocument& doc) {
        auto it = map.begin();
        while (it != map.end()) {
            // Qt Map iterator: key() and value()
            tinyxml2::XMLElement* child = doc.NewElement(val_to_string_helper(it.key()).c_str());
            element->InsertEndChild(child);
            to_xml(it.value(), child, doc);
            ++it;
        }
    }

    // Helper for Map Keys
    template <typename K>
    static std::string val_to_string_helper(const K& key) { return std::to_string(key); }
    static std::string val_to_string_helper(const std::string& key) { return key; }
    static std::string val_to_string_helper(const QString& key) { return key.toStdString(); }
#endif

    // --- from_xml implementations ---

    // Reflected Types
    template <typename T>
    static typename std::enable_if<Meta::has_reflection<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& obj) {
        if (!element) return;
        Meta::visit_members(obj, [&](const char* name, auto& member) {
            tinyxml2::XMLElement* child = element->FirstChildElement(name);
            if (child) {
                from_xml(child, member);
            }
        });
    }

    // Basic Types
    static void from_xml(tinyxml2::XMLElement* element, int& val) { 
        if(element && element->GetText()) val = std::stoi(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, unsigned int& val) { 
        if(element && element->GetText()) val = std::stoul(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, long& val) { 
        if(element && element->GetText()) val = std::stol(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, unsigned long& val) { 
        if(element && element->GetText()) val = std::stoul(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, long long& val) { 
        if(element && element->GetText()) val = std::stoll(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, unsigned long long& val) { 
        if(element && element->GetText()) val = std::stoull(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, double& val) { 
        if(element && element->GetText()) val = std::stod(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, float& val) { 
        if(element && element->GetText()) val = std::stof(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, bool& val) { 
        if(element && element->GetText()) {
            std::string text = element->GetText();
            val = (text == "true" || text == "1");
        }
    }
    // int8_t / uint8_t
    static void from_xml(tinyxml2::XMLElement* element, signed char& val) { 
        if(element && element->GetText()) val = (signed char)std::stoi(element->GetText()); 
    }
    static void from_xml(tinyxml2::XMLElement* element, unsigned char& val) { 
        if(element && element->GetText()) val = (unsigned char)std::stoul(element->GetText()); 
    }
    // Enum
    template <typename T>
    static typename std::enable_if<std::is_enum<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& val) {
        if(element && element->GetText()) val = static_cast<T>(std::stoi(element->GetText()));
    }
    
    static void from_xml(tinyxml2::XMLElement* element, std::string& val) { 
        if(element && element->GetText()) val = element->GetText(); 
    }

    // Smart Pointers
    template <typename T>
    static typename std::enable_if<Traits::is_smart_ptr<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& ptr) {
        if (!element) {
            ptr.reset();
            return;
        }
        using ElementType = typename T::element_type;
        ptr = std::make_shared<ElementType>(); 
        from_xml(element, *ptr);
    }

#ifdef O_SERIALIZE_USE_QT
    static void from_xml(tinyxml2::XMLElement* element, QString& val) {
        if(element && element->GetText()) val = QString::fromUtf8(element->GetText());
    }

    static void from_xml(tinyxml2::XMLElement* element, QDate& val) {
        if(element && element->GetText()) val = QDate::fromString(QString::fromUtf8(element->GetText()), Qt::ISODate);
    }
    static void from_xml(tinyxml2::XMLElement* element, QTime& val) {
        if(element && element->GetText()) val = QTime::fromString(QString::fromUtf8(element->GetText()), Qt::ISODate);
    }
    static void from_xml(tinyxml2::XMLElement* element, QDateTime& val) {
        if(element && element->GetText()) val = QDateTime::fromString(QString::fromUtf8(element->GetText()), Qt::ISODate);
    }

    static void from_xml(tinyxml2::XMLElement* element, QPoint& val) {
        if(element) {
            tinyxml2::XMLElement* c = element->FirstChildElement("x"); if(c) from_xml(c, val.rx());
            c = element->FirstChildElement("y"); if(c) from_xml(c, val.ry());
        }
    }
    static void from_xml(tinyxml2::XMLElement* element, QPointF& val) {
        if(element) {
            tinyxml2::XMLElement* c = element->FirstChildElement("x"); if(c) from_xml(c, val.rx());
            c = element->FirstChildElement("y"); if(c) from_xml(c, val.ry());
        }
    }
    static void from_xml(tinyxml2::XMLElement* element, QSize& val) {
        if(element) {
            tinyxml2::XMLElement* c = element->FirstChildElement("width"); if(c) from_xml(c, val.rwidth());
            c = element->FirstChildElement("height"); if(c) from_xml(c, val.rheight());
        }
    }
    static void from_xml(tinyxml2::XMLElement* element, QSizeF& val) {
        if(element) {
            tinyxml2::XMLElement* c = element->FirstChildElement("width"); if(c) from_xml(c, val.rwidth());
            c = element->FirstChildElement("height"); if(c) from_xml(c, val.rheight());
        }
    }
    static void from_xml(tinyxml2::XMLElement* element, QRect& val) {
        if(element) {
            int x=0,y=0,w=0,h=0;
            tinyxml2::XMLElement* c = element->FirstChildElement("x"); if(c) from_xml(c, x);
            c = element->FirstChildElement("y"); if(c) from_xml(c, y);
            c = element->FirstChildElement("width"); if(c) from_xml(c, w);
            c = element->FirstChildElement("height"); if(c) from_xml(c, h);
            val.setRect(x,y,w,h);
        }
    }
    static void from_xml(tinyxml2::XMLElement* element, QRectF& val) {
        if(element) {
            double x=0,y=0,w=0,h=0;
            tinyxml2::XMLElement* c = element->FirstChildElement("x"); if(c) from_xml(c, x);
            c = element->FirstChildElement("y"); if(c) from_xml(c, y);
            c = element->FirstChildElement("width"); if(c) from_xml(c, w);
            c = element->FirstChildElement("height"); if(c) from_xml(c, h);
            val.setRect(x,y,w,h);
        }
    }
    
    static void from_xml(tinyxml2::XMLElement* element, QColor& val) {
        if(element && element->GetText()) val.setNamedColor(QString::fromUtf8(element->GetText()));
    }
    
    static void from_xml(tinyxml2::XMLElement* element, QByteArray& val) {
        if(element && element->GetText()) val = QByteArray(element->GetText());
    }

    // Qt Smart Pointers
    template <typename T>
    static typename std::enable_if<Traits::is_qt_smart_ptr<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& ptr) {
        if (!element) {
            ptr.reset();
            return;
        }
        create_qt_smart_ptr_xml(ptr, element);
    }

    // Helpers for creating Qt smart pointers (XML)
    template <typename T>
    static auto create_qt_smart_ptr_xml(QSharedPointer<T>& ptr, tinyxml2::XMLElement* element) -> void {
        ptr = QSharedPointer<T>::create();
        from_xml(element, *ptr);
    }
    
    template <typename T>
    static auto create_qt_smart_ptr_xml(QScopedPointer<T>& ptr, tinyxml2::XMLElement* element) -> void {
        ptr.reset(new T());
        from_xml(element, *ptr);
    }
    
    template <typename T>
    static auto create_qt_smart_ptr_xml(QPointer<T>& ptr, tinyxml2::XMLElement*) -> void {
        ptr = nullptr;
    }

    // QPair
    template <typename T>
    static typename std::enable_if<Traits::is_qpair<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& pair) {
        if (!element) return;
        tinyxml2::XMLElement* first = element->FirstChildElement("first");
        if (first) from_xml(first, pair.first);
        
        tinyxml2::XMLElement* second = element->FirstChildElement("second");
        if (second) from_xml(second, pair.second);
    }
#endif

    // STL/Qt Container
    template <typename T>
    static typename std::enable_if<Traits::is_stl_container<T>::value || Traits::is_qt_container<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& container) {
        if (!element) return;
        container.clear();
        tinyxml2::XMLElement* child = element->FirstChildElement("item");
        while (child) {
            typename T::value_type val;
            from_xml(child, val);
            add_item(container, val);
            child = child->NextSiblingElement("item");
        }
    }

    // STL/Qt Map
    template <typename T>
    static typename std::enable_if<Traits::is_stl_map<T>::value || Traits::is_qt_map<T>::value>::type
    from_xml(tinyxml2::XMLElement* element, T& map) {
        if (!element) return;
        map.clear();
        tinyxml2::XMLElement* child = element->FirstChildElement();
        while (child) {
            typename T::mapped_type val;
            from_xml(child, val);
            // QMap supports operator[] with implicit string conversion
            insert_map_item(map, child->Name(), val);
            child = child->NextSiblingElement();
        }
    }
    
    // Helper to insert into map
    template <typename Map, typename Val>
    static void insert_map_item(Map& map, const char* key, const Val& val) {
        map[key] = val;
    }
};

} // namespace OSerialize

#endif // O_SERIALIZE_XML_H
