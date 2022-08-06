
#include "sudoku.hpp"

#include <iostream>     // cout
#include <fstream>      // ifstream
// #include <random>       // random_device, mt19937
#include <string>       // string, getline
#include <sstream>      // string_stream
#include <utility>      // move


int solve(std::istream& input)
{
    auto line = std::string{};
    auto stream = std::stringstream{};

    while (input)
    {
        std::getline(input, line);
        stream.str(std::move(line));

        if (line.empty())
            continue;

        if (line.front() == '#')
            continue;

        auto res = parse_packed<9>(stream);

        if (res.index() == 1)
        {
            std::cerr << "ERROR: " << std::get<std::string>(res) << "\n";
            return 1;
        }

        grid<9>& sudoku = std::get<grid<9>>(res);

        if (!sudoku.check())
        {
            std::cerr << "ERROR: " << "sudoku is invalid" << "\n";
            return 1;
        }

        auto sol = sudoku.solution();

        if (!sol)
        {
            std::cerr << "ERROR: " << "sudoku cannot be solved" << "\n";
            return 1;
        }

        print_packed(*sol);
        std::cout << std::endl;
    }

    return 0;
}


int main(int argc, char** argv)
{
    if (argc >= 2)
    {
        auto file_in = std::ifstream(argv[1]);
        if (!file_in)
            return std::cout << "ERROR: file cannot be opened\n", 1;
        return solve(file_in);
    }

    return solve(std::cin);
}
