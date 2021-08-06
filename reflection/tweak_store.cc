// Copyright (C) Force67 2021.
// For licensing information see LICENSE at the root of this distribution.

#include "tweak_store.h"
#include <llvm/Support/FileSystem.h>

#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace refl {
using namespace rapidjson;
using namespace llvm::sys;

TweaksDatabase* tweaks_db_instance = nullptr;

TweaksDatabase* get_persistant_state() {
  return tweaks_db_instance;
}

TweaksDatabase::TweaksDatabase(const std::string& file_path)
    : file_path_(std::move(file_path)) {
  tweaks_db_instance = this;
}

TweaksDatabase::~TweaksDatabase() {
  tweaks_db_instance = nullptr;
}

std::unique_ptr<TweaksDatabase> TweaksDatabase::LoadFromFile(const std::string& file_path) {
  // load into llvm slice.
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer =
      llvm::MemoryBuffer::getFile(file_path);

  // file most likely doest exist.
  if (std::error_code Result = buffer.getError()) {
    // TODO: dump the error nicely?
    return nullptr;
  }

  auto database = std::make_unique<TweaksDatabase>(file_path);
  if (!database->Parse(
          // TODO: validate if that works or if we have to:
          /*
            std::string str = back_buffer_->getBuffer().str();
  json_document_->Parse(str.c_str());*/
          (*buffer)->getBuffer().begin()))
    return nullptr;

  return database;
}

bool TweaksDatabase::StoreToFile() {
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
std::unique_ptr<rapidjson::Document> TweaksDatabase::Compose() {
  auto doc = std::make_unique<Document>();
  auto& allocator = doc->GetAllocator();

  Value root(kObjectType);

  Value version(kNumberType);
  version.Set(1.0f);

  Value exclusion_list(kArrayType);
  for (std::string& file : file_exclusions_) {
    exclusion_list.PushBack(file, allocator);
  }

  Value override_list(kArrayType);
  for (FuncRecord& r : func_records_) {
    Value record(kObjectType);
    record.AddMember("name", r.name, allocator);
    record.AddMember("signature", r.signature, allocator);
    record.AddMember("code-pattern", r.pattern, allocator);

    override_list.PushBack(record, allocator);
  }

  root.AddMember("version", version, allocator);
  root.AddMember("file-exclusion-list", exclusion_list, allocator);
  root.AddMember("override-list", override_list, allocator);

  return doc;
}

bool TweaksDatabase::Parse(const char* data) {
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
  func_records_.resize(override_list.Size());
  for (rapidjson::Value& v : override_list) {
    auto& entry = func_records_.emplace_back();
    entry.name = v["name"].GetString();
    entry.signature = v["signature"].GetString();
    entry.pattern = v["code-pattern"].GetString();
  }

  return true;
}
}  // namespace refl