/*
 *
 * (C) COPYRIGHT 2015-2016 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */



#if !defined(_KBASE_TLSTREAM_H)
#define _KBASE_TLSTREAM_H

#include <mali_kbase.h>

/*****************************************************************************/

/**
 * kbase_tlstream_init - initialize timeline infrastructure in kernel
 * Return: zero on success, negative number on error
 */
int kbase_tlstream_init(void);

/**
 * kbase_tlstream_term - terminate timeline infrastructure in kernel
 *
 * Timeline need have to been previously enabled with kbase_tlstream_init().
 */
void kbase_tlstream_term(void);

/**
 * kbase_tlstream_acquire - acquire timeline stream file descriptor
 * @kctx:  kernel common context
 * @fd:    timeline stream file descriptor
 * @flags: timeline stream flags
 *
 * This descriptor is meant to be used by userspace timeline to gain access to
 * kernel timeline stream. This stream is later broadcasted by user space to the
 * timeline client.
 * Only one entity can own the descriptor at any given time. Descriptor shall be
 * closed if unused. If descriptor cannot be obtained (i.e. when it is already
 * being used) argument fd will contain negative value.
 *
 * Return: zero on success (this does not necessarily mean that stream
 *         descriptor could be returned), negative number on error
 */
int kbase_tlstream_acquire(struct kbase_context *kctx, int *fd, u32 flags);

/**
 * kbase_tlstream_flush_streams - flush timeline streams.
 *
 * Function will flush pending data in all timeline streams.
 */
void kbase_tlstream_flush_streams(void);

/**
 * kbase_tlstream_reset_body_streams - reset timeline body streams.
 *
 * Function will discard pending data in all timeline body streams.
 */
void kbase_tlstream_reset_body_streams(void);

#if MALI_UNIT_TEST
/**
 * kbase_tlstream_test - start timeline stream data generator
 * @tpw_count: number of trace point writers in each context
 * @msg_delay: time delay in milliseconds between trace points written by one
 *             writer
 * @msg_count: number of trace points written by one writer
 * @aux_msg:   if non-zero aux messages will be included
 *
 * This test starts a requested number of asynchronous writers in both IRQ and
 * thread context. Each writer will generate required number of test
 * tracepoints (tracepoints with embedded information about writer that
 * should be verified by user space reader). Tracepoints will be emitted in
 * all timeline body streams. If aux_msg is non-zero writer will also
 * generate not testable tracepoints (tracepoints without information about
 * writer). These tracepoints are used to check correctness of remaining
 * timeline message generating functions. Writer will wait requested time
 * between generating another set of messages. This call blocks until all
 * writers finish.
 */
void kbase_tlstream_test(
		unsigned int tpw_count,
		unsigned int msg_delay,
		unsigned int msg_count,
		int          aux_msg);

/**
 * kbase_tlstream_stats - read timeline stream statistics
 * @bytes_collected: will hold number of bytes read by the user
 * @bytes_generated: will hold number of bytes generated by trace points
 */
void kbase_tlstream_stats(u32 *bytes_collected, u32 *bytes_generated);
#endif /* MALI_UNIT_TEST */

/*****************************************************************************/

#define TL_ATOM_STATE_IDLE 0
#define TL_ATOM_STATE_READY 1
#define TL_ATOM_STATE_DONE 2
#define TL_ATOM_STATE_POSTED 3

void __kbase_tlstream_tl_summary_new_ctx(void *context, u32 nr, u32 tgid);
void __kbase_tlstream_tl_summary_new_gpu(void *gpu, u32 id, u32 core_count);
void __kbase_tlstream_tl_summary_new_lpu(void *lpu, u32 nr, u32 fn);
void __kbase_tlstream_tl_summary_lifelink_lpu_gpu(void *lpu, void *gpu);
void __kbase_tlstream_tl_summary_new_as(void *as, u32 nr);
void __kbase_tlstream_tl_summary_lifelink_as_gpu(void *as, void *gpu);
void __kbase_tlstream_tl_new_ctx(void *context, u32 nr, u32 tgid);
void __kbase_tlstream_tl_new_atom(void *atom, u32 nr);
void __kbase_tlstream_tl_del_ctx(void *context);
void __kbase_tlstream_tl_del_atom(void *atom);
void __kbase_tlstream_tl_ret_ctx_lpu(void *context, void *lpu);
void __kbase_tlstream_tl_ret_atom_ctx(void *atom, void *context);
void __kbase_tlstream_tl_ret_atom_lpu(
		void *atom, void *lpu, const char *attrib_match_list);
