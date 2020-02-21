libaoscdk_deps = [
  dependency('glib-2.0'),
  dependency('gio-2.0'),
]

libaoscdk_srcs = files(
  'lib.c',

  'log/log.c',
)

subdir('include')

libaoscdk = library(
  'aoscdk',
  libaoscdk_srcs,
  dependencies: libaoscdk_deps,
  include_directories: libaoscdk_lib_incs,
  install: true
)
