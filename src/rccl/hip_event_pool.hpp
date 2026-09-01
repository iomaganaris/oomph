/*
 * ghex-org
 *
 * Copyright (c) 2014-2026, ETH Zurich
 * All rights reserved.
 *
 * Please, refer to the LICENSE file in the root directory.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include <hip/hip_runtime.h>

#include <oomph/util/moved_bit.hpp>

#include "hip_error.hpp"
#include "hip_event.hpp"

namespace oomph::detail
{
// Pool of hip_events.
//
// Simple wrapper over a vector of hip_events. Events can be popped from the
// pool. New events are created if the pool is empty. Events can be returned to
// the pool for reuse. Events do not need to originate from the pool. Not
// thread-safe.
class hip_event_pool
{
  private:
    std::vector<hip_event> m_events;

  public:
    hip_event_pool(std::size_t expected_pool_size)
    : m_events(expected_pool_size)
    {
    }

    hip_event_pool(const hip_event_pool&) = delete;
    hip_event_pool& operator=(const hip_event_pool&) = delete;
    hip_event_pool(hip_event_pool&& other) noexcept = delete;
    hip_event_pool& operator=(hip_event_pool&&) noexcept = delete;

  public:
    hip_event pop()
    {
        if (m_events.empty()) { return {}; }
        else
        {
            auto event{std::move(m_events.back())};
            m_events.pop_back();
            return event;
        }
    }

    void push(hip_event&& event) { m_events.push_back(std::move(event)); }
    void clear() { m_events.clear(); }
};

// Get a static instance of a hip_event_pool.
hip_event_pool& get_hip_event_pool();
} // namespace oomph::detail
