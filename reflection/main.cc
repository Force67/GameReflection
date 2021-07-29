
#include <cstdio>
#include <cppast/code_generator.hpp>          // for generate_code()
#include <cppast/cpp_entity_kind.hpp>         // for the cpp_entity_kind definition
#include <cppast/cpp_forward_declarable.hpp>  // for is_definition()
#include <cppast/cpp_namespace.hpp>           // for cpp_namespace
#include <cppast/libclang_parser.hpp>         // for libclang_parser, libclang_compile_config, cpp_entity,...
#include <cppast/visitor.hpp>                 // for visit()

#include <iostream>

// print error message
void print_error(const std::string& msg) {
  std::cerr << msg << '\n';
}

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

// prints the AST of a file
void print_ast(std::ostream& out, const cppast::cpp_file& file) {
  // print file name
  out << "AST for '" << file.name() << "':\n";
  std::string prefix;  // the current prefix string
  // recursively visit file and all children
  cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
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
        out << "+-";
      } else {
        if (info.event == cppast::visitor_info::container_entity_enter)
          prefix += "| ";
        out << "|-";
      }

      print_entity(out, e);
    }

    return true;
  });
}

// parse a file
std::unique_ptr<cppast::cpp_file> parse_file(const cppast::libclang_compile_config& config,
                                             const cppast::diagnostic_logger& logger,
                                             const std::string& filename,
                                             bool fatal_error) {
  // the entity index is used to resolve cross references in the AST
  // we don't need that, so it will not be needed afterwards
  cppast::cpp_entity_index idx;
  // the parser is used to parse the entity
  // there can be multiple parser implementations
  cppast::libclang_parser parser(type_safe::ref(logger));
  // parse the file
  auto file = parser.parse(idx, filename, config);
  if (fatal_error && parser.error())
    return nullptr;
  return file;
}

int main(int argc, char** argv) {
  // on windows we must supports msvc extensions.
  cppast::compile_flags flags;
  flags |= cppast::compile_flag::ms_extensions;
  flags |= cppast::compile_flag::ms_compatibility;

  // minimum language standard will be cxx17
  cppast::libclang_compile_config config;
  config.set_flags(cppast::cpp_standard::cpp_17, flags);

  cppast::stderr_diagnostic_logger logger;
  // debug only!
  logger.set_verbose(true);

  auto file = parse_file(config, logger, 
      R"(C:\Users\vince\Documents\Development\Force\GameReflection\testapp\attribute_test.h)", true);
  if (!file)
    __debugbreak();

   print_ast(std::cout, *file);
}