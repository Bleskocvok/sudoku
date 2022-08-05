#pragma once

#include <iostream>     // cout, cin
#include <ostream>      // ostream
#include <istream>      // istream
#include <array>        // array
#include <optional>     // optional, nullopt
#include <bitset>       // bitset
#include <string>       // string
#include <algorithm>    // generate_n, shuffle, min, sort
#include <vector>       // vector
#include <iterator>     // back_inserter
#include <utility>      // move, pair
#include <iterator>     // forward_iterator_tag
#include <stdexcept>    // logic_error
#include <sstream>      // stringstream
#include <variant>      // variant
#include <tuple>        // tuple, make_tuple

#include <stdint.h>


using fast = uint_fast8_t;
using index = fast; // uint_fast16_t;
using sud  = uint_fast8_t;


template <uint8_t Size>
class grid;


template<uint8_t Size>
void print(const grid<Size>& g, std::ostream& out = std::cout);


template<uint8_t Size>
void print_packed(const grid<Size>& g, std::ostream& out = std::cout);


template<uint8_t Size>
auto parse_packed(std::istream& in = std::cin)
    -> std::variant<grid<Size>, std::string>;


template<uint8_t Size>
auto parse(std::istream& in = std::cin)
    -> std::variant<grid<Size>, std::string>;


template<typename T>
constexpr inline T isqrt(T val)
{
    for (T i(0); i < val; i++)
        if (i * i >= val)
            return i;
    return T(0);
}


template<typename T, fast Begin, fast End>
struct range
{
    struct iterator
    {
        T val;

        using value_type = T;
        using difference_type = T;
        using reference = T&;
        using pointer = T*;
        using iterator_category = std::forward_iterator_tag;

        constexpr reference operator*() { return val; }
        constexpr iterator& operator++() { ++val; return *this; }
        constexpr bool operator!=(const iterator& other) const
        {
            return val != other.val;
        }
    };

    constexpr iterator begin() const { return iterator{ Begin }; }
    constexpr iterator end()   const { return iterator{ End   }; }
};


// template <uint8_t Size>
// class bag
// {
//     std::array<std::bitset<Size>, Size> rows{};
//     std::array<std::bitset<Size>, Size> cols{};
//     std::array<std::bitset<Size>, Size> blocks{};

//     constexpr auto block() const { return grid<Size>::block(); }

//     constexpr void _set(sud x, sud y, sud blk, sud val, bool b)
//     {
//         rows[y][val - 1] = b;
//         cols[x][val - 1] = b;
//         blocks[blk][val - 1] = b;
//     }

// public:
//     constexpr void set(sud x, sud y, sud blk, sud val)
//     {
//         _set(x, y, blk, val, true);
//     }

//     constexpr void reset(sud x, sud y, sud blk, sud val)
//     {
//         _set(x, y, blk, val, false);
//     }

//     constexpr bool possible(sud x, sud y, sud blk, sud val) const
//     {
//         return !rows[y][val - 1]
//              & !cols[x][val - 1]
//              & !blocks[blk][val - 1];
//     }

//     constexpr size_t count(sud x, sud y, sud blk) const
//     {
//         return (~rows[y] & ~cols[x] & ~blocks[blk]).count();
//     }
// };


template <uint8_t Size>
class bag
{
    std::array<uint16_t, Size> rows{ 0 };
    std::array<uint16_t, Size> cols{ 0 };
    std::array<uint16_t, Size> blks{ 0 };

public:
    constexpr void set(sud x, sud y, sud blk, sud val)
    {
        bit_set(x, y, blk, 0x1 << val);
    }

    constexpr void reset(sud x, sud y, sud blk, sud val)
    {
        bit_reset(x, y, blk, 0x1 << val);
    }

    constexpr bool possible(sud x, sud y, sud blk, sud val) const
    {
        return bit_possible(x, y, blk, 0x1 << val);
    }

    constexpr void bit_set(sud x, sud y, sud blk, uint16_t bit)
    {
        rows[y]   |= bit;
        cols[x]   |= bit;
        blks[blk] |= bit;
    }

    constexpr void bit_reset(sud x, sud y, sud blk, uint16_t bit)
    {
        rows[y]   ^= bit;
        cols[x]   ^= bit;
        blks[blk] ^= bit;
    }

