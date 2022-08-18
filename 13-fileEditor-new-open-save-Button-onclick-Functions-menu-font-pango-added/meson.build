project('tfe', 'c')

gtkdep = dependency('gtk4')

gnome=import('gnome')
resources = gnome.compile_resources('resources','tfe.gresource.xml')
gnome.compile_schemas(build_by_default: true, depend_files: 'org.gtk.example.gschema.xml')

sourcefiles=files('13-tfeapplication.c', '13-tfenotebook.c', '13-css.c','13-tfetextview.c')

executable('tfe', sourcefiles, resources, dependencies: gtkdep, export_dynamic: true, install: true)

schema_dir = get_option('prefix') / get_option('datadir') / 'glib-2.0/schemas/'
install_data('org.gtk.example.gschema.xml', install_dir: schema_dir)
meson.add_install_script('glib-compile-schemas', schema_dir)

