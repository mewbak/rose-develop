include_directories(${ROSE_INCLUDES})

if(enable-rosehpct)
  add_subdirectory(roseHPCT)
endif()
add_subdirectory(outliner)
add_subdirectory(intelPin)

########### next target ###############

if (enable-binary-analysis)
set(tutorials
  binaryAttribute binaryCallGraph binaryFileRead binaryFileWrite binaryHelloWorld binaryStrings ASTGraphGenerator
  ASTGraphGeneratorCustomized AST_PDF_Generator wholeASTGraphGenerator
  identityTranslator lineReplacement queryLibraryExample nestedQueryExample
  visitorTraversal prePostTraversal combinedTraversals traverseIRnodeTypes
  classicObjectOrientedVisitorPatternTraversal inheritedAttributeTraversal
  synthesizedAttributeTraversal inheritedAndSynthesizedAttributeTraversal
  accumulatorAttributeTraversal persistantAttributes nestedTraversal
  visitorMemoryPoolTraversal
  classicObjectOrientedVisitorPatternMemoryPoolTraversal loopRecognition
  typeInfoFromFunctionParameters resolveOverloadedFunction templateParameter
  instrumentEndOfFunction reductionRecognition instrumentationExample
  addVariableDeclaration addVariableDeclaration2
  addAssignmentStmt addExpression addFunctionCalls addFunctionDeclaration
  addFunctionDeclaration2 addFunctionDeclaration3 loopOptimization LoopProcessor
  binaryAnalysis_symbolTable
  bin_example buildCallGraph buildVFA livenessAnalysis CharmSupport
  debuggingIRnodeToString debuggingSourceCodePositionInformation
  commandlineProcessing interveneAtVariables changeFunction classHierarchyGraph
  dataBaseUsage dwarfAnalysis dwarfInstructionAddressToSourceLineAnalysis
  loopNestingInfoProcessing tauInstrumenter typeTransformation
  astFileIO_GenerateBinaryFile generatingUniqueNamesFromDeclaration
  symbolTableHandling scopeInformation customGraphGeneration inlineTransformations
  addComments addArbitraryTextForUnparser collectComments
  collectDefineDirectives partialRedundancyElimination
  codeGenerationFormatControl dataFlowGraph customGraphGenerationWithFiltering
  graphTraversal wholeGraphAST traversalShortCircuit
  customGraphGenerationWithColors astFileIO_ReadMultipleASTs
  constantFoldingTransformation constantUnFoldingTransformation
  selectedFileTranslation wholeGraphFilterOnPersistantAttributes
  graphGenerationOfSubtree virtualCFG interproceduralCFG
  virtualFunctionIdentification switchWithoutDefault addStruct
  sprintf_to_snprintf seedBugsExample_arrayIndexing volatileTypeModifier
  findVariableDeclarations
)
else()
set(tutorials
  ASTGraphGeneratorCustomized AST_PDF_Generator wholeASTGraphGenerator
  identityTranslator lineReplacement queryLibraryExample nestedQueryExample
  visitorTraversal prePostTraversal combinedTraversals traverseIRnodeTypes
  classicObjectOrientedVisitorPatternTraversal inheritedAttributeTraversal
  synthesizedAttributeTraversal inheritedAndSynthesizedAttributeTraversal
  accumulatorAttributeTraversal persistantAttributes nestedTraversal
  visitorMemoryPoolTraversal
  classicObjectOrientedVisitorPatternMemoryPoolTraversal loopRecognition
  typeInfoFromFunctionParameters resolveOverloadedFunction templateParameter
  instrumentEndOfFunction reductionRecognition instrumentationExample
  addVariableDeclaration addVariableDeclaration2
  addAssignmentStmt addExpression addFunctionCalls addFunctionDeclaration
  addFunctionDeclaration2 addFunctionDeclaration3 loopOptimization LoopProcessor
  bin_example buildCallGraph buildVFA livenessAnalysis CharmSupport
  debuggingIRnodeToString debuggingSourceCodePositionInformation
  commandlineProcessing interveneAtVariables changeFunction classHierarchyGraph
  dataBaseUsage dwarfAnalysis dwarfInstructionAddressToSourceLineAnalysis
  loopNestingInfoProcessing tauInstrumenter typeTransformation
  astFileIO_GenerateBinaryFile generatingUniqueNamesFromDeclaration
  symbolTableHandling scopeInformation customGraphGeneration inlineTransformations
  addComments addArbitraryTextForUnparser collectComments
  collectDefineDirectives partialRedundancyElimination
  codeGenerationFormatControl dataFlowGraph customGraphGenerationWithFiltering
  graphTraversal wholeGraphAST traversalShortCircuit
  customGraphGenerationWithColors astFileIO_ReadMultipleASTs
  constantFoldingTransformation constantUnFoldingTransformation
  selectedFileTranslation wholeGraphFilterOnPersistantAttributes
  graphGenerationOfSubtree virtualCFG interproceduralCFG
  virtualFunctionIdentification switchWithoutDefault addStruct
  sprintf_to_snprintf seedBugsExample_arrayIndexing volatileTypeModifier
  findVariableDeclarations
)
endif()

if(HAVE_PTHREAD_H)
  list(APPEND tutorials sharedMemoryTraversals)
endif()

foreach(tutorial ${tutorials})
  add_executable(${tutorial}Tutorial "${tutorial}.C")
  target_link_libraries(${tutorial}Tutorial ROSE_DLL ${Boost_LIBRARIES})
#  install(TARGETS ${tutorial}Tutorial DESTINATION bin)
endforeach()

install(TARGETS buildCallGraphTutorial DESTINATION bin)
install(TARGETS livenessAnalysisTutorial DESTINATION bin)
install(TARGETS interproceduralCFGTutorial DESTINATION bin)
install(TARGETS virtualCFGTutorial DESTINATION bin)

#if ROSE_HAVE_YICES
#yicesTutorialExample yicesTutorialExample.C)
#endif

if(HAVE_SYS_TIME_H)
  add_executable(codeCoverage codeCoverage.C coverageCodeFromShmuel.c)
  target_link_libraries(codeCoverage ROSE_DLL ${Boost_LIBRARIES})
endif()

add_executable(abstractHandle1 abstractHandle1.cpp)
target_link_libraries(abstractHandle1 ROSE_DLL ${Boost_LIBRARIES})

add_executable(abstractHandle2 abstractHandle2.cpp)
target_link_libraries(abstractHandle2 ROSE_DLL ${Boost_LIBRARIES})

########### install files ###############
