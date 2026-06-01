/*
 * lpmd_cgroup.c: task isolation via cgroup setting
 *
 * Copyright (C) 2025 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#define _GNU_SOURCE
#include "lpmd.h"

/* Direct cgroup v2 CPU isolation (no systemd required).
 * Creates a dedicated cgroup /sys/fs/cgroup/lpm/ with cpuset controller,
 * then sets exclusive CPU affinity for tasks placed in this cgroup. */

#define PATH_CGROUP			"/sys/fs/cgroup"
#define PATH_CG2_SUBTREE_CONTROL	PATH_CGROUP "/cgroup.subtree_control"

static int process_cpu_isolate(lpmd_config_state_t *state)
{
	if (lpmd_write_str ("/sys/fs/cgroup/lpm/cpuset.cpus.partition", "member", LPMD_LOG_DEBUG))
		return 1;

	if (!cpumask_equal(state->cpumask_idx, CPUMASK_ONLINE)) {
		if (lpmd_write_str ("/sys/fs/cgroup/lpm/cpuset.cpus.exclusive",
				    get_cpu_isolation_str(state->cpumask_idx), LPMD_LOG_DEBUG))
			return 1;
		if (lpmd_write_str ("/sys/fs/cgroup/lpm/cpuset.cpus.partition", "isolated", LPMD_LOG_DEBUG))
			return 1;
		if (lpmd_write_str ("/sys/fs/cgroup/lpm/cpuset.cpus",
				    get_cpu_isolation_str(state->cpumask_idx), LPMD_LOG_DEBUG))
			return 1;
	} else {
		if (lpmd_write_str ("/sys/fs/cgroup/lpm/cpuset.cpus",
				    get_cpu_isolation_str(CPUMASK_ONLINE), LPMD_LOG_DEBUG))
			return 1;
	}

	return 0;
}

int cgroup_cleanup(void)
{
	DIR *dir = opendir("/sys/fs/cgroup/lpm");
	if (dir) {
		closedir(dir);
		rmdir("/sys/fs/cgroup/lpm");
	}
	return 0;
}

int cgroup_init(lpmd_config_t *config)
{
	if (lpmd_write_str (PATH_CG2_SUBTREE_CONTROL, "+cpuset", LPMD_LOG_DEBUG))
		return 1;
	return mkdir ("/sys/fs/cgroup/lpm", 0744);
}

int process_cgroup(lpmd_config_state_t *state, enum lpm_cpu_process_mode mode)
{
	if (state->cpumask_idx == CPUMASK_NONE) {
		lpmd_log_debug ("Ignore cgroup processing\n");
		return 0;
	}

	lpmd_log_info ("Process Cgroup (isolate mode) ...\n");
	return process_cpu_isolate(state);
}
