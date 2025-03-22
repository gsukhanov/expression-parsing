#include "Expression.hpp"
#include <regex>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments!\n";
        exit(EXIT_FAILURE);
    }
    std::string type = argv[1];
    bool complex = false;
    for (int i = 2; i < argc; i++) {
        if (std::regex_search(std::string(argv[i]), std::regex("R((\d|^|\s)i\b)"))) {
            complex = true;
            break;
        }
    }
    if (type == "--eval") {
        if (complex) {
            Expression<std::complex<long double>> expr = construct_complex(std::string(argv[2]));
            std::vector<std::string> vars = {};
            std::vector<std::complex<long double>> vals = {};
            for (int i = 3; i < argc; i++) {
                std::string str(argv[i]);
                auto it = str.begin();
                while(it < str.end() && *it != '=') it++;
                if (*it != '=') {
                    std::cerr << "Nothing to evaluate!\n";
                    exit(EXIT_FAILURE);
                }
                std::string var(str.begin(), it);
                var.erase(std::remove(var.begin(), var.end(), ' '), var.end());
                vars.push_back(var);
                std::complex<long double> val;
                auto start = ++it;
                while (it < str.end() && (*it >= '0' && *it <= '9' || *it == '.')) it++;
                long double first = std::stold(std::string(start, it));
                while (it < str.end() && *it == ' ') it++;
                if (it == str.end()) val = first;
                else {
                    if (*it != '+') {
                        std::cerr << "Nothing to evaluate!\n";
                        exit(EXIT_FAILURE);
                    }
                    else {
                        start = ++it;
                        while(it < str.end() && *it == ' ') it++;
                        long double second = std::stold(std::string(start, it));
                        val = std::complex<long double>(first, second);
                    }
                }
                vals.push_back(val);
            }
            std::complex<long double> output = expr.calculate(vars, vals);
            std::cout << output.real() << " + " << output.imag() << "i\n";
        }
        else {
            Expression<long double> expr = construct_real(std::string(argv[2]));
            std::vector<std::string> vars = {};
            std::vector<long double> vals = {};
            for (int i = 3; i < argc; i++) {
                std::string str(argv[i]);
                auto it = str.begin();
                while(it < str.end() && *it != '=') it++;
                if (*it != '=') {
                    std::cerr << "Nothing to evaluate!\n";
                    exit(EXIT_FAILURE);
                }
                std::string var(str.begin(), it);
                var.erase(std::remove(var.begin(), var.end(), ' '), var.end());
                vars.push_back(var);
                long double val = std::stold(std::string(it + 1, str.end()));
                vals.push_back(val);
            }
            long double output = expr.calculate(vars, vals);
            std::cout << output << "\n";
        }
    }
    else if (type == "--diff") {
        if (argc != 5 || std::string(argv[3]) != "--by") {
            std::cerr << "Invalid request. Correct form: differentiator --diff \"EXPRESSION\" --by VARIABLE_NAME";
            exit(EXIT_FAILURE);
        }
        if (complex) {
            Expression<std::complex<long double>> expr = construct_complex(argv[2]);
            std::cout << expr.differentiate(argv[4]) << "\n";
        }
        else {
            Expression<long double> expr = construct_real(argv[2]);
            std::cout << expr.differentiate(argv[4]) << "\n";
        }
    }
    else {
        std::cerr << "Unknow operation!\n";
        exit(EXIT_FAILURE);
    }
}
