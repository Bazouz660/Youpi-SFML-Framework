/*
 * @ Author: Basile Trebus--Hamann
 * @ Create Time: 2023-07-25 21:23:53
 * @ Modified by: Basile Trebus--Hamann
 * @ Modified time: 2023-07-25 21:25:50
 * @ Description:
 */

#include "../../lib_headers/common.hpp"
#include <functional>
#include <queue>
#include <mutex>

#pragma once

namespace ypi::utils {

    template<typename T, typename... U>
    static size_t getAddress(std::function<T(U...)> f) {
        typedef T(fnType)(U...);
        fnType ** fnPointer = f.template target<fnType*>();
        return (size_t) *fnPointer;
    }

}
