#ifndef EXPRESSION_HEADER
#define EXPRESSION_HEADER
#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <complex>
#include <cmath>

// Вспомогательныые типы - перечисления, чтобы не плодить еще больше классов
enum class NodeKind : char {op, func, var, val, head};
enum class FunctionType : char {sin, cos, ln, exp};
enum class OperationType : char {add, sub, mult, div, pow};
enum class Sign : bool {pos, neg};


// Виртуальный базовый класс ноды дерева выражений.
// Содержит одно поле - вид ноды, чтобы не проверять кастом.
// Пока что я скорее верю, что не будет создан Operation с kind = var, например. В дальнейшем может исправлю.
template <typename T> class Node {
    public:
        NodeKind kind;
        Node() = default;
        virtual std::shared_ptr<Node<T>> clone() = 0;
        virtual Node<T>& substitute(std::string __name, T __value) = 0;
        virtual T calculate() = 0;
        virtual std::string to_string() = 0;
        virtual ~Node() = default;
};

// Подкласс головы - вспомогательный, пригодится при вычислениях, так как в дереве удобно работать с потомками.
template <typename T> class Head : public Node<T> {
    public:
        std::shared_ptr<Node<T>> next;
        Head();
        Head(const Head& other);
        Head(Head&& other);
        Head(std::shared_ptr<Node<T>> next);
        std::shared_ptr<Node<T>> clone() override;
        Head<T>& operator=(const Head& other);
        Head<T>& operator=(Head&& other);
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
        std::shared_ptr<Node<T>> left;
        std::shared_ptr<Node<T>> right;
        Operation();
        Operation(const Operation<T> &other);
        Operation(Operation<T> &&other);
        Operation(OperationType __type, std::shared_ptr<Node<T>> __left, std::shared_ptr<Node<T>> __right); 
        std::shared_ptr<Node<T>> clone() override;
        Operation<T>& operator=(const Operation<T> &other);
        Operation<T>& operator=(Operation<T> &&other);
        Operation<T>& substitute(std::string __name, T __value) override;
        T calculate() override;
        std::string to_string() override;
        ~Operation() = default;
};

// У функции есть два поля: тип и аргумент - указатель на ноду
template <typename T> class Function : public Node<T> {
    public:
        FunctionType type;
        std::shared_ptr<Node<T>> arg;
        Function();
        Function(const Function<T> &other);
        Function(Function<T> &&other);
        Function(FunctionType __type, std::shared_ptr<Node<T>> __arg);
        std::shared_ptr<Node<T>> clone() override;
        Function<T>& operator=(const Function<T> &other);
        Function<T>& operator=(Function<T> &&other);
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
        std::shared_ptr<Node<T>> clone() override;
        Variable<T>& operator=(Variable<T> &other);
        Variable<T>& operator=(Variable<T> &&other);
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
        std::shared_ptr<Node<T>> clone() override;
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
        std::shared_ptr<Head<T>> head;
        Expression() = default;
        Expression(T value);
        Expression(std::string var);
        Expression(std::shared_ptr<Head<T>> __head, std::unordered_set<std::string> __variables);
        Expression(const Expression<T> &other);
        Expression(Expression<T> &&other);
        Expression<T>& operator=(const Expression<T> &other);
        Expression<T>& operator=(Expression<T> &&other);
        Expression<T>& simplify();
        Expression<T>& self_substitute(std::string __name, T __value);
        std::unordered_set<std::string> get_variables() const;
        Expression<T> substitute(std::string __name, T __value) const;
        Expression<T> differentiate(std::string __name) const;
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
        ~Expression() = default;
        void display_variables() const;
};

template <typename T> std::ostream& operator<<(std::ostream &out, Expression<T> e);
template <typename T> Expression<T> sin(Expression<T> e);
template <typename T> Expression<T> cos(Expression<T> e);
template <typename T> Expression<T> ln(Expression<T> e);
template <typename T> Expression<T> exp(Expression<T> e);

// Вспомогательная функция дифференцирования ноды по указателю.
template <typename T> std::shared_ptr<Node<T>> diff_func(std::shared_ptr<Node<T>> node, std::string __name);

// Вспомогательная функция упрощения выражения.
template <typename T> std::shared_ptr<Node<T>> simpl_func(std::shared_ptr<Node<T>> node);

//Вспомогательные ункции парсинга.
void skip_spaces(std::string::iterator *it, std::string::iterator end);
long double parse_number(std::string::iterator *it, std::string::iterator end);
std::string parse_string(std::string::iterator *it, std::string::iterator end);
void skip_all(std::string::iterator *it, std::string::iterator end);
void find_end(std::string::iterator *it, std::string::iterator end);

//Парсинг действительных выражений.
std::shared_ptr<Node<long double>> parse_real(std::string::iterator *it, std::string::iterator end, std::unordered_set<std::string> *vars);

//Парсинг комплексных выражений.
std::shared_ptr<Node<std::complex<long double>>> parse_complex(std::string::iterator *it, std::string::iterator end, std::unordered_set<std::string> *vars);

//Функции создания выражения на основе строки.
Expression<long double> construct_real(std::string input);
Expression<std::complex<long double>> construct_complex(std::string input);

template <typename T> void Expression<T>::display_variables() const {
    for (auto it = variables.begin(); it != variables.end(); it++) std::cout << *it << " ";
}

bool iszero(long double number) {
    return std::abs(number) < 1e-6;
}

