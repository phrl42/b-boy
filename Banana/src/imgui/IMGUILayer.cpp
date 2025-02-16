#include "incs.h"
#include "imgui/IMGUILayer.h"

#include "../../vendor/IMGUI/imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "imgui_internal.h"
#include "imgui_memory_editor.h"

#include "Application.hpp"
#include "event/Input.hpp"
#include "event/KeyCode.h"

#include "../Sandbox/src/imgui_debug.h"
#include "../Sandbox/src/gbc/info.h"

#include <sstream>
#include <bitset>

namespace Banana
{
  const char* Hex_To_CString(const uint16_t val, const char* prefix, bool dbin = false)
  {
    std::string ret;
    std::stringstream hex;

    std::string binary = std::bitset<8>((uint8_t)val).to_string();
    hex << std::hex << val;
    ret = std::string(prefix) + hex.str();

    if(dbin) return binary.c_str();

    return ret.c_str();
  }

  std::string Hex_To_String(uint16_t val, const char* prefix)
  {
    std::string ret;
    std::stringstream hex;
    hex << std::hex << val;
    
    ret = std::string(prefix) + hex.str();

    return ret;
  }

  std::string Hex_To_StringI(int8_t val, const char* prefix)
  {
    std::string ret;
    std::stringstream hex;

    hex << std::hex << val;
    
    ret = std::string(prefix) + hex.str();

    return ret;
  }

  uint8_t Get_Bit_N(uint8_t src, uint8_t n)
  {
    src <<= 7 - n;
    src >>= 7;

    return src;
  }
  
  uint32_t String_To_Hex(const std::string& hex)
  {
    std::stringstream str;
    str << hex;

    uint16_t ret = 0;

    str >> std::hex >> ret;

    return ret;
  }
 
