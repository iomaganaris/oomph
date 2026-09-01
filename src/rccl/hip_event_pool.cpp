/*
 * ghex-org
 *
 * Copyright (c) 2014-2026, ETH Zurich
 * All rights reserved.
 *
 * Please, refer to the LICENSE file in the root directory.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hip_event_pool.hpp"

namespace oomph::detail
{
hip_event_pool&
get_hip_event_pool()
{
    static hip_event_pool pool{128};
    return pool;
}
} // namespace oomph::detail
