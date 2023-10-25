#include "incs.h"
#include "imgui/IMGUILayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Application.hpp"
#include "event/Input.hpp"
#include "event/KeyCode.h"

#include "../Sandbox/src/imgui_debug.h"

namespace Banana
{

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

		  ImGui::DockBuilderDockWindow("Debug", dock_right_id);
		  ImGui::DockBuilderDockWindow("Scene", dock_main_id);
		  ImGui::DockBuilderDockWindow("Info", dock_down_id);
    }

    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockflags);
    ImGui::End();

    ImGui::Begin("Debug", nullptr, 0);
    std::string msg = "FPS: 60";
    if(1 / dt < 59) msg = "FPS: " + std::to_string(1 / dt);

    ImGui::Text(msg.c_str());

    // put debug shit in here
    ImGui::End();
    
    ImGui::Begin("Info", nullptr, 0);
    // maybe put rom name here
    // maybe but restart buttons and stuff in here
    ImGui::Text("Info text");
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
