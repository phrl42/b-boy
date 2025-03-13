#include "incs.h"
#ifdef MACRO_GLFW
  #include "platform/GLFW/GLFWInput.h"
  Banana::Input *Banana::Input::Instance = new Banana::GLFWInput();
#endif