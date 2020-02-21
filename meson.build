project('libaoscdk', ['c'], version: '0.1', license: 'MIT')

subdir('include')
subdir('lib')

if get_option('build_utils')
  subdir('utils')
endif

if get_option('build_tests')
  subdir('tests')
endif

if get_option('build_docs')
  subdir('docs')
endif