bool iszero(std::complex<long double> number) {
    return std::abs(number) < 1e-6;
}

bool isone(long double number) {
    return std::abs(number - 1) < 1e-6;
}

bool isone(std::complex<long double> number) {
    return isone(number.real()) && iszero(number.imag());
}

std::string two_string(long double number) {
    char a[20];
    sprintf(a, "%.15g", number);
    return std::string(a);
}

template<typename T> std::string two_string(std::complex<T> number) {
    if (iszero(number.imag())) return two_string(number.real());
    if (iszero(number.real())) return two_string(number.imag()) + "i";
    return "(" + two_string(number.real()) + " + " + two_string(number.imag()) + "i)";
}

//---------------------------------------------------------------------------------------------------------------
// Конструкторы
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>::Expression(T value) {
    std::shared_ptr<Value<T>> next = std::make_shared<Value<T>>(value);
    std::shared_ptr<Head<T>> __head = std::make_shared<Head<T>>(next);
    head = __head;
}

template <typename T> Expression<T>::Expression(std::string var) {
    variables.insert(var);
    std::shared_ptr<Variable<T>> next = std::make_shared<Variable<T>>(var);
    std::shared_ptr<Head<T>> __head = std::make_shared<Head<T>>(next);
    head = __head;
}

template <typename T> Expression<T>::Expression(const Expression<T>& other){
    head = std::make_shared<Head<T>>(other.head->next->clone());
    variables = other.get_variables();
}

template <typename T> Expression<T>::Expression(Expression<T>&& other){
    head = other.head;
    other.head = nullptr;
    variables = other.get_variables();
}

template <typename T> Expression<T>::Expression(std::shared_ptr<Head<T>> __head, std::unordered_set<std::string> __variables) {
    head = __head;
    variables = __variables;
}

template <typename T> Head<T>::Head() {
    Node<T>::kind = NodeKind::head;
}

template <typename T> Head<T>::Head(const Head<T>& other) {
    Node<T>::kind = NodeKind::head;
    next = other.next->clone();
}

template <typename T> Head<T>::Head(Head<T>&& other) {
    Node<T>::kind = NodeKind::head;
    next = other.next;
    other.next = nullptr;
}

template <typename T> Head<T>::Head(std::shared_ptr<Node<T>> __next) {
    Node<T>::kind = NodeKind::head;
    next = __next;
}

template <typename T> Operation<T>::Operation() {
    Node<T>::kind = NodeKind::op;
}

template <typename T> Operation<T>::Operation(const Operation<T>& other) {
    Node<T>::kind = NodeKind::op;
    type = other.type;
    left = other.left->clone();
    right = other.right->clone();
}

template <typename T> Operation<T>::Operation(Operation<T>&& other) {
    Node<T>::kind = NodeKind::op;
    type = other.type;
    left = other.left;
    right = other.right;
    other.left = nullptr;
    other.right = nullptr;
}

template <typename T> Operation<T>::Operation(OperationType __type, std::shared_ptr<Node<T>> __left, std::shared_ptr<Node<T>> __right) {
    Node<T>::kind = NodeKind::op;
    type = __type;
    left = __left;
    right = __right;
}

template <typename T> Function<T>::Function() {
    Node<T>::kind = NodeKind::func;
}

template <typename T> Function<T>::Function(FunctionType __type, std::shared_ptr<Node<T>> __arg) {
    Node<T>::kind = NodeKind::func;
    type = __type;
    arg = __arg;
}

template <typename T> Function<T>::Function(const Function<T>& other) {
    Node<T>::kind = NodeKind::func;
    type = other.type;
    arg = other.arg->clone();
}

template <typename T> Function<T>::Function(Function<T>&& other) {
    Node<T>::kind = NodeKind::func;
    type = other.type;
    arg = other.arg;
    other.arg = nullptr;
}

template <typename T> Variable<T>::Variable() {
    Node<T>::kind = NodeKind::var;
}

template <typename T> Variable<T>::Variable(std::string __name) {
    Node<T>::kind = NodeKind::var;
    name = __name;
}

template <typename T> Value<T>::Value() {
    Node<T>::kind = NodeKind::val;
}

template <typename T> Value<T>::Value(T __value) {
    Node<T>::kind = NodeKind::val;
    value = __value;
}

//---------------------------------------------------------------------------------------------------------------
// Функции клонирования нод
//---------------------------------------------------------------------------------------------------------------

template <typename T> std::shared_ptr<Node<T>> Head<T>::clone() {
    std::cerr << "Something went wrong, head is an inner node of the tree.\n";
    exit(-1);
    return nullptr;
}

template <typename T> std::shared_ptr<Node<T>> Operation<T>::clone() {
    std::shared_ptr<Node<T>> copy = std::make_shared<Operation<T>>(type, left->clone(), right->clone());
    return copy;
}

template <typename T> std::shared_ptr<Node<T>> Function<T>::clone() {
    std::shared_ptr<Node<T>> copy = std::make_shared<Function<T>>(type, arg->clone());
    return copy;
}

template <typename T> std::shared_ptr<Node<T>> Variable<T>::clone() {
    std::shared_ptr<Node<T>> copy = std::make_shared<Variable<T>>(name);
    return copy;
}

template <typename T> std::shared_ptr<Node<T>> Value<T>::clone() {
    std::shared_ptr<Node<T>> copy = std::make_shared<Value<T>>(value);
    return copy;
}

