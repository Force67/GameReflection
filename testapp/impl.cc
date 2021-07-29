
#include "api.hpp"

namespace MyNamespace {

MyExportedClass::MyExportedClass() {
  m_MyPrivateInt = 1337;
  m_bMyPrivateBool = true;
}

MyExportedClass::~MyExportedClass() {
  m_MyPrivateInt = 0;
  m_bMyPrivateBool = false;
}

void MyExportedClass::MyMemberFunction() {
  m_MyPrivateInt = 1337222;
  m_bMyPrivateBool ^= m_bMyPrivateBool;
}
}