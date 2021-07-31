-- Copyright (C) Force67 <github.com/Force67>.
-- For licensing information see LICENSE at the root of this distribution.

project("reflection")
    kind("ConsoleApp")
    targetname("gamerefl")
    staticruntime("On")
    includedirs({
        blu.extdir .. "/fmt/include",
        blu.extdir .. "/cppast/include",
        blu.extdir .. "/type_safe/include",
        blu.extdir .. "/type_safe/external/debug_assert",
    })
    files({
        "**.cc",
        "**.h",
    })
    pchheader("pch.h")
    pchsource("pch.cc")
    forceincludes("pch.h")
    include_llvm()
    links({
        "cppast",
        "fmtlib"
    })