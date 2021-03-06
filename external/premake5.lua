
function include_llvm()
    links({
        "Version",
        "Remarks",
        "clangAnalysis",
        "clangARCMigrate",
        "clangAST",
        "clangASTMatchers",
        "clangBasic",
        "clangCodeGen",
        "clangCrossTU",
        "clangDependencyScanning",
        "clangDirectoryWatcher",
        "clangDriver",
        "clangDynamicASTMatchers",
        "clangEdit",
        "clangFormat",
        "clangFrontend",
        "clangFrontendTool",
        "clangHandleCXX",
        "clangHandleLLVM",
        "clangIndex",
        "clangLex",
        "clangParse",
        "clangRewrite",
        "clangRewriteFrontend",
        "clangSema",
        "clangSerialization",
        "clangStaticAnalyzerCheckers",
        "clangStaticAnalyzerCore",
        "clangStaticAnalyzerFrontend",
        "clangTesting",
        "clangTooling",
        "clangToolingASTDiff",
        "clangToolingCore",
        "clangToolingInclusions",
        "clangToolingRefactoring",
        "clangToolingSyntax",
        "clangTransformer",
        "libclang",
        "lldCOFF",
        "lldCommon",
        "lldCore",
        "lldDriver",
        "lldELF",
        "lldMachO",
        "lldMachO2",
        "lldMinGW",
        "lldReaderWriter",
        "lldWasm",
        "lldYAML",
        "LLVMAArch64AsmParser",
        "LLVMAArch64CodeGen",
        "LLVMAArch64Desc",
        "LLVMAArch64Disassembler",
        "LLVMAArch64Info",
        "LLVMAArch64Utils",
        "LLVMAggressiveInstCombine",
        "LLVMAMDGPUAsmParser",
        "LLVMAMDGPUCodeGen",
        "LLVMAMDGPUDesc",
        "LLVMAMDGPUDisassembler",
        "LLVMAMDGPUInfo",
        "LLVMAMDGPUUtils",
        "LLVMAnalysis",
        "LLVMARMAsmParser",
        "LLVMARMCodeGen",
        "LLVMARMDesc",
        "LLVMARMDisassembler",
        "LLVMARMInfo",
        "LLVMARMUtils",
        "LLVMAsmParser",
        "LLVMAsmPrinter",
        "LLVMAVRAsmParser",
        "LLVMAVRCodeGen",
        "LLVMAVRDesc",
        "LLVMAVRDisassembler",
        "LLVMAVRInfo",
        "LLVMBinaryFormat",
        "LLVMBitReader",
        "LLVMBitstreamReader",
        "LLVMBitWriter",
        "LLVMBPFAsmParser",
        "LLVMBPFCodeGen",
        "LLVMBPFDesc",
        "LLVMBPFDisassembler",
        "LLVMBPFInfo",
        "LLVM-C",
        "LLVMCFGuard",
        "LLVMCodeGen",
        "LLVMCore",
        "LLVMCoroutines",
        "LLVMCoverage",
        "LLVMDebugInfoCodeView",
        "LLVMDebugInfoDWARF",
        "LLVMDebugInfoGSYM",
        "LLVMDebugInfoMSF",
        "LLVMDebugInfoPDB",
        "LLVMDemangle",
        "LLVMDlltoolDriver",
        "LLVMDWARFLinker",
        "LLVMExecutionEngine",
        "LLVMExtensions",
        "LLVMFrontendOpenACC",
        "LLVMFrontendOpenMP",
        "LLVMFuzzMutate",
        "LLVMGlobalISel",
        "LLVMHexagonAsmParser",
        "LLVMHexagonCodeGen",
        "LLVMHexagonDesc",
        "LLVMHexagonDisassembler",
        "LLVMHexagonInfo",
        "LLVMInstCombine",
        "LLVMInstrumentation",
        "LLVMInterpreter",
        "LLVMipo",
        "LLVMIRReader",
        "LLVMJITLink",
        "LLVMLanaiAsmParser",
        "LLVMLanaiCodeGen",
        "LLVMLanaiDesc",
        "LLVMLanaiDisassembler",
        "LLVMLanaiInfo",
        "LLVMLibDriver",
        "LLVMLineEditor",
        "LLVMLinker",
        "LLVMLTO",
        "LLVMMC",
        "LLVMMCA",
        "LLVMMCDisassembler",
        "LLVMMCJIT",
        "LLVMMCParser",
        "LLVMMipsAsmParser",
        "LLVMMipsCodeGen",
        "LLVMMipsDesc",
        "LLVMMipsDisassembler",
        "LLVMMipsInfo",
        "LLVMMIRParser",
        "LLVMMSP430AsmParser",
        "LLVMMSP430CodeGen",
        "LLVMMSP430Desc",
        "LLVMMSP430Disassembler",
        "LLVMMSP430Info",
        "LLVMNVPTXCodeGen",
        "LLVMNVPTXDesc",
        "LLVMNVPTXInfo",
        "LLVMObjCARCOpts",
        "LLVMObject",
        "LLVMObjectYAML",
        "LLVMOption",
        "LLVMOrcError",
        "LLVMOrcJIT",
        "LLVMPasses",
        "LLVMPowerPCAsmParser",
        "LLVMPowerPCCodeGen",
        "LLVMPowerPCDesc",
        "LLVMPowerPCDisassembler",
        "LLVMPowerPCInfo",
        "LLVMProfileData",
        "LLVMRemarks",
        "LLVMRISCVAsmParser",
        "LLVMRISCVCodeGen",
        "LLVMRISCVDesc",
        "LLVMRISCVDisassembler",
        "LLVMRISCVInfo",
        "LLVMRISCVUtils",
        "LLVMRuntimeDyld",
        "LLVMScalarOpts",
        "LLVMSelectionDAG",
        "LLVMSparcAsmParser",
        "LLVMSparcCodeGen",
        "LLVMSparcDesc",
        "LLVMSparcDisassembler",
        "LLVMSparcInfo",
        "LLVMSupport",
        "LLVMSymbolize",
        "LLVMSystemZAsmParser",
        "LLVMSystemZCodeGen",
        "LLVMSystemZDesc",
        "LLVMSystemZDisassembler",
        "LLVMSystemZInfo",
        "LLVMTableGen",
        "LLVMTarget",
        "LLVMTextAPI",
        "LLVMTransformUtils",
        "LLVMVectorize",
        "LLVMWebAssemblyAsmParser",
        "LLVMWebAssemblyCodeGen",
        "LLVMWebAssemblyDesc",
        "LLVMWebAssemblyDisassembler",
        "LLVMWebAssemblyInfo",
        "LLVMWindowsManifest",
        "LLVMX86AsmParser",
        "LLVMX86CodeGen",
        "LLVMX86Desc",
        "LLVMX86Disassembler",
        "LLVMX86Info",
        "LLVMXCoreCodeGen",
        "LLVMXCoreDesc",
        "LLVMXCoreDisassembler",
        "LLVMXCoreInfo",
        "LLVMXRay",
        "LTO",
    })
    libdirs(blu.extdir .. "/llvm/llvm_windows-msvc-release-mt/lib")
    includedirs({
        blu.extdir .. "/llvm/llvm_windows-msvc-release-mt/include",
    })
