/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/*
 * Copyright 2022 NetaScale Systems Ltd.
 * All rights reserved.
 */

#ifndef MACHINE_VM_H_
#define MACHINE_VM_H_

#include <sys/queue.h>

#define PGSIZE 4096

#define USER_BASE 0x1000
#define HHDM_BASE 0xffff800000000000
#define KHEAP_BASE 0xffff800100000000
#define KERN_BASE 0xffffffff80000000

#define USER_SIZE 0x100000000
#define HHDM_SIZE 0x100000000  /* 4GiB */
#define KHEAP_SIZE 0x100000000 /* 4GiB */
#define KERN_SIZE 0x10000000   /* 256MiB */

#define P2V(addr) (((void *)(addr)) + HHDM_BASE)
#define V2P(addr) (((void *)(addr)) - HHDM_BASE)

/** entry for vm_page::pv_table's map of virtual mappings per physical page */
typedef struct pv_entry {
	LIST_ENTRY(pv_entry) pv_entries;
	struct vm_map	      *map;
	void		*vaddr;
} pv_entry_t;

#endif /* VM_H_ */
