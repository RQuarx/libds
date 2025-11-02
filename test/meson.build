shared = files('shared/xmalloc.c')


darray = executable(
    'darray',
    files('darray.c') + shared,
    include_directories: inc,
    link_with: libs,
)


list = executable(
    'list',
    files('list.c') + shared,
    include_directories: inc,
    link_with: libs,
)

test('darray', darray)
test('list', list)