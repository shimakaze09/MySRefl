//
// Created by Admin on 26/03/2025.
//

#pragma once

#include "Meta.h"

#include <memory>

namespace My::MySRefl {
class MetaGenerator {
 public:
  MetaGenerator();
  ~MetaGenerator();
  MetaGenerator(MetaGenerator&&) noexcept;
  MetaGenerator& operator=(MetaGenerator&&) noexcept;

  std::vector<TypeMeta> Parse(std::string_view cppCode);

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
}  // namespace My::MySRefl