void __kbase_tlstream_tl_nret_ctx_lpu(void *context, void *lpu);
void __kbase_tlstream_tl_nret_atom_ctx(void *atom, void *context);
void __kbase_tlstream_tl_nret_atom_lpu(void *atom, void *lpu);
void __kbase_tlstream_tl_ret_as_ctx(void *as, void *ctx);
void __kbase_tlstream_tl_nret_as_ctx(void *as, void *ctx);
void __kbase_tlstream_tl_ret_atom_as(void *atom, void *as);
void __kbase_tlstream_tl_nret_atom_as(void *atom, void *as);
void __kbase_tlstream_tl_dep_atom_atom(void *atom1, void *atom2);
void __kbase_tlstream_tl_ndep_atom_atom(void *atom1, void *atom2);
void __kbase_tlstream_tl_rdep_atom_atom(void *atom1, void *atom2);
void __kbase_tlstream_tl_attrib_atom_config(
		void *atom, u64 jd, u64 affinity, u32 config);
void __kbase_tlstream_tl_attrib_atom_priority(void *atom, u32 prio);
void __kbase_tlstream_tl_attrib_atom_state(void *atom, u32 state);
void __kbase_tlstream_tl_attrib_atom_priority_change(void *atom);
void __kbase_tlstream_tl_attrib_as_config(
		void *as, u64 transtab, u64 memattr, u64 transcfg);
void __kbase_tlstream_tl_event_atom_softstop_ex(void *atom);
void __kbase_tlstream_tl_event_lpu_softstop(void *lpu);
void __kbase_tlstream_tl_event_atom_softstop_issue(void *atom);
void __kbase_tlstream_jd_gpu_soft_reset(void *gpu);
void __kbase_tlstream_aux_pm_state(u32 core_type, u64 state);
void __kbase_tlstream_aux_pagefault(u32 ctx_nr, u64 page_count_change);
void __kbase_tlstream_aux_pagesalloc(u32 ctx_nr, u64 page_count);
void __kbase_tlstream_aux_devfreq_target(u64 target_freq);

#define TLSTREAM_ENABLED (1 << 31)

extern atomic_t kbase_tlstream_enabled;

#define __TRACE_IF_ENABLED(trace_name, ...)                         \
	do {                                                        \
		int enabled = atomic_read(&kbase_tlstream_enabled); \
		if (enabled & TLSTREAM_ENABLED)                     \
			__kbase_tlstream_##trace_name(__VA_ARGS__); \
	} while (0)

#define __TRACE_IF_ENABLED_LATENCY(trace_name, ...)                     \
	do {                                                            \
		int enabled = atomic_read(&kbase_tlstream_enabled);     \
		if (enabled & BASE_TLSTREAM_ENABLE_LATENCY_TRACEPOINTS) \
			__kbase_tlstream_##trace_name(__VA_ARGS__);     \
	} while (0)

/*****************************************************************************/