    constexpr bool bit_possible(sud x, sud y, sud blk, uint16_t bit) const
    {
        return ~(rows[y] | cols[x] | blks[blk]) & bit;
    }

    constexpr size_t count(sud x, sud y, sud blk) const
    {
        return std::bitset<16>(~rows[y] & ~cols[x] & ~blks[blk]).count();
    }
};


template <uint8_t Size>
class grid
{
    std::array<sud, Size * Size> data{ 0 };

    constexpr auto blk_idx(sud x, sud y) const
    {
        return (y / block()) * block() + (x / block());
    }

    constexpr bool validate() const
    {
        auto opts = bag<Size>{};
        for (fast y = 0; y < Size; y++)
        {
            for (fast x = 0; x < Size; x++)
            {
                if (!opts.possible(x, y, blk_idx(x, y), at(x, y)))
                    return false;
                opts.set(x, y, blk_idx(x, y), at(x, y));
            }
        }
        return true;
    }

    template<uint8_t S>
    friend auto parse(std::istream& in)
        -> std::variant<grid<S>, std::string>;

    template<uint8_t S>
    friend auto parse_packed(std::istream& in)
        -> std::variant<grid<S>, std::string>;

public:
    constexpr grid(std::array<sud, Size * Size> data)
        : data(std::move(data))
    { }

    constexpr grid() = default;

    static constexpr auto size() { return Size; }

    static constexpr sud block() { return isqrt(Size); }

    constexpr const sud& at(sud x, sud y) const { return data[x + y * Size]; }
    constexpr       sud& at(sud x, sud y)       { return data[x + y * Size]; }

    constexpr bool check() const
    {
        auto opts = bag<Size>{};
        for (fast y = 0; y < Size; y++)
        {
            for (fast x = 0; x < Size; x++)
            {
                sud val = at(x, y);
                if (val == 0)
                    continue;

                if (val > Size)
                    return false;

                if (!opts.possible(x, y, blk_idx(x, y), val))
                    return false;

                opts.set(x, y, blk_idx(x, y), val);
            }
        }
        return true;
    }

    constexpr bool is_solved() const
    {
        for (fast i = 0; i < data.size(); i++)
            if (data[i] == 0)
                return false;

        return validate();
    }

    bool possible_at(sud val, sud x, sud y)
    {
        auto prev = at(x, y);
        at(x, y) = val;
        bool ok = check();
        at(x, y) = prev;
        return ok;
    }

    template<typename Seq, typename Shuffle, typename Step>
    auto solve_rec_f(size_t i, grid& gr, bag<Size>& opts,
                     const std::vector<std::tuple<fast, fast, fast>>& zeroes,
                     Seq& seq, Shuffle shuffle, Step step) const
        -> std::optional<grid>
    {
        if (i >= zeroes.size())
        {
            return gr.validate() ? std::optional<grid>{ gr }
                                 : std::nullopt;
        }

        shuffle(seq);

        auto [x, y, blk] = zeroes[i];
        for (sud j : seq)
        {
            uint16_t bit = 1 << j;

            if (!opts.bit_possible(x, y, blk, bit))
                continue;

            opts.bit_set(x, y, blk, bit);
            gr.at(x, y) = j;

            step(gr);

            auto res = solve_rec_f(i + 1, gr, opts, zeroes, seq, shuffle, step);
            if (res)
                return res;

            opts.bit_reset(x, y, blk, bit);
        }
        gr.at(x, y) = 0;

        return std::nullopt;
    }

    template<typename Seq, typename Shuffle, typename Step>
    auto solution(Seq& seq, Shuffle shuffle, Step step) const
        -> std::optional<grid> 
    {
        auto copy = *this;
        auto opts = bag<Size>{};
        auto zeroes = std::vector<std::tuple<fast, fast, fast>>{};
        zeroes.reserve(Size * Size);    // upper bound for size

        for (int y = 0; y < Size; y++)
        {
            for (int x = 0; x < Size; x++)
            {
                if (copy.at(x, y) != 0)
                    opts.set(x, y, blk_idx(x, y), copy.at(x, y));
                else
                    zeroes.emplace_back(x, y, blk_idx(x, y));
            }
        }

        std::sort(zeroes.begin(), zeroes.end(),
            [&](const auto& a, const auto& b)
            {
                auto [ax, ay, ablk] = a;
                auto [bx, by, bblk] = b;
                auto acount = opts.count(ax, ay, ablk);
                auto bcount = opts.count(bx, by, bblk);
                return std::make_tuple(acount, -ay, -ax)
                     < std::make_tuple(bcount, -by, -bx);
            });

        return solve_rec_f(0, copy, opts, zeroes, seq, shuffle, step);
    }

