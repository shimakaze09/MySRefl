//
// Created by Admin on 12/03/2025.
//

#include "AutoRefl.h"

#include <MyANTLR/ParserCpp14/CPP14Lexer.h>
#include <MyANTLR/ParserCpp14/CPP14Parser.h>

#include <iostream>
#include <sstream>

using namespace My::MySRefl;
using namespace std;
using namespace antlr4;

string AutoRefl::Parse(string_view code) {
  // [ 1. clear ]
  typeInfos.clear();
  curNamespace.clear();
  curMetas = nullptr;
  curTypeInfo = nullptr;
  curAccessSpecifier = AccessSpecifier::PRIVATE;

  // [2. parse]
  ANTLRInputStream input(code.data());
  CPP14Lexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  CPP14Parser parser(&tokens);
  tree::ParseTree* tree = parser.translationunit();

  tree->accept(this);

  // [3. generate]
  stringstream ss;
  const string indent = "    ";

  ss << "// This file is generated by My::MySRefl::AutoRefl" << endl
     << endl
     << "#pragma once" << endl
     << endl
     << "#include <MySRefl/MySRefl.h>" << endl
     << endl;

  for (const auto& typeinfo : typeInfos) {
    string ns;
    for (auto a_ns : typeinfo.ns) ns += a_ns + "::";
    string type = ns + typeinfo.name;

    ss << "template<>" << endl
       << "struct My::MySRefl::TypeInfo<" << type << ">" << endl
       << indent << ": My::MySRefl::TypeInfoBase<" << type << ">" << endl
       << "{" << endl;

    // type attrs
    ss << indent << "static constexpr AttrList attrs = {";
    if (!typeinfo.metas.empty()) {
      ss << endl;
      for (const auto& [key, value] : typeinfo.metas) {
        ss << indent << indent << "Attr{" << "\"" << key << "\""
           << (value.empty() ? "" : (", " + value)) << "}," << endl;
      }
      ss << indent << "};" << endl;  // end AttrList
    } else
      ss << "};" << endl;  // end AttrList
    ss << endl;

    // field
    ss << indent << "static constexpr FieldList fields = {" << endl;
    // var
    for (const auto& varInfo : typeinfo.varInfos) {
      if (varInfo.access != AccessSpecifier::PUBLIC) continue;

      ss << indent << indent << "Field {"
         << "\"" << varInfo.name << "\", "
         << "&" << type << "::" << varInfo.name;

      if (!varInfo.metas.empty()) {
        ss << "," << endl << indent << indent << indent << "AttrList {" << endl;
        for (const auto& [key, value] : varInfo.metas) {
          ss << indent << indent << indent << indent << "Attr {" << "\"" << key
             << "\"" << (value.empty() ? "" : (", " + value)) << " }," << endl;
        }
        ss << indent << indent << indent << "}" << endl;  // end AttrList
      }

      ss << indent << indent << "}," << endl;
    }

    // func
    map<string, bool> overloadMap;
    for (const auto& funcInfo : typeinfo.funcInfos) {
      auto target = overloadMap.find(funcInfo.name);
      if (target != overloadMap.end())
        target->second = true;
      else
        overloadMap.emplace_hint(target, funcInfo.name, false);
    }

    for (const auto& funcInfo : typeinfo.funcInfos) {
      if (funcInfo.access != AccessSpecifier::PUBLIC) continue;

      // name
      string name;
      bool isSpecial = false;
      if (funcInfo.ret.empty()) {
        isSpecial = true;
        // constructor / destructor
        if (funcInfo.name.find('~') != string::npos)
          name = "__destructor";
        else
          name = "__constructor";
      } else
        name = funcInfo.name;

      ss << indent << indent << "Fields{"
         << "\"" << name << "\", ";

      // value
      if (isSpecial) {
        if (name == "__constructor") {
          ss << "WrapConstructor<" << type << "(";
          for (size_t i = 0; i < funcInfo.params.size(); i++) {
            string type;
            for (size_t j = 0; j < funcInfo.params[i].specifiers.size(); j++) {
              type += funcInfo.params[i].specifiers[j];
              if (j < funcInfo.params[i].specifiers.size() - 1) type += " ";
            }
            ss << type;
            if (i < funcInfo.params.size() - 1) ss << ", ";
          }
          ss << ")>()";
        } else
          ss << "WrapDestructor<" << type << ">()";
      } else {
        if (overloadMap.find(funcInfo.name)->second) {
          ss << "static_cast<" << funcInfo.ret;
          if (!funcInfo.isStatic) ss << "(" << type << "::*)";
          ss << "(";  // arguments begin
          for (size_t i = 0; i < funcInfo.params.size(); i++) {
            string type;
            for (size_t j = 0; j < funcInfo.params[i].specifiers.size(); j++) {
              type += funcInfo.params[i].specifiers[j];
              if (j < funcInfo.params[i].specifiers.size() - 1) type += " ";
            }
            ss << type;
            if (i < funcInfo.params.size() - 1) ss << ", ";
          }
          ss << ")";  // arguments end
          for (size_t k = 0; k < funcInfo.qualifiers.size(); k++) {
            ss << funcInfo.qualifiers[k];
            if (k < funcInfo.qualifiers.size() - 1) ss << " ";
          }
          ss << ">(&" << type << "::" << funcInfo.name << ")";
        } else
          ss << "&" << type << "::" << funcInfo.name;
      }

      if (!funcInfo.metas.empty() || !funcInfo.params.empty()) {
        ss << "," << endl << indent << indent << indent << "AttrList {" << endl;
        for (const auto& [key, value] : funcInfo.metas) {
          ss << indent << indent << indent << indent << "Attr{" << "\"" << key
             << "\"" << (value.empty() ? "" : (", " + value)) << "}," << endl;
        }
        for (size_t i = 0; i < funcInfo.params.size(); i++) {
          ss << indent << indent << indent << indent << "Attr{" << "\"__arg_"
             << i << "\"";
          if (!funcInfo.params[i].name.empty() ||
              !funcInfo.params[i].defaultValue.empty() ||
              !funcInfo.params[i].metas.empty()) {
            ss << "," << endl
               << indent << indent << indent << indent << indent << "AttrList{"
               << endl
               << indent << indent << indent << indent << indent << indent;
            if (!funcInfo.params[i].name.empty()) {
              ss << "Attr{\"__name\", \"" << funcInfo.params[i].name << "\"},"
                 << endl;
            }
            if (!funcInfo.params[i].defaultValue.empty()) {
              ss << indent << indent << indent << indent << indent << indent
                 << "Attr{\"__default_value\", "
                 << funcInfo.params[i].defaultValue << "}," << endl;
            }
            for (const auto& [key, value] : funcInfo.params[i].metas) {
              ss << indent << indent << indent << indent << indent << indent
                 << "Attr{" << "\"" << key << "\""
                 << (value.empty() ? "" : (", " + value)) << "}," << endl;
            }
            // end argument AttrList
            ss << indent << indent << indent << indent << indent << "}" << endl;
            ss << indent << indent << indent << indent << "},"
               << endl;  // end argument Attr
          } else
            ss << "}," << endl;  // end argument Attr
        }
        ss << indent << indent << indent << "}" << endl;  // function attr list
        ss << indent << indent << "}," << endl;           // end Field
      } else
        ss << "}," << endl;  // end Field
    }
    ss << indent << "};" << endl;  // end FieldList

    ss << "};" << endl;  // end TypeInfo
  }

  return ss.str();
}

