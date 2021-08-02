// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include <iostream>
#include "ast_debug.h"

namespace refl {

namespace {
// prints the AST entry of a cpp_entity (base class for all entities),
// will only print a single line
void print_entity(std::ostream& out, const cppast::cpp_entity& e) {
  // print name and the kind of the entity
  if (!e.name().empty())
    out << e.name();
  else
    out << "<anonymous>";
  out << " (" << cppast::to_string(e.kind()) << ")";

  // print whether or not it is a definition
  if (cppast::is_definition(e))
    out << " [definition]";

  // print number of attributes
  if (!e.attributes().empty())
    out << " [" << e.attributes().size() << " attribute(s)]";

  if (e.kind() == cppast::cpp_entity_kind::language_linkage_t)
    // no need to print additional information for language linkages
    out << '\n';
  else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
    // cast to cpp_namespace
    auto& ns = static_cast<const cppast::cpp_namespace&>(e);
    // print whether or not it is inline
    if (ns.is_inline())
      out << " [inline]";
    out << '\n';
  } else {
    // print the declaration of the entity
    // it will only use a single line
    // derive from code_generator and implement various callbacks for printing
    // it will print into a std::string
    class code_generator : public cppast::code_generator {
      std::string str_;           // the result
      bool was_newline_ = false;  // whether or not the last token was a newline
                                  // needed for lazily printing them

     public:
      code_generator(const cppast::cpp_entity& e) {
        // kickoff code generation here
        cppast::generate_code(*this, e);
      }

      // return the result
      const std::string& str() const noexcept { return str_; }

     private:
      // called to retrieve the generation options of an entity
      generation_options do_get_options(const cppast::cpp_entity&, cppast::cpp_access_specifier_kind) override {
        // generate declaration only
        return code_generator::declaration;
      }

      // no need to handle indentation, as only a single line is used
      void do_indent() override {}
      void do_unindent() override {}

      // called when a generic token sequence should be generated
      // there are specialized callbacks for various token kinds,
      // to e.g. implement syntax highlighting
      void do_write_token_seq(cppast::string_view tokens) override {
        if (was_newline_) {
          // lazily append newline as space
          str_ += ' ';
          was_newline_ = false;
        }
        // append tokens
        str_ += tokens.c_str();
      }

      // called when a newline should be generated
      // we're lazy as it will always generate a trailing newline,
      // we don't want
      void do_write_newline() override { was_newline_ = true; }

    } generator(e);
    // print generated code
    out << ": `" << generator.str() << '`' << '\n';
  }
}
}  // namespace

void DumpAst(std::ostream& out, const cppast::cpp_file& file) {
  // print file name
  std::printf("AST for %s\n", file.name().c_str());

  std::string prefix;  // the current prefix string
  // recursively visit file and all children
  cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
    const auto ee = e.kind();
#if 1
    if (e.kind() == cppast::cpp_entity_kind::file_t || cppast::is_templated(e) || cppast::is_friended(e))
      // no need to do anything for a file,
      // templated and friended entities are just proxies, so skip those as well
      // return true to continue visit for children
      return true;
    else if (info.event == cppast::visitor_info::container_entity_exit) {
      // we have visited all children of a container,
      // remove prefix
      prefix.pop_back();
      prefix.pop_back();
    } else {
      out << prefix;  // print prefix for previous entities
      // calculate next prefix
      if (info.last_child) {
        if (info.event == cppast::visitor_info::container_entity_enter)
          prefix += "  ";
        std::printf("+-");
      } else {
        if (info.event == cppast::visitor_info::container_entity_enter)
          prefix += "| ";
        std::printf("|-");
      }

      print_entity(out, e);
    }
#endif

    return true;
  });
}

void DumpAstSimple(const cppast::cpp_file& file) {
  std::string prefix;
  // visit each entity in the file
  cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
    if (info.event == cppast::visitor_info::container_entity_exit)  // exiting an old container
      prefix.pop_back();
    else if (info.event == cppast::visitor_info::container_entity_enter)
    // entering a new container
    {
      std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
      prefix += "\t";
    } else  // if (info.event == cppast::visitor_info::leaf_entity) // a non-container entity
      std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
  });
}
}  // namespace refl