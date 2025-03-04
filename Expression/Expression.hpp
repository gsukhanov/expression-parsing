#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>

// Вспомогательныые типы - перечисления, чтобы не плодить еще больше классов
enum class NodeKind : char {op, func, var, val, head};
enum class FunctionType : char {sin, cos, ln, exp};
enum class OperationType : char {add, sub, mult, div, pow};


// Виртуальный базовый класс ноды дерева выражений.
// Содержит одно поле - вид ноды, чтобы не проверять кастом.
// Пока что я скорее верю, что не будет создан Operation с kind = var, например. В дальнейшем может исправлю.
template <typename T> class Node {
    public:
        NodeKind kind;
        Node() = default;
        virtual Node<T>& substitute(std::string __name, T __value) = 0;
        virtual T calculate() = 0;
        virtual std::string to_string() = 0;
        virtual ~Node() = default;
};

// Подкласс головы - вспомогательный, пригодится при вычислениях, так как в дереве удобно работать с потомками.
template <typename T> class Head : public Node<T> {
    public:
        Node<T> *next;
        Head();
        Head(Node<T> *next);
        Head<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Head() = default;
};

// Далее идут подклассы ноды: операция, функция, переменная и число.

// У операции есть три поля: тип операции, левый и правый операнды - указатели на две ноды.
template <typename T> class Operation : public Node<T> {
    public:
        OperationType type;
        Node<T> *left;
        Node<T> *right;
        Operation();
        Operation(const Operation<T> &other) = default;
        Operation(Operation<T> &&other) = default;
        Operation(OperationType __type, Node<T> *__left, Node<T> *__right); 
        Operation<T>& operator=(Operation<T> &other) = default;
        Operation<T>& operator=(Operation<T> &&other) = default;
        Operation<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Operation() = default;
};

// У функции есть два поля: тип и аргумент - указатель на ноду
template <typename T> class Function : public Node<T> {
    public:
        FunctionType type;
        Node<T> *arg;
        Function();
        Function(const Function<T> &other) = default;
        Function(Function<T> &&other) = default;
        Function(FunctionType __type, Node<T> *__arg);
        Function<T>& operator=(Function<T> &other) = default;
        Function<T>& operator=(Function<T> &&other) = default;
        Function<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Function() = default;
};

// У переменной есть одно поле: её название
template <typename T> class Variable : public Node<T> {
    public:
        std::string name;
        Variable();
        Variable(const Variable<T> &other) = default;
        Variable(Variable<T> &&other) = default;
        Variable(std::string __name);
        Variable<T>& operator=(Variable<T> &other) = default;
        Variable<T>& operator=(Variable<T> &&other) = default;
        Variable<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Variable() = default;
};

// У числа есть одно поле: его значение
template <typename T> class Value : public Node<T> {
    public:
        T value;
        Value();
        Value(const Value<T> &other) = default;
        Value(Value<T> &&other) = default;
        Value(T __value);
        Value<T>& operator=(Value<T> &other) = default;
        Value<T>& operator=(Value<T> &&other) = default;
        Value<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Value() = default;
};

// Основной класс - выражение. Именно с ним и работает пользователь.
// Он содержит указатель на вершину дерева выражений и множество называний переменных.
template <typename T> class Expression {
    private:
        std::unordered_set<std::string> variables = {};
    public:
        Head<T> *head;
        Expression() = default;
        Expression(T value);
        Expression(std::string var);
        Expression(Head<T> *__head, std::unordered_set<std::string> __variables);
        Expression(const Expression<T> &other) = default;
        Expression(Expression<T> &&other) = default;
        //Expression<T>(std::string input);
        Expression<T>& operator=(Expression<T> &other) = default;
        Expression<T>& operator=(Expression<T> &&other) = default;
        ~Expression() = default;
        Expression<T>& self_substitute(std::string __name, T __value);
        std::unordered_set<std::string> get_variables();
        Expression<T> substitute(std::string __name, T __value) const;
        T calculate(std::vector<std::string> vars, std::vector<T> vals) const;
        std::string to_string();
        Expression<T>& operator +=(const Expression<T> &other);
        Expression<T> operator +(const Expression<T> &other) const;
        Expression<T>& operator -=(const Expression<T> &other);
        Expression<T> operator -(const Expression<T> &other) const;
        Expression<T>& operator *=(const Expression<T> &other);
        Expression<T> operator *(const Expression<T> &other) const;
        Expression<T>& operator /=(const Expression<T> &other);
        Expression<T> operator /(const Expression<T> &other) const;
        Expression<T>& operator ^=(const Expression<T> &other);
        Expression<T> operator ^(const Expression<T> &other) const;
};

template <typename T> std::ostream& operator<<(std::ostream &out, Expression<T> e);
template <typename T> Expression<T> sin(Expression<T> e);
template <typename T> Expression<T> cos(Expression<T> e);
template <typename T> Expression<T> ln(Expression<T> e);
template <typename T> Expression<T> exp(Expression<T> e);