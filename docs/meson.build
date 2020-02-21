doxygen = find_program('doxygen', required: true)
dot = find_program('dot', required: false)

doxygen_config = configuration_data()
doxygen_config.set('PROJECT_NAME', meson.project_name())
doxygen_config.set('PROJECT_VERSION', meson.project_version())
doxygen_config.set('SRCDIR', meson.current_source_dir() / '..')
doxygen_config.set('BUILDDIR', meson.current_build_dir() / '..')

if (dot.found())
  doxygen_config.set('HAVE_DOT', 'YES')
else
  doxygen_config.set('HAVE_DOT', 'NO')
endif

doxyfile = configure_file(
  input: 'Doxyfile.in',
  output: 'Doxyfile',
  configuration: doxygen_config,
  install: false
)

custom_target(
  'docs',
  input: doxyfile,
  output: 'html',
  command: [doxygen, doxyfile]
)
