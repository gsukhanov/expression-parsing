#include "Expression.hpp"
#include <regex>
#include <algorithm>
#include <complex>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments!\n";
        exit(EXIT_FAILURE);
    }
    std::string type = argv[1];
    bool complex = false;
    for (int i = 2; i < argc; i++) {
        if (std::regex_search(std::string(argv[i]), std::regex(R"((\d|^|\s|\+|\-)i\b)"))) {
            complex = true;
            break;
        }
    }
    if (type == "--eval") {
        if (complex) {
            std::regex reg(R"(^\s*([a-zA-Z_]\w*)\s*=\s*([-+]?\d*\.?\d+)?\s*([+-]?)\s*(\d*\.?\d*)(i)?\s*$)");
            std::smatch match;
            Expression<std::complex<double>> expr;
            expr = construct_complex(argv[2]);
            std::vector<std::complex<double>> vals = {};
            std::vector<std::string> vars = {};
            for(int i = 3; i < argc; i++) {
                std::string str(argv[i]);
                if (std::regex_match(str, match, reg)) {
                    double real = 0.0, imag = 0.0;
                    std::string var = match[1].str();
                    if (match[2].matched && !match[2].str().empty()) real = std::stod(match[2].str());
                    if (match[3].matched || match[4].matched) {
                        std::string sign = match[3].matched ? match[3].str() : "+";
                        std::string value = (match[4].matched && !match[4].str().empty()) ? match[4].str() : "1";
                        if (match[5].matched) imag = std::stod(value);
                        if (sign == "-") imag = -imag;
                    }
                    vars.push_back(var);
                    vals.push_back(std::complex<double>(real, imag));
                }
            }
            std::cout << "\n";
            std::cout << two_string(expr.calculate(vars, vals)) << "\n";
        }
        else {
            std::regex reg(R"(^\s*([a-zA-Z]\w*)\s*=\s*([-+]?\d*.?\d+)\s*$)");
            std::smatch match;
            Expression<double> expr;
            expr = construct_real(argv[2]);
            std::vector<double> vals = {};
            std::vector<std::string> vars = {};
            for(int i = 3; i < argc; i++) {
                std::string str(argv[i]);
                if (std::regex_match(str, match, reg)) {
                    double real = std::stod(match[2].str());
                    std::string var = match[1].str();
                    vars.push_back(var);
                    vals.push_back(real);
                }
            }
            std::cout << expr.calculate(vars, vals) << "\n";
        }
    }
    else if (type == "--diff") {
        if (argc != 5 || std::string(argv[3]) != "--by") {
            std::cerr << "Invalid request. Correct form: differentiator --diff \"EXPRESSION\" --by VARIABLE_NAME\n";
            exit(EXIT_FAILURE);
        }
        if (complex) {
            Expression<std::complex<double>> expr = construct_complex(argv[2]);
            std::cout << expr.differentiate(argv[4]) << "\n";
        }
        else {
            Expression<double> expr = construct_real(argv[2]);
            std::cout << expr.differentiate(argv[4]) << "\n";
        }
    }
    else {
        std::cerr << "Unknow operation!\n";
        exit(EXIT_FAILURE);
    }
}
