#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

// 定义 Person 类
class Person {
public:
    std::string name;
    int age;
    Person(const std::string& n, int a) : name(n), age(a) {}
    
    // 重载相等运算符
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// size_t hasher(const Person& p) {//hash函数，得到hash码
// 	return hash<string>()(p.firstname) + hash<string>()(p.lastname) + hash<int>()(p.age);
// }

// 自定义哈希函数 - 故意制造冲突
//Custom Hash Functions for C++ Unordered Containers
// 方式三：使用偏特化实现hash函数
// 因为C++标准库已经提供了hash的类模板，并且也提供了int、string、char等偏特化版本，
// 所以这里可以直接对自定义类型做一个偏特化版本。
namespace std {
    template <>
    struct hash<Person> {
        size_t operator()(const Person& p) const {
            // 简单地只使用 age 作为哈希（会产生冲突）
            return hash<int>()(p.age);
        } 
    };
}

int main() {
    // 创建哈希对象
    std::hash<Person> hasher;

    // 打印几个具有相同年龄的人的哈希值
    cout << "Hash values:" << endl;
    cout << "Alice(25): " << hasher(Person("Alice", 25)) << endl;
    cout << "Bob(25): " << hasher(Person("Bob", 25)) << endl;
    cout << "Charlie(25): " << hasher(Person("Charlie", 25)) << endl;

    // 创建 unordered_map
    //https://en.cppreference.com/w/cpp/container/unordered_map
    //     template<
    //     class Key,
    //     class T,
    //     class Hash = std::hash<Key>,
    //     class KeyEqual = std::equal_to<Key>,
    //     class Allocator = std::allocator<std::pair<const Key, T>>
    // > class unordered_map;

    unordered_map<Person, string> employees;
    employees[Person("Alice", 25)] = "HR";// 自定义哈希函数 - 故意制造冲突
    employees[Person("Bob", 25)] = "Engineering";// 自定义哈希函数 - 故意制造冲突
    employees[Person("Charlie", 25)] = "Marketing";// 自定义哈希函数 - 故意制造冲突
    employees[Person("Troy", 25)] = "Marketing"; // 自定义哈希函数 - 故意制造冲突
    employees[Person("Charlie", 25)] = "Marketing"; //重复元素

    // 打印所有员工信息
    for (const auto& pair : employees) {
        cout << "Name: " << pair.first.name 
             << ", Age: " << pair.first.age 
             << ", Department: " << pair.second << endl;
    }

    // 打印哈希表的状态
    cout << "\nHash table stats:" << endl;
    cout << "Bucket count: " << employees.bucket_count() << endl;
    //计算哈希表中的存储桶数
    //Returns the number of buckets in the container.

    cout << "Load factor: " << employees.load_factor() << endl; 
    //计算哈希表中每个存储桶的平均元素数。
    //Returns the average number of elements per bucket,



    return 0;
}
