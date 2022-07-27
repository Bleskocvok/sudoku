
#include "sudoku.hpp"

#include <iostream>     // cout
#include <random>       // random_device, mt19937


int main(int argc, char** argv)
{
    // int filled = 35;
    // if (argc < 2)
    // {
    //     std::cerr << "usage: " << argv[0] << " [FILLED]\n";
    //     std::cerr << "using default value: " << filled << "\n";
    // }
    // else
    // {
    //     filled = std::stoi(argv[1]);
    // }

    // auto seeder = std::random_device{};
    // auto seed = seeder();
    // auto rand = std::mt19937{ seed };

    // std::cout << "seed: " << seed << "\n";

    // unsigned long long i = 0;
    // while (true)
    // {
    //     auto gr = generate<9>(filled, rand);
    //     if (gr.check())
    //     {
    //         auto sol = gr.solution();
    //         if (sol)
    //         {
    //             std::cout << "input:\n";
    //             print(gr);
    //             std::cout << "solution:\n";
    //             print(*sol);
    //             break;
    //         }
    //     }

    //     // std::cout << "failed\n";
    //     // print(gr);
    //     if (i % 10000 == 0)
    //         std::cout << i << std::endl;
    //     i++;
    // }
    // return 0;

    // grid<9> ahoj(
    // {
    //     0, 6, 2,  0, 0, 7,  0, 4, 3,
    //     0, 0, 0,  0, 5, 6,  1, 0, 0,
    //     0, 7, 0,  3, 0, 0,  5, 9, 0,

    //     0, 0, 0,  0, 4, 0,  2, 0, 0,
    //     1, 9, 7,  0, 8, 0,  4, 6, 5,
    //     0, 0, 6,  0, 7, 0,  0, 0, 0,

    //     0, 8, 3,  0, 0, 2,  0, 5, 0,
    //     0, 0, 5,  7, 3, 0,  0, 0, 0,
    //     4, 2, 0,  5, 0, 0,  7, 3, 0,
    // });

    // sud _ = 0;
    // grid<9> ahoj(
    // {
    //     _, _, 8,  _, _, 7,  4, _, _,
    //     _, 3, _,  _, 8, _,  _, 5, _,
    //     _, _, 5,  3, _, 6,  _, _, _,

    //     6, _, _,  _, 1, _,  _, _, _,
    //     _, _, _,  _, 2, 4,  _, 7, 9,
    //     _, _, _,  _, _, _,  8, _, _,

    //     _, 1, _,  _, 4, _,  5, _, _,
    //     _, 7, 6,  8, _, _,  1, _, 2,
    //     _, _, _,  7, _, _,  _, _, _,
    // });

    sud _ = 0;
    grid<9> ahoj(
    {
        _, _, _,  _, _, _,  _, _, 5,
        _, _, _,  _, _, _,  _, _, _,
        2, _, _,  _, _, _,  _, _, _,

        _, _, _,  5, 2, _,  _, _, _,
        _, _, _,  6, _, _,  _, _, _,
        _, _, _,  _, _, 9,  _, _, _,

        _, _, _,  _, _, _,  _, _, _,
        _, _, 1,  _, _, 5,  _, _, _,
        _, _, 8,  _, _, 1,  _, _, _,
    });

    print(ahoj);

    auto r = ahoj.solution();

    if (r)
    {
        print(*r);
        std::cout << r->validate() << std::endl;
    }
    else
    {
        std::cout << "no solution\n";
    }

    // std::cout << ahoj.check() << std::endl;
    // std::cout << ahoj.validate() << std::endl;
    return 0;
}
