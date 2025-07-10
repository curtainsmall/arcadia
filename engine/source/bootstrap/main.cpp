#include "core/main.hpp"
#include "editor/editor_setup.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

ACDA_MAIN_FN_DECL
{
    return Arcadia::Main(Arcadia::EditorSetup);
}