/**
 * KBASE_TLSTREAM_TL_SUMMARY_NEW_CTX - create context object in timeline
 *                                     summary
 * @context: name of the context object
 * @nr:      context number
 * @tgid:    thread Group Id
 *
 * Function emits a timeline message informing about context creation. Context
 * is created with context number (its attribute), that can be used to link
 * kbase context with userspace context.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_NEW_CTX(context, nr, tgid) \
	__TRACE_IF_ENABLED(tl_summary_new_ctx, context, nr, tgid)

/**
 * KBASE_TLSTREAM_TL_SUMMARY_NEW_GPU - create GPU object in timeline summary
 * @gpu:        name of the GPU object
 * @id:         id value of this GPU
 * @core_count: number of cores this GPU hosts
 *
 * Function emits a timeline message informing about GPU creation. GPU is
 * created with two attributes: id and core count.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_NEW_GPU(gpu, id, core_count) \
	__TRACE_IF_ENABLED(tl_summary_new_gpu, gpu, id, core_count)

/**
 * KBASE_TLSTREAM_TL_SUMMARY_NEW_LPU - create LPU object in timeline summary
 * @lpu: name of the Logical Processing Unit object
 * @nr:  sequential number assigned to this LPU
 * @fn:  property describing this LPU's functional abilities
 *
 * Function emits a timeline message informing about LPU creation. LPU is
 * created with two attributes: number linking this LPU with GPU's job slot
 * and function bearing information about this LPU abilities.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_NEW_LPU(lpu, nr, fn) \
	__TRACE_IF_ENABLED(tl_summary_new_lpu, lpu, nr, fn)

/**
 * KBASE_TLSTREAM_TL_SUMMARY_LIFELINK_LPU_GPU - lifelink LPU object to GPU
 * @lpu: name of the Logical Processing Unit object
 * @gpu: name of the GPU object
 *
 * Function emits a timeline message informing that LPU object shall be deleted
 * along with GPU object.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_LIFELINK_LPU_GPU(lpu, gpu) \
	__TRACE_IF_ENABLED(tl_summary_lifelink_lpu_gpu, lpu, gpu)

/**
 * KBASE_TLSTREAM_TL_SUMMARY_NEW_AS - create address space object in timeline summary
 * @as: name of the address space object
 * @nr: sequential number assigned to this address space
 *
 * Function emits a timeline message informing about address space creation.
 * Address space is created with one attribute: number identifying this
 * address space.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_NEW_AS(as, nr) \
	__TRACE_IF_ENABLED(tl_summary_new_as, as, nr)

/**
 * KBASE_TLSTREAM_TL_SUMMARY_LIFELINK_AS_GPU - lifelink address space object to GPU
 * @as:  name of the address space object
 * @gpu: name of the GPU object
 *
 * Function emits a timeline message informing that address space object
 * shall be deleted along with GPU object.
 * This message is directed to timeline summary stream.
 */
#define KBASE_TLSTREAM_TL_SUMMARY_LIFELINK_AS_GPU(as, gpu) \
	__TRACE_IF_ENABLED(tl_summary_lifelink_as_gpu, as, gpu)

/**
 * KBASE_TLSTREAM_TL_NEW_CTX - create context object in timeline
 * @context: name of the context object
 * @nr:      context number
 * @tgid:    thread Group Id
 *
 * Function emits a timeline message informing about context creation. Context
 * is created with context number (its attribute), that can be used to link
 * kbase context with userspace context.
 */
#define KBASE_TLSTREAM_TL_NEW_CTX(context, nr, tgid) \
	__TRACE_IF_ENABLED(tl_new_ctx, context, nr, tgid)

/**
 * KBASE_TLSTREAM_TL_NEW_ATOM - create atom object in timeline
 * @atom: name of the atom object
 * @nr:   sequential number assigned to this atom
 *
 * Function emits a timeline message informing about atom creation. Atom is
 * created with atom number (its attribute) that links it with actual work
 * bucket id understood by hardware.
 */
#define KBASE_TLSTREAM_TL_NEW_ATOM(atom, nr) \
	__TRACE_IF_ENABLED(tl_new_atom, atom, nr)

/**
 * KBASE_TLSTREAM_TL_DEL_CTX - destroy context object in timeline
 * @context: name of the context object
 *
 * Function emits a timeline message informing that context object ceased to
 * exist.
 */
#define KBASE_TLSTREAM_TL_DEL_CTX(context) \
	__TRACE_IF_ENABLED(tl_del_ctx, context)

/**
 * KBASE_TLSTREAM_TL_DEL_ATOM - destroy atom object in timeline
 * @atom: name of the atom object
 *
 * Function emits a timeline message informing that atom object ceased to
 * exist.
 */