//---------------------------------------------------------------------------------------------------------------
// Операторы копирования и перемещения
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>& Expression<T>::operator=(const Expression<T>& other){
    head = std::make_shared<Head<T>>(other->head);
    variables = other.get_variables();
    return *this;
}

template <typename T> Expression<T>& Expression<T>::operator=(Expression<T>&& other){
    head = other.head;
    other.head = nullptr;
    variables = other.get_variables();
    return *this;
}

template <typename T> Head<T>& Head<T>::operator=(const Head& other) {
    Node<T>::kind = NodeKind::head;
    next = other.next->clone();
    return *this;
}

template <typename T> Head<T>& Head<T>::operator=(Head&& other) {
    Node<T>::kind = NodeKind::head;
    next = other.next;
    other.next = nullptr;
    return *this;
}

template <typename T> Operation<T>& Operation<T>::operator=(const Operation<T>& other) {
    Node<T>::kind = NodeKind::op;
    left = other.left->clone();
    right = other.right->clone();
    return *this;
}

template <typename T> Operation<T>& Operation<T>::operator=(Operation<T>&& other) {
    Node<T>::kind = NodeKind::op;
    left = other.left;
    right = other.right;
    other.left = nullptr;
    other.right = nullptr;
    return *this;
}

template <typename T> Function<T>& Function<T>::operator=(const Function<T>& other) {
    Node<T>::kind = NodeKind::var;
    arg = other.arg->clone();
    return *this;
}

