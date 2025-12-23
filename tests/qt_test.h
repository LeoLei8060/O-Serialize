#ifndef QT_TEST_H
#define QT_TEST_H

#ifdef O_SERIALIZE_USE_QT

#include "o_serialize/json.h"
#include <cassert>
#include <iostream>
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

using namespace OSerialize;

namespace QtTest {

void test_qstring()
{
    std::cout << "Testing QString..." << std::endl;
    QString     original = "Hello Qt";
    std::string json = JSON::obj_to_string(original);
    QString     parsed = JSON::string_to_obj<QString>(json);
    assert(original == parsed);
}

void test_qstringlist()
{
    std::cout << "Testing QStringList..." << std::endl;
    QStringList original = {"A", "B", "C"};
    std::string json = JSON::obj_to_string(original);
    QStringList parsed = JSON::string_to_obj<QStringList>(json);
    assert(original == parsed);
}

void test_qvector()
{
    std::cout << "Testing QVector..." << std::endl;
    QVector<int> original = {10, 20, 30};
    std::string  json = JSON::obj_to_string(original);
    QVector<int> parsed = JSON::string_to_obj<QVector<int>>(json);
    assert(original == parsed);
}

void test_qlist()
{
    std::cout << "Testing QList..." << std::endl;
    QList<double> original = {1.5, 2.5};
    std::string   json = JSON::obj_to_string(original);
    QList<double> parsed = JSON::string_to_obj<QList<double>>(json);
    assert(original == parsed);
}

void test_qstack()
{
    std::cout << "Testing QStack..." << std::endl;
    QStack<int> original;
    original.push(1);
    original.push(2);
    std::string json = JSON::obj_to_string(original);
    QStack<int> parsed = JSON::string_to_obj<QStack<int>>(json);
    assert(original == parsed);
}

void test_qqueue()
{
    std::cout << "Testing QQueue..." << std::endl;
    QQueue<int> original;
    original.enqueue(1);
    original.enqueue(2);
    std::string json = JSON::obj_to_string(original);
    QQueue<int> parsed = JSON::string_to_obj<QQueue<int>>(json);
    assert(original == parsed);
}

void test_qset()
{
    std::cout << "Testing QSet..." << std::endl;
    QSet<int>   original = {1, 2, 3};
    std::string json = JSON::obj_to_string(original);
    QSet<int>   parsed = JSON::string_to_obj<QSet<int>>(json);
    assert(original == parsed);
}

void test_qmap()
{
    std::cout << "Testing QMap..." << std::endl;
    QMap<QString, int> original;
    original.insert("one", 1);
    original.insert("two", 2);
    std::string        json = JSON::obj_to_string(original);
    QMap<QString, int> parsed = JSON::string_to_obj<QMap<QString, int>>(json);
    assert(original == parsed);
    assert((original == JSON::string_to_obj<QMap<QString, int>>(json))); // assert宏导致异常，需要添加()来解决
}

void test_qhash()
{
    std::cout << "Testing QHash..." << std::endl;
    QHash<QString, int> original;
    original.insert("key1", 100);
    original.insert("key2", 200);
    std::string         json = JSON::obj_to_string(original);
    QHash<QString, int> parsed = JSON::string_to_obj<QHash<QString, int>>(json);
    assert(original == parsed);
}

void test_qdate()
{
    std::cout << "Testing QDate..." << std::endl;
    QDate       original = QDate::currentDate();
    std::string json = JSON::obj_to_string(original);
    QDate       parsed = JSON::string_to_obj<QDate>(json);
    assert(original == parsed);
}

void test_qtime()
{
    std::cout << "Testing QTime..." << std::endl;
    // QTime stores milliseconds but JSON format is ISODate (HH:mm:ss) usually, check impl.
    // If ISODate includes MS, it's fine. If not, precision loss.
    // Implementation uses Qt::ISODate.
    // QTime::toString(Qt::ISODate) usually includes MS if not zero?
    // Let's set time to something safe (no MS) or check.
    QTime       original(12, 34, 56);
    std::string json = JSON::obj_to_string(original);
    QTime       parsed = JSON::string_to_obj<QTime>(json);
    assert(original == parsed);
}

void test_qdatetime()
{
    std::cout << "Testing QDateTime..." << std::endl;
    QDateTime   original(QDate(2023, 1, 1), QTime(12, 0, 0));
    std::string json = JSON::obj_to_string(original);
    QDateTime   parsed = JSON::string_to_obj<QDateTime>(json);
    assert(original == parsed);
}

void test_qpoint()
{
    std::cout << "Testing QPoint..." << std::endl;
    QPoint      original(10, 20);
    std::string json = JSON::obj_to_string(original);
    QPoint      parsed = JSON::string_to_obj<QPoint>(json);
    assert(original == parsed);
}

void test_qrect()
{
    std::cout << "Testing QRect..." << std::endl;
    QRect       original(0, 0, 100, 200);
    std::string json = JSON::obj_to_string(original);
    QRect       parsed = JSON::string_to_obj<QRect>(json);
    assert(original == parsed);
}

void test_qvariant()
{
    std::cout << "Testing QVariant..." << std::endl;
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
    std::cout << "Testing QSharedPointer..." << std::endl;
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
        return str == other.str && strList == other.strList && vec == other.vec && list == other.list
               && map == other.map && date == other.date && dt == other.dt && pt == other.pt && rect == other.rect
               && var == other.var
               && ((ptr.isNull() && other.ptr.isNull()) || (!ptr.isNull() && !other.ptr.isNull() && *ptr == *other.ptr));
    }
};
} // namespace QtTest

O_SERIALIZE_STRUCT(QtTest::AllQtTypes, str, strList, vec, list, map, date, dt, pt, rect, var, ptr, type);

namespace QtTest {
void test_all_qt_types()
{
    std::cout << "Testing AllQtTypes struct..." << std::endl;
    AllQtTypes original;
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
    original.type = AllQtTypes::kTypeB;

    std::string json = JSON::obj_to_string(original);
    AllQtTypes  parsed = JSON::string_to_obj<AllQtTypes>(json);

    assert(original == parsed);
    std::cout << "AllQtTypes struct passed." << std::endl;
}

void run_all()
{
    test_qstring();
    test_qstringlist();
    test_qvector();
    test_qlist();
    test_qstack();
    test_qqueue();
    test_qset();
    test_qmap();
    test_qhash();
    test_qdate();
    test_qtime();
    test_qdatetime();
    test_qpoint();
    test_qrect();
    test_qvariant();
    test_qsharedpointer();
    test_all_qt_types();
}
} // namespace QtTest

#else
namespace QtTest {
void run_all()
{
    std::cout << "Qt support disabled. Skipping Qt tests." << std::endl;
}
} // namespace QtTest
#endif

#endif
