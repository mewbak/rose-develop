#include "sage3basic.h"
#include "ProgramAbstractionLayer.h"
#include "Normalization.h"

#include <iostream>

using namespace std;

SPRAY::ProgramAbstractionLayer::ProgramAbstractionLayer()
  :_modeArrayElementVariableId(true),_labeler(0),_variableIdMapping(0) {
}

SPRAY::ProgramAbstractionLayer::~ProgramAbstractionLayer() {
}

void SPRAY::ProgramAbstractionLayer::setModeArrayElementVariableId(bool val) {
  _modeArrayElementVariableId=val; 
}

bool SPRAY::ProgramAbstractionLayer::getModeArrayElementVariableId() {
  return _modeArrayElementVariableId;; 
}

SgProject* SPRAY::ProgramAbstractionLayer::getRoot() {
  return _root;
}

void SPRAY::ProgramAbstractionLayer::initialize(SgProject* root) {
  _root=root;
  CodeThorn::Normalization lowering;
  lowering.setInliningOption(getInliningOption());
  lowering.normalizeAst(root,getNormalizationLevel());
  _variableIdMapping=new VariableIdMapping();
  getVariableIdMapping()->setModeVariableIdForEachArrayElement(getModeArrayElementVariableId());
  getVariableIdMapping()->computeVariableSymbolMapping(root);
  _labeler=new Labeler(root);
  _functionIdMapping=new FunctionIdMapping();
  getFunctionIdMapping()->computeFunctionSymbolMapping(root);
}

SPRAY::Labeler* SPRAY::ProgramAbstractionLayer::getLabeler(){
  ROSE_ASSERT(_labeler!=0);
  return _labeler;
}

SPRAY::VariableIdMapping* SPRAY::ProgramAbstractionLayer::getVariableIdMapping(){
  ROSE_ASSERT(_variableIdMapping!=0);
  return _variableIdMapping;
}

SPRAY::FunctionIdMapping* SPRAY::ProgramAbstractionLayer::getFunctionIdMapping(){
  ROSE_ASSERT(_functionIdMapping!=0);
  return _functionIdMapping;
}

void SPRAY::ProgramAbstractionLayer::setNormalizationLevel(unsigned int level) {
  _normalizationLevel=level;
}

bool SPRAY::ProgramAbstractionLayer::getNormalizationLevel() {
  return _normalizationLevel;
}

void SPRAY::ProgramAbstractionLayer::setInliningOption(bool flag) {
  _inliningOption=flag;
}

bool SPRAY::ProgramAbstractionLayer::getInliningOption() {
  return _inliningOption;
}
