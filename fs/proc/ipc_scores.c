// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/cpumask.h>
#include <asm/topology.h>
#include "internal.h"

static int ipc_scores_proc_show(struct seq_file *m, void *v)
{
    for (int cpu = 0; cpu < nr_cpu_ids; cpu++) {
        seq_printf(m, "cpu%d", cpu);
        for (unsigned short class = 0; ; class++) {
            unsigned long ipc_score = intel_hfi_get_ipcc_score(class, cpu);
            if (ipc_score == -EINVAL)
                break;

            seq_printf(m, " %lu", ipc_score);
        }
        seq_printf(m, "\n");
    }

    return 0;
}

static int __init proc_ipc_scores_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("ipc_scores", 0, NULL, ipc_scores_proc_show);
	pde_make_permanent(pde);
	return 0;
}
fs_initcall(proc_ipc_scores_init);
