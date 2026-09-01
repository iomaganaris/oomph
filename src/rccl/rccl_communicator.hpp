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

#include <thread>

#include <rccl/rccl.h>

#include <oomph/util/mpi_error.hpp>
#include <oomph/util/moved_bit.hpp>

#include "../mpi_comm.hpp"
#include "hip_error.hpp"
#include "rccl_error.hpp"

namespace oomph::detail
{
class rccl_comm
{
    ncclComm_t             m_comm;
    oomph::util::moved_bit m_moved;

  public:
    rccl_comm(mpi_comm mpi_comm)
    {
        ncclUniqueId id;
        if (mpi_comm.rank() == 0) { OOMPH_CHECK_RCCL_RESULT(ncclGetUniqueId(&id)); }

        OOMPH_CHECK_MPI_RESULT(MPI_Bcast(&id, sizeof(id), MPI_BYTE, 0, mpi_comm.get()));

        OOMPH_CHECK_RCCL_RESULT(ncclCommInitRank(&m_comm, mpi_comm.size(), id, mpi_comm.rank()));
        ncclResult_t result;
        OOMPH_CHECK_RCCL_RESULT(ncclCommGetAsyncError(m_comm, &result));
        while (result == ncclInProgress)
        {
            std::this_thread::yield();
            OOMPH_CHECK_RCCL_RESULT(ncclCommGetAsyncError(m_comm, &result));
        }
        OOMPH_CHECK_RCCL_RESULT(result);
    }
    rccl_comm(rccl_comm&&) noexcept = default;
    rccl_comm& operator=(rccl_comm&&) noexcept = default;
    rccl_comm(rccl_comm const&) = delete;
    rccl_comm& operator=(rccl_comm const&) = delete;
    ~rccl_comm() noexcept
    {
        if (!m_moved)
        {
            OOMPH_CHECK_HIP_RESULT_NO_THROW(hipDeviceSynchronize());
            OOMPH_CHECK_RCCL_RESULT_NO_THROW(ncclCommDestroy(m_comm));
        }
    }

    ncclComm_t get() const noexcept { return m_comm; }
};
} // namespace oomph::detail
