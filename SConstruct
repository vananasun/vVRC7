import fnmatch
import sys
import os
from subprocess import call



# User variables
VST2_SDK                = 'E:/dev/.____Libraries/VST3 SDK'
OPENGL_AND_GLEW_HEADERS = 'E:/dev/.____Libraries/GL/include'
GLEW_LIBRARY            = 'E:/dev/.____Libraries/GL/lib/x64'
WEBP_INC                = 'E:/dev/.____Libraries/libwebp_x64/include'
WEBP_LIB                = 'E:/dev/.____Libraries/libwebp_x64/lib'



env = Environment(
    CPPPATH=['./src', VST2_SDK, OPENGL_AND_GLEW_HEADERS, WEBP_INC],
    CPPDEFINES=['WIN32'],
    CCFLAGS='/nologo /EHsc ',
    LIBS=['user32', 'gdi32', 'opengl32', 'glew32s', 'libwebp'],
    LIBPATH=[GLEW_LIBRARY, WEBP_LIB]
)
env.Append(LINKFLAGS='/DEF:src/exports.def')
env.Append(LINKFLAGS='/SUBSYSTEM:CONSOLE')
env.Append(LINKFLAGS='build/assets.res ')

# Debug and release target
release = ARGUMENTS.get('release', 0)
if int(release):
    env.Append(CCFLAGS = '/O2')
    env.Append(LINKFLAGS = '/O2')
else:
    env.Append(CCFLAGS = '/Od')
    env.Append(LINKFLAGS = '/DEBUG:FULL')
    env.Append(CPPDEFINES = 'DEBUG')



# Gather list of sources
sources = []
for root, dirnames, filenames in os.walk('src'):
    for filename in filenames:
        if fnmatch.fnmatch(filename, '*.cpp'):
            sources.append(str(os.path.join(root, filename)))

# Build DLL
call('windres src/assets.rc build/assets.res')
VariantDir('build', 'src', duplicate=0)
sources = [s.replace('src', 'build', 1) for s in sources]
env.SharedLibrary('build/vVRC7.dll', source=sources)
