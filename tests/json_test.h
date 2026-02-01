#ifndef JSON_TEST_H
#define JSON_TEST_H

#include "o_serialize/json.h"
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

namespace JsonTest {

namespace Stl {

void test_int()
{
    std::cout << "[JSON] Testing int..." << std::endl;
    int         original = 123;
    std::string json = JSON::obj_to_string(original);
    int         parsed = JSON::string_to_obj<int>(json);
    assert(original == parsed);
}

void test_double()
{
    std::cout << "[JSON] Testing double..." << std::endl;
    double      original = 123.456;
    std::string json = JSON::obj_to_string(original);
    double      parsed = JSON::string_to_obj<double>(json);
    assert(abs(original - parsed) < 0.0001);
}

void test_string()
{
    std::cout << "[JSON] Testing std::string..." << std::endl;
    std::string original = "Hello World";
    std::string json = JSON::obj_to_string(original);
    std::string parsed = JSON::string_to_obj<std::string>(json);
    assert(original == parsed);
}

void test_vector()
{
    std::cout << "[JSON] Testing std::vector..." << std::endl;
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::string      json = JSON::obj_to_string(original);
    std::vector<int> parsed = JSON::string_to_obj<std::vector<int>>(json);
    assert(original == parsed);
}

void test_list()
{
    std::cout << "[JSON] Testing std::list..." << std::endl;
    std::list<std::string> original = {"a", "b", "c"};
    std::string            json = JSON::obj_to_string(original);
    std::list<std::string> parsed = JSON::string_to_obj<std::list<std::string>>(json);
    assert(original == parsed);
}

void test_deque()
{
    std::cout << "[JSON] Testing std::deque..." << std::endl;
    std::deque<float> original = {1.1f, 2.2f};
    std::string       json = JSON::obj_to_string(original);
    std::deque<float> parsed = JSON::string_to_obj<std::deque<float>>(json);
    assert(original.size() == parsed.size());
    assert(abs(original[0] - parsed[0]) < 0.0001);
}

void test_set()
{
    std::cout << "[JSON] Testing std::set..." << std::endl;
    std::set<int> original = {1, 5, 2, 4}; // 1, 2, 4, 5
    std::string   json = JSON::obj_to_string(original);
    std::set<int> parsed = JSON::string_to_obj<std::set<int>>(json);
    assert(original == parsed);
}

void test_map()
{
    std::cout << "[JSON] Testing std::map..." << std::endl;
    std::map<std::string, int> original = {{"one", 1}, {"two", 2}};
    std::string                json = JSON::obj_to_string(original);
    std::map<std::string, int> parsed = JSON::string_to_obj<std::map<std::string, int>>(json);
    assert(original == parsed);
}

void test_pair()
{
    std::cout << "[JSON] Testing std::pair..." << std::endl;
    std::pair<int, std::string> original = {42, "answer"};
    std::string                 json = JSON::obj_to_string(original);
    std::pair<int, std::string> parsed = JSON::string_to_obj<std::pair<int, std::string>>(json);
    assert(original == parsed);
}

void test_tuple()
{
    std::cout << "[JSON] Testing std::tuple..." << std::endl;
    std::tuple<int, double, std::string> original = {1, 3.14, "tuple"};
    std::string                          json = JSON::obj_to_string(original);
    auto parsed = JSON::string_to_obj<std::tuple<int, double, std::string>>(json);
    assert(original == parsed);
}

void test_shared_ptr()
{
    std::cout << "[JSON] Testing std::shared_ptr..." << std::endl;
    std::shared_ptr<int> original = std::make_shared<int>(999);
    std::string          json = JSON::obj_to_string(original);
    auto                 parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
    assert(*original == *parsed);

    std::shared_ptr<int> nullPtr;
    json = JSON::obj_to_string(nullPtr);
    parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
    assert(parsed == nullptr);
}

struct AllStlTypes
{
    int                         i;
    double                      d;
    std::string                 s;
    std::vector<int>            vec;
    std::map<std::string, int>  map;
    std::pair<int, std::string> pair;
    std::tuple<int, double>     tuple;
    std::shared_ptr<int>        ptr;

