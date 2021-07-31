#pragma once

namespace refl {
type_safe::optional<std::vector<std::string>> FindFiles(const std::string& input_path);
}