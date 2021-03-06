//===--- ASTPrinter.h - Class for printing the AST --------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_AST_ASTPRINTER_H
#define SWIFT_AST_ASTPRINTER_H

#include "swift/Basic/LLVM.h"
#include "swift/Basic/UUID.h"
#include "swift/AST/Identifier.h"
#include "llvm/ADT/StringRef.h"

namespace swift {
  class Decl;
  class DeclContext;
  class ModuleEntity;
  class TypeDecl;
  class Type;
  struct TypeLoc;
  class Pattern;
  class ExtensionDecl;
  class NominalTypeDecl;
  struct PrintOptions;

/// Describes the context in which a name is being printed, which
/// affects the keywords that need to be escaped.
enum class PrintNameContext {
  /// Normal context
  Normal,
  /// Generic parameter context, where 'Self' is not escaped.
  GenericParameter,
  /// Function parameter context, where keywords other than let/var/inout are
  /// not escaped.
  FunctionParameterExternal,
  FunctionParameterLocal,
};

/// An abstract class used to print an AST.
class ASTPrinter {
  unsigned CurrentIndentation = 0;
  unsigned PendingNewlines = 0;
  const Decl *PendingDeclPreCallback = nullptr;
  const Decl *PendingDeclLocCallback = nullptr;
  Optional<PrintNameContext> PendingNamePreCallback;
  const NominalTypeDecl *SynthesizeTarget = nullptr;

  void printTextImpl(StringRef Text);

public:
  virtual ~ASTPrinter() {}

  virtual void printText(StringRef Text) = 0;

  /// Called after the printer decides not to print D.
  virtual void avoidPrintDeclPost(const Decl *D) {};
  /// Called before printing of a declaration.
  virtual void printDeclPre(const Decl *D) {}
  /// Called before printing at the point which would be considered the location
  /// of the declaration (normally the name of the declaration).
  virtual void printDeclLoc(const Decl *D) {}
  /// Called after printing the name of the declaration.
  virtual void printDeclNameEndLoc(const Decl *D) {}
  /// Called after printing the name of a declaration, or in the case of
  /// functions its signature.
  virtual void printDeclNameOrSignatureEndLoc(const Decl *D) {}
  /// Called after finishing printing of a declaration.
  virtual void printDeclPost(const Decl *D) {}

  /// Called before printing a type.
  virtual void printTypePre(const TypeLoc &TL) {}
  /// Called after printing a type.
  virtual void printTypePost(const TypeLoc &TL) {}

  /// Called when printing the referenced name of a type declaration, possibly
  /// from deep inside another type.
  virtual void printTypeRef(const TypeDecl *TD, Identifier Name);

  /// Called when printing the referenced name of a module.
  virtual void printModuleRef(ModuleEntity Mod, Identifier Name);

  /// Called before printing a synthesized extension.
  virtual void printSynthesizedExtensionPre(const ExtensionDecl *ED,
                                            const NominalTypeDecl *NTD) {}

  /// Called after printing a synthesized extension.
  virtual void printSynthesizedExtensionPost(const ExtensionDecl *ED,
                                             const NominalTypeDecl *NTD) {}

  /// Called before printing a name in the given context.
  virtual void printNamePre(PrintNameContext Context) {}
  /// Called after printing a name in the given context.
  virtual void printNamePost(PrintNameContext Context) {}

  // Helper functions.

  void printSeparator(bool &first, StringRef separator) {
    if (first) {
      first = false;
    } else {
      printTextImpl(separator);
    }
  }

  ASTPrinter &operator<<(StringRef Text) {
    printTextImpl(Text);
    return *this;
  }

  ASTPrinter &operator<<(unsigned long long N);
  ASTPrinter &operator<<(UUID UU);

  ASTPrinter &operator<<(DeclName name);

  void printName(Identifier Name,
                 PrintNameContext Context = PrintNameContext::Normal);

  void setIndent(unsigned NumSpaces) {
    CurrentIndentation = NumSpaces;
  }

  void setSynthesizedTarget(NominalTypeDecl *Target) {
    SynthesizeTarget = Target;
  }

  void printNewline() {
    PendingNewlines++;
  }

  virtual void printIndent();

  /// Schedule a \c printDeclPre callback to be called as soon as a
  /// non-whitespace character is printed.
  void callPrintDeclPre(const Decl *D) {
    PendingDeclPreCallback = D;
  }

  /// Schedule a \c printDeclLoc callback to be called as soon as a
  /// non-whitespace character is printed.
  void callPrintDeclLoc(const Decl *D) {
    PendingDeclLocCallback = D;
  }

  /// Schedule a \c printNamePre callback to be called as soon as a
  /// non-whitespace character is printed.
  void callPrintNamePre(PrintNameContext Context) {
    PendingNamePreCallback = Context;
  }

  /// To sanitize a malformed utf8 string to a well-formed one.
  static std::string sanitizeUtf8(StringRef Text);
  static bool printTypeInterface(Type Ty, DeclContext *DC, std::string &Result);
  static bool printTypeInterface(Type Ty, DeclContext *DC, llvm::raw_ostream &Out);

private:
  virtual void anchor();
};

/// An AST printer for a raw_ostream.
class StreamPrinter : public ASTPrinter {
protected:
  raw_ostream &OS;

public:
  explicit StreamPrinter(raw_ostream &OS) : OS(OS) {}

  void printText(StringRef Text) override;
};

bool shouldPrint(const Decl *D, PrintOptions &Options);
bool shouldPrintPattern(const Pattern *P, PrintOptions &Options);

} // namespace swift

#endif // LLVM_SWIFT_AST_ASTPRINTER_H
