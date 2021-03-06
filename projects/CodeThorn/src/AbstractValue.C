// -*- mode: C++; coding: utf-8; -*-
/*************************************************************
 * Copyright: (C) 2012 by Markus Schordan                    *
 * Author   : Markus Schordan                                *
 * License  : see file LICENSE in the CodeThorn distribution *
 *************************************************************/

#include "sage3basic.h"
#include "AbstractValue.h"
#include "assert.h"
#include "CommandLineOptions.h"
#include <iostream>
#include <climits>
#include "Miscellaneous.h"
#include "Miscellaneous2.h"
#include "CodeThornException.h"
#include "VariableIdMapping.h"
#include "TypeSizeMapping.h"

using namespace std;
using namespace SPRAY;
using namespace CodeThorn;

SgTypeSizeMapping* AbstractValue::_typeSizeMapping=nullptr;

istream& CodeThorn::operator>>(istream& is, AbstractValue& value) {
  value.fromStream(is);
  return is;
}


// default constructor
AbstractValue::AbstractValue():valueType(AbstractValue::BOT),intValue(0) {}

// type conversion
// TODO: represent value 'unitialized' here
AbstractValue::AbstractValue(VariableId varId):valueType(AbstractValue::PTR),variableId(varId),intValue(0) {
}

// type conversion
AbstractValue::AbstractValue(bool val) {
  if(val) {
    valueType=AbstractValue::INTEGER;
    intValue=1;
  } else {
    valueType=AbstractValue::INTEGER;
    intValue=0;
  }
}

void AbstractValue::setTypeSizeMapping(SgTypeSizeMapping* typeSizeMapping) {
#if 0
  if(AbstractValue::_typeSizeMapping!=nullptr) {
    delete _typeSizeMapping;
  }
#endif
  AbstractValue::_typeSizeMapping=typeSizeMapping;
}

SgTypeSizeMapping* AbstractValue::getTypeSizeMapping() {
  return  _typeSizeMapping;
}

AbstractValue::TypeSize AbstractValue::calculateTypeSize(SPRAY::BuiltInType btype) {
  ROSE_ASSERT(AbstractValue::_typeSizeMapping);
  return AbstractValue::_typeSizeMapping->getTypeSize(btype);
}

void AbstractValue::setValue(long long int val) {
  ROSE_ASSERT(typeSize!=0);
  // TODO: truncate here if necessary
  intValue=val;
}

void AbstractValue::setValue(long double fval) {
  ROSE_ASSERT(typeSize!=0);
  // TODO: adapt here if necessary
  floatValue=fval;
}

AbstractValue AbstractValue::createIntegerValue(SPRAY::BuiltInType btype, long long int ival) {
  AbstractValue aval;
  aval.initInteger(btype,ival);
  return aval;
}

void AbstractValue::initInteger(SPRAY::BuiltInType btype, long long int ival) {
  valueType=AbstractValue::INTEGER;
  setTypeSize(calculateTypeSize(btype));
  setValue(ival);
}

void AbstractValue::initFloat(SPRAY::BuiltInType btype, long double fval) {
  valueType=AbstractValue::FLOAT;
  setTypeSize(calculateTypeSize(btype));
  setValue(fval);
}

// type conversion
AbstractValue::AbstractValue(Top e) {valueType=AbstractValue::TOP;intValue=0;}
// type conversion
AbstractValue::AbstractValue(Bot e) {valueType=AbstractValue::BOT;intValue=0;}

AbstractValue::AbstractValue(unsigned char x) {
  initInteger(BITYPE_UCHAR,x);
}
AbstractValue::AbstractValue(signed char x) {
  initInteger(BITYPE_SCHAR,x);
}
AbstractValue::AbstractValue(short x) {
  initInteger(BITYPE_SSHORT,x);
}
AbstractValue::AbstractValue(int x) {
  initInteger(BITYPE_SINT,x);
}
AbstractValue::AbstractValue(long int x) {
  initInteger(BITYPE_SLONG,x);
}
AbstractValue::AbstractValue(long long int x) {
  initInteger(BITYPE_SLONG_LONG,x);
}