  void replace_first(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
    ) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;
    s.replace(pos, toReplace.length(), replaceWith);
  }

  IMGUILayer::IMGUILayer(const std::string& name)
    : name(name)
  {
    
  }

  void IMGUILayer::OnAttach()
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //fonts
    ImFontAtlas* fontAtlas = io.Fonts;
    ImFontConfig fontConfig = ImFontConfig();
    //set default range (utf-8)
    fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesDefault();
    fontAtlas->AddFontFromFileTTF("assets/fonts/mononoki.ttf", 20, &fontConfig);
    //any new fonts were added to the font pool
    fontConfig.MergeMode = true;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
  }

  void IMGUILayer::OnDetach()
  {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  void IMGUILayer::OnUpdate(float dt)
  {
    static bool show = true;
    static bool p_open = true;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // dockspace stuff
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    ImGuiDockNodeFlags dockflags = ImGuiDockNodeFlags_PassthruCentralNode;//ImGuiDockNodeFlags_None;
    window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;// | ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGuiViewport& viewport = *ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport.Pos);
    ImGui::SetNextWindowSize(viewport.Size);
    ImGui::SetNextWindowViewport(viewport.ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("docking", &p_open, window_flags);
    ImGui::PopStyleVar(3);
    ImGuiID dockspaceID = ImGui::GetID("dockspace");

    static bool initialized = false;

    if (!initialized)
    {
      initialized = true;
      ImGui::DockBuilderRemoveNode(dockspaceID);
      ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspaceID, ImVec2(Application::GetInstance().GetWindow().GetWidth() + 500, Application::GetInstance().GetWindow().GetHeight() + 500));

      ImGuiID dock_main_id = dockspaceID;
      ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
      ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
      ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
      ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
      ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);
      
      ImGui::DockBuilderFinish(dockspaceID);
	 
      ImGui::DockBuilderDockWindow("Scene", dock_main_id);
      ImGui::DockBuilderDockWindow("Tiles", dock_right_id);
      ImGui::DockBuilderDockWindow("CPU", dock_right_id);
      ImGui::DockBuilderDockWindow("MEM", dock_main_id);
      ImGui::DockBuilderDockWindow("Debugger", dock_down_id);
      ImGui::DockBuilderDockWindow("PPU", dock_right_id);
      ImGui::DockBuilderDockWindow("Interrupt", dock_right_id);
      ImGui::DockBuilderDockWindow("Timer", dock_right_id);
      ImGui::DockBuilderDockWindow("ROM", dock_right_id);
    }

    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockflags);
    ImGui::End();

    ImGui::Begin("Tiles", nullptr, 0);
    ImGui::Text("Map 1:");
    ImGui::Image((void*)Stats::tmap1_id, ImVec2(250*2, 800*2), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::Text("Map 2:");
    ImGui::Image((void*)Stats::tmap2_id, ImVec2(250*2, 800*2), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::Text("Tiles:");
    ImGui::Image((void*)Stats::tiles_id, ImVec2(250, 250), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::End();

    ImGui::Begin("Debugger", nullptr, 0);

    static std::string adstop = "FFFFF";
    static std::string breakaddr = "FFFF";

    static std::string adop = "FFF";
    static std::string breakop = "FFF";
    
    ImGui::Text(std::string("Break: 0x" + breakaddr).c_str());
    
    ImGui::PushItemWidth(100);
    if(ImGui::InputText("$", adstop.data(), 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue))
    {
      breakaddr = adstop;
      Stats::spec->breakaddr = String_To_Hex(adstop);
    }

    static bool menu = false;
    
    if(String_To_Hex(breakaddr) == Stats::spec->cpu.PC && menu)
    {
      Stats::spec->dstate = GBC::Debug::STOP;
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(50);
    if(ImGui::InputText(" ", adop.data(), 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue))
    {
      breakop = adop;
      Stats::spec->breakop = String_To_Hex(breakop);
    }

    static bool step_mode = false;
     
    if(ImGui::ImageButton((void*)Stats::play_id, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      breakaddr = adstop;
      if(!step_mode)
      {
	Stats::spec->dstate = GBC::Debug::RUN;
      }
      else
      {
	Stats::spec->dstate = GBC::Debug::STEP;
      }

    }
    
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)Stats::stop_id, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      Stats::spec->dstate = GBC::Debug::STOP;
      step_mode = false;
    }
    auto red = ImVec4(1, 0, 0, 1);
    auto green = ImVec4(0, 1, 0, 1);
    auto blue = ImVec4(0, 0, 1, 1);
    
    ImGui::Text("Step Mode:");
    ImGui::SameLine();
    if(step_mode) ImGui::TextColored(green, "ON");
    if(!step_mode) ImGui::TextColored(red, "OFF");
    if(ImGui::ImageButton((void*)Stats::step_id, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      step_mode = !step_mode;
      
      Stats::spec->dstate = step_mode ? GBC::Debug::STEP : GBC::Debug::STOP;
    }
    
    ImGui::End();

    ImGui::Begin("Interrupt", nullptr, 0);
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_IE, false), "[FFFF] IE: "));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_IF, false), "[FF0F] IF: "));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_JOYPAD, false), "JOYPAD: "));
    ImGui::End();

    ImGui::Begin("Timer", nullptr, 0);
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_DIV, false), "DIV: "));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_TIMA, false), "[FF05] TIMA: "));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_TMA, false), "[FF06] TMA: "));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_TAC, false), "[FF07] TAC: "));
    ImGui::End();

    ImGui::Begin("ROM", nullptr, 0);
    ImGui::Text((std::string("Loaded: ") + std::string(Stats::spec->rom.Get_Rom_Name())).c_str());
    ImGui::End();
    
    ImGui::Begin("PPU", nullptr, 0);
    ImGui::Text(Stats::spec->ppu.GetRenderMode().c_str());
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_LCDC, false), "", true));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_STAT, false), "", true));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_SCX, false), "[FF42] SCX: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_SCY, false), "[FF43] SCY: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_WX, false), "[FF4B] WX: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_WY, false), "[FF4A] WY: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_LY, false), "[FF44] LY: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_LYC, false), "[FF45] LYC: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_DMA, false), "[FF46] DMA: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_BGP, false), "[FF47] BGP: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_OBP0, false), "[FF48] OBP0: ", false));
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(A_OBP1, false), "[FF49] OBP1: ", false));
    ImGui::End();
    
    ImGui::Begin("CPU", nullptr, 0);
    ImGui::Text((std::string("Frames: ") + std::to_string(Stats::spec->ppu.frames)).c_str());
    ImGui::Text(std::string("Cycles: " + std::to_string(Stats::spec->bus.cycles)).c_str());
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.AF >> 8, "A: "));
    ImGui::Text(Hex_To_CString((uint8_t)Stats::spec->cpu.AF, "F: "));
    ImGui::SameLine();
    ImGui::TextColored((Get_Bit_N(Stats::spec->cpu.AF, 7) == 1) ? green : red, "Z");
    ImGui::SameLine();
    ImGui::TextColored((Get_Bit_N(Stats::spec->cpu.AF, 6) == 1) ? green : red, "N");
    ImGui::SameLine();
    ImGui::TextColored((Get_Bit_N(Stats::spec->cpu.AF, 5) == 1) ? green : red, "H");
    ImGui::SameLine();
    ImGui::TextColored((Get_Bit_N(Stats::spec->cpu.AF, 4) == 1) ? green : red, "C");

    ImGui::Text("BC:");
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)(Stats::spec->cpu.BC >> 8)), ""));
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)Stats::spec->cpu.BC), ""));


    ImGui::Text("DE:");
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)(Stats::spec->cpu.DE >> 8)), ""));
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)Stats::spec->cpu.DE), ""));
	
    ImGui::Text("HL:");
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)(Stats::spec->cpu.HL >> 8)), ""));
    ImGui::SameLine();
    ImGui::Text(Hex_To_CString(((uint8_t)Stats::spec->cpu.HL), ""));
	
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.SP, "SP: "));

    std::string msg = "FPS: " + std::to_string(1 / dt);
    ImGui::Text(msg.c_str());
    ImGui::Text(Stats::spec->io.serial);
    // maybe put rom name here
    // maybe but restart buttons and stuff in here

    switch(Stats::spec->dstate)
    {
    case GBC::Debug::STOP:
    {
      ImGui::TextColored(red, "[ DEBUG STOP ]");
      break;
    }

    case GBC::Debug::RUN:
    {
      ImGui::TextColored(green, "[ DEBUG RUN ]");
      break;
    }

    case GBC::Debug::STEP:
    {
      ImGui::TextColored(blue, "[ DEBUG STEP ]");
    }

    default:
    {
      break;
    }
    }

    ImGui::Text("");
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(Stats::spec->cpu.PC, false), "$"));
 
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.PC, "PC: 0x"));
    ImGui::SameLine();
    switch(Stats::spec->cpu.state)
    {
    case GBC::State::STOP:
    {
      ImGui::TextColored(red, "[ STOP ]");
      break;
    }
    case GBC::State::HALT:
    {
      ImGui::TextColored(red, "[ HALT ]");
      break;
    }
    case GBC::State::RUN:
    {
      ImGui::TextColored(green, "[ RUN ]");
      break;
    }
    default:
    {
      break;
    }
    
    }

    if(ImGui::Button("DEBUG"))
    {
      menu = !menu;
    }

    if(menu)
    {
      ImGui::BeginChild("Scrolling");
      auto char_height = ImGui::CalcTextSize("NIGG").y;
      for (int n = 0; n < Stats::spec->instructions.size(); n++)
      {
	auto chose = red;
	if(Stats::spec->instructions[n].first == Stats::spec->cpu.PC) chose = green;
	ImGui::TextColored(chose, "[%x]: %s", Stats::spec->instructions[n].first, Stats::spec->instructions[n].second.c_str());
      }
      if(Stats::spec->dstate != GBC::Debug::STOP) ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + (Stats::spec->cpu.PC * char_height) + char_height * 25);
      ImGui::EndChild();
    }

    ImGui::End();
    
    static MemoryEditor mem_edit;
    uint8_t ram[0x10000];
    for(int i = 0; i <= 0xFFFF; i++)
    {
      ram[i] = Stats::spec->bus.Read(i, false);
    }
    mem_edit.DrawWindow("MEM", ram, sizeof(uint8_t) * GBC_RAM_SIZE); 


    ImGui::SetNextWindowSize(ImVec2(166*3, 144*3), ImGuiCond_Always);
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
    ImVec2 winsize = ImGui::GetWindowSize();
    
    ImGui::Image((void*)Stats::screen_id, {winsize.x - 10, winsize.y - 42}, {0, 0}, {1, 1});
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();
  }

  void IMGUILayer::OnEvent(Event& event)
  {

  }

};
