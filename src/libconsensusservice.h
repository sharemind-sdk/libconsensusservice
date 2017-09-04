/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#ifndef SHAREMIND_LIBCONSENSUSSERVICE_H
#define SHAREMIND_LIBCONSENSUSSERVICE_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef SHAREMIND_ICONST
#ifdef SHAREMIND_INTERNAL_
#define SHAREMIND_ICONST
#else
#define SHAREMIND_ICONST const
#endif
#endif /* SHAREMIND_ICONST */


typedef uint8_t SharemindConsensusResultType;

typedef struct SharemindConsensusDatum_ {
    void const * data;
    size_t size;
} SharemindConsensusDatum;

typedef enum SharemindConsensusFacilityError_ {

    /** No error */
    SHAREMIND_CONSENSUS_FACILITY_OK,

    /** Unknown error */
    SHAREMIND_CONSENSUS_FACILITY_UNKNOWN_ERROR,

    /** Operation type did not provide pointer to all methods */
    SHAREMIND_CONSENSUS_FACILITY_BAD_OPERATION_TYPE,

    /** Failed to allocate required memory */
    SHAREMIND_CONSENSUS_FACILITY_OUT_OF_MEMORY,

    /** Multiple operation types with the same name were added */
    SHAREMIND_CONSENSUS_FACILITY_DUPLICATE_OPERATION_TYPE,

    /** Asynchronous operation has not finished */
    SHAREMIND_CONSENSUS_FACILITY_AGAIN,

    /** Distributed operation did not succeed on all miners */
    SHAREMIND_CONSENSUS_FACILITY_FAIL

} SharemindConsensusFacilityError;

/**
 * \brief Structure for implementing a distributed operation for the consensus
 *        service.
 */
typedef struct SharemindOperationType_ {

    /**
     * Callback to be called to test equivalence of proposals in the
     * leader node.
     * \param[in] proposals array of proposals from all miners.
     * \param[in] count number of proposals.
     * \returns whether the proposals are equivalent.
     */
    bool (*equivalent)(const SharemindConsensusDatum * proposals, size_t count);

    /**
     * Callback to be called when the proposed operation is executed.
     * \param[in] proposals array of proposals from all miners.
     * \param[in] count number of proposals.
     * \param[in] callbackPtr the pointer passed to propose or blockingPropose.
     * \returns result code.
     */
    SharemindConsensusResultType (*execute)(
            const SharemindConsensusDatum * proposals,
            size_t count,
            void * callbackPtr);

    /**
     * Callback to be called after the proposed operation has been executed
     * on all miners.
     * \param[in] proposals array of proposals from all miners.
     * \param[in] count number of proposals.
     * \param[in] results result codes received from all miners.
     * \param[in] callbackPtr the pointer passed to propose or blockingPropose.
     */
    void (*commit)(const SharemindConsensusDatum * proposals,
                   size_t count,
                   const SharemindConsensusResultType * results,
                   void * callbackPtr);

    /**
     * The name of the operation.
     */
    const char * name;

} SharemindOperationType;

struct SharemindConsensusFacility_;
typedef struct SharemindConsensusFacility_ SharemindConsensusFacility;
struct SharemindConsensusFacility_ {

    /**
     * Propose an operation.
     * \param facility pointer to this SharemindConsensusFacility object.
     * \param operationType name of the operation type
     * \param size size of operation data in bytes
     * \param data pointer to operation data
     * \param callbackPtr pointer to data that's passed to callback
     * functions of the operation type
     * \param sequenceNo the sequence number of the proposal will be
     * written to this location. It can be used to retrieve the
     * operation's result.
     * \retval SHAREMIND_CONSENSUS_FACILITY_NOT_STARTED if the
     * consensus service has not been started
     */
    SharemindConsensusFacilityError
    (* SHAREMIND_ICONST propose)(SharemindConsensusFacility * facility,
                                 const char * operationType,
                                 size_t size,
                                 const void * data,
                                 void * callbackPtr,
                                 uint32_t * sequenceNo);

    /**
     * Propose an operation. Blocks until the operation has
     * finished. Returns SHAREMIND_CONSENSUS_FACILITY_NOT_STARTED if
     * the consensus service has not been started yet.
     * \param facility pointer to this SharemindConsensusFacility object.
     * \param operationType name of the operation type
     * \param size size of operation data in bytes
     * \param data pointer to operation data
     * \param callbackPtr pointer to data that's passed to callback
     * functions of the operation type
     * \retval returns SHAREMIND_CONSENSUS_FACILITY_OK if the call was
     * successful
     */
    SharemindConsensusFacilityError
    (* SHAREMIND_ICONST blocking_propose)(SharemindConsensusFacility * facility,
                                          const char * operationType,
                                          size_t size,
                                          const void * data,
                                          void * callbackPtr);

    /**
     * Check if an asynchronous operation has finished.
     * \param sequenceNo sequence number of the proposal
     * \retval returns SHAREMIND_CONSENSUS_FACILITY_OK if the
     * operation has finished on all miners
     * SHAREMIND_CONSENSUS_FACILITY_AGAIN if the operation has not
     * finished, SHAREMIND_CONSENSUS_FACILITY_FAIL otherwise
     */
    SharemindConsensusFacilityError
    (* SHAREMIND_ICONST done)(SharemindConsensusFacility * facility,
                              uint32_t seq);

    /**
     * Add an operation type. Returns
     * SHAREMIND_CONSENSUS_FACILITY_BAD_OPERATION_TYPE if the type does not
     * provide all required methods or
     * SHAREMIND_CONSENSUS_FACILITY_DUPLICATE_OPERATION_TYPE if a type with the
     * same name has already been added.
     * \param facility pointer to this SharemindConsensusFacility object.
     * \param[in] operationType struct with pointers to the callback methods
     */
    SharemindConsensusFacilityError
    (* SHAREMIND_ICONST add_operation_type)(
            SharemindConsensusFacility * facility,
            const SharemindOperationType * operationType);

};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_LIBCONSENSUSSERVICE_H */
