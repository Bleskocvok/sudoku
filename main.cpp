
#include "sudoku.hpp"

#include <iostream>     // cout
#include <fstream>      // ifstream
#include <random>       // random_device, mt19937
#include <string>       // string, getline
#include <sstream>      // string_stream
#include <utility>      // move
#include <cstdio>       // printf


template<typename Parser>
int solve(std::istream& input, Parser parser)
{
    auto line = std::string{};
    auto stream = std::stringstream{};

    while (input)
    {
        std::getline(input, line);

        if (line.empty())
            continue;

        if (line.front() == '#')
            continue;

        stream.str(std::move(line));

        auto res = parser(stream);

        if (res.index() == 1)
        {
            std::cerr << "ERROR: " << std::get<std::string>(res) << "\n";
            continue;
        }

        grid<9>& sudoku = std::get<grid<9>>(res);

        if (!sudoku.check())
        {
            std::cerr << "ERROR: " << "sudoku is invalid" << "\n";
            continue;
        }

        auto sol = sudoku.solution();

        if (!sol)
        {
            std::cerr << "ERROR: " << "sudoku cannot be solved" << "\n";
            continue;
        }

        print_packed(*sol);
        std::cout << std::endl;
    }

    return 0;
}


void usage(const char* argv0)
{
    std::printf(
        "Usage: <solve | generate> <packed | pretty> [file | clues] [seed] %s\n"
        , argv0);
}


int main(int argc, char** argv)
{
    bool solve_mode = true;
    bool packed = true;
    int clues = 0;
    unsigned long long seed = 0;

    auto get_parser = [&](){ return packed ? parse_packed<9> : parse<9>; };

    if (argc > 1)
    {
        if (argv[1] == std::string("solve"))
            solve_mode = true;
        else if(argv[1] == std::string("generate"))
            solve_mode = false;
        else
            return usage(argv[0]), 1;
    }

    if (argc > 2)
    {
        if (argv[2] == std::string("packed"))
            packed = true;
        else if(argv[2] == std::string("pretty"))
            packed = false;
        else
            return usage(argv[0]), 1;
    }

    if (argc > 3)
    {
        if (solve_mode)
        {
            auto file_in = std::ifstream(argv[3]);
            if (!file_in)
                return std::cout << "ERROR: file cannot be opened\n", 1;
            return solve(file_in, get_parser());
        }

        clues = std::stoi(argv[3]);
    }

    if (argc > 4)
    {
        seed = std::stoi(argv[4]);
    }
    else
    {
        auto rd = std::random_device();
        seed = rd();
    }

    if (!solve_mode)
    {
        auto gen = std::mt19937(seed);
        auto res = grid<9>::generate(9 * 9 - clues, gen);
        if (packed)
            print_packed(res);
        else
            print(res);
        return 0;
    }

    return solve(std::cin, get_parser());
}