#define KBASE_TLSTREAM_TL_DEL_ATOM(atom) \
	__TRACE_IF_ENABLED(tl_del_atom, atom)

/**
 * KBASE_TLSTREAM_TL_RET_CTX_LPU - retain context by LPU
 * @context: name of the context object
 * @lpu:     name of the Logical Processing Unit object
 *
 * Function emits a timeline message informing that context is being held
 * by LPU and must not be deleted unless it is released.
 */
#define KBASE_TLSTREAM_TL_RET_CTX_LPU(context, lpu) \
	__TRACE_IF_ENABLED(tl_ret_ctx_lpu, context, lpu)

/**
 * KBASE_TLSTREAM_TL_RET_ATOM_CTX - retain atom by context
 * @atom:    name of the atom object
 * @context: name of the context object
 *
 * Function emits a timeline message informing that atom object is being held
 * by context and must not be deleted unless it is released.
 */
#define KBASE_TLSTREAM_TL_RET_ATOM_CTX(atom, context) \
	__TRACE_IF_ENABLED(tl_ret_atom_ctx, atom, context)

/**
 * KBASE_TLSTREAM_TL_RET_ATOM_LPU - retain atom by LPU
 * @atom:              name of the atom object
 * @lpu:               name of the Logical Processing Unit object
 * @attrib_match_list: list containing match operator attributes
 *
 * Function emits a timeline message informing that atom object is being held
 * by LPU and must not be deleted unless it is released.
 */
#define KBASE_TLSTREAM_TL_RET_ATOM_LPU(atom, lpu, attrib_match_list) \
	__TRACE_IF_ENABLED(tl_ret_atom_lpu, atom, lpu, attrib_match_list)

/**
 * KBASE_TLSTREAM_TL_NRET_CTX_LPU - release context by LPU
 * @context: name of the context object
 * @lpu:     name of the Logical Processing Unit object
 *
 * Function emits a timeline message informing that context is being released
 * by LPU object.
 */
#define KBASE_TLSTREAM_TL_NRET_CTX_LPU(context, lpu) \
	__TRACE_IF_ENABLED(tl_nret_ctx_lpu, context, lpu)

/**
 * KBASE_TLSTREAM_TL_NRET_ATOM_CTX - release atom by context
 * @atom:    name of the atom object
 * @context: name of the context object
 *
 * Function emits a timeline message informing that atom object is being
 * released by context.
 */
#define KBASE_TLSTREAM_TL_NRET_ATOM_CTX(atom, context) \
	__TRACE_IF_ENABLED(tl_nret_atom_ctx, atom, context)

/**
 * KBASE_TLSTREAM_TL_NRET_ATOM_LPU - release atom by LPU
 * @atom: name of the atom object
 * @lpu:  name of the Logical Processing Unit object
 *
 * Function emits a timeline message informing that atom object is being
 * released by LPU.
 */
#define KBASE_TLSTREAM_TL_NRET_ATOM_LPU(atom, lpu) \
	__TRACE_IF_ENABLED(tl_nret_atom_lpu, atom, lpu)

/**
 * KBASE_TLSTREAM_TL_RET_AS_CTX - lifelink address space object to context
 * @as:  name of the address space object
 * @ctx: name of the context object
 *
 * Function emits a timeline message informing that address space object
 * is being held by the context object.
 */
#define KBASE_TLSTREAM_TL_RET_AS_CTX(as, ctx) \
	__TRACE_IF_ENABLED(tl_ret_as_ctx, as, ctx)

/**
 * KBASE_TLSTREAM_TL_NRET_AS_CTX - release address space by context
 * @as:  name of the address space object
 * @ctx: name of the context object
 *
 * Function emits a timeline message informing that address space object
 * is being released by atom.
 */
#define KBASE_TLSTREAM_TL_NRET_AS_CTX(as, ctx) \
	__TRACE_IF_ENABLED(tl_nret_as_ctx, as, ctx)

