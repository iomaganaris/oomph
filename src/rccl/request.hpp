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

#include <variant>

#include <hip/hip_runtime.h>

#include "cached_hip_event.hpp"
#include "group_hip_event.hpp"
#include "hip_error.hpp"
#include "hip_event.hpp"

namespace oomph
{
struct rccl_request
{
    bool is_ready() const
    {
        return std::visit([](auto const& event) { return event.is_ready(); }, m_event);
    }

    // We store either a single event for a particular request, or a shared
    // event that signals the end of a RCCL group.
    std::variant<detail::cached_hip_event, detail::group_hip_event> m_event;
};
} // namespace oomph
