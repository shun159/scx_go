
#ifndef __SCX_COMMON_BPF_H
#define __SCX_COMMON_BPF_H

#define BPF_STRUCT_OPS(name, args...)                                          \
  SEC("struct_ops/" #name)                                                     \
  BPF_PROG(name, ##args)

#define BPF_STRUCT_OPS_SLEEPABLE(name, args...)                                \
  SEC("struct_ops.s/" #name)                                                   \
  BPF_PROG(name, ##args)

#define SCX_OPS_DEFINE(__name, ...)                                            \
  SEC(".struct_ops.link")                                                      \
  struct sched_ext_ops __name = {__VA_ARGS__};

s32 scx_bpf_create_dsq(u64 dsq_id, s32 node) __ksym;

__s32 scx_bpf_select_cpu_dfl(struct task_struct *p, __s32 prev_cpu,
                             __u64 wake_flags, bool *is_idle) __ksym;

void scx_bpf_dispatch(struct task_struct *p, __u64 dsq_id, __u64 slice,
                      __u64 enq_flags) __ksym;

void scx_bpf_dispatch_vtime(struct task_struct *p, u64 dsq_id, u64 slice,
                            u64 vtime, u64 enq_flags) __ksym;

bool scx_bpf_consume(u64 dsq_id) __ksym;

#endif