antlrcpp::Any AutoRefl::visitOriginalnamespacedefinition(
    CPP14Parser::OriginalnamespacedefinitionContext* ctx) {
  curNamespace.push_back(ctx->Identifier()->getText());
  auto result = visitChildren(ctx);
  curNamespace.pop_back();
  return result;
}

antlrcpp::Any AutoRefl::visitClassspecifier(
    CPP14Parser::ClassspecifierContext* ctx) {
  typeInfos.emplace_back();
  curTypeInfo = &typeInfos.back();
  curTypeInfo->ns = curNamespace;
  auto result = visitChildren(ctx);
  curTypeInfo = nullptr;
  return result;
}

antlrcpp::Any AutoRefl::visitClasshead(CPP14Parser::ClassheadContext* ctx) {
  curMetas = &curTypeInfo->metas;
  auto result = visitChildren(ctx);
  curMetas = nullptr;
  return result;
}

antlrcpp::Any AutoRefl::visitClasskey(CPP14Parser::ClasskeyContext* ctx) {
  curTypeInfo->classkey = ctx->getText();
  if (curTypeInfo->classkey == "struct")
    curAccessSpecifier = AccessSpecifier::PUBLIC;
  else
    curAccessSpecifier = AccessSpecifier::PRIVATE;

  return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitClassname(CPP14Parser::ClassnameContext* ctx) {
  curTypeInfo->name = ctx->getText();
  return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitAttribute(CPP14Parser::AttributeContext* ctx) {
  if (curMetas) {
    auto argCtx = ctx->attributeargumentclause();
    string arg = argCtx ? argCtx->balancedtokenseq()->getText() : "";
    curMetas->emplace(ctx->attributetoken()->getText(), move(arg));
  }
  return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitAccessspecifier(
    CPP14Parser::AccessspecifierContext* ctx) {
  auto accessStr = ctx->getText();
  if (accessStr == "public")
    curAccessSpecifier = AccessSpecifier::PUBLIC;
  else if (accessStr == "protected")
    curAccessSpecifier = AccessSpecifier::PROTECTED;
  else
    curAccessSpecifier = AccessSpecifier::PRIVATE;
  return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitMemberdeclaration(
    CPP14Parser::MemberdeclarationContext* ctx) {
  if (ctx->usingdeclaration() || ctx->static_assertdeclaration() ||
      ctx->templatedeclaration() || ctx->aliasdeclaration() ||
      ctx->emptydeclaration())
    return {};

  curFieldInfo.access = curAccessSpecifier;
  curMetas = &curFieldInfo.metas;
  inMember = true;

  auto result = visitChildren(ctx);

  if (curFieldInfo.isFunc) {
    FuncInfo info;
    info.metas = move(curFieldInfo.metas);
    info.name = move(curFieldInfo.name);
    info.access = curFieldInfo.access;
    for (const auto& t : curFieldInfo.type_specifiers) info.ret += t;
    info.params = move(curFieldInfo.params);
    info.qualifiers = move(curFieldInfo.qualifiers);
    info.isStatic = curFieldInfo.isStatic;
    curTypeInfo->funcInfos.emplace_back(move(info));
  } else {
    VarInfo info;
    info.metas = move(curFieldInfo.metas);
    info.name = move(curFieldInfo.name);
    info.access = curFieldInfo.access;
    info.isStatic = curFieldInfo.isStatic;
    curTypeInfo->varInfos.emplace_back(move(info));
  }

  curFieldInfo.metas.clear();
  curFieldInfo.name.clear();
  curFieldInfo.type_specifiers.clear();
  curFieldInfo.nontype_specifiers.clear();
  curFieldInfo.isFunc = false;
  curFieldInfo.isStatic = false;
  inMember = false;
  curMetas = nullptr;

  return result;
}

antlrcpp::Any AutoRefl::visitNoptrdeclarator(
    CPP14Parser::NoptrdeclaratorContext* ctx) {
  if (!inMember) return {};

  if (!curParam)
    curFieldInfo.isFunc =
        curFieldInfo.isFunc || ctx->parametersandqualifiers() != nullptr;

  return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitParametersandqualifiers(
    CPP14Parser::ParametersandqualifiersContext* ctx) {
  for (auto cur = ctx->cvqualifierseq(); cur != nullptr;
       cur = cur->cvqualifierseq()) {
    if (cur->cvqualifier()) curFieldInfo.qualifiers.push_back(cur->getText());
  }
  if (ctx->refqualifier())
    curFieldInfo.qualifiers.push_back(ctx->refqualifier()->getText());
  if (ctx->exceptionspecification())
    curFieldInfo.qualifiers.push_back(ctx->exceptionspecification()->getText());
  auto rst = visitChildren(ctx);
  curMetas = &curFieldInfo.metas;
  curParam = nullptr;
  return rst;
}

antlrcpp::Any AutoRefl::visitParameterdeclaration(
    CPP14Parser::ParameterdeclarationContext* ctx) {
  curFieldInfo.params.emplace_back();
  curParam = &curFieldInfo.params.back();
  curMetas = &curParam->metas;
  auto rst = visitChildren(ctx);
  return rst;
}

antlrcpp::Any AutoRefl::visitInitializerclause(
    CPP14Parser::InitializerclauseContext* ctx) {
  if (curParam) curParam->defaultValue = ctx->getText();
  return {};
}

antlrcpp::Any AutoRefl::visitDeclspecifier(
    CPP14Parser::DeclspecifierContext* ctx) {
  if (!inMember) return visitChildren(ctx);

  if (!curParam) {
    if (ctx->typespecifier())
      curFieldInfo.type_specifiers.push_back(ctx->getText());
    else
      curFieldInfo.nontype_specifiers.push_back(ctx->getText());
    if (ctx->getText() == "static") curFieldInfo.isStatic = true;
  } else
    curParam->specifiers.push_back(ctx->getText());

  return {};
}

antlrcpp::Any AutoRefl::visitPtroperator(CPP14Parser::PtroperatorContext* ctx) {
  if (!inMember) return {};

  if (!curParam)
    curFieldInfo.type_specifiers.push_back(ctx->getText());
  else
    curParam->specifiers.push_back(ctx->getText());

  return {};
}

antlrcpp::Any AutoRefl::visitUnqualifiedid(
    CPP14Parser::UnqualifiedidContext* ctx) {
  if (!inMember) return {};
  if (curParam)
    curParam->name = ctx->getText();
  else
    curFieldInfo.name = ctx->getText();

  return {};
  // return visitChildren(ctx);
}

antlrcpp::Any AutoRefl::visitFunctionbody(
    CPP14Parser::FunctionbodyContext* ctx) {
  return {};  // jump
              // return visitChildren(ctx);
}
