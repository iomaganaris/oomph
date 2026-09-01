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

#include <memory>

#include "cached_hip_event.hpp"

namespace oomph::detail
{
// A shared hip_event suitable for use with RCCL groups.
//
// A cached_hip_event stored in a shared_ptr for shared usage between multiple
// requests.
struct group_hip_event
{
    std::shared_ptr<cached_hip_event> m_event;

    group_hip_event()
    : m_event(std::make_shared<cached_hip_event>())
    {
    }
    group_hip_event(const group_hip_event&) = default;
    group_hip_event& operator=(const group_hip_event&) = default;
    group_hip_event(group_hip_event&&) = default;
    group_hip_event& operator=(group_hip_event&&) = default;

    void record(hipStream_t stream) { m_event->record(stream); }

    bool is_ready() const { return m_event->is_ready(); }

    hipEvent_t get() { return m_event->get(); }
};
} // namespace oomph::detail