    bool operator==(const AllStlTypes &other) const
    {
        return i == other.i && std::abs(d - other.d) < 0.001 && s == other.s && vec == other.vec
               && map == other.map && pair == other.pair && tuple == other.tuple
               && ((!ptr && !other.ptr) || (ptr && other.ptr && *ptr == *other.ptr));
    }
};

void test_all_stl_types()
{
    std::cout << "[JSON] Testing AllStlTypes struct..." << std::endl;
    AllStlTypes original;
    original.i = 100;
    original.d = 99.9;
    original.s = "Complete";
    original.vec = {10, 20, 30};
    original.map = {{"key", 100}};
    original.pair = {5, "five"};
    original.tuple = {1, 2.0};
    original.ptr = std::make_shared<int>(777);

    std::string json = JSON::obj_to_string(original);
    AllStlTypes parsed = JSON::string_to_obj<AllStlTypes>(json);

    assert(original == parsed);

#ifdef O_SERIALIZE_USE_QT
    QSharedPointer<AllStlTypes> original_qptr = QSharedPointer<AllStlTypes>::create(original);
    std::string                 json_qptr = JSON::obj_to_string(original_qptr);
    QSharedPointer<AllStlTypes> parsed_qptr = JSON::string_to_obj<QSharedPointer<AllStlTypes>>(
        json_qptr);
    assert(*original_qptr.get() == *parsed_qptr.get());
#endif
}

void test_file_io()
{
    std::cout << "[JSON] Testing file IO..." << std::endl;
    AllStlTypes original;
    original.i = 888;
    original.d = 123.456;
    original.s = "File IO Test";
    original.vec = {1, 2, 3};
    original.map = {{"file", 1}};
    original.pair = {10, "ten"};
    original.tuple = {2, 3.3};
    original.ptr = std::make_shared<int>(555);

    std::string filepath = "test_output.json";

    bool success = JSON::obj_to_file(original, filepath);
    assert(success);

    AllStlTypes parsed = JSON::file_to_obj<AllStlTypes>(filepath);
    assert(original == parsed);
}

} // namespace Stl
} // namespace JsonTest

O_SERIALIZE_STRUCT(JsonTest::Stl::AllStlTypes, i, d, s, vec, map, pair, tuple, ptr);

namespace JsonTest {

#ifdef O_SERIALIZE_USE_QT
namespace Qt {

void test_qstring()
{
    std::cout << "[JSON] Testing QString..." << std::endl;
    QString     original = "Hello Qt";
    std::string json = JSON::obj_to_string(original);
    QString     parsed = JSON::string_to_obj<QString>(json);
    assert(original == parsed);
}

void test_qstringlist()
{
    std::cout << "[JSON] Testing QStringList..." << std::endl;
    QStringList original = {"A", "B", "C"};
    std::string json = JSON::obj_to_string(original);
    QStringList parsed = JSON::string_to_obj<QStringList>(json);
    assert(original == parsed);
}

void test_qvector()
{
    std::cout << "[JSON] Testing QVector..." << std::endl;
    QVector<int> original = {10, 20, 30};
    std::string  json = JSON::obj_to_string(original);
    QVector<int> parsed = JSON::string_to_obj<QVector<int>>(json);
    assert(original == parsed);
}

void test_qlist()
{
    std::cout << "[JSON] Testing QList..." << std::endl;
    QList<double> original = {1.5, 2.5};
    std::string   json = JSON::obj_to_string(original);
    QList<double> parsed = JSON::string_to_obj<QList<double>>(json);
    assert(original == parsed);
}

void test_qstack()
{
    std::cout << "[JSON] Testing QStack..." << std::endl;
    QStack<int> original;
    original.push(1);
    original.push(2);
    std::string json = JSON::obj_to_string(original);
    QStack<int> parsed = JSON::string_to_obj<QStack<int>>(json);
    assert(original == parsed);
}

void test_qqueue()
{
    std::cout << "[JSON] Testing QQueue..." << std::endl;
    QQueue<int> original;
    original.enqueue(1);
    original.enqueue(2);
    std::string json = JSON::obj_to_string(original);
    QQueue<int> parsed = JSON::string_to_obj<QQueue<int>>(json);
    assert(original == parsed);
}

void test_qset()
{
    std::cout << "[JSON] Testing QSet..." << std::endl;
    QSet<int>   original = {1, 2, 3};
    std::string json = JSON::obj_to_string(original);
    QSet<int>   parsed = JSON::string_to_obj<QSet<int>>(json);
    assert(original == parsed);
}

void test_qmap()
{
    std::cout << "[JSON] Testing QMap..." << std::endl;
    QMap<QString, int> original;
    original.insert("one", 1);
    original.insert("two", 2);
    std::string        json = JSON::obj_to_string(original);
    QMap<QString, int> parsed = JSON::string_to_obj<QMap<QString, int>>(json);
    assert(original == parsed);
    assert((original == JSON::string_to_obj<QMap<QString, int>>(json)));
}

void test_qhash()
{
    std::cout << "[JSON] Testing QHash..." << std::endl;
    QHash<QString, int> original;
    original.insert("key1", 100);
    original.insert("key2", 200);
    std::string         json = JSON::obj_to_string(original);
    QHash<QString, int> parsed = JSON::string_to_obj<QHash<QString, int>>(json);
    assert(original == parsed);
}

void test_qdate()
{
    std::cout << "[JSON] Testing QDate..." << std::endl;
    QDate       original = QDate::currentDate();
    std::string json = JSON::obj_to_string(original);
    QDate       parsed = JSON::string_to_obj<QDate>(json);
    assert(original == parsed);
}

void test_qtime()
{
    std::cout << "[JSON] Testing QTime..." << std::endl;
    QTime       original(12, 34, 56);
    std::string json = JSON::obj_to_string(original);
    QTime       parsed = JSON::string_to_obj<QTime>(json);
    assert(original == parsed);
}

void test_qdatetime()
{
    std::cout << "[JSON] Testing QDateTime..." << std::endl;
    QDateTime   original(QDate(2023, 1, 1), QTime(12, 0, 0));
    std::string json = JSON::obj_to_string(original);
    QDateTime   parsed = JSON::string_to_obj<QDateTime>(json);
    assert(original == parsed);
}

void test_qpoint()
{
    std::cout << "[JSON] Testing QPoint..." << std::endl;
    QPoint      original(10, 20);
    std::string json = JSON::obj_to_string(original);
    QPoint      parsed = JSON::string_to_obj<QPoint>(json);
    assert(original == parsed);
}

void test_qrect()
{
    std::cout << "[JSON] Testing QRect..." << std::endl;
    QRect       original(0, 0, 100, 200);
    std::string json = JSON::obj_to_string(original);
    QRect       parsed = JSON::string_to_obj<QRect>(json);
    assert(original == parsed);
}

void test_qvariant()
{
    std::cout << "[JSON] Testing QVariant..." << std::endl;
    QVariant    original = 123;
    std::string json = JSON::obj_to_string(original);
    QVariant    parsed = JSON::string_to_obj<QVariant>(json);
    assert(parsed.toInt() == 123);

    original = QString("VariantString");
    json = JSON::obj_to_string(original);
    parsed = JSON::string_to_obj<QVariant>(json);
    assert(parsed.toString() == "VariantString");
}

void test_qsharedpointer()
{
    std::cout << "[JSON] Testing QSharedPointer..." << std::endl;
    QSharedPointer<int> original = QSharedPointer<int>::create(42);
    std::string         json = JSON::obj_to_string(original);
    auto                parsed = JSON::string_to_obj<QSharedPointer<int>>(json);
    assert(*parsed == *original);

    QSharedPointer<int> nullPtr;
    json = JSON::obj_to_string(nullPtr);
    parsed = JSON::string_to_obj<QSharedPointer<int>>(json);
    assert(parsed.isNull());
}

struct AllQtTypes
{
    enum EType { kTypeA = 0, kTypeB };
    QString             str;
    QStringList         strList;
    QVector<int>        vec;
    QList<double>       list;
    QMap<QString, int>  map;
    QDate               date;
    QDateTime           dt;
    QPoint              pt;
    QRect               rect;
    QVariant            var;
    QSharedPointer<int> ptr;
    EType               type;

