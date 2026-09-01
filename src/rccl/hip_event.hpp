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

#include <hip/hip_runtime.h>

#include <oomph/util/moved_bit.hpp>

#include "hip_error.hpp"

namespace oomph::detail
{
// RAII wrapper for a hipEvent_t.
//
// Move-only wrapper around hipEvent_t that automatically destroys the
// underlying event on destruction. Can be used to record events on streams.
struct hip_event
{
    hipEvent_t             m_event;
    oomph::util::moved_bit m_moved;
    bool                   m_recorded{false};

    hip_event()
    {
        OOMPH_CHECK_HIP_RESULT(hipEventCreateWithFlags(&m_event, hipEventDisableTiming));
    }
    hip_event(hip_event&& other) noexcept = default;
    hip_event& operator=(hip_event&& other) noexcept = default;
    hip_event(const hip_event&) = delete;
    hip_event& operator=(const hip_event&) = delete;
    ~hip_event() noexcept
    {
        if (!m_moved) { OOMPH_CHECK_HIP_RESULT_NO_THROW(hipEventDestroy(m_event)); }
    }

    operator bool() noexcept { return !m_moved; }

    void record(hipStream_t stream)
    {
        assert(!m_moved);
        OOMPH_CHECK_HIP_RESULT(hipEventRecord(m_event, stream));
        m_recorded = true;
    }

    bool is_ready() const
    {
        if (m_moved || !m_recorded) { return false; }

        hipError_t res = hipEventQuery(m_event);
        if (res == hipSuccess) { return true; }
        else if (res == hipErrorNotReady) { return false; }
        else
        {
            OOMPH_CHECK_HIP_RESULT(res);
            return false;
        }
    }

    hipEvent_t get()
    {
        assert(!m_moved);
        return m_event;
    }
};
} // namespace oomph::detail
