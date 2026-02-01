#include "app/MyApp.h"
#include <filesystem>

wxIMPLEMENT_APP(MyApp);

std::cout << "Working Dir: " << std::filesystem::current_path() << std::endl;
