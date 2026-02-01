#ifndef XML_TEST_H
#define XML_TEST_H

#include "o_serialize/xml.h"
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

namespace XmlTest {

namespace Stl {

void test_int()
{
    std::cout << "[XML] Testing int..." << std::endl;
    int         original = 123;
    std::string xml = XML::obj_to_string(original);
    int         parsed = XML::string_to_obj<int>(xml);
    assert(original == parsed);
}

void test_double()
{
    std::cout << "[XML] Testing double..." << std::endl;
    double      original = 123.456;
    std::string xml = XML::obj_to_string(original);
    double      parsed = XML::string_to_obj<double>(xml);
    // simple float comparison
    assert(abs(original - parsed) < 0.0001);
}

void test_string()
{
    std::cout << "[XML] Testing std::string..." << std::endl;
    std::string original = "Hello World";
    std::string xml = XML::obj_to_string(original);
    std::string parsed = XML::string_to_obj<std::string>(xml);
    assert(original == parsed);
}

void test_vector()
{
    std::cout << "[XML] Testing std::vector..." << std::endl;
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::string      xml = XML::obj_to_string(original);
    std::vector<int> parsed = XML::string_to_obj<std::vector<int>>(xml);
    assert(original == parsed);
}

void test_list()
{
    std::cout << "[XML] Testing std::list..." << std::endl;
    std::list<std::string> original = {"a", "b", "c"};
    std::string            xml = XML::obj_to_string(original);
    std::list<std::string> parsed = XML::string_to_obj<std::list<std::string>>(xml);
    assert(original == parsed);
}

void test_deque()
{
    std::cout << "[XML] Testing std::deque..." << std::endl;
    std::deque<float> original = {1.1f, 2.2f};
    std::string       xml = XML::obj_to_string(original);
    std::deque<float> parsed = XML::string_to_obj<std::deque<float>>(xml);
    assert(original.size() == parsed.size());
    assert(abs(original[0] - parsed[0]) < 0.0001);
}

void test_set()
{
    std::cout << "[XML] Testing std::set..." << std::endl;
    std::set<int> original = {1, 5, 2, 4}; // 1, 2, 4, 5
    std::string   xml = XML::obj_to_string(original);
    std::set<int> parsed = XML::string_to_obj<std::set<int>>(xml);
    assert(original == parsed);
}

void test_map()
{
    std::cout << "[XML] Testing std::map..." << std::endl;
    std::map<std::string, int> original = {{"one", 1}, {"two", 2}};
    std::string                xml = XML::obj_to_string(original);
    std::map<std::string, int> parsed = XML::string_to_obj<std::map<std::string, int>>(xml);
    assert(original == parsed);

    // XML 支持非字符串 Key，测试 int key
    std::cout << "[XML] Testing std::map<int, int>..." << std::endl;
    std::map<int, int> int_map = {{1, 10}, {2, 20}};
    std::string        int_xml = XML::obj_to_string(int_map);
    std::map<int, int> int_parsed = XML::string_to_obj<std::map<int, int>>(int_xml);
    assert(int_map == int_parsed);
}

void test_pair()
{
    std::cout << "[XML] Testing std::pair..." << std::endl;
    std::pair<int, std::string> original = {42, "answer"};
    std::string                 xml = XML::obj_to_string(original);
    std::pair<int, std::string> parsed = XML::string_to_obj<std::pair<int, std::string>>(xml);
    assert(original == parsed);
}

void test_tuple()
{
    std::cout << "[XML] Testing std::tuple..." << std::endl;
    std::tuple<int, double, std::string> original = {1, 3.14, "tuple"};
    std::string                          xml = XML::obj_to_string(original);
    auto parsed = XML::string_to_obj<std::tuple<int, double, std::string>>(xml);
    assert(original == parsed);
}

void test_shared_ptr()
{
    std::cout << "[XML] Testing std::shared_ptr..." << std::endl;
    std::shared_ptr<int> original = std::make_shared<int>(999);
    std::string          xml = XML::obj_to_string(original);
    auto                 parsed = XML::string_to_obj<std::shared_ptr<int>>(xml);
    assert(*original == *parsed);

    std::shared_ptr<int> nullPtr;
    xml = XML::obj_to_string(nullPtr);
    parsed = XML::string_to_obj<std::shared_ptr<int>>(xml);
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
    std::cout << "[XML] Testing AllStlTypes struct..." << std::endl;
    AllStlTypes original;
    original.i = 100;
    original.d = 99.9;
    original.s = "Complete";
    original.vec = {10, 20, 30};
    original.map = {{"key", 100}};
    original.pair = {5, "five"};
    original.tuple = {1, 2.0};
    original.ptr = std::make_shared<int>(777);

    std::string xml = XML::obj_to_string(original);
    AllStlTypes parsed = XML::string_to_obj<AllStlTypes>(xml);

    assert(original == parsed);

#ifdef O_SERIALIZE_USE_QT
    QSharedPointer<AllStlTypes> original_qptr = QSharedPointer<AllStlTypes>::create(original);
    std::string                 xml_qptr = XML::obj_to_string(original_qptr);
    QSharedPointer<AllStlTypes> parsed_qptr = XML::string_to_obj<QSharedPointer<AllStlTypes>>(
        xml_qptr);
    assert(*original_qptr.get() == *parsed_qptr.get());
#endif
}

void test_file_io()
{
    std::cout << "[XML] Testing file IO..." << std::endl;
    AllStlTypes original;
    original.i = 888;
    original.d = 123.456;
    original.s = "File IO Test";
    original.vec = {1, 2, 3};
    original.map = {{"file", 1}};
    original.pair = {10, "ten"};
    original.tuple = {2, 3.3};
    original.ptr = std::make_shared<int>(555);

    std::string filepath = "test_output.xml";

    // Test obj_to_file
    bool success = XML::obj_to_file(original, filepath);
    assert(success);

    // Test file_to_obj
    AllStlTypes parsed = XML::file_to_obj<AllStlTypes>(filepath);
    assert(original == parsed);
}

} // namespace Stl
} // namespace XmlTest

