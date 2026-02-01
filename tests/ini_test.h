#ifndef INI_TEST_H
#define INI_TEST_H

#include "o_serialize/ini.h"
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#ifdef O_SERIALIZE_USE_QT
#include <QDate>
#include <QDateTime>
#include <QHash>
#include <QLinkedList>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QPointF>
#include <QQueue>
#include <QRect>
#include <QRectF>
#include <QSet>
#include <QSharedPointer>
#include <QSize>
#include <QSizeF>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include <QVector>
#endif

using namespace OSerialize;

namespace IniTest {

namespace Stl {

void test_int()
{
    std::cout << "[INI] Testing int..." << std::endl;
    int         original = 123;
    std::string ini = INI::obj_to_string(original);
    int         parsed = INI::string_to_obj<int>(ini);
    assert(original == parsed);
}

void test_double()
{
    std::cout << "[INI] Testing double..." << std::endl;
    double      original = 123.456;
    std::string ini = INI::obj_to_string(original);
    double      parsed = INI::string_to_obj<double>(ini);
    assert(abs(original - parsed) < 0.0001);
}

void test_string()
{
    std::cout << "[INI] Testing std::string..." << std::endl;
    std::string original = "Hello World";
    std::string ini = INI::obj_to_string(original);
    std::string parsed = INI::string_to_obj<std::string>(ini);
    assert(original == parsed);
}

void test_map()
{
    std::cout << "[INI] Testing std::map..." << std::endl;
    std::map<std::string, int> original = {{"one", 1}, {"two", 2}};
    std::string                ini = INI::obj_to_string(original);
    std::map<std::string, int> parsed = INI::string_to_obj<std::map<std::string, int>>(ini);
    assert(original == parsed);
}

void test_pair()
{
    std::cout << "[INI] Testing std::pair..." << std::endl;
    std::pair<int, std::string> original = {42, "answer"};
    std::string                 ini = INI::obj_to_string(original);
    std::pair<int, std::string> parsed = INI::string_to_obj<std::pair<int, std::string>>(ini);
    assert(original == parsed);
}

void test_tuple()
{
    std::cout << "[INI] Testing std::tuple..." << std::endl;
    std::tuple<int, double, std::string> original = {1, 3.14, "tuple"};
    std::string                          ini = INI::obj_to_string(original);
    auto parsed = INI::string_to_obj<std::tuple<int, double, std::string>>(ini);
    assert(original == parsed);
}

void test_shared_ptr()
{
    std::cout << "[INI] Testing std::shared_ptr..." << std::endl;
    std::shared_ptr<int> original = std::make_shared<int>(999);
    std::string          ini = INI::obj_to_string(original);
    auto                 parsed = INI::string_to_obj<std::shared_ptr<int>>(ini);
    assert(*original == *parsed);

    std::shared_ptr<int> nullPtr;
    ini = INI::obj_to_string(nullPtr);
    parsed = INI::string_to_obj<std::shared_ptr<int>>(ini);
    assert(parsed == nullptr);
}

struct IniStlTypes
{
    int                         i;
    double                      d;
    std::string                 s;
    std::map<std::string, int>  map;
    std::pair<int, std::string> pair;
    std::tuple<int, double>     tuple;
    std::shared_ptr<int>        ptr;

    bool operator==(const IniStlTypes &other) const
    {
        return i == other.i && std::abs(d - other.d) < 0.001 && s == other.s && map == other.map
               && pair == other.pair && tuple == other.tuple
               && ((!ptr && !other.ptr) || (ptr && other.ptr && *ptr == *other.ptr));
    }
};

void test_all_stl_types()
{
    std::cout << "[INI] Testing IniStlTypes struct..." << std::endl;
    IniStlTypes original;
    original.i = 100;
    original.d = 99.9;
    original.s = "Complete";
    original.map = {{"key", 100}};
    original.pair = {5, "five"};
    original.tuple = {1, 2.0};
    original.ptr = std::make_shared<int>(777);

    std::string ini = INI::obj_to_string(original);
    IniStlTypes parsed = INI::string_to_obj<IniStlTypes>(ini);

    assert(original == parsed);
}

void test_file_io()
{
    std::cout << "[INI] Testing file IO..." << std::endl;
    IniStlTypes original;
    original.i = 888;
    original.d = 123.456;
    original.s = "File IO Test";
    original.map = {{"file", 1}};
    original.pair = {10, "ten"};
    original.tuple = {2, 3.3};
    original.ptr = std::make_shared<int>(555);

    std::string filepath = "test_output.ini";

    bool success = INI::obj_to_file(original, filepath);
    assert(success);

    IniStlTypes parsed = INI::file_to_obj<IniStlTypes>(filepath);
    assert(original == parsed);
}

} // namespace Stl
} // namespace IniTest

