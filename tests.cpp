#include "Expression.hpp"

int main()
{
    {
        Expression<long double> expr1("x");
        Expression<long double> expr2("y");
        Expression<long double> expr3(5.0);
        Expression<long double> expr = expr1 + expr2 - expr3;
        std::string result = expr.to_string();
        std::string expect = "x + y - 5";
        std::cout << "Test 1. Arithmetic, addition and subtraction. Result: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<std::complex<long double>> expr1("lambda");
        Expression<std::complex<long double>> expr2(std::complex(10.934, 5.2));
        Expression<std::complex<long double>> expr3("sigma");
        Expression<std::complex<long double>> expr = (expr1 * expr2) / expr3;
        std::string result = expr.to_string();
        std::string expect = "(lambda * (10.934 + 5.2i)) / sigma";
        std::cout << "Test 2. Artihmetic, multiplication and division. Result: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("(1 * x + 0 * y + 0) / 1 + 1 ^ x");
        std::string original = expr.to_string();
        expr.simplify();
        std::string result = expr.to_string();
        std::string expect = "x + 1";
        std::cout << "Test 3. Simplifications. Original expression: " << original << "\nResult: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("15 * sin(x) + 120 / y - 8");
        std::string original = expr.to_string();
        expr = expr.substitute("x", 18.2);
        std::string result = expr.to_string();
        std::string expect = "15sin(18.2) + 120 / y - 8";
        std::cout << "Test 4. Substitution. Original expression: " << original << "\nResult: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<std::complex<long double>> expr = construct_complex("(12 + 6i) * x + 9");
        std::string original = expr.to_string();
        expr = expr.substitute("x", std::complex<long double>(5, 17));
        std::string result = expr.to_string();
        std::string expect = "(12 + 6i) * (5 + 17i) + 9";
        std::cout << "Test 5. Substitution. Original expression: " << original << "\nResult: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("x ^ 2 + 3 * x");
        std::string original = expr.to_string();
        long double point = 13.8;
        long double result = expr.calculate({"x"}, {point});
        std::string expect = "231.84";
        std::cout << "Test 6. Calculation. Original expression: " << original << "\nPoint of calculation: " << point << "\nResult: " << result << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (two_string(result) == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<std::complex<long double>> expr = construct_complex("(10 + 5i) / x + (8 - 6i)^2 + x^2");
        std::string original = expr.to_string();
        std::complex<long double> point(12, 7);
        std::complex<long double> result = expr.calculate({"x"},{point});
        std::string expect = "(158.80310880829 + 151.948186528497i)";
        std::cout << "Test 7. Calculation. Original expression: " << original << "\nPoint of calculation: " << two_string(point) << "\nResult: " << two_string(result) << "\n" << "Expected result: " << expect << "\n" << "Verdict: ";
        if (two_string(result) == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("x^5 + ln(x) - (7 * x) + 18");
        std::string original = expr.to_string();
        expr = expr.differentiate("x");
        std::string result = expr.to_string();
        std::string expect = "5 * (x ^ 4) + 1 / x - 7";
        std::cout << "Test 8. Differentiation of sum. Original expression:" << original << "\nResult: " << result << "\nExpected result:" << expect << "\nVerdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("ln(y) + y * (ln(x) * sin(x))");
        std::string original = expr.to_string();
        expr = expr.differentiate("x");
        std::string result = expr.to_string();
        std::string expect = "y * (1 / x) * sin(x) + cos(x) * y * ln(x)";
        std::cout << "Test 9. Differentiation of product. Original expression:" << original << "\nResult: " << result << "\n" << "Expected result:" << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("exp(x) / cos(x)");
        std::string original = expr.to_string();
        expr = expr.differentiate("x");
        std::string result = expr.to_string();
        std::string expect = "(exp(x) * cos(x) - -1sin(x) * exp(x)) / (cos(x) ^ 2)";
        std::cout << "Test 10. Differentiation of quotient. Original expression:" << original << "\nResult: " << result << "\n" << "Expected result:" << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n";
        else std::cout << "FAIL\n";
        std::cout << "Yes, it's ugly. No, I don't care.\n\n";
    }

    {
        Expression<long double> expr = construct_real("sin(x)^ln(x)");
        std::string original = expr.to_string();
        expr = expr.differentiate("x");
        std::string result = expr.to_string();
        std::string expect = "((1 / x) * ln(sin(x)) + (cos(x) / sin(x)) * ln(x)) * (sin(x) ^ ln(x))";
        std::cout << "Test 11. Differentiation of power. Original expression:" << original << "\nResult: " << result << "\n" << "Expected result:" << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

    {
        Expression<long double> expr = construct_real("sin(exp(cos(x * y)))");
        std::string original = expr.to_string();
        expr = expr.differentiate("x");
        std::string result = expr.to_string();
        std::string expect = "y * -1sin(x * y) * exp(cos(x * y)) * cos(exp(cos(x * y)))";
        std::cout << "Test 12. Differentiation of composition. Original expression:" << original << "\nResult: " << result << "\n" << "Expected result:" << expect << "\n" << "Verdict: ";
        if (result == expect) std::cout << "OK\n\n";
        else std::cout << "FAIL\n\n";
    }

} 
