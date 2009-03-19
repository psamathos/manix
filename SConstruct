env = Environment(CPPPATH=['common/src'])
env.ParseConfig('pkg-config --cflags --libs xcb xcb-render xcb-renderutil freetype2')

common_files = Glob('common/src/manix/x/*.cpp')
wm_files = Glob('wm/src/*.cpp')

env['CPPPATH'] += ['wm/src']
wm = env.Program('manix-wm', [common_files, wm_files])
