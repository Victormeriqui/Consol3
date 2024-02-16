#ifdef SYS_WINDOWS
#define _HAS_STD_BYTE 0
#endif

#include "Display/FrameBuffer.hpp"
#include "Engine/Consol3Engine.hpp"
#include "Game/Dual/DualGame.hpp"
#include "Game/IGame.hpp"
#include "Game/Raster/RasterGame.hpp"
#include "Game/Voxel/VoxelGame.hpp"

#ifdef SYS_WINDOWS
#include "Display/Windows/DitheredFrameDrawer.hpp"
#include "Display/Windows/DitheredGreyscaleFrameDrawer.hpp"
#include "Display/Windows/GreyscaleFrameDrawer.hpp"
#include "Display/Windows/WindowsTerminalManager.hpp"
#include "Engine/Input/WindowsInputManager.hpp"
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#elif defined(SYS_LINUX)
#include "Display/Linux/LinuxTerminalManager.hpp"
#include "Display/Windows/WindowsStructsForLinux.hpp"
#include "Engine/Input/LinuxInputManager.hpp"
#endif

#include "Display/Multiplatform/TextOnlyFrameDrawer.hpp"
#include "Display/Multiplatform/VT24BitFrameDrawer.hpp"
#include "Display/Multiplatform/VT8BitFrameDrawer.hpp"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

using namespace Display;
using namespace Engine;

int main(int argc, char* argv[])
{
    uint16_t width  = 100;
    uint16_t height = 100;

    // set current dir to executable dir so resource loading works as intended
    std::filesystem::path executable_path = std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path();
    std::filesystem::current_path(executable_path);

    std::vector<std::shared_ptr<IFrameDrawer>> frame_drawers;

    std::shared_ptr<FrameBuffer<uint32_t>> uint32_t_framebuffer   = std::make_shared<FrameBuffer<uint32_t>>(width, height);
    std::shared_ptr<FrameBuffer<uint8_t>> uint8_t_framebuffer     = std::make_shared<FrameBuffer<uint8_t>>(width, height);
    std::shared_ptr<FrameBuffer<CHAR_INFO>> char_info_framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
    std::shared_ptr<FrameBuffer<char>> char_framebuffer           = std::make_shared<FrameBuffer<char>>(width, height);

    std::shared_ptr<Engine::Input::IInputManager> input_manager;
    std::shared_ptr<Game::IGame> game;

#ifdef SYS_WINDOWS
    // multiplatform frame drawers need to be given a terminal manager
    std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal = std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Consolas", 4, 4);
    frame_drawers.emplace_back(std::make_shared<Multiplatform::VT24BitFrameDrawer<CHAR_INFO>>(uint32_t_framebuffer, windows_terminal));
    frame_drawers.emplace_back(std::make_shared<Multiplatform::VT8BitFrameDrawer<CHAR_INFO>>(uint8_t_framebuffer, windows_terminal));

    std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal_terminal_font = std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Terminal", 4, 4);
    frame_drawers.emplace_back(std::make_shared<Multiplatform::TextOnlyFrameDrawer<CHAR_INFO>>(char_info_framebuffer, windows_terminal_terminal_font));

    frame_drawers.emplace_back(std::make_shared<Windows::DitheredFrameDrawer>(char_info_framebuffer));
    frame_drawers.emplace_back(std::make_shared<Windows::GreyscaleFrameDrawer>(char_info_framebuffer));
    frame_drawers.emplace_back(std::make_shared<Windows::DitheredGreyscaleFrameDrawer>(char_info_framebuffer));

    input_manager = std::make_shared<Engine::Input::WindowsInputManager>();

#elif defined(SYS_LINUX)
    // multiplatform frame drawers need to be given a terminal manager
    std::shared_ptr<ITerminalManager<char>> linux_terminal_manager = std::make_shared<Linux::LinuxTerminalManager>(width, height);

    frame_drawers.emplace_back(std::make_shared<Multiplatform::VT24BitFrameDrawer<char>>(uint32_t_framebuffer, linux_terminal_manager));
    frame_drawers.emplace_back(std::make_shared<Multiplatform::VT8BitFrameDrawer<char>>(uint8_t_framebuffer, linux_terminal_manager));
    frame_drawers.emplace_back(std::make_shared<Multiplatform::TextOnlyFrameDrawer<char>>(char_framebuffer, linux_terminal_manager));

    input_manager = std::make_shared<Engine::Input::LinuxInputManager>();
#endif

#ifdef GAME_RASTER
    game = std::make_shared<Game::Raster::RasterGame>(input_manager);
#elif defined(GAME_VOXEL)
    game = std::make_shared<Game::Voxel::VoxelGame>(input_manager);
#else
    game = std::make_shared<Game::Dual::DualGame>(input_manager);
#endif

    Consol3Engine engine = Consol3Engine(game, input_manager);

    for (const std::shared_ptr<IFrameDrawer>& frame_drawer : frame_drawers)
        engine.RegisterFrameDrawer(frame_drawer);

    engine.Start();

    // char a;
    // scanf_s("%c", &a, 1);

    return 0;
}
