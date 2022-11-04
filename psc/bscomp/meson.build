flex = find_program('flex', required: true)

lgen = generator(flex,
    output : ['@PLAINNAME@.cc', '@PLAINNAME@.hh'],
    arguments : ['-o', '@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@'])

kern_srcs += [ lgen.process('scanner.ll'), lemgen.process('parser.yy') ]
kern_incs += include_directories('./')
