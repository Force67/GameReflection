// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

namespace cppast {
class cpp_file;
}

namespace refl {
void DumpAst(std::ostream& out, const cppast::cpp_file& file);
void DumpAstSimple(const cppast::cpp_file& file);
}