    bool operator==(const AllQtTypes &other) const
    {
        return str == other.str && strList == other.strList && vec == other.vec
               && list == other.list && map == other.map && date == other.date && dt == other.dt
               && pt == other.pt && rect == other.rect && var == other.var
               && ((ptr.isNull() && other.ptr.isNull())
                   || (!ptr.isNull() && !other.ptr.isNull() && *ptr == *other.ptr));
    }
};

} // namespace Qt
#endif

} // namespace JsonTest

#ifdef O_SERIALIZE_USE_QT
O_SERIALIZE_STRUCT(
    JsonTest::Qt::AllQtTypes, str, strList, vec, list, map, date, dt, pt, rect, var, ptr, type);
#endif

namespace JsonTest {

void run_all()
{
    std::cout << "========== Running JSON Tests ==========" << std::endl;
    Stl::test_int();
    Stl::test_double();
    Stl::test_string();
    Stl::test_vector();
    Stl::test_list();
    Stl::test_deque();
    Stl::test_set();
    Stl::test_map();
    Stl::test_pair();
    Stl::test_tuple();
    Stl::test_shared_ptr();
    Stl::test_all_stl_types();
    Stl::test_file_io();

#ifdef O_SERIALIZE_USE_QT
    Qt::test_qstring();
    Qt::test_qstringlist();
    Qt::test_qvector();
    Qt::test_qlist();
    Qt::test_qstack();
    Qt::test_qqueue();
    Qt::test_qset();
    Qt::test_qmap();
    Qt::test_qhash();
    Qt::test_qdate();
    Qt::test_qtime();
    Qt::test_qdatetime();
    Qt::test_qpoint();
    Qt::test_qrect();
    Qt::test_qvariant();
    Qt::test_qsharedpointer();

    {
        std::cout << "[JSON] Testing AllQtTypes struct..." << std::endl;
        Qt::AllQtTypes original;
        original.str = "Qt Struct";
        original.strList = QStringList({"Item1", "Item2"});
        original.vec = {1, 2, 3};
        original.list = {1.1, 2.2};
        original.map.insert("key", 999);
        original.date = QDate(2023, 10, 1);
        original.dt = QDateTime(QDate(2023, 10, 1), QTime(10, 0, 0));
        original.pt = QPoint(5, 5);
        original.rect = QRect(0, 0, 50, 50);
        original.var = 100;
        original.ptr = QSharedPointer<int>::create(888);
        original.type = Qt::AllQtTypes::kTypeB;

        std::string    json = JSON::obj_to_string(original);
        Qt::AllQtTypes parsed = JSON::string_to_obj<Qt::AllQtTypes>(json);

        assert(original == parsed);
        std::cout << "[JSON] AllQtTypes struct passed." << std::endl;
    }
#else
    std::cout << "Qt support disabled. Skipping Qt tests." << std::endl;
#endif
    std::cout << "========== JSON Tests Finished ==========" << std::endl;
}

} // namespace JsonTest

#endif // JSON_TEST_H
