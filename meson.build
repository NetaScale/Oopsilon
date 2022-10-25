project('Scriptigent', ['c', 'cpp' ],
    default_options: [ 'cpp_std=c++20' ])

arch = 'amd64'

# this is only needed for arch=amd64
executable('limine-deploy', 'limine/limine-deploy.c', native: true)

subdir('psc')