/**
 * KBASE_TLSTREAM_TL_RET_ATOM_AS - retain atom by address space
 * @atom: name of the atom object
 * @as:   name of the address space object
 *
 * Function emits a timeline message informing that atom object is being held
 * by address space and must not be deleted unless it is released.
 */
#define KBASE_TLSTREAM_TL_RET_ATOM_AS(atom, as) \
	__TRACE_IF_ENABLED(tl_ret_atom_as, atom, as)

/**
 * KBASE_TLSTREAM_TL_NRET_ATOM_AS - release atom by address space
 * @atom: name of the atom object
 * @as:   name of the address space object
 *
 * Function emits a timeline message informing that atom object is being
 * released by address space.
 */
#define KBASE_TLSTREAM_TL_NRET_ATOM_AS(atom, as) \
	__TRACE_IF_ENABLED(tl_nret_atom_as, atom, as)

/**
 * KBASE_TLSTREAM_TL_DEP_ATOM_ATOM - parent atom depends on child atom
 * @atom1: name of the child atom object
 * @atom2: name of the parent atom object that depends on child atom
 *
 * Function emits a timeline message informing that parent atom waits for
 * child atom object to be completed before start its execution.
 */
#define KBASE_TLSTREAM_TL_DEP_ATOM_ATOM(atom1, atom2) \
	__TRACE_IF_ENABLED(tl_dep_atom_atom, atom1, atom2)

/**
 * KBASE_TLSTREAM_TL_NDEP_ATOM_ATOM - dependency between atoms resolved
 * @atom1: name of the child atom object
 * @atom2: name of the parent atom object that depended on child atom
 *
 * Function emits a timeline message informing that parent atom execution
 * dependency on child atom has been resolved.
 */
#define KBASE_TLSTREAM_TL_NDEP_ATOM_ATOM(atom1, atom2) \
	__TRACE_IF_ENABLED(tl_ndep_atom_atom, atom1, atom2)

/**
 * KBASE_TLSTREAM_TL_RDEP_ATOM_ATOM - information about already resolved dependency between atoms
 * @atom1: name of the child atom object
 * @atom2: name of the parent atom object that depended on child atom
 *
 * Function emits a timeline message informing that parent atom execution
 * dependency on child atom has been resolved.
 */
#define KBASE_TLSTREAM_TL_RDEP_ATOM_ATOM(atom1, atom2) \
	__TRACE_IF_ENABLED(tl_rdep_atom_atom, atom1, atom2)

/**
 * KBASE_TLSTREAM_TL_ATTRIB_ATOM_CONFIG - atom job slot attributes
 * @atom:     name of the atom object
 * @jd:       job descriptor address
 * @affinity: job affinity
 * @config:   job config
 *
 * Function emits a timeline message containing atom attributes.
 */
#define KBASE_TLSTREAM_TL_ATTRIB_ATOM_CONFIG(atom, jd, affinity, config) \
	__TRACE_IF_ENABLED(tl_attrib_atom_config, atom, jd, affinity, config)

/**
 * KBASE_TLSTREAM_TL_ATTRIB_ATOM_PRIORITY - atom priority
 * @atom: name of the atom object
 * @prio: atom priority
 *
 * Function emits a timeline message containing atom priority.
 */
#define KBASE_TLSTREAM_TL_ATTRIB_ATOM_PRIORITY(atom, prio) \
	__TRACE_IF_ENABLED_LATENCY(tl_attrib_atom_priority, atom, prio)

/**
 * KBASE_TLSTREAM_TL_ATTRIB_ATOM_STATE - atom state
 * @atom:  name of the atom object
 * @state: atom state
 *
 * Function emits a timeline message containing atom state.
 */
#define KBASE_TLSTREAM_TL_ATTRIB_ATOM_STATE(atom, state) \
	__TRACE_IF_ENABLED_LATENCY(tl_attrib_atom_state, atom, state)

/**
 * KBASE_TLSTREAM_TL_ATTRIB_ATOM_PRIORITY_CHANGE - atom caused priority change
 * @atom:  name of the atom object
 *
 * Function emits a timeline message signalling priority change
 */
