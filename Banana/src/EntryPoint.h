#pragma once

// fixes clangd issue

#include "Application.hpp"

extern Banana::Application* Banana::CreateApplication();

int main()
{
  Banana::Application* app = Banana::CreateApplication();

  app->Run();

  delete app;

  return 0;
}