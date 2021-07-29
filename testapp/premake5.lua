-- Copyright (C) Force67 <github.com/Force67>.
-- For licensing information see LICENSE at the root of this distribution.

project("testapp")
    kind("SharedLib")
    files({
        "**.cc",
        "**.hpp",
    })
    includedirs({
        ".",
        "../",
        blu.extdir .. "/llvm/llvm_windows-msvc-release-mt/include",
    })