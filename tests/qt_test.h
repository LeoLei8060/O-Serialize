#ifndef QT_TEST_H
#define QT_TEST_H

#ifdef O_SERIALIZE_USE_QT

#include <cassert>
#include <iostream>
#include <string>
#include <QColor>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QPointer>
#include <QQueue>
#include <QRect>
#include <QScopedPointer>
#include <QSet>
#include <QSharedPointer>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include "o_serialize/ini.h"
#include "o_serialize/json.h"
#include "o_serialize/o_serialize.h"
#include "o_serialize/xml.h"

// Custom struct with Qt types
struct QtPerson
{
    QString            name;
    int                age;
    QStringList        skills;
    QMap<int, QString> maps;

    bool operator==(const QtPerson &other) const
    {
        return name == other.name && age == other.age && skills == other.skills && maps == other.maps;
    }
};

O_SERIALIZE_STRUCT(QtPerson, name, age, skills, maps)

namespace QtTest {

void test_qt_types()
{
    std::cout << "[Qt] Testing Basic Types..." << std::endl;

    QString s = "Hello Qt";

    // JSON
    std::string s_json = OSerialize::JSON::stringify(s);
    assert(OSerialize::JSON::parse<QString>(s_json) == s);

    // XML
    std::string s_xml = OSerialize::XML::stringify(s, "string");
    assert(OSerialize::XML::parse<QString>(s_xml, "string") == s);

    // INI
    std::string s_ini = OSerialize::INI::stringify(s, "default");
    assert(OSerialize::INI::parse<QString>(s_ini, "default") == s);

    std::cout << "[Qt] Basic Types Passed!" << std::endl;
}

void test_qt_containers()
{
    std::cout << "[Qt] Testing Containers..." << std::endl;

    QStringList list;
    list << "One" << "Two";
    QList<int> qlist;
    qlist << 10 << 20;
    QVector<double> qvec;
    qvec << 1.1 << 2.2;
    QSet<QString> qset;
    qset << "A" << "B";

    // JSON
    assert(OSerialize::JSON::parse<QStringList>(OSerialize::JSON::stringify(list)) == list);
    assert(OSerialize::JSON::parse<QList<int>>(OSerialize::JSON::stringify(qlist)) == qlist);
    assert(OSerialize::JSON::parse<QVector<double>>(OSerialize::JSON::stringify(qvec)) == qvec);
    assert(OSerialize::JSON::parse<QSet<QString>>(OSerialize::JSON::stringify(qset)) == qset);

    // XML
    assert(OSerialize::XML::parse<QStringList>(OSerialize::XML::stringify(list, "l"), "l") == list);
    assert(OSerialize::XML::parse<QList<int>>(OSerialize::XML::stringify(qlist, "l"), "l") == qlist);

    // INI
    assert(OSerialize::INI::parse<QStringList>(OSerialize::INI::stringify(list, "l"), "l") == list);

    std::cout << "[Qt] Containers Passed!" << std::endl;
}

void test_qt_maps()
{
    std::cout << "[Qt] Testing Maps..." << std::endl;

    QMap<QString, int> qmap;
    qmap["one"] = 1;
    qmap["two"] = 2;

    QHash<QString, double> qhash;
    qhash["pi"] = 3.14;
    qhash["e"] = 2.71;

    // JSON
    auto qmap_json = OSerialize::JSON::parse<QMap<QString, int>>(OSerialize::JSON::stringify(qmap));
    assert(qmap_json == qmap);

    auto qhash_json = OSerialize::JSON::parse<QHash<QString, double>>(OSerialize::JSON::stringify(qhash));
    assert(qhash_json == qhash);

    // XML
    auto qmap_xml = OSerialize::XML::parse<QMap<QString, int>>(OSerialize::XML::stringify(qmap, "map"), "map");
    assert(qmap_xml == qmap);

    auto qhash_xml = OSerialize::XML::parse<QHash<QString, double>>(OSerialize::XML::stringify(qhash, "hash"), "hash");
    assert(qhash_xml == qhash);

    // INI
    auto qmap_ini = OSerialize::INI::parse<QMap<QString, int>>(OSerialize::INI::stringify(qmap, "map"), "map");
    assert(qmap_ini == qmap);

    std::cout << "[Qt] Maps Passed!" << std::endl;
}

void test_qt_geometry()
{
    std::cout << "[Qt] Testing Geometry..." << std::endl;

    QPoint pt(10, 20);
    QRect  rect(0, 0, 100, 200);

    // JSON
    assert(OSerialize::JSON::parse<QPoint>(OSerialize::JSON::stringify(pt)) == pt);
    assert(OSerialize::JSON::parse<QRect>(OSerialize::JSON::stringify(rect)) == rect);

    // XML
    assert(OSerialize::XML::parse<QPoint>(OSerialize::XML::stringify(pt, "pt"), "pt") == pt);
    assert(OSerialize::XML::parse<QRect>(OSerialize::XML::stringify(rect, "rect"), "rect") == rect);

    std::cout << "[Qt] Geometry Passed!" << std::endl;
}

void test_qt_datetime()
{
    std::cout << "[Qt] Testing DateTime..." << std::endl;

    QDate     date = QDate::currentDate();
    QDateTime dt = QDateTime::currentDateTime();
    // Remove milliseconds for consistency in ISO string if needed,
    // but QDateTime::toString(Qt::ISODate) usually stable.

    // JSON
    assert(OSerialize::JSON::parse<QDate>(OSerialize::JSON::stringify(date)) == date);
    // Note: Floating point precision or timezone might affect strict equality string match,
    // but serialization cycle should hold.
    QDateTime dt_parsed = OSerialize::JSON::parse<QDateTime>(OSerialize::JSON::stringify(dt));
    // Simple check (string format match)
    assert(dt.toString(Qt::ISODate) == dt_parsed.toString(Qt::ISODate));

    std::cout << "[Qt] DateTime Passed!" << std::endl;
}

void test_qt_pointers()
{
    std::cout << "[Qt] Testing Smart Pointers..." << std::endl;

    QSharedPointer<int>    ptr = QSharedPointer<int>::create(123);
    QScopedPointer<double> ptr_d(new double(1.2));

    // JSON
    auto ptr_json = OSerialize::JSON::parse<QSharedPointer<int>>(OSerialize::JSON::stringify(ptr));
    assert(*ptr_json == 123);
    auto ptrd_json = OSerialize::JSON::parse<QSharedPointer<double>>(OSerialize::JSON::stringify(ptr_d));
    assert(*ptrd_json == 1.2);

    // XML
    auto ptr_xml = OSerialize::XML::parse<QSharedPointer<int>>(OSerialize::XML::stringify(ptr, "p"), "p");
    assert(*ptr_xml == 123);
    auto ptrd_xml = OSerialize::XML::parse<QSharedPointer<double>>(OSerialize::XML::stringify(ptr_d, "p"), "p");
    assert(*ptrd_xml == 1.2);

    std::cout << "[Qt] Smart Pointers Passed!" << std::endl;
}

void test_qt_variant()
{
    std::cout << "[Qt] Testing QVariant..." << std::endl;

    QVariant     v1 = 42;
    QVariant     v2 = QString("test");
    QVariantList vList;
    vList << 1 << "A";
    QVariant v3 = vList;
    QVariant v4 = 1.2;

    // JSON
    assert(OSerialize::JSON::parse<QVariant>(OSerialize::JSON::stringify(v1)) == v1);
    assert(OSerialize::JSON::parse<QVariant>(OSerialize::JSON::stringify(v2)) == v2);
    assert(OSerialize::JSON::parse<QVariant>(OSerialize::JSON::stringify(v4)) == v4);

    QVariant v3_parsed = OSerialize::JSON::parse<QVariant>(OSerialize::JSON::stringify(v3));
    assert(v3_parsed.toList() == vList);

    std::cout << "[Qt] QVariant Passed!" << std::endl;
}

void test_qt_custom_structs()
{
    std::cout << "[Qt] Testing Custom Structs..." << std::endl;

    QtPerson p;
    p.name = "Bob";
    p.age = 25;
    p.skills << "Qt" << "C++";
    p.maps[1] = "test1";
    p.maps[2] = "test2";

    // JSON
    std::string p_json = OSerialize::JSON::stringify(p);
    assert(OSerialize::JSON::parse<QtPerson>(p_json) == p);

    // XML
    std::string p_xml = OSerialize::XML::stringify(p, "Person");
    assert(OSerialize::XML::parse<QtPerson>(p_xml, "Person") == p);

    // INI
    std::string p_ini = OSerialize::INI::stringify(p, "Person");
    assert(OSerialize::INI::parse<QtPerson>(p_ini, "Person") == p);

    std::cout << "[Qt] Custom Structs Passed!" << std::endl;
}

void run_all()
{
    test_qt_types();
    test_qt_containers();
    test_qt_maps();
    test_qt_geometry();
    test_qt_datetime();
    test_qt_pointers();
    test_qt_variant();
    test_qt_custom_structs();
}
} // namespace QtTest

#else

namespace QtTest {
void run_all()
{
    std::cout << "[Qt] Qt support not enabled. Skipping tests." << std::endl;
}
} // namespace QtTest

#endif // O_SERIALIZE_USE_QT

#endif // QT_TEST_H