AbstractValue::AbstractValue(unsigned short int x) {
  initInteger(BITYPE_USHORT,x);
}
AbstractValue::AbstractValue(unsigned int x) {
  initInteger(BITYPE_UINT,x);
}
AbstractValue::AbstractValue(unsigned long int x) {
  initInteger(BITYPE_ULONG,x);
}
AbstractValue::AbstractValue(unsigned long long int x) {
  initInteger(BITYPE_ULONG_LONG,x);
}
AbstractValue::AbstractValue(float x) {
  initFloat(BITYPE_FLOAT,x);
}
AbstractValue::AbstractValue(double x) {
  initFloat(BITYPE_DOUBLE,x);
}
AbstractValue::AbstractValue(long double x) {
  initFloat(BITYPE_LONG_DOUBLE,x);
}

AbstractValue AbstractValue::createNullPtr() {
  AbstractValue aval(0);
  // create an integer 0, not marked as pointer value.
  return aval;
}

AbstractValue 
AbstractValue::createAddressOfVariable(SPRAY::VariableId varId) {
  return AbstractValue::createAddressOfArray(varId);
}
AbstractValue 
AbstractValue::createAddressOfArray(SPRAY::VariableId arrayVarId) {
  return AbstractValue::createAddressOfArrayElement(arrayVarId,AbstractValue(0));
}
AbstractValue 
AbstractValue::createAddressOfArrayElement(SPRAY::VariableId arrayVariableId, 
                                             AbstractValue index) {
  AbstractValue val;
  if(index.isTop()) {
    return Top();
  } else if(index.isBot()) {
    return Bot();
  } else if(index.isConstInt()) {
    val.valueType=PTR;
    val.variableId=arrayVariableId;
    ROSE_ASSERT(index.isConstInt());
    val.intValue=index.getIntValue();
    return val;
  } else {
    cerr<<"Error: createAddressOfArray: unknown index type."<<endl;
    exit(1);
  }
}

std::string AbstractValue::valueTypeToString() const {
  switch(valueType) {
  case TOP: return "top";
  case INTEGER: return "constint";
  case FLOAT: return "float";
  case PTR: return "ptr";
  case REF: return "ref";
  case BOT: return "bot";
  default:
    return "unknown";
  }
}

// currently maps to isTop(); in preparation for explicit handling of
// undefined values.
bool AbstractValue::isUndefined() const {return isTop();}

bool AbstractValue::isTop() const {return valueType==AbstractValue::TOP;}
bool AbstractValue::isTrue() const {return valueType==AbstractValue::INTEGER && intValue!=0;}
bool AbstractValue::isFalse() const {return valueType==AbstractValue::INTEGER && intValue==0;}
bool AbstractValue::isBot() const {return valueType==AbstractValue::BOT;}
bool AbstractValue::isConstInt() const {return valueType==AbstractValue::INTEGER;}
bool AbstractValue::isPtr() const {return (valueType==AbstractValue::PTR);}
bool AbstractValue::isNullPtr() const {return valueType==AbstractValue::INTEGER && intValue==0;}

long AbstractValue::hash() const {
  if(isTop()) return LONG_MAX;
  else if(isBot()) return LONG_MIN;
  else if(isConstInt()) return getIntValue();
  else if(isPtr()) {
    VariableId varId=getVariableId();
    ROSE_ASSERT(varId.isValid());
    return varId.getIdCode()+getIntValue();
  }
  else throw CodeThorn::Exception("Error: AbstractValue hash: unknown value.");
}

AbstractValue AbstractValue::operatorNot() {
  AbstractValue tmp;
  switch(valueType) {
  case AbstractValue::INTEGER: 
    tmp.valueType=valueType;
    if(intValue==0) {
      tmp.intValue=1;
    } else {
      tmp.intValue=0;
    }
    break;
  case AbstractValue::TOP: tmp=Top();break;
  case AbstractValue::BOT: tmp=Bot();break;
  default:
    throw CodeThorn::Exception("Error: AbstractValue operation '!' failed.");
  }
  return tmp;
}

