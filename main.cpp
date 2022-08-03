
#include "sudoku.hpp"

#include <iostream>     // cout
// #include <random>       // random_device, mt19937
#include <string>       // string, getline
#include <sstream>      // string_stream
#include <utility>      // move


int main()
{
    auto line = std::string{};
    auto stream = std::stringstream{};

    while (std::cin)
    {
        std::getline(std::cin, line);
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
