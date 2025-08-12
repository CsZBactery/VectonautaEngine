#include "EngineGUI.h"
#include "Window.h"
#include "A_Racer.h"
#include <imgui-SFML.h>
#include <algorithm>

// Inicializa ImGui-SFML y aplica el tema actual
void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window)
{
  ImGui::SFML::Init(window->getInternal());
  setTheme(m_currentTheme);
}

// Construye y actualiza la UI por frame (menus, paneles, etc.)
void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
  sf::Time deltaTime,
  float raceTimer)
{
  // Sincroniza ImGui con SFML (input, tiempo)
  ImGui::SFML::Update(window->getInternal(), deltaTime);

  // Menú superior y panel lateral
  renderMenuBar();
  renderControlPanel();

  // Ventana de estadísticas (FPS y temporizador de carrera)
  ImGui::Begin("Stats", nullptr,
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Timer: %.2f s", raceTimer);
  ImGui::End();

  // Ventana de corredores/podio
  ImGui::Begin("Racers / Podio", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  // Copia local para ordenar sin afectar el vector original
  auto sorted = m_racers;
  // Orden descendente por progreso (el que va más avanzado primero)
  std::sort(sorted.begin(), sorted.end(),
    [](auto& a, auto& b) {
      return a->getProgress() > b->getProgress();
    });

  int idx = 1;
  for (auto& r : sorted) {
    // Etiqueta: posición en la lista + nombre + (lugar si ya terminó)
    std::string label = std::to_string(idx) + ". " +
      r->getName() +
      " (P" + std::to_string(r->getPlace() ? r->getPlace() : idx) + ")";
    // Porcentaje de progreso en texto corto
    char buf[32];
    std::snprintf(buf, 32, "%.1f%%", r->getProgress() * 100.f);
    ImGui::Text("%s %s", label.c_str(), buf);

    // Botón para reiniciar SOLO este corredor
    if (ImGui::SmallButton(("Reset##" + std::to_string(idx)).c_str()))
      r->reset();
    idx++;
  }
  ImGui::End();
}

// Envía los draw lists de ImGui al render target
void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window)
{
  ImGui::SFML::Render(window->getInternal());
}

// Libera recursos de ImGui-SFML
void EngineGUI::destroy()
{
  ImGui::SFML::Shutdown();
}

// Reenvía eventos de SFML a ImGui (teclado, mouse, wheel, etc.)
void EngineGUI::processEvent(const EngineUtilities::TSharedPointer<Window>& window,
  const sf::Event& event)
{
  ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

// Cambia de tema y aplica el estilo correspondiente
void EngineGUI::setTheme(Theme theme)
{
  m_currentTheme = theme;
  switch (theme) {
  case Theme::Grey:             setupGreyGUIStyle();            break;
  case Theme::Dark:             setupDarkGUIStyle();            break;
  case Theme::VectonautaEngine: setupVectonautaEngineStyle();   break;
  }
}

// Menú superior (File/Game/Theme)
void EngineGUI::renderMenuBar()
{
  if (!ImGui::BeginMainMenuBar()) return;

  // Menú "File"
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Exit")) m_requestQuit = true;
    ImGui::EndMenu();
  }

  // Menú "Game" (pausa, reset, velocidad)
  if (ImGui::BeginMenu("Game")) {
    if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
    if (ImGui::MenuItem("Reset")) m_requestReset = true;
    ImGui::Separator();
    ImGui::Text("Speed:");
    ImGui::SliderFloat("##speed", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
    ImGui::EndMenu();
  }

  // Menú "Theme" (selección rápida de estilo)
  if (ImGui::BeginMenu("Theme")) {
    const char* names[] = { "Grey","Dark","VectonautaEngine" };
    for (int i = 0; i < 3; i++)
      if (ImGui::MenuItem(names[i], nullptr, m_currentTheme == Theme(i)))
        setTheme(Theme(i));
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

// Panel lateral con controles rápidos (tema, pausa, reset, velocidad, exit)
void EngineGUI::renderControlPanel()
{
  ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  // Selector de tema
  const char* names[] = { "Grey","Dark","VectonautaEngine" };
  int cur = int(m_currentTheme);
  if (ImGui::Combo("Theme", &cur, names, 3))
    setTheme(Theme(cur));

  // Botones de pausa y reset global
  if (ImGui::Button(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
  ImGui::SameLine();
  if (ImGui::Button("Reset All")) m_requestReset = true;

  // Multiplicador de velocidad de simulación
  ImGui::SliderFloat("Speed Mul", &m_speedMultiplier, 0.1f, 3.f, "%.2f");

  // Salir de la aplicación
  if (ImGui::Button("Exit")) m_requestQuit = true;

  ImGui::End();
}

// Estilo clásico (gris claro)
void EngineGUI::setupGreyGUIStyle() { ImGui::StyleColorsClassic(); }

// Estilo oscuro por defecto
void EngineGUI::setupDarkGUIStyle() { ImGui::StyleColorsDark(); }

// Estilo personalizado (colores y redondeos)
void EngineGUI::setupVectonautaEngineStyle()
{
  ImGui::StyleColorsDark();
  auto& s = ImGui::GetStyle();
  s.WindowRounding = 6.f;  s.FrameRounding = 4.f;

  // Color acento "dorado"
  ImVec4 gold{ 0.92f, 0.75f, 0.20f, 1.0f };
  s.Colors[ImGuiCol_ButtonHovered] = gold;
  s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.65f, 0.18f, 1.0f);
  s.Colors[ImGuiCol_Border] = gold;

  // Aquí puedes ajustar más colores según tu identidad visual
}
