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

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include <oomph/context.hpp>

namespace oomph::test
{
inline bool
is_nccl_backend(oomph::context const& ctxt)
{
    return ctxt.get_transport_option("name") == std::string("nccl");
}

inline bool
is_nccl_backend()
{
    static const bool result = is_nccl_backend(oomph::context(MPI_COMM_WORLD, false));
    return result;
}

inline bool
is_rccl_backend(oomph::context const& ctxt)
{
    return ctxt.get_transport_option("name") == std::string("rccl");
}

inline bool
is_rccl_backend()
{
    static const bool result = is_rccl_backend(oomph::context(MPI_COMM_WORLD, false));
    return result;
}

// NCCL-like backends (NCCL and RCCL) share the same semantics and
// restrictions: ordered communication, no cancellation, no thread_safe
// option, and self-send/recv only within groups.
inline bool
is_nccl_like_backend(oomph::context const& ctxt)
{
    return is_nccl_backend(ctxt) || is_rccl_backend(ctxt);
}

inline bool
is_nccl_like_backend()
{
    static const bool result = is_nccl_like_backend(oomph::context(MPI_COMM_WORLD, false));
    return result;
}

inline void
handle_nccl_like_thread_safe_exception(std::runtime_error const& e)
{
    if (is_nccl_backend())
    {
        EXPECT_EQ(e.what(), std::string("NCCL not supported with thread_safe = true"));
    }
    else if (is_rccl_backend())
    {
        EXPECT_EQ(e.what(), std::string("RCCL not supported with thread_safe = true"));
    }
    else { throw; }
}
} // namespace oomph::test
