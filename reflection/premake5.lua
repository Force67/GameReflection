-- Copyright (C) Force67 <github.com/Force67>.
-- For licensing information see LICENSE at the root of this distribution.

project("symsup")
    kind("ConsoleApp")
    targetname("SymSup")
    staticruntime("On")
    files({
        "**.cc",
        "**.h",
    })
    pchheader("pch.h")
    pchsource("pch.cc")
    forceincludes("pch.h")
    include_llvm()