#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]  // Need for Windows Forms
    int WINAPI
    WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
      Application::EnableVisualStyles();
      Application::SetCompatibleTextRenderingDefault(false);

      // Запуск формы
      ComputerManagementApp::Form1 form;
      Application::Run(% form);

      return 0;
    }