#include "incs.h"
#include "imgui/IMGUILayer.h"

#include "../../vendor/IMGUI/imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Application.hpp"
#include "event/Input.hpp"
#include "event/KeyCode.h"

#include "../Sandbox/src/imgui_debug.h"
#include "../Sandbox/src/gbc/info.h"

#include <sstream>

namespace Banana
{
  std::vector<std::pair<uint16_t, std::string>> instructions;
  const char* Hex_To_CString(const uint16_t val, const char* prefix)
  {
    std::string ret;
    std::stringstream hex;
    
    hex << std::hex << val;
    ret = std::string(prefix) + hex.str();

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
 
  void replace_first(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
    ) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;
    s.replace(pos, toReplace.length(), replaceWith);
  }

  void add_address(uint16_t address, std::string mnemonic)
  {
    for(auto pair : instructions)
    {
      if(address == pair.first)
      {
	return;
      }
    }

    replace_first(mnemonic, "e8", std::string(Hex_To_String(Stats::spec->bus.Read(address+1), "$(") + ")"));
    replace_first(mnemonic, "n8", std::string(Hex_To_String(Stats::spec->bus.Read(address+1), "$(") + ")"));
    replace_first(mnemonic, "a8", std::string(Hex_To_String(Stats::spec->bus.Read(address+1), "$(") + ")"));

    replace_first(mnemonic, "a16", std::string(Hex_To_String(Stats::spec->bus.Read(address+2) << 8 | Stats::spec->bus.Read(address+1), "$(") + ")"));

    replace_first(mnemonic, "n16", std::string(Hex_To_String(Stats::spec->bus.Read(address+2) << 8 | Stats::spec->bus.Read(address+1) , "$(") + ")"));
    instructions.push_back(std::pair<uint16_t, std::string>(address, mnemonic));
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
    //set default range (uft-8)
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

      ImGui::DockBuilderDockWindow("Tiles", dock_right_id);
      ImGui::DockBuilderDockWindow("Disassembler", dock_right_id);
      ImGui::DockBuilderDockWindow("Scene", dock_main_id);
      ImGui::DockBuilderDockWindow("Registers", dock_down_id);
      ImGui::DockBuilderDockWindow("Debugger", dock_down_id);
    }

    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockflags);
    ImGui::End();

    ImGui::Begin("Tiles", nullptr, 0);
    ImGui::End();

    ImGui::Begin("Debugger", nullptr, 0);
    if(ImGui::ImageButton((void*)Stats::play_id, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      LOG("Play");
    }
    ImGui::SameLine();
    if(ImGui::ImageButton((void*)Stats::stop_id, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      LOG("Stop");
    }
    ImGui::End();

    ImGui::Begin("Disassembler", nullptr, 0);
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.PC, "PC: 0x"));
    ImGui::BeginChild("Scrolling");
    add_address(Stats::spec->cpu.PC, opcode_info[Stats::spec->bus.Read(Stats::spec->cpu.PC)]);
    auto red = ImVec4(1, 0, 0, 1);
    auto green = ImVec4(0, 1, 0, 1);
    for (int32_t n = instructions.size()-1; n >= 0; n--)
    {
      auto chose = red;
      if(instructions[n].first == Stats::spec->cpu.PC) chose = green;
      ImGui::TextColored(chose, "[%x]: %s", instructions[n].first, instructions[n].second.c_str());
    }
    ImGui::EndChild(); 
    ImGui::Text(Hex_To_CString(Stats::spec->bus.Read(Stats::spec->cpu.PC), "[PC]: $"));
    ImGui::End();
    
    ImGui::Begin("Registers", nullptr, 0);
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.AF, "AF: 0x"));
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.BC, "BC: 0x"));
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.DE, "DE: 0x"));
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.HL, "HL: 0x"));
    ImGui::Text(Hex_To_CString(Stats::spec->cpu.SP, "SP: 0x"));

    std::string msg = "FPS: 60";
    if(1 / dt < 59)
      msg = "FPS: " + std::to_string(1 / dt);
    ImGui::Text(msg.c_str());
    // maybe put rom name here
    // maybe but restart buttons and stuff in here
    ImGui::End();
    
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
    ImVec2 winsize = ImGui::GetWindowSize();
    
    ImGui::Image((void*)Application::GetInstance().fb->GetColorAttachmentID(), {winsize.x - 10, winsize.y - 42}, {0, 1}, {1, 0});
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();
  }

  void IMGUILayer::OnEvent(Event& event)
  {

  }

};
