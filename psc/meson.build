kern_c_args = [ '-ffreestanding', '-fno-stack-protector', '-nostdlib',
	'-fno-stack-check', '-fno-pic', '-fno-pie', '-mabi=sysv', '-fno-omit-frame-pointer',
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
  'qjs/quickjs.c',

  'kern/cxx.cc'
)

kern_incs = [ include_directories(arch + '/include', './', './libcxx/include',
  # 
  './libcxxabi/include'), libc_includes, include_directories('./libcshim') ]

libcxxabi_c_args = [ kern_c_args, '-D_LIBCXXABI_HAS_NO_THREADS',
    '-D_LIBCPP_BUILDING_LIBRARY' ]
klibcxxabi = static_library('libkcxxabi',
  'libcxxabi/src/abort_message.cpp', 'libcxxabi/src/cxa_aux_runtime.cpp',
  'libcxxabi/src/cxa_default_handlers.cpp', 'libcxxabi/src/cxa_demangle.cpp',
  'libcxxabi/src/cxa_exception_storage.cpp', 'libcxxabi/src/cxa_exception.cpp',
  'libcxxabi/src/cxa_guard.cpp', 'libcxxabi/src/cxa_handlers.cpp',
  'libcxxabi/src/cxa_personality.cpp',
  'libcxxabi/src/cxa_vector.cpp', 'libcxxabi/src/cxa_virtual.cpp',
  'libcxxabi/src/fallback_malloc.cpp', 'libcxxabi/src/private_typeinfo.cpp',
  'libcxxabi/src/stdlib_exception.cpp', 'libcxxabi/src/stdlib_new_delete.cpp',
  'libcxxabi/src/stdlib_stdexcept.cpp', 'libcxxabi/src/stdlib_typeinfo.cpp',
	pic: true,
	include_directories: [ kern_incs,  'libcxx/src'],
	c_args: libcxxabi_c_args,
  cpp_args: libcxxabi_c_args,
)

libcxx_c_args = [ kern_c_args, '-D_LIBCPP_BUILDING_LIBRARY', '-DLIBCXXRT',
  '-DLIBCXX_BUILDING_LIBCXXABI' ]
klibcxx = static_library('klibcxx',
 'libcxx/src/algorithm.cpp', 'libcxx/src/hash.cpp', 
 'libcxx/src/any.cpp',
 'libcxx/src/atomic.cpp', 'libcxx/src/stdexcept.cpp', 'libcxx/src/string.cpp',
 'libcxx/src/bind.cpp', 'libcxx/src/legacy_debug_handler.cpp', 
 'libcxx/src/charconv.cpp', 'libcxx/src/legacy_pointer_safety.cpp', 'libcxx/src/system_error.cpp',
 'libcxx/src/memory.cpp', 'libcxx/src/typeinfo.cpp', 'libcxx/src/utility.cpp',
 'libcxx/src/valarray.cpp',
 'libcxx/src/exception.cpp', 'libcxx/src/new.cpp', 'libcxx/src/variant.cpp',
 'libcxx/src/format.cpp', 'libcxx/src/optional.cpp', 'libcxx/src/vector.cpp',
 'libcxx/src/functional.cpp',  'libcxx/src/verbose_abort.cpp',
 'libcxx/src/future.cpp', 
	pic: true,
	include_directories: [ kern_incs,  'libcxx/src'],
	c_args: libcxx_c_args,
  cpp_args: libcxx_c_args,
)

subdir('libm')
#kern_srcs += files (
#  ''
#)
#subdir('libcxx')
subdir(arch)