    auto solution() const -> std::optional<grid> 
    {
        auto nums = range<fast, 1, Size + 1>{};
        return solution(nums, [](const auto&){}, [](const auto&){});
    }

    template<typename Rand>
    static grid generate(int unfilled, Rand& rand)
    {
        grid empty{};

        auto vals = std::vector<sud>{};
        std::generate_n(std::back_inserter(vals), Size,
                    [n = 0]() mutable { return ++n; });

        auto shuffle = [&rand](auto& vals)
        {
            std::shuffle(vals.begin(), vals.end(), rand);
        };

        auto res = empty.solution(vals, shuffle, [](const auto&){});

        if (!res)
            throw std::logic_error("empty sudoku did not yield a solution");

        auto mk_indices = []()
        {
            auto res = std::vector<std::pair<sud, sud>>{};
            res.reserve(Size * Size);
            for (int x = 0; x < Size; x++)
                for (int y = 0; y < Size; y++)
                    res.emplace_back(x, y);
            return res;
        };

        static auto indices = mk_indices();
        shuffle(indices);

        for (int i = 0; i < std::min<int>(unfilled, indices.size()); i++)
        {
            auto [x, y] = indices[i];
            res->at(x, y) = 0;
        }

        return *res;
    }
};



template<uint8_t Size>
void print(const grid<Size>& g, std::ostream& out)
{
    auto line = [&]()
    {
        for (fast i = 0; i < g.block(); i++)
            out << "+-"
                << std::string(g.block() * 2, '-');
        out << "+\n";
    };

    line();

    for (fast y = 0; y < Size; y++)
    {
        out << "| ";

        auto* sp = "";
        for (fast x = 0; x < Size; x++)
        {
            out << sp;
            sp = " ";

            int val = static_cast<int>(g.at(x, y));

            val == 0 ? (out << ".") : (out << val);

            if (x % g.block() == g.block() - 1)
                out << " |";
        }

        out << "\n";

        if (y % g.block() == g.block() - 1)
            line();
    }
}


// TODO: solve issue for grid<Size>, where Size > 9
static const auto PACKED_SYMBOLS = std::string{ ".123456789" };


template<uint8_t Size>
void print_packed(const grid<Size>& g, std::ostream& out)
{
    for (fast y = 0; y < Size; y++)
        for (fast x = 0; x < Size; x++)
            out << PACKED_SYMBOLS.at(unsigned(g.at(x, y)));
}


template<uint8_t Size>
auto parse_packed(std::istream& in)
    -> std::variant<grid<Size>, std::string>
{
    auto res = grid<Size>{};

    for (int i = 0; i < Size * Size; i++)
    {
        if (!in)
            return "unexpected eof";
        char c = in.get();
        if (auto n = PACKED_SYMBOLS.find(c); n != PACKED_SYMBOLS.npos)
            res.data[i] = n;
        else
            return (std::string("invalid symbol '") += c) += "'";
    }
    return res;
}


template<uint8_t Size>
auto parse(std::istream& in) -> std::variant<grid<Size>, std::string>
{
    auto res = grid<Size>{};

    auto str = std::stringstream{};
    print(res, str);
    auto fmt = std::move(str).str();

    static const auto nums = std::string{ "0123456789" };

    int i = 0;
    for (char f : fmt)
    {
        if (!in)
            return "unexpected eof";
        char c = in.get();
        switch (f)
        {
            case '.':
                if (auto n = nums.find(c); n != nums.npos)
                    res.data[i++] = n;
                else
                    return "invalid number";
                break;

            default:
                if (f != c)
                    return "invalid symbol";
                break;
        }
    }

    return res;
}
