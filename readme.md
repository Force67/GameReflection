## GameRefl
GameRefl adds the ability to supplement c++ code with custom attributes and export them to a c++/json file. Its main use it to add static reflection to hooking code in a tiltedphoques project.
## Setup
gamerefl needs to discover files in order to gather a deeper understanding of the codebase, to do this, you need to hook it up in your build system:

- For Premake:<br/>
  Install [premake-export-compile-commands](https://github.com/tarruda/premake-export-compile-commands) and hook it up to run within your prebuild flow. Then execute gamerefl with ```gamerefl <path_to_commands> -file-provider=CompileCommands```.
- For XMake:<br/>
  Setup a flow to generate compile_commands by running ```xmake project -k compile_commands```. Then point gamerefl to compile commands by using: ```gamerefl <path_to_commands> -file-provider=CompileCommands```.