O_SERIALIZE_STRUCT(XmlTest::Stl::AllStlTypes, i, d, s, vec, map, pair, tuple, ptr);

namespace XmlTest {

#ifdef O_SERIALIZE_USE_QT
namespace Qt {

void test_qstring()
{
    std::cout << "[XML] Testing QString..." << std::endl;
    QString     original = "Hello Qt";
    std::string xml = XML::obj_to_string(original);
    QString     parsed = XML::string_to_obj<QString>(xml);
    assert(original == parsed);
}

void test_qstringlist()
{
    std::cout << "[XML] Testing QStringList..." << std::endl;
    QStringList original = {"A", "B", "C"};
    std::string xml = XML::obj_to_string(original);
    QStringList parsed = XML::string_to_obj<QStringList>(xml);
    assert(original == parsed);
}

void test_qvector()
{
    std::cout << "[XML] Testing QVector..." << std::endl;
    QVector<int> original = {10, 20, 30};
    std::string  xml = XML::obj_to_string(original);
    QVector<int> parsed = XML::string_to_obj<QVector<int>>(xml);
    assert(original == parsed);
}

void test_qlist()
{
    std::cout << "[XML] Testing QList..." << std::endl;
    QList<double> original = {1.5, 2.5};
    std::string   xml = XML::obj_to_string(original);
    QList<double> parsed = XML::string_to_obj<QList<double>>(xml);
    assert(original == parsed);
}

void test_qstack()
{
    std::cout << "[XML] Testing QStack..." << std::endl;
    QStack<int> original;
    original.push(1);
    original.push(2);
    std::string xml = XML::obj_to_string(original);
    QStack<int> parsed = XML::string_to_obj<QStack<int>>(xml);
    assert(original == parsed);
}

void test_qqueue()
{
    std::cout << "[XML] Testing QQueue..." << std::endl;
    QQueue<int> original;
    original.enqueue(1);
    original.enqueue(2);
    std::string xml = XML::obj_to_string(original);
    QQueue<int> parsed = XML::string_to_obj<QQueue<int>>(xml);
    assert(original == parsed);
}

void test_qset()
{
    std::cout << "[XML] Testing QSet..." << std::endl;
    QSet<int>   original = {1, 2, 3};
    std::string xml = XML::obj_to_string(original);
    QSet<int>   parsed = XML::string_to_obj<QSet<int>>(xml);
    assert(original == parsed);
}

void test_qmap()
{
    std::cout << "[XML] Testing QMap..." << std::endl;
    QMap<QString, int> original;
    original.insert("one", 1);
    original.insert("two", 2);
    std::string        xml = XML::obj_to_string(original);
    QMap<QString, int> parsed = XML::string_to_obj<QMap<QString, int>>(xml);
    assert(original == parsed);
    assert((original == XML::string_to_obj<QMap<QString, int>>(xml)));
}

void test_qhash()
{
    std::cout << "[XML] Testing QHash..." << std::endl;
    QHash<QString, int> original;
    original.insert("key1", 100);
    original.insert("key2", 200);
    std::string         xml = XML::obj_to_string(original);
    QHash<QString, int> parsed = XML::string_to_obj<QHash<QString, int>>(xml);
    assert(original == parsed);
}

void test_qdate()
{
    std::cout << "[XML] Testing QDate..." << std::endl;
    QDate       original = QDate::currentDate();
    std::string xml = XML::obj_to_string(original);
    QDate       parsed = XML::string_to_obj<QDate>(xml);
    assert(original == parsed);
}

void test_qtime()
{
    std::cout << "[XML] Testing QTime..." << std::endl;
    QTime       original(12, 34, 56);
    std::string xml = XML::obj_to_string(original);
    QTime       parsed = XML::string_to_obj<QTime>(xml);
    assert(original == parsed);
}

void test_qdatetime()
{
    std::cout << "[XML] Testing QDateTime..." << std::endl;
    QDateTime   original(QDate(2023, 1, 1), QTime(12, 0, 0));
    std::string xml = XML::obj_to_string(original);
    QDateTime   parsed = XML::string_to_obj<QDateTime>(xml);
    assert(original == parsed);
}

void test_qpoint()
{
    std::cout << "[XML] Testing QPoint..." << std::endl;
    QPoint      original(10, 20);
    std::string xml = XML::obj_to_string(original);
    QPoint      parsed = XML::string_to_obj<QPoint>(xml);
    assert(original == parsed);
}

void test_qrect()
{
    std::cout << "[XML] Testing QRect..." << std::endl;
    QRect       original(0, 0, 100, 200);
    std::string xml = XML::obj_to_string(original);
    QRect       parsed = XML::string_to_obj<QRect>(xml);
    assert(original == parsed);
}

void test_qvariant()
{
    std::cout << "[XML] Testing QVariant..." << std::endl;
    QVariant    original = QString("VariantString");
    std::string xml = XML::obj_to_string(original);
    QVariant    parsed = XML::string_to_obj<QVariant>(xml);
    assert(parsed.toString() == "VariantString");

    original = 123;
    xml = XML::obj_to_string(original);
    parsed = XML::string_to_obj<QVariant>(xml);
    assert(parsed.toString() == "123");
}

void test_qsharedpointer()
{
    std::cout << "[XML] Testing QSharedPointer..." << std::endl;
    QSharedPointer<int> original = QSharedPointer<int>::create(42);
    std::string         xml = XML::obj_to_string(original);
    auto                parsed = XML::string_to_obj<QSharedPointer<int>>(xml);
    assert(*parsed == *original);

    QSharedPointer<int> nullPtr;
    xml = XML::obj_to_string(nullPtr);
    parsed = XML::string_to_obj<QSharedPointer<int>>(xml);
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
               && pt == other.pt && rect == other.rect && var.toString() == other.var.toString()
               && ((ptr.isNull() && other.ptr.isNull())
                   || (!ptr.isNull() && !other.ptr.isNull() && *ptr == *other.ptr));
    }
};

} // namespace Qt
#endif

} // namespace XmlTest

#ifdef O_SERIALIZE_USE_QT
O_SERIALIZE_STRUCT(
    XmlTest::Qt::AllQtTypes, str, strList, vec, list, map, date, dt, pt, rect, var, ptr, type);
#endif

namespace XmlTest {

void run_all()
{
    std::cout << "========== Running XML Tests ==========" << std::endl;
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
        std::cout << "[XML] Testing AllQtTypes struct..." << std::endl;
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

        std::string    xml = XML::obj_to_string(original);
        Qt::AllQtTypes parsed = XML::string_to_obj<Qt::AllQtTypes>(xml);

        assert(original == parsed);
        std::cout << "[XML] AllQtTypes struct passed." << std::endl;
    }
#else
    std::cout << "Qt support disabled. Skipping Qt tests." << std::endl;
#endif
    std::cout << "========== XML Tests Finished ==========" << std::endl;
}

} // namespace XmlTest

#endif // XML_TEST_H
