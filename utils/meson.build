util_libaoscdk_deps = [
  dependency('glib-2.0'),
]

util_libaoscdk_srcs = files(
  'libaoscdk.c'
)

executable(
  'libaoscdk',
  util_libaoscdk_srcs,
  include_directories: global_include,
  dependencies: util_libaoscdk_deps,
  link_with: libaoscdk,
  install: true
)
