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

#include "hip_event.hpp"
#include "hip_event_pool.hpp"

namespace oomph::detail
{
// A hip_event backed by a hip_event_pool.
//
// Same semantics as hip_event, but the event is retrieved from a static
// hip_event_pool on construction and returned to the pool on destruction.
struct cached_hip_event
{
    hip_event m_event;

    cached_hip_event()
    : m_event(get_hip_event_pool().pop())
    {
    }
    cached_hip_event(cached_hip_event&& other) noexcept = default;
    cached_hip_event& operator=(cached_hip_event&& other) noexcept = default;
    cached_hip_event(const cached_hip_event&) = delete;
    cached_hip_event& operator=(const cached_hip_event&) = delete;
    ~cached_hip_event() noexcept
    {
        if (m_event) { get_hip_event_pool().push(std::move(m_event)); }
    }

    operator bool() noexcept { return bool(m_event); }

    void record(hipStream_t stream) { return m_event.record(stream); }

    bool is_ready() const { return m_event.is_ready(); }

    hipEvent_t get() { return m_event.get(); }
};
} // namespace oomph::detail
