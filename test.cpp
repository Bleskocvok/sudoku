
#include "sudoku.hpp"

#include <iostream>     // cout

#include <cstdio>       // printf
#include <cassert>      // assert


using sudoku = grid<9>;


void solvable(const sudoku& s)
{
    assert(s.check());
    auto sol = s.solution();
    assert(sol);
    assert(sol->is_solved());
}


void consistency(sudoku s)
{
    assert(s.is_solved());    // prerequisite

    for (int y = 0; y < s.size(); y++)
    {
        for (int x = 0; x < s.size(); x++)
        {
            auto prev = s.at(x, y);
            s.at(x, y) = 0;
            assert(s.check());
            assert(!s.is_solved());

            auto sol = s.solution();
            assert(sol);
            assert(prev == sol->at(x, y));  // the result must be the same
            s.at(x, y) = prev;
        }
    }
}


int main()
{
    auto zeroes = sudoku{};
    solvable(zeroes);

    // sud _ = 0;
    // auto longdoku = grid<9>
    // { {
    //     _, _, _,  _, _, _,  _, _, 5,
    //     _, _, _,  _, _, _,  _, _, _,
    //     2, _, _,  _, _, _,  _, _, _,

    //     _, _, _,  5, 2, _,  _, _, _,
    //     _, _, _,  6, _, _,  _, _, _,
    //     _, _, _,  _, _, 9,  _, _, _,

    //     _, _, _,  _, _, _,  _, _, _,
    //     _, _, 1,  _, _, 5,  _, _, _,
    //     _, _, 8,  _, _, 1,  _, _, _,
    // } };
    // solvable(longdoku);

    sud _ = 0;
    grid<9> fastdoku(
    {
        _, _, 8,  _, _, 7,  4, _, _,
        _, 3, _,  _, 8, _,  _, 5, _,
        _, _, 5,  3, _, 6,  _, _, _,

        6, _, _,  _, 1, _,  _, _, _,
        _, _, _,  _, 2, 4,  _, 7, 9,
        _, _, _,  _, _, _,  8, _, _,

        _, 1, _,  _, 4, _,  5, _, _,
        _, 7, 6,  8, _, _,  1, _, 2,
        _, _, _,  7, _, _,  _, _, _,
    });
    solvable(fastdoku);

    consistency(fastdoku.solution().value());

    std::printf("OK\n");

    return 0;
}
