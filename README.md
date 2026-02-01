# O-Serialize

**O-Serialize** 是一个轻量级、非侵入式的 C++ 序列化/反序列化框架，支持 JSON、XML 和 INI 三种常见格式。它旨在简化 C++ 对象与数据格式之间的转换，并提供对 STL 和 Qt 数据类型的原生支持。

## ✨ 特性

*   **多格式支持**：统一的 API 接口支持 JSON (基于 RapidJSON)、XML (基于 TinyXML2) 和 INI (基于 inipp)。
*   **非侵入式反射**：通过宏定义即可完成结构体成员的注册，无需修改原有结构体定义。
*   **STL 容器支持**：支持 `std::vector`, `std::list`, `std::map`, `std::deque`, `std::set` 等标准容器。
*   **复杂类型支持**：支持 `std::pair`, `std::tuple`, `std::shared_ptr`, `std::variant` 等高级类型。
*   **Qt 深度集成**：可选开启 Qt 支持，完美兼容 `QString`, `QList`, `QMap`, `QHash`, `QDateTime`, `QRect`, `QPoint`, `QVariant` 等 Qt 类型。
*   **简单易用**：提供统一的 `obj_to_string` / `string_to_obj` 和 `obj_to_file` / `file_to_obj` 接口。

## 📦 依赖

*   **RapidJSON** (已包含在 `3rdparty`)
*   **TinyXML2** (已包含在 `3rdparty`)
*   **inipp** (已包含在 `3rdparty`)
*   **Qt** (可选，需安装 Qt5 或 Qt6)

## 🛠️ 集成与编译

项目使用 CMake 构建。

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

如果在 Qt 环境下使用，请确保 CMake 能找到 Qt 安装路径，并开启 Qt 支持（默认自动检测）：

```cmake
# CMakeLists.txt 中
add_definitions(-DO_SERIALIZE_USE_QT)
```

## 🚀 快速开始

### 1. 定义结构体并注册

使用 `O_SERIALIZE_STRUCT` 宏注册需要序列化的成员。

```cpp
#include "o_serialize/o_serialize.h"
#include <string>
#include <vector>

struct User {
    std::string name;
    int age;
    std::vector<std::string> hobbies;
    
    // 支持嵌套结构体（需同样注册）
    struct Address {
        std::string city;
        std::string street;
    } addr;
};

// 注册反射信息（宏需在全局命名空间使用）
O_SERIALIZE_STRUCT(User::Address, city, street)
O_SERIALIZE_STRUCT(User, name, age, hobbies, addr)
```

### 2. 序列化与反序列化

#### JSON 格式

```cpp
#include "o_serialize/json.h"

void test_json() {
    User user;
    user.name = "Alice";
    user.age = 25;
    user.hobbies = {"Coding", "Reading"};
    user.addr = {"New York", "5th Avenue"};

    // 序列化为字符串
    std::string jsonStr = OSerialize::JSON::obj_to_string(user);
    std::cout << jsonStr << std::endl;

    // 反序列化
    User parsedUser = OSerialize::JSON::string_to_obj<User>(jsonStr);
    
    // 文件读写
    OSerialize::JSON::obj_to_file(user, "user.json");
    User fileUser = OSerialize::JSON::file_to_obj<User>("user.json");
}
```

#### XML 格式

```cpp
#include "o_serialize/xml.h"

void test_xml() {
    User user = ...;
    // 接口与 JSON 一致
    std::string xmlStr = OSerialize::XML::obj_to_string(user, "UserRoot"); // 可指定根节点名称
    User parsedUser = OSerialize::XML::string_to_obj<User>(xmlStr, "UserRoot");
}
```

#### INI 格式

```cpp
#include "o_serialize/ini.h"

void test_ini() {
    User user = ...;
    // INI 格式适合简单的配置结构
    std::string iniStr = OSerialize::INI::obj_to_string(user);
    User parsedUser = OSerialize::INI::string_to_obj<User>(iniStr);
}
```

## 🧩 支持的类型列表

### 基础类型
*   `int`, `double`, `float`, `bool`, `long`, `short`, `unsigned ...`
*   `std::string`, `char*`
*   `enum` (作为整数处理)

### STL 容器与类型
*   `std::vector<T>`
*   `std::list<T>`
*   `std::deque<T>`
*   `std::set<T>`
*   `std::map<K, V>` (Key 需为 string 或可转换为 string)
*   `std::pair<K, V>`
*   `std::tuple<...>`
*   `std::shared_ptr<T>` (支持空指针检测)
*   `std::variant<...>`

### Qt 类型 (需开启 `O_SERIALIZE_USE_QT`)
*   **字符串/容器**: `QString`, `QStringList`, `QList`, `QVector`, `QQueue`, `QStack`, `QSet`
*   **映射**: `QMap`, `QHash`
*   **时间**: `QDate`, `QTime`, `QDateTime`
*   **几何**: `QPoint`, `QPointF`, `QSize`, `QSizeF`, `QRect`, `QRectF`
*   **其他**: `QVariant`, `QColor`, `QByteArray`
*   **智能指针**: `QSharedPointer`, `QScopedPointer`

## 📄 许可证

本项目开源，具体许可证请查看 LICENSE 文件。