AbstractValue AbstractValue::operatorOr(AbstractValue other) {
  AbstractValue tmp;
  // all TOP cases
  if(isTop()   && other.isTop())   return Top();
  if(isTop()   && other.isTrue())  return true;
  if(isTrue()  && other.isTop())   return true;
  if(isTop()   && other.isFalse()) return Top();
  if(isFalse() && other.isTop())   return Top();
  // all BOT cases
  if(valueType==BOT) {
    tmp.valueType=other.valueType; 
    tmp.intValue=other.intValue;
    return tmp;
  }
  if(other.valueType==BOT) {
    tmp.valueType=valueType; 
    tmp.intValue=intValue;
    return tmp;
  }
  // otherwise, usual bool cases:
  assert(isConstInt()&&other.isConstInt());
  if(isTrue()  && other.isTrue())  return true;
  if(isTrue()  && other.isFalse()) return true;
  if(isFalse() && other.isTrue())  return true;
  if(isFalse() && other.isFalse()) return false;
  throw CodeThorn::Exception("Error: AbstractValue operation|| failed.");
}

AbstractValue AbstractValue::operatorAnd(AbstractValue other) {
  AbstractValue tmp;
  // all TOP cases
  if(isTop()   && other.isTop())   return Top();
  if(isTop()   && other.isTrue())  return Top();
  if(isTrue()  && other.isTop())   return Top();
  if(isTop()   && other.isFalse()) return false;
  if(isFalse() && other.isTop())   return false;
  // all BOT cases
  if(valueType==BOT) {
    tmp.valueType=other.valueType;
    tmp.intValue=other.intValue;
    return tmp;
  }
  if(other.valueType==BOT) {
    tmp.valueType=valueType; 
    tmp.intValue=intValue;
    return tmp;
  }
  // otherwise, usual bool cases:
  assert(isConstInt()&&other.isConstInt());
  if(isTrue() && other.isTrue())   return true;
  if(isTrue() && other.isFalse())  return false;
  if(isFalse() && other.isTrue())  return false;
  if(isFalse() && other.isFalse()) return false;
  throw CodeThorn::Exception("Error: AbstractValue operation&& failed.");
}
 
bool CodeThorn::strictWeakOrderingIsSmaller(const AbstractValue& c1, const AbstractValue& c2) {
  if (c1.getValueType()!=c2.getValueType()) {
    return c1.getValueType()<c2.getValueType();
  } else {
    ROSE_ASSERT(c1.getValueType()==c2.getValueType()); 
    if(c1.isConstInt() && c2.isConstInt()) {
      return c1.getIntValue()<c2.getIntValue();
    } else if(c1.isPtr() && c2.isPtr()) {
      if(c1.getVariableId()!=c2.getVariableId()) {
        return c1.getVariableId()<c2.getVariableId();
      } else {
        if(c1.getIntValue()!=c2.getIntValue()) {
          return c1.getIntValue()<c2.getIntValue();
        } else {
          return c1.getValueSize()<c2.getValueSize();
        }
      }
    } else if (c1.isBot()==c2.isBot()) {
      return false;
    } else if (c1.isTop()==c2.isTop()) {
      return false;
    } else {
      throw CodeThorn::Exception("Error: AbstractValue::strictWeakOrderingIsSmaller: unknown equal values.");
    }
  }
}

bool CodeThorn::strictWeakOrderingIsEqual(const AbstractValue& c1, const AbstractValue& c2) {
  if(c1.getValueType()==c2.getValueType()) {
    if(c1.isConstInt() && c2.isConstInt())
      return c1.getIntValue()==c2.getIntValue() && c1.getValueSize()==c2.getValueSize();
    else if(c1.isPtr() && c2.isPtr()) {
      return c1.getVariableId()==c2.getVariableId() && c1.getIntValue()==c2.getIntValue() && c1.getValueSize()==c2.getValueSize();
    } else {
      ROSE_ASSERT((c1.isTop()&&c2.isTop()) || (c1.isBot()&&c2.isBot()));
      return true;
    }
  } else {
    // different value types
    return false;
  }
}

bool CodeThorn::AbstractValueCmp::operator()(const AbstractValue& c1, const AbstractValue& c2) const {
  return CodeThorn::strictWeakOrderingIsSmaller(c1,c2);
}

//bool AbstractValue::operator==(AbstractValue other) const {
//  return CodeThorn::strictWeakOrderingIsEqual(*this,other);
//}
//bool AbstractValue::operator!=(AbstractValue other) const {
//  return !CodeThorn::strictWeakOrderingIsEqual(*this,other);
//}
bool AbstractValue::operator==(const AbstractValue other) const {
  return CodeThorn::strictWeakOrderingIsEqual(*this,other);
}
bool AbstractValue::operator!=(const AbstractValue other) const {
  return !CodeThorn::strictWeakOrderingIsEqual(*this,other);
}
bool AbstractValue::operator<(AbstractValue other) const {
  return CodeThorn::strictWeakOrderingIsSmaller(*this,other);
}