O_SERIALIZE_STRUCT(IniTest::Stl::IniStlTypes, i, d, s, map, pair, tuple, ptr);

namespace IniTest {

#ifdef O_SERIALIZE_USE_QT
namespace Qt {

void test_qstring()
{
    std::cout << "[INI] Testing QString..." << std::endl;
    QString     original = "Hello Qt";
    std::string ini = INI::obj_to_string(original);
    QString     parsed = INI::string_to_obj<QString>(ini);
    assert(original == parsed);
}

void test_qmap()
{
    std::cout << "[INI] Testing QMap..." << std::endl;
    QMap<QString, int> original;
    original.insert("one", 1);
    original.insert("two", 2);
    std::string        ini = INI::obj_to_string(original);
    QMap<QString, int> parsed = INI::string_to_obj<QMap<QString, int>>(ini);
    assert(original == parsed);
}

void test_qdate()
{
    std::cout << "[INI] Testing QDate..." << std::endl;
    QDate       original = QDate::currentDate();
    std::string ini = INI::obj_to_string(original);
    QDate       parsed = INI::string_to_obj<QDate>(ini);
    assert(original == parsed);
}

void test_qvariant()
{
    std::cout << "[INI] Testing QVariant..." << std::endl;
    QVariant    original = QString("VariantString");
    std::string ini = INI::obj_to_string(original);
    QVariant    parsed = INI::string_to_obj<QVariant>(ini);
    assert(parsed.toString() == "VariantString");
}

void test_qsharedpointer()
{
    std::cout << "[INI] Testing QSharedPointer..." << std::endl;
    QSharedPointer<int> original = QSharedPointer<int>::create(42);
    std::string         ini = INI::obj_to_string(original);
    auto                parsed = INI::string_to_obj<QSharedPointer<int>>(ini);
    assert(*parsed == *original);

    QSharedPointer<int> nullPtr;
    ini = INI::obj_to_string(nullPtr);
    parsed = INI::string_to_obj<QSharedPointer<int>>(ini);
    assert(parsed.isNull());
}

struct IniQtTypes
{
    QString             str;
    QMap<QString, int>  map;
    QDate               date;
    QVariant            var;
    QSharedPointer<int> ptr;

    bool operator==(const IniQtTypes &other) const
    {
        return str == other.str && map == other.map && date == other.date
               && var.toString() == other.var.toString()
               && ((ptr.isNull() && other.ptr.isNull())
                   || (!ptr.isNull() && !other.ptr.isNull() && *ptr == *other.ptr));
    }
};

} // namespace Qt
#endif

} // namespace IniTest

#ifdef O_SERIALIZE_USE_QT
O_SERIALIZE_STRUCT(IniTest::Qt::IniQtTypes, str, map, date, var, ptr);
#endif

namespace IniTest {

void run_all()
{
    std::cout << "========== Running INI Tests ==========" << std::endl;
    Stl::test_int();
    Stl::test_double();
    Stl::test_string();
    Stl::test_map();
    Stl::test_pair();
    Stl::test_tuple();
    Stl::test_shared_ptr();
    Stl::test_all_stl_types();
    Stl::test_file_io();

#ifdef O_SERIALIZE_USE_QT
    Qt::test_qstring();
    Qt::test_qmap();
    Qt::test_qdate();
    Qt::test_qvariant();
    Qt::test_qsharedpointer();

    {
        std::cout << "[INI] Testing IniQtTypes struct..." << std::endl;
        Qt::IniQtTypes original;
        original.str = "Qt Struct";
        original.map.insert("key", 999);
        original.date = QDate(2023, 10, 1);
        original.var = "Variant";
        original.ptr = QSharedPointer<int>::create(888);

        std::string    ini = INI::obj_to_string(original);
        Qt::IniQtTypes parsed = INI::string_to_obj<Qt::IniQtTypes>(ini);

        assert(original == parsed);
        std::cout << "[INI] IniQtTypes struct passed." << std::endl;
    }
#else
    std::cout << "Qt support disabled. Skipping Qt tests." << std::endl;
#endif
    std::cout << "========== INI Tests Finished ==========" << std::endl;
}

} // namespace IniTest

#endif // INI_TEST_H