template <typename T> Function<T>& Function<T>::operator=(Function<T>&& other) {
    Node<T>::kind = NodeKind::var;
    arg = other.arg;
    other.arg = nullptr;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------
// Функции преобразования в строку и вывода (пока что ставлю лишние скобки, позже улучшу)
//---------------------------------------------------------------------------------------------------------------

template <typename T> std::string Expression<T>::to_string(){
    return head->to_string();
}

template <typename T> std::string Head<T>::to_string(){
    return next->to_string();
}

template <typename T> std::string Operation<T>::to_string(){
    std::string sym;
    if (type == OperationType::add) {
        return left->to_string() + " + " + right->to_string();
    }
    if (type == OperationType::sub) {
        if (right->kind != NodeKind::op) return left->to_string() + " - " + right->to_string();
        if (std::dynamic_pointer_cast<Operation<T>>(right)->type != OperationType::add 
        && std::dynamic_pointer_cast<Operation<T>>(right)->type != OperationType::sub) return left->to_string() + " - " + right->to_string();
        return left->to_string() + " - (" + right->to_string() + ")";
    }
    if (type == OperationType::mult) {
        if (left->kind == NodeKind::val && (right->kind == NodeKind::var || right->kind == NodeKind::func)) return left->to_string() + right->to_string();
        if (left->kind != NodeKind::op) {
            if (right->kind != NodeKind::op) {
                return left->to_string() + " * " + right->to_string();
            }
            else if (std::dynamic_pointer_cast<Operation<T>>(right)->type == OperationType::mult) return left->to_string() + " * " + right->to_string();
            else return left->to_string() + " * (" + right->to_string() + ")";
        }
        else if (right->kind != NodeKind::op) {
            if (std::dynamic_pointer_cast<Operation<T>>(left)->type == OperationType::mult) return left->to_string() + " * " + right->to_string();
            else return "(" + left->to_string() + ") * " + right->to_string();
        }
        else if (std::dynamic_pointer_cast<Operation<T>>(left)->type == OperationType::mult 
            && std::dynamic_pointer_cast<Operation<T>>(right)->type == OperationType::mult) 
            return left->to_string() + " * " + right->to_string();
        else return "(" + left->to_string() + ") * (" + right->to_string() + ")";
    }
    if (type == OperationType::div) {
        if (left->kind != NodeKind::op) {
            if (right->kind != NodeKind::op) return left->to_string() + " / " + right->to_string();
            else return left->to_string() + " / (" + right->to_string() + ")";
        }
        else if (right->kind != NodeKind::op) return "(" + left->to_string() + ") / " + right->to_string();
        return "(" + left->to_string() + ") / (" + right->to_string() + ")";
    }
    if (type == OperationType::pow) {
        if (left->kind != NodeKind::op) {
            if (right->kind != NodeKind::op) return left->to_string() + " ^ " + right->to_string();
            else return left->to_string() + " ^ (" + right->to_string() + ")";
        }
        else if (right->kind != NodeKind::op) return "(" + left->to_string() + ") ^ " + right->to_string();
        return "(" + left->to_string() + ") ^ (" + right->to_string() + ")";
    }
    std::cerr << "Something went horribly wrong, an operation has no type!\n Its left and right operands are: " << left->to_string() << " " << right->to_string();
    exit(EXIT_FAILURE);
}

template <typename T> std::string Function<T>::to_string(){
    std::string sym;
    if (type == FunctionType::sin) sym = "sin(";
    if (type == FunctionType::cos) sym = "cos(";
    if (type == FunctionType::ln) sym = "ln(";
    if (type == FunctionType::exp) sym = "exp(";
    return sym + arg->to_string() + ")";
}

template <typename T> std::string Variable<T>::to_string(){
    return name;
}

template <typename T> std::string Value<T>::to_string(){
    return two_string(value);
}

template <typename T> std::ostream& operator<<(std::ostream &out, Expression<T> e) {
    out << e.to_string();
    return out;
}

//---------------------------------------------------------------------------------------------------------------
// Арифметические операции
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>& Expression<T>::operator +=(const Expression<T> &other) {
    std::shared_ptr<Expression<T>> copy = std::make_shared<Expression<T>>(other);
    std::shared_ptr<Operation<T>> op = std::make_shared<Operation<T>>(OperationType::add, head->next, copy->head->next);
    variables.merge(copy->get_variables());
    head->next = op;
    return *this;
}

template <typename T> Expression<T> Expression<T>::operator +(const Expression<T> &other) const {
    Expression<T> copy(*this);
    copy += other;
    return copy;
}

template <typename T> Expression<T>& Expression<T>::operator -=(const Expression<T> &other) {
    std::shared_ptr<Expression<T>> copy = std::make_shared<Expression<T>>(other);
    std::shared_ptr<Operation<T>> op = std::make_shared<Operation<T>>(OperationType::sub, head->next, copy->head->next);
    variables.merge(copy->get_variables());
    head->next = op;
    return *this;
}

template <typename T> Expression<T> Expression<T>::operator -(const Expression<T> &other) const {
    Expression<T> copy(*this);
    copy -= other;
    return copy;
}

template <typename T> Expression<T>& Expression<T>::operator *=(const Expression<T> &other) {
    std::shared_ptr<Expression<T>> copy = std::make_shared<Expression<T>>(other);
    std::shared_ptr<Operation<T>> op = std::make_shared<Operation<T>>(OperationType::mult, head->next, copy->head->next);
    variables.merge(copy->get_variables());
    head->next = op;
    return *this;
}

template <typename T> Expression<T> Expression<T>::operator *(const Expression<T> &other) const {
    Expression<T> copy(*this);
    copy *= other;
    return copy;
}

template <typename T> Expression<T>& Expression<T>::operator /=(const Expression<T> &other) {
    std::shared_ptr<Expression<T>> copy = std::make_shared<Expression<T>>(other);
    std::shared_ptr<Operation<T>> op = std::make_shared<Operation<T>>(OperationType::div, head->next, copy->head->next);
    variables.merge(copy->get_variables());
    head->next = op;
    return *this;
}

template <typename T> Expression<T> Expression<T>::operator /(const Expression<T> &other) const {
    Expression<T> copy(*this);
    copy /= other;
    return copy;    
}

template <typename T> Expression<T>& Expression<T>::operator ^=(const Expression<T> &other) {
    std::shared_ptr<Expression<T>> copy = std::make_shared<Expression<T>>(other);
    std::shared_ptr<Operation<T>> op = std::make_shared<Operation<T>>(OperationType::pow, head->next, copy->head->next);
    variables.merge(copy->get_variables());
    head->next = op;
    return *this;
}

template <typename T> Expression<T> Expression<T>::operator ^(const Expression<T> &other) const {
    Expression<T> copy(*this);
    copy ^= other;
    return copy;    
}

//---------------------------------------------------------------------------------------------------------------
// Математические функции
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T> sin(Expression<T> e) {
    Expression<T> copy(e);
    std::shared_ptr<Function<T>> func = std::make_shared<Function<T>>(FunctionType::sin, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> cos(Expression<T> e) {
    Expression<T> copy(e);
    std::shared_ptr<Function<T>> func = std::make_shared<Function<T>>(FunctionType::cos, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> ln(Expression<T> e) {
    Expression<T> copy(e);
    std::shared_ptr<Function<T>> func = std::make_shared<Function<T>>(FunctionType::ln, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> exp(Expression<T> e) {
    Expression<T> copy(e);
    std::shared_ptr<Function<T>> func = std::make_shared<Function<T>>(FunctionType::exp, copy.head->next);
    copy.head->next = func;
    return *copy;
}

//---------------------------------------------------------------------------------------------------------------
// Функция упрощения
//---------------------------------------------------------------------------------------------------------------

template <typename T> std::shared_ptr<Node<T>> simpl_func(std::shared_ptr<Node<T>> node) {
    if (node->kind == NodeKind::head) {
        std::shared_ptr<Head<T>> head = std::dynamic_pointer_cast<Head<T>>(node);
        head->next = simpl_func(head->next);
    }
    else if (node->kind == NodeKind::func) {
        std::shared_ptr<Function<T>> function = std::dynamic_pointer_cast<Function<T>>(node);
        function->arg = simpl_func(function->arg);
        if (function->arg->kind == NodeKind::val) {
            std::shared_ptr<Node<T>> value = std::make_shared<Value<T>>(function->calculate());
            node = value;
        }
    }
    else if (node->kind == NodeKind::op) {
        std::shared_ptr<Operation<T>> operation = std::dynamic_pointer_cast<Operation<T>>(node);
        operation->left = simpl_func(operation->left);
        operation->right = simpl_func(operation->right);
        if (operation->right->kind == NodeKind::val) {
            std::shared_ptr<Value<T>> right_value = std::dynamic_pointer_cast<Value<T>>(operation->right);
            if (operation->type == OperationType::div && iszero(right_value->value)) {
                exit(EXIT_FAILURE);
            }
            else if (operation->left->kind == NodeKind::val) {
                std::shared_ptr<Node<T>> value = std::make_shared<Value<T>>(operation->calculate());
                node = value;
            }
            else if (isone(right_value->value)) {
                if (operation->type == OperationType::mult || 
                    operation->type == OperationType::pow || 
                    operation->type == OperationType::div) {
                    std::shared_ptr<Node<T>> left = operation->left->clone();
                    node = left;
                }
            }
            else if (iszero(right_value->value)) {
                if (operation->type == OperationType::add ||
                    operation->type == OperationType::sub) {
                    std::shared_ptr<Node<T>> left = operation->left->clone();
                    node = left;
                }
                if (operation->type == OperationType::mult) {
                    std::shared_ptr<Node<T>> zero = std::make_shared<Value<T>>((T)0);
                    node = zero;
                }
                if (operation->type == OperationType::pow) {
                    std::shared_ptr<Node<T>> one = std::make_shared<Value<T>>((T)1);
                    node = one;
                }
            }
        } 
        else if (operation->left->kind == NodeKind::val) {
            std::shared_ptr<Value<T>> left_value = std::dynamic_pointer_cast<Value<T>>(operation->left);
            if (isone(left_value->value)) {
                if (operation->type == OperationType::mult) {
                    std::shared_ptr<Node<T>> right = operation->right->clone();
                    node = right;
                }
                else if (operation->type == OperationType::pow) {
                    std::shared_ptr<Node<T>> one = std::make_shared<Value<T>>((T)1);
                    node = one;
                }
            }
            else if (iszero(left_value->value)) {
                if (operation->type == OperationType::add ||
                    operation->type == OperationType::sub) {
                    std::shared_ptr<Node<T>> right = operation->right->clone();
                    node = right;
                }
                else if (operation->type == OperationType::mult || 
                    operation->type == OperationType::pow || 
                    operation->type == OperationType::div) {
                    std::shared_ptr<Node<T>> zero = std::make_shared<Value<T>>((T)0);
                    node = zero;
                }
            }
            else if (isone(left_value->value)) {
                if (operation->type == OperationType::mult) {
                    std::shared_ptr<Node<T>> right = operation->right->clone();
                    node = right;
                }
                else if (operation->type == OperationType::pow) {
                    std::shared_ptr<Node<T>> one = std::make_shared<Value<T>>((T)1);
                    node = one;
                }
            }
        }
    }
    return node;
}

template <typename T> Expression<T>& Expression<T>::simplify() {
    head = std::dynamic_pointer_cast<Head<T>>(simpl_func(std::dynamic_pointer_cast<Node<T>>(head)));
    return *this;
}

//---------------------------------------------------------------------------------------------------------------
// Функции подстановки и вычисления
//---------------------------------------------------------------------------------------------------------------

template <typename T> std::unordered_set<std::string> Expression<T>::get_variables() const {
    return variables;
}

template <typename T> Expression<T>& Expression<T>::self_substitute(std::string __name, T __value) {
    auto pos = variables.begin();
    while(pos != variables.end() && *pos != __name) pos++;
    if (pos == variables.end()) {
        std::cerr <<"\"" << __name << "\" - no such variable!";
        exit(EXIT_FAILURE);
    }
    variables.erase(pos);
    head->substitute(__name, __value);
    return *this;
}

template <typename T> Expression<T> Expression<T>::substitute(std::string __name, T __value) const {
    Expression<T> copy = *this;
    copy.self_substitute(__name, __value);
    return copy;
}

template <typename T> T Expression<T>::calculate(std::vector<std::string> vars, std::vector<T> vals) const {
    if (vars.size() > vals.size()) {
        std::cerr << "More variables than values!";
        exit(EXIT_FAILURE);
    }
    else if (vars.size() < vals.size()) {
        std::cerr << "More values than variables!";
        exit(EXIT_FAILURE);
    }
    Expression<T> copy = *this;
    auto itval = vals.begin();
    for (auto it = vars.begin(); it < vars.end(); it++) {
        if (copy.get_variables().find(*it) != copy.get_variables().end()) {
            copy.self_substitute(*it, *itval);
        }
        else {
            std::cerr <<"\"" << *it << "\" - no such variable!";
            exit(EXIT_FAILURE);
        }
        itval++;
    }
    return copy.head->calculate();
}

template <typename T> Head<T>& Head<T>::substitute(std::string __name, T __value) {
    if (next->kind == NodeKind::var) {
        std::shared_ptr<Variable<T>> nextvar = std::dynamic_pointer_cast<Variable<T>>(next);
        if (nextvar->name == __name) {
            std::shared_ptr<Value<T>> __next = std::make_shared<Value<T>>(__value);
            next = __next;
        }
    }
    else next->substitute(__name, __value);
    return *this;
}

template <typename T> T Head<T>::calculate() {
    return next->calculate();
}

template <typename T> Operation<T>& Operation<T>::substitute(std::string __name, T __value) {
    if (left->kind == NodeKind::var) {
        std::shared_ptr<Variable<T>> leftvar = std::dynamic_pointer_cast<Variable<T>>(left);
        if (leftvar->name == __name) {
            std::shared_ptr<Value<T>> __left = std::make_shared<Value<T>>(__value);
            left = __left;
        }
    }
    else left->substitute(__name, __value);
    if (right->kind == NodeKind::var) {
        std::shared_ptr<Variable<T>> rightvar = std::dynamic_pointer_cast<Variable<T>>(right);
        if (rightvar->name == __name) {
            std::shared_ptr<Value<T>> __right = std::make_shared<Value<T>>(__value);
            right = __right;
        }
    }
    else right->substitute(__name, __value);
    return *this;
}

template <typename T> T Operation<T>::calculate() {
    if (type == OperationType::add) return left->calculate() + right->calculate();
    if (type == OperationType::sub) return left->calculate() - right->calculate();
    if (type == OperationType::mult) return left->calculate() * right->calculate();
    if (type == OperationType::div) return left->calculate() / right->calculate();
    if (type == OperationType::pow) return std::pow(left->calculate(), right->calculate());
    std::cerr << "Something went wrong, an operation has no type.\n";
    exit(EXIT_FAILURE);
}

template <typename T> Function<T>& Function<T>::substitute(std::string __name, T __value) {
    if (arg->kind == NodeKind::var) {
        std::shared_ptr<Variable<T>> argvar = std::dynamic_pointer_cast<Variable<T>>(arg);
        if (argvar->name == __name) {
            std::shared_ptr<Value<T>> __arg = std::make_shared<Value<T>>(__value);
            arg = __arg;
        }
    }
    else arg->substitute(__name, __value);
    return *this;
}

template <typename T> T Function<T>::calculate() {
    if (type == FunctionType::sin) return std::sin(arg->calculate());
    if (type == FunctionType::cos) return std::cos(arg->calculate());
    if (type == FunctionType::ln) return std::log(arg->calculate());
    if (type == FunctionType::exp) return std::exp(arg->calculate());
    std::cerr << "Something went wrong, a function has no type.\n";
    exit(EXIT_FAILURE);
}

template <typename T> Variable<T>& Variable<T>::substitute(std::string __name, T __value) {
    std::cerr << "Something went wrong, substitue function was called for a variable.\n";
    exit(EXIT_FAILURE);
    return *this;
}

template <typename T> T Variable<T>::calculate() {
    std::cerr << "Something went wrong, trying to calcualte a variable.\n" << "Variable name: " << name << "\n";
    exit(EXIT_FAILURE);
    return -1;
}

template <typename T> Value<T>& Value<T>::substitute(std::string __name, T __value) {
    return *this;
}

template <typename T> T Value<T>::calculate() {
    return value;
}

//---------------------------------------------------------------------------------------------------------------
// Функции дифференцирования:
//---------------------------------------------------------------------------------------------------------------


template <typename T> Expression<T> Expression<T>::differentiate(std::string __name) const {
    Expression<T> copy = Expression<T>(*this);
    copy.simplify();
    copy.head->next = diff_func(head->next, __name);
    copy.simplify();
    return copy;
}

template <typename T> std::shared_ptr<Node<T>> diff_func(std::shared_ptr<Node<T>> node, std::string __name) {
    if (node->kind == NodeKind::head) {
        std::shared_ptr<Head<T>> head = std::dynamic_pointer_cast<Head<T>>(node);
        head->next = diff_func(head->next, __name);
    }
    else if (node->kind == NodeKind::op) {
        std::shared_ptr<Operation<T>> operation = std::dynamic_pointer_cast<Operation<T>>(node);
        if (operation->type == OperationType::add ||
            operation->type == OperationType::sub) {
            operation->left = diff_func(operation->left, __name);
            operation->right = diff_func(operation->right, __name);
        }
        else if (operation->type == OperationType::mult) {
            if (operation->right->kind == NodeKind::val || 
                (operation->right->kind == NodeKind::var && 
                std::dynamic_pointer_cast<Variable<T>>(operation->right)->name != __name))
                operation->left = diff_func(operation->left, __name);
            else if (operation->left->kind == NodeKind::val || 
                (operation->left->kind == NodeKind::var && 
                std::dynamic_pointer_cast<Variable<T>>(operation->left)->name != __name))
                operation->right = diff_func(operation->right, __name);
            else {
                std::shared_ptr<Operation<T>> left = std::make_shared<Operation<T>>(OperationType::mult, diff_func(operation->left->clone(), __name), operation->right);
                std::shared_ptr<Operation<T>> right = std::make_shared<Operation<T>>(OperationType::mult, diff_func(operation->right->clone(), __name), operation->left);
                operation->left = left;
                operation->right = right;
                operation->type = OperationType::add;
            }
        }
        else if (operation->type == OperationType::div) {
            if (operation->right->kind == NodeKind::val || 
                (operation->right->kind == NodeKind::var && 
                std::dynamic_pointer_cast<Variable<T>>(operation->right)->name != __name)) 
                operation->left = diff_func(operation->left, __name);
            else {
                std::shared_ptr<Operation<T>> left = std::make_shared<Operation<T>>(OperationType::mult, diff_func(operation->left->clone(), __name), operation->right);
                std::shared_ptr<Operation<T>> right = std::make_shared<Operation<T>>(OperationType::mult, diff_func(operation->right->clone(), __name), operation->left);
                std::shared_ptr<Operation<T>> num = std::make_shared<Operation<T>>(OperationType::sub, left, right);
                std::shared_ptr<Operation<T>> denom = std::make_shared<Operation<T>>(OperationType::pow, operation->right->clone(), std::make_shared<Value<T>>(2));
                operation->left = num;
                operation->right = denom;
            }
        }
        else if (operation->type == OperationType::pow) {
            if (operation->right->kind == NodeKind::val) {
                std::shared_ptr<Value<T>> value = std::dynamic_pointer_cast<Value<T>>(operation->right);
                std::shared_ptr<Node<T>> left = value->clone();
                value->value -= 1;
                std::shared_ptr<Node<T>> inner = std::make_shared<Operation<T>>(OperationType::mult, left, node);
                std::shared_ptr<Node<T>> outer = std::make_shared<Operation<T>>(OperationType::mult, inner, diff_func(operation->left->clone(), __name));
                node = outer;
            }
            else {
                std::shared_ptr<Node<T>> logarithm = std::make_shared<Function<T>>(FunctionType::ln, operation->left);
                std::shared_ptr<Node<T>> in_power = std::make_shared<Operation<T>>(OperationType::mult, operation->right, logarithm);
                std::shared_ptr<Node<T>> outer = std::make_shared<Operation<T>>(OperationType::mult, diff_func(in_power, __name), node);
                node = outer;
            }
        }
    }
    else if (node->kind == NodeKind::func) {
        std::shared_ptr<Function<T>> function = std::dynamic_pointer_cast<Function<T>>(node);
        std::shared_ptr<Node<T>> arg = function->arg->clone();
        std::shared_ptr<Node<T>> inner = diff_func(function->arg->clone(), __name)->clone();
        if (function->type == FunctionType::sin) {
            std::shared_ptr<Function<T>> outer = std::make_shared<Function<T>>(FunctionType::cos, arg);
            std::shared_ptr<Operation<T>> diff = std::make_shared<Operation<T>>(OperationType::mult, inner, outer);
            node = diff;
        }
        else if (function->type == FunctionType::cos) {
            std::shared_ptr<Function<T>> sinus = std::make_shared<Function<T>>(FunctionType::sin, arg);
            std::shared_ptr<Operation<T>> outer = std::make_shared<Operation<T>>(OperationType::mult, std::make_shared<Value<T>>((T)-1), sinus);
            std::shared_ptr<Operation<T>> diff = std::make_shared<Operation<T>>(OperationType::mult, inner, outer);
            node = diff;
        }
        else if (function->type == FunctionType::ln) {
            std::shared_ptr<Operation<T>> diff = std::make_shared<Operation<T>>(OperationType::div, inner, arg);
            node = diff;
        }
        else if (function->type == FunctionType::exp) {
            std::shared_ptr<Operation<T>> diff = std::make_shared<Operation<T>>(OperationType::mult, inner, function);
            node = diff;
        }
    }
    else if (node->kind == NodeKind::var) {
        std::shared_ptr<Variable<T>> variable = std::dynamic_pointer_cast<Variable<T>>(node);
        if (variable->name == __name) node = std::make_shared<Value<T>>((T)1);
        else node = std::make_shared<Value<T>>((T)0);
    }
    else if (node->kind == NodeKind::val) node = std::make_shared<Value<T>>((T)0);
    return node;
}


//---------------------------------------------------------------------------------------------------------------
// Функции для парсинга:
//---------------------------------------------------------------------------------------------------------------

void skip_spaces(std::string::iterator *it, std::string::iterator end) {
    while (*it < end && **it == ' ') (*it)++;
}

long double parse_number(std::string::iterator *it, std::string::iterator end) {
    auto start = *it;
    while (*it < end && **it >= '0' && **it <= '9') (*it)++;
    if (**it == '.' && *it + 1 < end && *(*it + 1) <= '9' && *(*it + 1) >= '0' ) {
        (*it)++;
        while (**it > '0' && **it < '9' && *it < end) (*it)++;
    }
    return std::stold(std::string(start, *it));
}

std::string parse_string(std::string::iterator *it, std::string::iterator end) {
    auto start = *it;
    while (*it < end && ((**it >= 'a' && **it <= 'z') || (**it >= 'A' && ** it <= 'Z') || **it == '_' || (**it >= '0' && **it <= '9'))) (*it)++;
    return std::string(start, *it);
}

void skip_all(std::string::iterator *it, std::string::iterator end) {
    while (*it < end && **it != ')') {
        (*it)++;
        if (**it == '(') skip_all(it, end);
    }
}

void find_end(std::string::iterator *it, std::string::iterator end) {
    while (*it < end && **it != ' ') {
        if (**it == '(') skip_all(it, end);
        (*it)++;
    }
}

std::shared_ptr<Node<long double>> parse_real(std::string::iterator *it, std::string::iterator end, std::unordered_set<std::string> *vars) {
    std::shared_ptr<Node<long double>> current = nullptr;
    while (*it < end && **it && **it != ')') {
        if (**it == ' ') skip_spaces(it, end);
        else if (**it == '(') {
            (*it)++;
            current = parse_real(it, end, vars);
            (*it)++;
        }
        else if (**it >= '0' && **it <= '9') current = std::make_shared<Value<long double>>(parse_number(it, end));
        else if ((**it >= 'a' && **it <= 'z') || (**it >= 'A' && **it <= 'Z')) {
            std::string word = parse_string(it, end);
            if (word == "sin") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<long double>>(FunctionType::sin, parse_real(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "cos") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<long double>>(FunctionType::cos, parse_real(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "exp") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<long double>>(FunctionType::exp, parse_real(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "ln") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<long double>>(FunctionType::ln, parse_real(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else {
                vars->insert(word);
                current = std::make_shared<Variable<long double>>(word);
            }
        }
        else if (**it == '+') {
            (*it)++;
            skip_spaces(it, end);
            std::shared_ptr<Operation<long double>> operation = std::make_shared<Operation<long double>>(OperationType::add, current, parse_real(it, end, vars));
            current = operation;
        }
        else if (**it == '-') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<long double>> operation = std::make_shared<Operation<long double>>(OperationType::sub, current, parse_real(it, copy, vars));
            current = operation;
        }
        else if (**it == '*') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<long double>> operation = std::make_shared<Operation<long double>>(OperationType::mult, current, parse_real(it, copy, vars));
            current = operation;
        }
        else if (**it == '/') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<long double>> operation = std::make_shared<Operation<long double>>(OperationType::div, current, parse_real(it, copy, vars));
            current = operation;
        }
        else if (**it == '^') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<long double>> operation = std::make_shared<Operation<long double>>(OperationType::pow, current, parse_real(it, copy, vars));
            current = operation;
        }
        else {
            std::cerr << "Non-algebraic expression! Met wrong symbol:" << (int)**it << "\n";
            exit(EXIT_FAILURE);
        }
    }
    return current;
}

std::shared_ptr<Node<std::complex<long double>>> parse_complex(std::string::iterator *it, std::string::iterator end, std::unordered_set<std::string> *vars) {
    std::shared_ptr<Node<std::complex<long double>>> current = nullptr;
    while (*it < end && **it != ')') {
        if (**it == ' ') skip_spaces(it, end);
        else if (**it == '(') {
            (*it)++;
            current = parse_complex(it, end, vars);
            (*it)++;
        }
        else if (**it >= '0' && **it <= '9') {
            long double first = parse_number(it, end);
            if (**it == ' ') skip_spaces(it, end);
            if (**it == '+') {
                (*it)++;
                if (**it == ' ') skip_spaces(it, end);
                if (**it >= '0' && **it <= '9') {
                    long double second = parse_number(it, end);
                    if (**it == 'i') {
                        current = std::make_shared<Value<std::complex<long double>>>(std::complex<long double>(first, second));
                        (*it)++;
                        if (*it < end && ((**it >= 'a' && **it <= 'z') || (**it <= 'Z' && **it >= 'A'))) {
                            std::cerr << "Cannot parse expression, expected a complex number!\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                    else {
                        std::shared_ptr<Value<std::complex<long double>>> left = std::make_shared<Value<std::complex<long double>>>(std::complex<long double>(first, 0));
                        std::shared_ptr<Value<std::complex<long double>>> right = std::make_shared<Value<std::complex<long double>>>(std::complex<long double>(second, 0));
                        current = std::make_shared<Operation<std::complex<long double>>>(OperationType::add, left, right);
                    }
                }
            }
            else current = std::make_shared<Value<std::complex<long double>>>(std::complex<long double>(first, 0));
        }
        else if ((**it >= 'a' && **it <= 'z') || (**it >= 'A' && **it <= 'Z')) {
            std::string word = parse_string(it, end);
            if (word == "sin") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<std::complex<long double>>>(FunctionType::sin, parse_complex(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "cos") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<std::complex<long double>>>(FunctionType::sin, parse_complex(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "exp") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<std::complex<long double>>>(FunctionType::sin, parse_complex(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "ln") {
                if (**it == '(') {
                    (*it)++;
                    current = std::make_shared<Function<std::complex<long double>>>(FunctionType::sin, parse_complex(it, end, vars));
                    (*it)++;
                }
                else {
                    std::cerr << "Function has no argument!\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if (word == "i") {
                current = std::make_shared<Value<std::complex<long double>>>(std::complex<long double>(0, 1));
            }
            else {
                vars->insert(word);
                current = std::make_shared<Variable<std::complex<long double>>>(word);
            }
        }
        else if (**it == '+') {
            (*it)++;
            skip_spaces(it, end);
            std::shared_ptr<Operation<std::complex<long double>>> operation = std::make_shared<Operation<std::complex<long double>>>(OperationType::add, current, parse_complex(it, end, vars));
            current = operation;
        }
        else if (**it == '-') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<std::complex<long double>>> operation = std::make_shared<Operation<std::complex<long double>>>(OperationType::sub, current, parse_complex(it, copy, vars));
            current = operation;
        }
        else if (**it == '*') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<std::complex<long double>>> operation = std::make_shared<Operation<std::complex<long double>>>(OperationType::mult, current, parse_complex(it, copy, vars));
            current = operation;
        }
        else if (**it == '/') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<std::complex<long double>>> operation = std::make_shared<Operation<std::complex<long double>>>(OperationType::div, current, parse_complex(it, copy, vars));
            current = operation;
        }
        else if (**it == '^') {
            (*it)++;
            skip_spaces(it, end);
            auto copy = *it;
            find_end(&copy, end);
            std::shared_ptr<Operation<std::complex<long double>>> operation = std::make_shared<Operation<std::complex<long double>>>(OperationType::pow, current, parse_complex(it, copy, vars));
            current = operation;
        }
        else {
            std::cerr << "Non-algebraic expression! Met wrong symbol:" << **it << "\n";
            exit(EXIT_FAILURE);
        }
    }
    return current;
}

Expression<long double> construct_real(std::string input) {
    std::string::iterator it = input.begin();
    std::string::iterator end = input.end();
    std::unordered_set<std::string> __vars = {};
    std::shared_ptr<Head<long double>> __head = std::make_shared<Head<long double>>(parse_real(&it, end, &__vars));
    return Expression<long double>(__head, __vars);
}

Expression<std::complex<long double>> construct_complex(std::string input) {
    std::string::iterator it = input.begin();
    std::string::iterator end = input.end();
    std::unordered_set<std::string> __vars = {};
    std::shared_ptr<Head<std::complex<long double>>> __head = std::make_shared<Head<std::complex<long double>>>(parse_complex(&it, end, &__vars));
    return Expression<std::complex<long double>>(__head, __vars);
}


#endif