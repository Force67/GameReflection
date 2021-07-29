-- Copyright (C) Force67 <github.com/Force67>.
-- For licensing information see LICENSE at the root of this distribution.

include("./build")

-- this is a workaround on windows to ensure cppast chooses
-- the right constructor for tinyprocess.
characterset("MBCS")

filter ("configurations:Debug")
    defines("FMX_DEBUG")

filter("configurations:Release")
    runtime("Release")
    optimize("Speed")

filter("configurations:Shipping")
    runtime("Release")
    optimize("Speed")
    flags({
        "LinkTimeOptimization"
    })

filter("language:C or C++")
    vectorextensions("SSE4.1")

filter("language:C++")
    cppdialect("C++17")

filter("system:windows")
    defines("OS_WIN")
    
workspace("GameReflection")
    configurations({
        "Debug",
        "Release",
        "Shipping"
    })
    flags {
        "MultiProcessorCompile"
    }
    architecture("x86_64")
    defines("NOMINMAX")

    group("Libraries")
    include("./external")

    group("App")
    include("./testapp")
    include("./reflection")