#define KBASE_TLSTREAM_TL_ATTRIB_ATOM_PRIORITY_CHANGE(atom) \
	__TRACE_IF_ENABLED_LATENCY(tl_attrib_atom_priority_change, atom)

/**
 * KBASE_TLSTREAM_TL_ATTRIB_AS_CONFIG - address space attributes
 * @as:       assigned address space
 * @transtab: configuration of the TRANSTAB register
 * @memattr:  configuration of the MEMATTR register
 * @transcfg: configuration of the TRANSCFG register (or zero if not present)
 *
 * Function emits a timeline message containing address space attributes.
 */
#define KBASE_TLSTREAM_TL_ATTRIB_AS_CONFIG(as, transtab, memattr, transcfg) \
	__TRACE_IF_ENABLED(tl_attrib_as_config, as, transtab, memattr, transcfg)

/**
 * KBASE_TLSTREAM_TL_EVENT_ATOM_SOFTSTOP_ex
 * @atom:       atom identifier
 */
#define KBASE_TLSTREAM_TL_EVENT_ATOM_SOFTSTOP_EX(atom) \
	__TRACE_IF_ENABLED(tl_event_atom_softstop_ex, atom)

/**
 * KBASE_TLSTREAM_TL_EVENT_LPU_softstop
 * @lpu:        name of the LPU object
 */
#define KBASE_TLSTREAM_TL_EVENT_LPU_SOFTSTOP(lpu) \
	__TRACE_IF_ENABLED(tl_event_lpu_softstop, lpu)

/**
 * KBASE_TLSTREAM_TL_EVENT_ATOM_SOFTSTOP_issue
 * @atom:       atom identifier
 */
#define KBASE_TLSTREAM_TL_EVENT_ATOM_SOFTSTOP_ISSUE(atom) \
	__TRACE_IF_ENABLED(tl_event_atom_softstop_issue, atom)

/**
 * KBASE_TLSTREAM_JD_GPU_SOFT_RESET - The GPU is being soft reset
 * @gpu:        name of the GPU object
 *
 * This imperative tracepoint is specific to job dumping.
 * Function emits a timeline message indicating GPU soft reset.
 */
#define KBASE_TLSTREAM_JD_GPU_SOFT_RESET(gpu) \
	__TRACE_IF_ENABLED(jd_gpu_soft_reset, gpu)

/**
 * KBASE_TLSTREAM_AUX_PM_STATE - timeline message: power management state
 * @core_type: core type (shader, tiler, l2 cache, l3 cache)
 * @state:     64bits bitmask reporting power state of the cores (1-ON, 0-OFF)
 */
#define KBASE_TLSTREAM_AUX_PM_STATE(core_type, state) \
	__TRACE_IF_ENABLED(aux_pm_state, core_type, state)

/**
 * KBASE_TLSTREAM_AUX_PAGEFAULT - timeline message: MMU page fault event
 *                                resulting in new pages being mapped
 * @ctx_nr:            kernel context number
 * @page_count_change: number of pages to be added
 */
#define KBASE_TLSTREAM_AUX_PAGEFAULT(ctx_nr, page_count_change) \
	__TRACE_IF_ENABLED(aux_pagefault, ctx_nr, page_count_change)

/**
 * KBASE_TLSTREAM_AUX_PAGESALLOC - timeline message: total number of allocated
 *                                 pages is changed
 * @ctx_nr:     kernel context number
 * @page_count: number of pages used by the context
 */
#define KBASE_TLSTREAM_AUX_PAGESALLOC(ctx_nr, page_count) \
	__TRACE_IF_ENABLED(aux_pagesalloc, ctx_nr, page_count)

/**
 * KBASE_TLSTREAM_AUX_DEVFREQ_TARGET - timeline message: new target DVFS
 *                                     frequency
 * @target_freq: new target frequency
 */
#define KBASE_TLSTREAM_AUX_DEVFREQ_TARGET(target_freq) \
	__TRACE_IF_ENABLED(aux_devfreq_target, target_freq)

#endif /* _KBASE_TLSTREAM_H */
