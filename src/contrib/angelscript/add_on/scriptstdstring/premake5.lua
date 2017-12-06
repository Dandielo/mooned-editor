project "AsScriptStdString"
	kind "StaticLib"
	
	includedirs { "../../include" }

	files { "**.cpp", "**.h" }
	
	vpaths { 
		["Source/*"] = { "**.cpp", "**.h" } 
	}
