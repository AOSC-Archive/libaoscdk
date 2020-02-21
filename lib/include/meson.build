# Inject build system variables into the source code for later use.
libaoscdk_srcs += configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: {
    'PROJECT_NAME': meson.project_name(),
    'PROJECT_VERSION': meson.project_version(),
  },
)

libaoscdk_lib_incs = include_directories('.')