end

--project(cppast VERSION 0.0)

project("cppast")
    kind("StaticLib")
    staticruntime("On")
    files({
        "cppast/src/**.cpp",
        "cppast/include/**.h",
        "cppast/external/tpl/process.cpp",
        "cppast/external/tpl/process.hpp",
        "cppast/external/tpl/process_win.cpp",
    })
    includedirs({
        "cppast/include",
        "cppast/external/tpl",
        "type_safe/include",
        "type_safe/external/debug_assert"
    })
    defines({
        "CPPAST_VERSION_MAJOR=\"0\"",
        "CPPAST_VERSION_MINOR=\"0\"",
        "CPPAST_CLANG_BINARY=\"C:\\Program Files\\LLVM\\bin\\clang++.exe\""
    })
    include_llvm()

project("cppast_tool")
    kind("ConsoleApp")
    staticruntime("On")
    files({
        "cppast/tool/main.cpp",
        "cppast/tool/cxxopts.hpp",
    })
    includedirs({
        "cppast/tool/include",
        blu.extdir .. "/cppast/include",
        blu.extdir .. "/type_safe/include",
        blu.extdir .. "/type_safe/external/debug_assert",
    })
    links("cppast")

project("fmtlib")
    kind("StaticLib")
    staticruntime("On")
    files({
        --"fmt/src/fmt.cc",
        "fmt/src/format.cc",
        "fmt/src/os.cc",
        "fmt/include/fmt/*.h",
    })
    includedirs({
        "fmt/include",
    })