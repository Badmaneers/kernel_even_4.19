#ifndef _LINUX_UCLAMP_H
#define _LINUX_UCLAMP_H

#include <linux/static_key.h>
#include <linux/sched.h>      // for struct task_struct, cpu_rq()
#include <linux/sched/topology.h> // for rq definition, maybe

/* External static key, defined in a C file */
extern struct static_key_false sched_uclamp_used;

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA \
	DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

#define for_each_clamp_id(clamp_id) \
	for ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

static inline unsigned int uclamp_bucket_id(unsigned int clamp_value)
{
	return min_t(unsigned int, clamp_value / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

static inline unsigned int uclamp_bucket_base_value(unsigned int clamp_value)
{
	return UCLAMP_BUCKET_DELTA * uclamp_bucket_id(clamp_value);
}

static inline unsigned int uclamp_none(enum uclamp_id clamp_id)
{
	return (clamp_id == UCLAMP_MIN) ? 0 : SCHED_CAPACITY_SCALE;
}

static inline void uclamp_se_set(struct uclamp_se *uc_se,
				 unsigned int value, bool user_defined)
{
	uc_se->value = value;
	uc_se->bucket_id = uclamp_bucket_id(value);
	uc_se->user_defined = user_defined;
}

#endif /* _LINUX_UCLAMP_H */
