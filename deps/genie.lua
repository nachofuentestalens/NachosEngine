solution "ESAT"
	configurations{"debug","release"}
	includedirs("../include")
	location("../build")
	targetdir("../build/bin")
	debugdir("../build/bin")
	platforms{"x32"}
	libdirs{"./SDL2-2.0.3/lib/x86"}
	links{"SDL2"}
	links{"SDL2main"}
	libdirs{"./glew/lib"}
	libdirs{"./assimp/lib/assimp_release-dll_win32"}
	links{"Assimp32"}
	links{"glew32"}
	links{"glew32s"}
	links{"glew32d"}
	links{"glew32sd"}
	links{"opengl32"}

		
	project "NachosEngine"
		kind "ConsoleApp"
		language "C++"
		files { "../src/**.h", "../src/**.cpp", "../src/**.cc", "../src/**.c", "../include/NachosEngine/**.h"}
		
		configuration "Debug"
			flags { "symbols" }
			
		configuration "Release" 
			flags { "Optimize" }
			
	startproject("NachosEngine")