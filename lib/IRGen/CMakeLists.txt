add_swift_library(swiftIRGen
  DebugTypeInfo.cpp
  EnumPayload.cpp
  ExtraInhabitants.cpp
  Fulfillment.cpp
  GenArchetype.cpp
  GenCast.cpp
  GenClangDecl.cpp
  GenClangType.cpp
  GenClass.cpp
  GenControl.cpp
  GenCoverage.cpp
  GenDecl.cpp
  GenFunc.cpp
  GenHeap.cpp
  GenInit.cpp
  GenMeta.cpp
  GenObjC.cpp
  GenEnum.cpp
  GenExistential.cpp
  GenOpaque.cpp
  GenPoly.cpp
  GenProto.cpp
  GenReflection.cpp
  GenStruct.cpp
  GenTuple.cpp
  GenType.cpp
  GenValueWitness.cpp
  IRGen.cpp
  IRGenDebugInfo.cpp
  IRGenFunction.cpp
  IRGenModule.cpp
  IRGenSIL.cpp
  Linking.cpp
  LocalTypeData.cpp
  SwiftTargetInfo.cpp
  StructLayout.cpp
  TypeLayoutVerifier.cpp
  LINK_LIBRARIES
    swiftAST
    swiftLLVMPasses
    swiftSIL
    swiftSILGen

    # Clang dependencies.
    # FIXME: Clang should really export these in some reasonable manner.
    clangCodeGen
    clangAST
)
