#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>

struct FlagFilter
{
    enum FlagState
    {
        SuppressFirst,
        SuppressSecond,
        SuppressBoth
    };

    FlagFilter() = default;

    FlagFilter(char * str)
    {
        size_t n = strlen(str);
        if (n <= 1 || n > 4 || str[0] != '-') {
            throw std::invalid_argument("wrong flag format");
        }
        for (size_t i = 1; i < n; i++) {
            int cnv = str[i] - '0';
            if (cnv >= 1 && cnv <= 3) {
                flags[cnv - 1] = true;
            }
            else {
                throw std::invalid_argument("wrong flag value");
            }
        }
    }

    void WriteFirst(std::string & val)
    {
        Write(val, FlagState::SuppressFirst);
    }

    void WriteSecond(std::string & val)
    {
        Write(val, FlagState::SuppressSecond);
    }

    void WriteBoth(std::string & val)
    {
        Write(val, FlagState::SuppressBoth);
    }

    void Write(std::string & val, FlagState pos)
    {
        if (!flags[pos]) {
            for (size_t i = 0; i < pos; i++) {
                if (!flags[i]) {
                    std::cout << "\t";
                }
            }
            std::cout << val << std::endl;
        }
    }

    std::array<bool, 3> flags = {};
};

void DoComm(std::istream & first_source, std::istream & second_source, FlagFilter output)
{
    std::string first, second;
    bool was_read = false;
    while (std::getline(first_source, first)) {
        if (was_read && first > second) {
            output.WriteSecond(second);
        }
        while ((!was_read || first > second) && std::getline(second_source, second)) {
            was_read = true;
            if (first > second) {
                output.WriteSecond(second);
                was_read = false;
            }
        }
        if (first == second) {
            was_read = false;
            output.WriteBoth(first);
        }
        else {
            output.WriteFirst(first);
        }
    }
    if (was_read) {
        output.WriteSecond(second);
    }
    while (std::getline(second_source, second)) {
        output.WriteSecond(second);
    }
}

std::istream * OpenFile(std::string filename)
{
    if (filename == "-") {
        return &std::cin;
    }
    std::ifstream * stream = new std::ifstream(filename);
    if (stream->fail()) {
        delete stream;
        throw std::invalid_argument("can`t open file " + filename);
    }
    return stream;
}

void DeleteStream(std::istream * stream)
{
    if (stream != nullptr && stream != &std::cin) {
        delete stream;
    }
}

int main(int argc, char ** argv)
{
    if (3 <= argc && argc <= 4) {
        std::istream *first(nullptr), *second(nullptr);
        try {
            first = OpenFile(argv[argc == 3 ? 1 : 2]);
            second = OpenFile(argv[argc == 3 ? 2 : 3]);
            DoComm(*first, *second, argc == 3 ? FlagFilter() : FlagFilter(argv[1]));
        }
        catch (const std::invalid_argument & error) {
            std::cerr << error.what();
        }
        DeleteStream(first);
        DeleteStream(second);
    }
    else {
        std::cerr << "Wrong arguments count\n";
    }

    return 0;
}
