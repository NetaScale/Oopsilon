ldscript = meson.current_source_dir()/'linker.ld'

kern_srcs += files ('intr.c', 'kmain.c', 'locore.S', 'machdep.c', 'vm.c')

vxkern = executable('vxkern', kern_srcs,
    c_args : [ kern_c_args ],
    link_args : [ '-nostdlib', '-static',
	  '-Wl,-z,max-page-size=0x1000', '-Wl,--build-id=none',
	  '-Wl,-T' + ldscript, '-lgcc' ],
    link_with: libm,
    include_directories: [ kern_incs ],
)
