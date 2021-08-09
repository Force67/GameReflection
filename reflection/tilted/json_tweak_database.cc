// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include <llvm/Support/FileSystem.h>

#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "tilted/json_tweak_database.h".h"

namespace refl {
using namespace rapidjson;
using namespace llvm::sys;

JsonTweakDatabase::JsonTweakDatabase(const std::string& file_path)
    : file_path_(std::move(file_path)) {
}

JsonTweakDatabase::~JsonTweakDatabase() {
}

std::unique_ptr<JsonTweakDatabase> JsonTweakDatabase::LoadFromFile(const std::string& file_path) {
  // load into llvm slice.
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer =
      llvm::MemoryBuffer::getFile(file_path);

  // file most likely doest exist.
  if (std::error_code Result = buffer.getError()) {
    // TODO: dump the error nicely?
    return nullptr;
  }

  auto database = std::make_unique<JsonTweakDatabase>(file_path);
  if (!database->Parse(
          // TODO: validate if that works or if we have to:
          /*
            std::string str = back_buffer_->getBuffer().str();
  json_document_->Parse(str.c_str());*/
          (*buffer)->getBuffer().begin()))
    return nullptr;

  return database;
}

bool JsonTweakDatabase::StoreToFile() {
  if (auto doc = Compose()) {
    rapidjson::StringBuffer string_buffer;
    string_buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
    doc->Accept(writer);

    // TODO: validate that we are not forgetting nullterm?

    // upload it to write buffer and dump it out.
    ErrorOr<std::unique_ptr<WriteThroughMemoryBuffer>> buffer =
        WriteThroughMemoryBuffer::getFile(file_path_, string_buffer.GetSize());

    if (std::error_code Result = buffer.getError()) {
      // TODO: dump the error nicely?
      return false;
    }

    return std::memmove((*buffer)->getBufferStart(), string_buffer.GetString(), string_buffer.GetSize()) != nullptr;
  }

  return false;
}

// in fairness, this should return a managed buffer ptr..
// instead of a doc
std::unique_ptr<rapidjson::Document> JsonTweakDatabase::Compose() {
  auto doc = std::make_unique<Document>();
  auto& allocator = doc->GetAllocator();

  Value root(kObjectType);

  Value version(kNumberType);
  version.Set(1.0f);

  Value exclusion_list(kArrayType);
  for (const std::string& file : file_exclusions_) {
    Value record(kStringType);
    record.SetString(file.c_str(), allocator);

    exclusion_list.PushBack(record.Move(), allocator);
  }

  Value override_list(kArrayType);
  for (AttributeRecord& r : attrib_records_) {
    Value record(kObjectType);
    record.AddMember("name", rapidjson::StringRef(r.name.c_str()), allocator);
    record.AddMember("signature", rapidjson::StringRef(r.signature.c_str()), allocator);
    record.AddMember("code-pattern", rapidjson::StringRef(r.pattern.c_str()), allocator);

    override_list.PushBack(record.Move(), allocator);
  }

  root.AddMember("version", version, allocator);
  root.AddMember("file-exclusion-list", exclusion_list, allocator);
  root.AddMember("override-list", override_list, allocator);


  return doc;
}

bool JsonTweakDatabase::Parse(const char* data) {
  // gulp all of the file in one go
  rapidjson::Document doc;
  doc.Parse(data);

  if (doc.HasParseError()) {
    // doc.GetParseError();
    return false;
  }

  auto& exclusion_list = doc["file-exclusion-list"].GetArray();
  file_exclusions_.resize(exclusion_list.Size());
  for (rapidjson::Value& v : exclusion_list) {
    file_exclusions_.emplace_back(v.GetString());
  }

  // TODO: store type info.
  auto& override_list = doc["override-list"].GetArray();
  attrib_records_.resize(override_list.Size());
  for (rapidjson::Value& v : override_list) {
    auto& entry = attrib_records_.emplace_back();
    entry.name = v["name"].GetString();
    entry.signature = v["signature"].GetString();
    entry.pattern = v["code-pattern"].GetString();
  }

  return true;
}
}  // namespace refl