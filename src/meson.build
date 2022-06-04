wayland_client = dependency('wayland-client')
wayland_egl = dependency('wayland-egl')
egl = dependency('egl')
gl = dependency('gl')
cairo = dependency('cairo')
cairo_gl = dependency('cairo-gl')

sourcefiles = [
  'intagli_types.c',
  'intagli.c',
  'intagli_wayland.c',
  'backends/wayland/xdg-shell.c'
]

lib_intagli = static_library('intagli', sourcefiles, dependencies: [wayland_client, egl, wayland_egl, gl, cairo, cairo_gl], include_directories : project_include_dir)

dep_intagli = declare_dependency(link_with: lib_intagli, include_directories : project_include_dir)
