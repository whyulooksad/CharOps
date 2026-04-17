#include "CLI/CLI.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <tuple>


std::string url_encode(const std::string &value) {
    std::ostringstream string;
    string.fill('0');
    string << std::hex;

    for (char c : value) {
        // if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
        //     string << c;
        // } else {
            string << '%' << std::setw(2) << int((unsigned char) c);
        // }
    }
    return string.str();
}

template <typename Operation>
std::tuple<std::string, std::string> operation_mode(const std::string &payload, const std::string &badchars, Operation _op) {
    std::string result1, result2;
    for (char c : payload) {
        bool found = false;
        for (int j = 33; j <= 126; j++) {
            for (int k = 33; k <= 126; k++) {
                char result = _op(j, k);
                if (c == result && badchars.find(j) == std::string::npos && badchars.find(k) == std::string::npos) {
                    result1 += static_cast<char>(j);
                    result2 += static_cast<char>(k);
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            return std::make_tuple("Oops!", "");
        }
    }
    return std::make_tuple(result1, result2);
}
char xor_mode(int j, int k) { return (j ^ k); }
char or_mode(int j, int k) { return (j | k); }
char and_mode(int j, int k) { return (j & k); }

std::string not_mode(const std::string &payload, const std::string &badchars) {
    std::string result;
    for (char c : payload) {
        char x = ~c; 
        if (badchars.find(x) == std::string::npos) {
            result += x;
        } else {
            return "Oops!";
        }
    }
    return result;
}

int main(int argc, char **argv) {
    CLI::App app{"command-line tool"};

    std::string payload;
    std::string badchars;
    int mode;
    bool urlencode = false;

    app.add_option("-p,--payload", payload, "Payload")->required();
    app.add_option("-b,--badchars", badchars, "Bad characters");
    app.add_option("-m,--mode", mode, "Mode: 0=XOR, 1=OR, 2=AND, 3=NOT")->required();
    app.add_flag("-u,--urlencode", urlencode, "Output urlencode result");

    CLI11_PARSE(app, argc, argv);

    std::string result;
    std::string s1, s2;
    switch (mode) {
        case 0:
            std::tie(s1, s2) = operation_mode<decltype(xor_mode)>(payload, badchars, xor_mode);
            result = s1 + "^" + s2;
            break;
        case 1:
            std::tie(s1, s2) = operation_mode<decltype(or_mode)>(payload, badchars,or_mode);
            result = s1 + "|" + s2;
            break;
        case 2:
            std::tie(s1, s2) = operation_mode<decltype(and_mode)>(payload, badchars,and_mode);
            result = s1 + "&" + s2;
            break;
        case 3:
            result = url_encode(not_mode(payload, badchars));
            break;
        default:
            std::cerr << "Invalid mode!" << std::endl;
            return 1;
    }

    if (urlencode) {
        result = url_encode(result);
    }

    std::cout << result << std::endl;

    return 0;
}