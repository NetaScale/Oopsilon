ldscript = meson.current_source_dir()/'linker.ld'

kern_srcs += files ('crti.s', 'crtn.s', 'intr.c', 'kmain.c', 'locore.S',
    'machdep.c', 'vm.c')

vxkern = executable('vxkern', kern_srcs,
    c_args : [ kern_c_args ],
    cpp_args: [ kern_c_args ],
    link_args : [ '-nostdlib', '-static',
	  '-Wl,-z,max-page-size=0x1000', '-Wl,--build-id=none',
	  '-Wl,-T' + ldscript, '-lgcc', '/opt/x86_64-elf/lib/gcc/x86_64-elf/12.2.0/crtbegin.o',
      '/opt/x86_64-elf/lib/gcc/x86_64-elf/12.2.0/crtend.o' ],
    link_with: [ libm, klibcxxabi, klibcxx ],
    include_directories: [ kern_incs ],
)
