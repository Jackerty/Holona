#!/usr/bin/env python
import os
import sys

env = SConscript("Include/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Tweak this if you want to use different folders or more folders to store your source code in.
env.Append(CPPPATH=["Src/"])
env.Append(CPPFLAGS=["-IInclude/include/godot_cpp","-IInclude/include","-IInclude/gen/include/godot_cpp","-IInclude/gen/include","-IInclude/gdextension"])
# Tell we are build GD_EXTENSION version.
env.Append(CPPDEFINES=["-DGD_EXTENSION","-DTOOLS_ENABLED"])
sources = Glob("Src/*.cpp")

if env["platform"] == "macos":
	library = env.SharedLibrary(
		"Bin/Holona.framework",
		source=sources,
	)
elif env["platform"] == "windows":
	library = env.SharedLibrary(
		"Bin/Holona.dll",
		source=sources,
	)
else:
	# I suppose adding lib is operating system standard... Although I think GCC ignores this...
	env['SHLIBPREFIX'] = ''
	library = env.SharedLibrary(
		"Bin/Holona.so",
		source=sources,
	)

Default(library)
