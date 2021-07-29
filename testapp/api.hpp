#pragma once

#define EXPM_VISIBLE

namespace MyNamespace {

class EXPM_VISIBLE MyExportedClass {
 public:
  MyExportedClass();
  ~MyExportedClass();

  void MyMemberFunction();

 private:
  int m_MyPrivateInt;
  bool m_bMyPrivateBool;
};
}  // namespace MyNamespace