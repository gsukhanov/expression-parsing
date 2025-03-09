#include "Expression.hpp"
#include <cmath>

//---------------------------------------------------------------------------------------------------------------
// Конструкторы
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>::Expression(T value) {
    Value<T> *next = new Value<T>(value);
    Head<T> *__head = new Head(next);
    head = __head;
}

template <typename T> Expression<T>::Expression(std::string var) {
    Variable<T> *next = new Variable<T>(var);
    Head<T> *__head = new Head(next);
    variables.emplace(var);
    head = __head;
}

template <typename T> Expression<T>::Expression(const Expression<T>& other){
    head = new Head<T>(*(other.head));
    variables = other.get_variables();
}

template <typename T> Expression<T>::Expression(Expression<T>&& other){
    head = other.head;
    other.head = nullptr;
    variables = other.get_variables();
}

template <typename T> Expression<T>::Expression(Head<T> *__head, std::unordered_set<std::string> __variables) {
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

template <typename T> Head<T>::Head(Node<T> *__next) {
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

template <typename T> Operation<T>::Operation(OperationType __type, Node<T> *__left, Node<T> *__right) {
    Node<T>::kind = NodeKind::op;
    type = __type;
    left = __left;
    right = __right;
}

template <typename T> Function<T>::Function() {
    Node<T>::kind = NodeKind::func;
}

template <typename T> Function<T>::Function(FunctionType __type, Node<T> *__arg) {
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

template <typename T> Head<T>* Head<T>::clone() {
    std::cerr << "Something went wrong, head is an inner node of the tree.\n";
    exit(-1);
    return nullptr;
}

template <typename T> Operation<T>* Operation<T>::clone() {
    Operation<T> *copy = new Operation<T>(type, left->clone(), right->clone());
    return copy;
}

template <typename T> Function<T>* Function<T>::clone() {
    Function<T> *copy = new Function<T>(type, arg->clone());
    return copy;
}

template <typename T> Variable<T>* Variable<T>::clone() {
    Variable<T> *copy = new Variable<T>(name);
    return copy;
}

template <typename T> Value<T>* Value<T>::clone() {
    Value<T> *copy = new Value<T>(value);
    return copy;
}

//---------------------------------------------------------------------------------------------------------------
// Операторы копирования и перемещения
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>& Expression<T>::operator=(const Expression<T>& other){
    head = new Head<T>(other->head);
    variables = other.get_variables();
}

template <typename T> Expression<T>& Expression<T>::operator=(Expression<T>&& other){
    head = other.head;
    other.head = nullptr;
    variables = other.get_variables();
}

template <typename T> Head<T>& Head<T>::operator=(const Head& other) {
    Node::kind = NodeKind::head;
    next = other.next->clone();
}

template <typename T> Head<T>& Head<T>::operator=(Head&& other) {
    Node::kind = NodeKind::head;
    next = other.next;
    other.next = nullptr;
}

template <typename T> Operation<T>& Operation<T>::operator=(const Operation<T>& other) {
    Node::kind = NodeKind::op;
    type = other.type;
    left = other.left->clone();
    right = other.right->clone();
}

template <typename T> Operation<T>& Operation<T>::operator=(Operation<T>&& other) {
    Node::kind = NodeKind::op;
    type = other.type;
    left = other.left;
    right = other.right;
    other.left = nullptr;
    other.right = nullptr;
}

template <typename T> Function<T>& Function<T>::operator=(const Function<T>& other) {
    Node::kind = NodeKind::func;
    type = other.type;
    arg = other.arg->clone();
}

template <typename T> Function<T>& Function<T>::operator=(Function<T>&& other) {
    Node::kind = NodeKind::func;
    type = other.type;
    arg = other.arg;
    other.arg = nullptr;    
}


//---------------------------------------------------------------------------------------------------------------
// Деструкторы
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>::~Expression() {
    delete head;
}

template <typename T> Operation<T>::~Operation() {
    delete left;
    delete right;
}

template <typename T> Function<T>::~Function() {
    delete arg;
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
    if (type == OperationType::add) sym = " + ";
    if (type == OperationType::sub) sym = " - ";
    if (type == OperationType::mult) sym = " * ";
    if (type == OperationType::div) sym = " / ";
    if (type == OperationType::pow) sym = " ^ ";
    return "(" + left->to_string() + ")" + sym + "(" + right->to_string() + ")";
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
    return std::to_string(value);
}

template <typename T> std::ostream& operator<<(std::ostream &out, Expression<T> e) {
    out << e.to_string();
    return out;
}

//---------------------------------------------------------------------------------------------------------------
// Арифметические операции
//---------------------------------------------------------------------------------------------------------------

template <typename T> Expression<T>& Expression<T>::operator +=(const Expression<T> &other) {
    Expression<T> *copy = new Expression<T>(other);
    Operation<T> *op = new Operation<T>(OperationType::add, head->next, copy->head->next);
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
    Expression<T> *copy = new Expression<T>(other);
    Operation<T> *op = new Operation<T>(OperationType::sub, head->next, copy->head->next);
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
    Expression<T> *copy = new Expression<T>(other);
    Operation<T> *op = new Operation<T>(OperationType::mult, head->next, copy->head->next);
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
    Expression<T> *copy = new Expression<T>(other);
    Operation<T> *op = new Operation<T>(OperationType::div, head->next, copy->head->next);
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
    Expression<T> *copy = new Expression<T>(other);
    Operation<T> *op = new Operation<T>(OperationType::pow, head->next, copy->head->next);
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
    Function<T> *func = new Function<T>(FunctionType::sin, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> cos(Expression<T> e) {
    Expression<T> copy(e);
    Function<T> *func = new Function<T>(FunctionType::cos, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> ln(Expression<T> e) {
    Expression<T> copy(e);
    Function<T> *func = new Function<T>(FunctionType::ln, copy.head->next);
    copy.head->next = func;
    return copy;
}

template <typename T> Expression<T> exp(Expression<T> e) {
    Expression<T> copy(e);
    Function<T> *func = new Function<T>(FunctionType::exp, copy.head->next);
    copy.head->next = func;
    return *copy;
}

//---------------------------------------------------------------------------------------------------------------
// Функции подстановки и вычисления
//---------------------------------------------------------------------------------------------------------------

template <typename T> std::unordered_set<std::string> Expression<T>::get_variables() {
    return variables;
}

template <typename T> Expression<T>& Expression<T>::self_substitute(std::string __name, T __value) {
    auto pos = variables.begin();
    while(*pos != __name && pos != variables.end()) pos++;
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
    for (auto it = vars.begin(), itval = vals.begin(); it < vars.end(); it++, itval++) {
        //if (copy.get_variables().contains(*it)) { <- в новом c++, будет в итоговой сборке
        if (copy.get_variables().find(*it) != copy.get_variables().end()) {
            copy.self_substitute(*it, *itval);
        }
        else {
            std::cerr <<"\"" << *it << "\" - no such variable!";
            exit(EXIT_FAILURE);
        }
    }
    return head->calculate();
}

template <typename T> Head<T>& Head<T>::substitute(std::string __name, T __value) {
    if (next->kind == NodeKind::var) {
        Variable<T> *nextvar = dynamic_cast<Variable<T> *>(next);
        if (nextvar->name == __name) {
            delete next;
            Value<T> *__next = new Value<T>(__value);
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
        Variable<T> *leftvar = dynamic_cast<Variable<T> *>(left);
        if (leftvar->name == __name) {
            delete left;
            Value<T> *__left = new Value<T>(__value);
            left = __left;
        }
    }
    else left->substitute(__name, __value);
    if (right->kind == NodeKind::var) {
        Variable<T> *rightvar = dynamic_cast<Variable<T> *>(right);
        if (rightvar->name == __name) {
            delete right;
            Value<T> *__right = new Value<T>(__value);
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
    std::cout << "Something went wrong, an operation has no type.\n";
    exit(EXIT_FAILURE);
}

template <typename T> Function<T>& Function<T>::substitute(std::string __name, T __value) {
    if (arg->kind == NodeKind::var) {
        Variable<T> *argvar = dynamic_cast<Variable<T> *>(arg);
        if (argvar->name == __name) {
            delete arg;
            Value<T> *__arg = new Value<T>(__value);
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
    std::cout << "Something went wrong, a function has no type.\n";
    exit(EXIT_FAILURE);
}

template <typename T> Variable<T>& Variable<T>::substitute(std::string __name, T __value) {
    std::cerr << "Something went wrong, substitue function was called for a variable.\n";
    exit(EXIT_FAILURE);
    return *this;
}

template <typename T> T Variable<T>::calculate() {
    std::cerr << "Something went wrong, trying to calcualte a variable.\n";
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
    copy.head = differentiate(head);
    return copy;
}

/*template <typename T> Node<T>* differentiate(Node<T> *node, std::string __name) {
    if (node->kind == NodeKind::head) {
        Head<T> *head = dynamic_cast<Head<T>*>(node);
        node = differentiate(head->next);
    }
    else if (node->kind == NodeKind::op) {

    }
    else if (node->kind == NodeKind::func) {
        Function<T> *func = dynamic_cast<Function<T>*>(node);
        if (func->type == FunctionType::sin) {
            Node<T> *inner = func->arg->clone()->differentiate();
            func->type = FunctionType::cos;
            if (inner->kind = NodeKind::val) {
                Value<T> *inner_val = dynamic_cast<Value<T>*>(inner);
//                if (inner_val->value)
//надо придумать проверку на нули и единицы, чтобы упрощать выражение в ходе вычислений
            }
        }
    }
    else if (node->kind == NodeKind::var) {
        Variable<T>* var = dynamic_cast<Variable<T>*>(node);
        if (var->name == __name) {
            delete node;
            node = new Value(1);
        }
        else {
            delete node;
            node = new Value(0);
        }
    }
    else if (node->kind == NodeKind::val) {
        delete node;
        node = new Value(0);
    }
    return node;
}*/

//---------------------------------------------------------------------------------------------------------------
// Тест для проверки:
//---------------------------------------------------------------------------------------------------------------

int main(void) {
    Expression<double> e("lambda");
    Expression<double> f("phi");
    e += ln(f);
    Expression<double> g(e);
    std::cout << e << "\n";
    e.self_substitute("lambda", 12.6);
    std::cout << e << "\n";
    e.self_substitute("phi", 15.2);
    std::cout << e << "\n";
    std::cout << g.calculate({"lambda", "phi"}, {12.6, 15.2}) << "\n";
    return 0;
}