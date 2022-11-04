project('Scriptigent', ['c', 'cpp' ], default_options: [ 'cpp_std=c++20' ])

arch = get_option('arch')

# this is only needed for arch=amd64
executable('limine-deploy', 'limine/limine-deploy.c', native: true)

subdir('tools')
subdir('vendor/lemonpp')
if (arch != 'hosted')
	mlibc = subproject('mlibc', default_options: [
		'headers_only=true'
	])
	libc_includes = mlibc.get_variable('libc_include_dirs')
endif
subdir('psc')
