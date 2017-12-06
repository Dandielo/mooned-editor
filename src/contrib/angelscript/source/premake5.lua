project "AngelScript"
	kind "StaticLib"

	files { "**.cpp", "**.h" }
	removefiles { "as_callfunc_*" }

	vpaths { 
		["Source/*"] = { "**.cpp", "**.h", "**.asm", "*.S" } 
	}

	filter { "platforms:x64" }
		files { 
			"as_callfunc_x64_msvc.cpp",
			"as_callfunc_x64_msvc_asm.asm" 
		}

	filter { "files:**.asm" }
		buildmessage "Assembling %{file.relpath}"

		buildcommands { 
			"ml64 /Fo %{cfg.objdir}/%{file.basename}.obj /c %{file.relpath}"
		}

		buildoutputs { 
			"%{cfg.objdir}/%{file.basename}.obj"
		}