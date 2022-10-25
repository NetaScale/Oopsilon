kern_c_args = [ '-ffreestanding', '-fno-stack-protector', '-nostdlib',
	'-fno-stack-check', '-fno-pic', '-fno-pie', '-mabi=sysv',
	'-mno-3dnow', '-mno-sse3', '-mno-red-zone',
	'-mcmodel=kernel', '-D_KERNEL' ]

kern_c_args += [ '--param', 'asan-globals=0', '--param', 'asan-stack=0',
  '-fsanitize=kernel-address' ]

kern_srcs = files(
  'kern/kasan.c', 'kern/kmem_slab.c', 'kern/liballoc.c',
  'kern/liballoc_sysdep.c', 'kern/task.c', 'kern/vmem.c',

  'libkern/klib.c', 'libkern/uuid.c',

  'vm/vm_kernel.c','vm/vm_page.c', 'vm/vm_pageout.c', 'vm/vm.c',

  'qjs/cutils.c', 'qjs/libbf.c', 'qjs/libregexp.c', 'qjs/libunicode.c',
  'qjs/quickjs.c'
)

kern_incs = [ include_directories(arch + '/include', './', './libcshim') ]

subdir('libm')
subdir(arch)