// TODO: comparison with nullptr
AbstractValue AbstractValue::operatorEq(AbstractValue other) const {
  // all TOP cases
  if(valueType==TOP || other.valueType==TOP) { 
    return CodeThorn::Top();
  }
  // all BOT cases
  if(valueType==BOT) {
    return other;
  } else if(other.valueType==BOT) { 
    return *this;
  } else if(isPtr() && other.isPtr()) {
    return AbstractValue(variableId==other.variableId && intValue==other.intValue && getValueSize()==other.getValueSize());
  } else if(isConstInt() && other.isConstInt()) {
    return AbstractValue(intValue==other.intValue && getValueSize()==other.getValueSize());
  } else {
    return AbstractValue(Top()); // all other cases can be true or false
  }
}

AbstractValue AbstractValue::operatorNotEq(AbstractValue other) const {
  return ((*this).operatorEq(other)).operatorNot();
}

AbstractValue AbstractValue::operatorLess(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()<other.getIntValue();
}

AbstractValue AbstractValue::operatorLessOrEq(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()<=other.getIntValue();
}

AbstractValue AbstractValue::operatorMoreOrEq(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()>=other.getIntValue();
}

AbstractValue AbstractValue::operatorMore(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()>other.getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseOr(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()|other.getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseXor(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()^other.getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseAnd(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return other;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()&other.getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseComplement() const {
  if(isTop()||isBot())
    return *this;
  assert(isConstInt());
  return ~getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseShiftLeft(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return *this;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()<<other.getIntValue();
}

AbstractValue AbstractValue::operatorBitwiseShiftRight(AbstractValue other) const {
  if(isTop()||other.isTop())
    return Top();
  if(isBot())
    return *this;
  if(other.isBot())
    return *this;
  assert(isConstInt()&&other.isConstInt());
  return getIntValue()>>other.getIntValue();
}

string AbstractValue::toLhsString(SPRAY::VariableIdMapping* vim) const {
  switch(valueType) {
  case TOP: return "top";
  case BOT: return "bot";
  case INTEGER: {
    stringstream ss;
    ss<<getIntValue();
    return ss.str();
  }
  case PTR: {
    stringstream ss;
    if(vim->getNumberOfElements(variableId)==1) {
      ss<<variableId.toString(vim); // variables are arrays of size 1
    } else {
      ss<<variableId.toString(vim)<<"["<<getIntValue()<<"]";
    }
    return ss.str();
  }
  default:
    throw CodeThorn::Exception("Error: AbstractValue::toLhsString operation failed. Unknown abstraction type.");
  }
}

string AbstractValue::toRhsString(SPRAY::VariableIdMapping* vim) const {
  switch(valueType) {
  case TOP: return "top";
  case BOT: return "bot";
  case INTEGER: {
    stringstream ss;
    ss<<getIntValue();
    return ss.str();
  }
  case PTR: {
    stringstream ss;
    ss<<"&"; // on the rhs an abstract pointer is always a pointer value of some abstract value
    if(vim->getNumberOfElements(variableId)==1) {
      ss<<variableId.toString(vim); // variables are arrays of size 1
    } else {
      ss<<variableId.toString(vim)<<"["<<getIntValue()<<"]";
    }
    return ss.str();
  }
  default:
    throw CodeThorn::Exception("Error: AbstractValue::toRhsString operation failed. Unknown abstraction type.");
  }
}

string AbstractValue::arrayVariableNameToString(SPRAY::VariableIdMapping* vim) const {
  switch(valueType) {
  case PTR: {
    stringstream ss;
    ss<<variableId.toString(vim);
    return ss.str();
  }
  default:
    throw CodeThorn::Exception("Error: AbstractValue::arrayVariableNameToString operation failed. Unknown abstraction type.");
  }
}

string AbstractValue::toString(SPRAY::VariableIdMapping* vim) const {
  switch(valueType) {
  case TOP: return "top";
  case BOT: return "bot";
  case INTEGER: {
    stringstream ss;
    ss<<getIntValue();
    return ss.str();
  }
  case FLOAT: {
    return getFloatValueString();
  }
  case PTR: {
    //    if(vim->hasArrayType(variableId)||vim->hasClassType(variableId)||vim->hasReferenceType(variableId)||vim->isHeapMemoryRegionId(variableId)) {
      stringstream ss;
      ss<<"("
        <<variableId.toString(vim)
        <<","
        <<getIntValue()
        <<")";
      return ss.str();
      //    } else {
      //      return variableId.toString(vim);
      //    }
  }
  default:
    throw CodeThorn::Exception("Error: AbstractValue::toString operation failed. Unknown abstraction type.");
  }
}

string AbstractValue::toString() const {
  switch(valueType) {
  case TOP: return "top";
  case BOT: return "bot";
  case INTEGER: {
    stringstream ss;
    ss<<getIntValue();
    return ss.str();
  }
  case FLOAT: {
    return getFloatValueString();
  }
  case PTR: {
    stringstream ss;
    ss<<"("<<variableId.toString()<<","<<getIntValue()<<")";
    return ss.str();
  }
  default:
    throw CodeThorn::Exception("Error: AbstractValue::toString operation failed. Unknown abstraction type.");
  }
}

void AbstractValue::fromStream(istream& is) {
  int tmpintValue=0;
  if(SPRAY::Parse::checkWord("top",is)) {
    valueType=TOP;
    intValue=0;
  } else if(SPRAY::Parse::checkWord("bot",is)) {
    valueType=BOT;
    intValue=0;
  } else if(SPRAY::Parse::integer(is,tmpintValue)) {
    valueType=INTEGER;
    intValue=tmpintValue;
  } else {
    throw CodeThorn::Exception("Error: ConstIntLattic::fromStream failed.");
  }
}

AbstractValue::ValueType AbstractValue::getValueType() const {
  return valueType;
}

AbstractValue::TypeSize AbstractValue::getTypeSize() const {
  return typeSize;
}

// deprecated
AbstractValue::TypeSize AbstractValue::getValueSize() const {
  return getTypeSize();
}

void AbstractValue::setTypeSize(AbstractValue::TypeSize typeSize) {
  this->typeSize=typeSize;
}

int AbstractValue::getIndexIntValue() const { 
  if(valueType!=PTR) {
    cerr << "AbstractValue: valueType="<<valueTypeToString()<<endl;
    throw CodeThorn::Exception("Error: AbstractValue::getIndexIntValue operation failed.");
  }
  else 
    return intValue;
}

int AbstractValue::getIntValue() const { 
  // TODO: PTR will be removed once all ptrs are adapted to getIndexIntValue
  if(valueType!=INTEGER && valueType!=PTR) {
    cerr << "AbstractValue: valueType="<<valueTypeToString()<<endl;
    throw CodeThorn::Exception("Error: AbstractValue::getIntValue operation failed.");
  }
  else 
    return intValue;
}

std::string AbstractValue::getFloatValueString() const { 
   if(valueType!=FLOAT) {
     cerr << "AbstractValue: valueType="<<valueTypeToString()<<endl;
     throw CodeThorn::Exception("Error: AbstractValue::getFloatValueString operation failed.");
   } else {
     stringstream ss;
     ss<<floatValue;
     return ss.str();
   }
}

SPRAY::VariableId AbstractValue::getVariableId() const { 
  if(valueType!=PTR && valueType!=REF) {
    cerr << "AbstractValue: valueType="<<valueTypeToString()<<endl;
    throw CodeThorn::Exception("Error: AbstractValue::getVariableId operation failed.");
  }
  else 
    return variableId;
}

// arithmetic operators
AbstractValue AbstractValue::operatorUnaryMinus() {
  AbstractValue tmp;
  switch(valueType) {
  case AbstractValue::INTEGER: 
    tmp.valueType=AbstractValue::INTEGER;
    tmp.intValue=-intValue; // unary minus
    break;
  case AbstractValue::TOP: tmp=Top();break;
  case AbstractValue::BOT: tmp=Bot();break;
  case AbstractValue::PTR:
    throw CodeThorn::Exception("Error: AbstractValue operator unary minus on pointer value.");
  default:
    throw CodeThorn::Exception("Error: AbstractValue operation unaryMinus failed.");
  }
  return tmp;
}

AbstractValue AbstractValue::operatorAdd(AbstractValue& a,AbstractValue& b) {
  if(a.isTop() || b.isTop())
    return Top();
  if(a.isBot())
    return b;
  if(b.isBot())
    return a;
  if(a.isPtr() && b.isConstInt()) {
    AbstractValue val=a;
    val.intValue+=b.intValue;
    return val;
  } else if(a.isConstInt() && b.isPtr()) {
    AbstractValue val=b;
    val.intValue+=a.intValue;
    return val;
  } else if(a.isPtr() && b.isPtr()) {
    throw CodeThorn::Exception("Error: invalid operands of type pointer to binary ‘operator+’.");
  } else if(a.isConstInt() && b.isConstInt()) {
    return a.getIntValue()+b.getIntValue();
  } else {
    throw CodeThorn::Exception("Error: undefined behavior in '+' operation.");
  }
}
AbstractValue AbstractValue::operatorSub(AbstractValue& a,AbstractValue& b) {
  if(a.isTop() || b.isTop())
    return Top();
  if(a.isBot())
    return b;
  if(b.isBot())
    return a;
  if(a.isPtr() && b.isPtr()) {
    if(a.getVariableId()==b.getVariableId()) {
      AbstractValue val;
      val.intValue=a.intValue-b.intValue;
      val.valueType=INTEGER;
      val.variableId=a.variableId; // same as b.variableId
      return val;
    } else {
      return Top(); // subtraction of incompatible pointers gives arbitrary value
    }
  } else if(a.isPtr() && b.isConstInt()) {
    AbstractValue val=a;
    val.intValue-=b.intValue;
    return val;
  } else if(a.isConstInt() && b.isPtr()) {
    throw CodeThorn::Exception("Error: forbidden operation in '-' operation. Attempt to subtract pointer from integer.");
  } else if(a.isConstInt() && b.isConstInt()) {
    return a.getIntValue()-b.getIntValue();
  } else {
    throw CodeThorn::Exception("Error: undefined behavior in '-' operation.");
  }
}
AbstractValue AbstractValue::operatorMul(AbstractValue& a,AbstractValue& b) {
  if(a.isTop() || b.isTop())
    return Top();
  if(a.isBot())
    return b;
  if(b.isBot())
    return a;
  // TODO multiplication of pointer values
  assert(a.isConstInt() && b.isConstInt());
  return a.getIntValue()*b.getIntValue();
}
AbstractValue AbstractValue::operatorDiv(AbstractValue& a,AbstractValue& b) {
  if(a.isTop() || b.isTop())
    return Top();
  if(a.isBot())
    return b;
  if(b.isBot())
    return a;
  // TODO division of pointer values
  assert(a.isConstInt() && b.isConstInt());
  return a.getIntValue()/b.getIntValue();

}
AbstractValue AbstractValue::operatorMod(AbstractValue& a,AbstractValue& b) {
  if(a.isTop() || b.isTop())
    return Top();
  if(a.isBot())
    return b;
  if(b.isBot())
    return a;
  // TODO modulo of pointer values
  assert(a.isConstInt() && b.isConstInt());
  return a.getIntValue()%b.getIntValue();
}

AbstractValue CodeThorn::operator+(AbstractValue& a,AbstractValue& b) {
  return AbstractValue::operatorAdd(a,b);
}
AbstractValue CodeThorn::operator-(AbstractValue& a,AbstractValue& b) {
  return AbstractValue::operatorSub(a,b);
}
AbstractValue CodeThorn::operator*(AbstractValue& a,AbstractValue& b) {
  return AbstractValue::operatorMul(a,b);
}
AbstractValue CodeThorn::operator/(AbstractValue& a,AbstractValue& b) {
  return AbstractValue::operatorDiv(a,b);
}
AbstractValue CodeThorn::operator%(AbstractValue& a,AbstractValue& b) {
  return AbstractValue::operatorMod(a,b);
}

AbstractValueSet& CodeThorn::operator+=(AbstractValueSet& s1, AbstractValueSet& s2) {
  for(AbstractValueSet::iterator i=s2.begin();i!=s2.end();++i) {
    s1.insert(*i);
  }
  return s1;
}
