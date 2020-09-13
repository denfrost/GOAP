/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_EQUAL = 1 << 0;
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_NOTEQUAL = 1 << 1;
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_GREATER = 1 << 2;
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_LESS = 1 << 3;
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_GREATEREQUAL = 1 << 4;
    static constexpr uint32_t FLAG_SEMAPHORE_TEST_LESSEQUAL = 1 << 5;
    //////////////////////////////////////////////////////////////////////////
    static constexpr uint32_t FLAG_SEMAPHORE_APPLY_ASSIGN = 1 << 10;
    static constexpr uint32_t FLAG_SEMAPHORE_APPLY_ADD = 1 << 11;
    static constexpr uint32_t FLAG_SEMAPHORE_APPLY_SUBTRACT = 1 << 12;
    //////////////////////////////////////////////////////////////////////////
    static constexpr uint32_t FLAG_SEMAPHORE_MASK_TEST = (1 << 10) - 1;
    static constexpr uint32_t FLAG_SEMAPHORE_MASK_APPLY = ~FLAG_SEMAPHORE_MASK_TEST;
    //////////////////////////////////////////////////////////////////////////
}