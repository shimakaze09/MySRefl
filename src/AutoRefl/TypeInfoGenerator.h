//
// Created by Admin on 26/03/2025.
//

#pragma once

#include <MySRefl/config.h>
#include "Meta.h"

namespace My::MySRefl {
class TypeInfoGenerator {
 public:
  enum class ConstMode { Constepxr, Const, NonConst };

  struct Config {
    bool nonNamespaceNameWithoutQuotation{false};
    bool namespaceNameWithQuotation{false};
    bool isAttrValueToFunction{false};

    std::string_view nameof_namespace = MyMeta::nameof_namespace;

    bool isInitializerAsAttr{false};

    std::string_view nameof_initializer = MyMeta::nameof_initializer;

    std::string_view nameof_constructor = MyMeta::nameof_constructor;
    std::string_view nameof_destructor = MyMeta::nameof_destructor;

    bool generateDefaultFunctions{true};
    std::string_view nameof_default_functions =
        MyMeta::nameof_default_functions;
    ConstMode attrListConstMode{ConstMode::Constepxr};
    ConstMode fieldListConstMode{ConstMode::Constepxr};
  };

  TypeInfoGenerator() = default;

  TypeInfoGenerator(Config config) : config{std::move(config)} {}

  std::string Generate(const std::vector<TypeMeta>& typeMetas);

 private:
  Config config;
};
}  // namespace My::MySRefl
