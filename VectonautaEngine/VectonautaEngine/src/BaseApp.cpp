// BaseApp.cpp
#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "A_Racer.h"
#include "ECS/Transform.h"
#include "CShape.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // (no lo usamos ahora, pero muchas cabeceras tuyas lo incluyen)
#include <SFML/Window.hpp>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>

// =========================================================
// Helper de entrada: funciona en SFML 2.6+ (scancodes) y 2.5-
// =========================================================
#ifndef SFML_VERSION_MAJOR
#define SFML_VERSION_MAJOR 2
#define SFML_VERSION_MINOR 5
#endif

#if (SFML_VERSION_MAJOR > 2) || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 6)
// SFML 2.6+: tenemos Scancode
static bool keyDownKC(sf::Keyboard::Key key, sf::Keyboard::Scancode sc) {
  return sf::Keyboard::isKeyPressed(key) || sf::Keyboard::isKeyPressed(sc);
}
#define KEY(k, sc) keyDownKC((k), (sc))
#else
// SFML 2.5 o menor: solo Key
static bool keyDownK(sf::Keyboard::Key key) {
  return sf::Keyboard::isKeyPressed(key);
}
#define KEY(k, sc) keyDownK((k))
#endif

// ===== Helpers geométricos / path =====
namespace {

  inline float vlen(const sf::Vector2f& v) { return std::sqrt(v.x * v.x + v.y * v.y); }
  inline sf::Vector2f vnorm(const sf::Vector2f& v) {
    float L = vlen(v);
    return (L > 1e-6f) ? sf::Vector2f{ v.x / L, v.y / L } : sf::Vector2f{ 0.f,0.f };
  }
  inline sf::Vector2f vperp(const sf::Vector2f& v) { return sf::Vector2f{ -v.y, v.x }; }

  // Densifica una polilínea cerrada para que los segmentos no superen maxSegLen
  std::vector<sf::Vector2f> densifyClosed(const std::vector<sf::Vector2f>& pts, float maxSegLen) {
    std::vector<sf::Vector2f> out;
    if (pts.size() < 2) return out;
    const int N = (int)pts.size();
    out.reserve(N * 4);
    for (int i = 0; i < N; ++i) {
      const sf::Vector2f A = pts[i];
      const sf::Vector2f B = pts[(i + 1) % N];
      out.push_back(A);
      const float d = vlen(B - A);
      if (d > maxSegLen) {
        int steps = std::max(1, (int)std::floor(d / maxSegLen));
        sf::Vector2f step = (B - A) * (1.f / (float)(steps + 1));
        for (int k = 1; k <= steps; ++k) out.push_back(A + step * (float)k);
      }
    }
    return out;
  }

  // Offset lateral con bisectriz (más suave)
  std::vector<sf::Vector2f> offsetClosed(const std::vector<sf::Vector2f>& path, float offsetPx) {
    const int N = (int)path.size();
    if (N < 2 || std::abs(offsetPx) < 1e-6f) return path;
    std::vector<sf::Vector2f> res(N);
    for (int i = 0; i < N; ++i) {
      const sf::Vector2f Pm = path[(i - 1 + N) % N];
      const sf::Vector2f P = path[i];
      const sf::Vector2f Pp = path[(i + 1) % N];

      sf::Vector2f t1 = vnorm(P - Pm);
      sf::Vector2f t2 = vnorm(Pp - P);
      sf::Vector2f t = vnorm(t1 + t2);
      if (t.x == 0.f && t.y == 0.f) t = t1;

      sf::Vector2f nrm = vnorm(vperp(t));   // normal a la izquierda
      res[i] = P + nrm * offsetPx;
    }
    return res;
  }

  // Dibuja una polilínea cerrada
  void drawClosedPath(Window& w, const std::vector<sf::Vector2f>& p, sf::Color col) {
    if (p.size() < 2) return;
    sf::VertexArray va(sf::PrimitiveType::LineStrip);
    va.resize(p.size() + 1);
    for (std::size_t i = 0; i < p.size(); ++i) {
      va[i].position = p[i];
      va[i].color = col;
    }
    va[p.size()].position = p[0];
    va[p.size()].color = col;
    w.draw(va);

    sf::CircleShape c(3.f); c.setFillColor(col);
    for (auto& pt : p) { c.setPosition(pt - sf::Vector2f{ 3.f,3.f }); w.draw(c); }
  }

  // Rutas de assets relativizadas a $(ProjectDir)/bin
  static std::filesystem::path assetPath(const std::string& rel) {
    auto p = std::filesystem::current_path() / "bin" / rel;
    std::error_code ec;
    auto canon = std::filesystem::weakly_canonical(p, ec);
    return ec ? p : canon;
  }

} // namespace

// ===== Estado de editor =====
static bool s_editMode = false;
static std::vector<sf::Vector2f> s_editPts;

// ===== Estado del jugador (persistente en este TU) =====
static sf::Vector2f s_playerVel{ 0.f, 0.f };
static float        s_playerAngleDeg = 0.f;
static bool         s_playerInit = false;

// Parámetros de control (expuestos en la ventanita Player Control)
static float s_pcAccel = 400.f;  // px/s^2
static float s_pcTurnRad = 2.60f;  // rad/s
static float s_pcMaxSpeed = 300.f;  // px/s
static float s_pcFriction = 0.90f;  // 0..1 por frame @60Hz aprox.

// Guarda / carga de ruta (x y por línea)
static bool savePathTxt(const std::string& rel, const std::vector<sf::Vector2f>& pts) {
  auto path = assetPath(rel);
  std::filesystem::create_directories(path.parent_path());
  std::ofstream f(path);
  if (!f) return false;
  for (auto& p : pts) f << p.x << " " << p.y << "\n";
  return true;
}
static bool loadPathTxt(const std::string& rel, std::vector<sf::Vector2f>& out) {
  auto path = assetPath(rel);
  std::ifstream f(path);
  if (!f) return false;
  out.clear();
  float x, y; while (f >> x >> y) out.push_back({ x,y });
  return !out.empty();
}

// ---------------- BaseApp ----------------

BaseApp::~BaseApp() {}

// --- Movimiento del jugador (aplica sobre el Transform del corredor seleccionado)
void BaseApp::updatePlayerControl(float dt)
{
  if (m_playerIdx < 0 || m_playerIdx >= (int)m_racers.size()) return;
  auto& r = m_racers[m_playerIdx];
  if (!r) return;

  auto xf = r->getComponent<Transform>();
  if (!xf) return;

  // Inicializa ángulo con el del Transform la primera vez
  if (!s_playerInit) {
    s_playerAngleDeg = xf->getRotation();
    s_playerVel = { 0.f, 0.f };
    s_playerInit = true;
  }

  // -------- Entrada (WASD + Flechas) ----------
  bool acc = KEY(sf::Keyboard::Key::W, sf::Keyboard::Scancode::W)
    || KEY(sf::Keyboard::Key::Up, sf::Keyboard::Scancode::Up);

  bool brk = KEY(sf::Keyboard::Key::S, sf::Keyboard::Scancode::S)
    || KEY(sf::Keyboard::Key::Down, sf::Keyboard::Scancode::Down);

  float steer = 0.f;
  if (KEY(sf::Keyboard::Key::A, sf::Keyboard::Scancode::A) ||
    KEY(sf::Keyboard::Key::Left, sf::Keyboard::Scancode::Left))  steer -= 1.f;
  if (KEY(sf::Keyboard::Key::D, sf::Keyboard::Scancode::D) ||
    KEY(sf::Keyboard::Key::Right, sf::Keyboard::Scancode::Right)) steer += 1.f;

  // -------- Física muy simple ----------
  constexpr float PI = 3.1415926535f;
  constexpr float RAD2DEG = 180.f / PI;
  constexpr float DEG2RAD = PI / 180.f;

  // Giro (rad/s -> deg/s)
  s_playerAngleDeg += steer * (s_pcTurnRad * RAD2DEG) * dt;

  // Dirección forward desde el ángulo
  const float ang = s_playerAngleDeg * DEG2RAD;
  sf::Vector2f fwd{ std::cos(ang), std::sin(ang) };

  // Acelera / frena
  if (acc) {
    s_playerVel += fwd * (s_pcAccel * dt);
  }
  if (brk) {
    s_playerVel -= fwd * (0.8f * s_pcAccel * dt);
  }

  // Fricción (aprox. como potencia por 60 Hz)
  float fr = std::pow(s_pcFriction, dt * 60.f);
  s_playerVel *= fr;

  // Clamp de velocidad
  float spd = vlen(s_playerVel);
  if (spd > s_pcMaxSpeed) {
    s_playerVel = s_playerVel * (s_pcMaxSpeed / spd);
  }

  // Integración
  xf->setPosition(xf->getPosition() + s_playerVel * dt);
  xf->setRotation(s_playerAngleDeg);
}

int BaseApp::run()
{
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed");
    return -1;
  }

  float raceTimer = 0.f;

  while (m_windowPtr->isOpen()) {
    // Eventos
    m_windowPtr->handleEvents([&](const sf::Event& e) {
      gui.processEvent(m_windowPtr, e);

      if (e.is<sf::Event::Closed>()) {
        m_windowPtr->close();
      }
      if (e.is<sf::Event::KeyPressed>()) {
        auto kp = e.getIf<sf::Event::KeyPressed>();
        if (!kp) return;
        if (kp->scancode == sf::Keyboard::Scancode::Escape) m_windowPtr->close();
        if (kp->scancode == sf::Keyboard::Scancode::E)      s_editMode = !s_editMode;
        if (s_editMode && kp->scancode == sf::Keyboard::Scancode::Z && !s_editPts.empty())
          s_editPts.pop_back();
        if (s_editMode && kp->scancode == sf::Keyboard::Scancode::C)
          s_editPts.clear();
        if (s_editMode && kp->scancode == sf::Keyboard::Scancode::F && s_editPts.size() >= 3) {
          // Cierra lazo si hace falta
          if (vlen(s_editPts.front() - s_editPts.back()) > 5.f)
            s_editPts.push_back(s_editPts.front());

          // Densificar y aplicar carriles
          m_path = densifyClosed(s_editPts, 30.f);

          std::vector<std::vector<sf::Vector2f>> lanes;
          lanes.push_back(m_path);
          lanes.push_back(offsetClosed(m_path, +12.f));
          lanes.push_back(offsetClosed(m_path, -12.f));
          lanes.push_back(offsetClosed(m_path, +24.f));

          for (std::size_t i = 0; i < m_racers.size(); ++i) {
            auto lane = lanes[std::min<std::size_t>(i, lanes.size() - 1)];
            m_racers[i]->setPath(lane);
            if (auto xf = m_racers[i]->getComponent<Transform>())
              xf->setPosition(lane.front());
          }
        }
      }

      if (e.is<sf::Event::MouseButtonPressed>()) {
        if (!s_editMode) return;
        auto mb = e.getIf<sf::Event::MouseButtonPressed>();
        if (!mb || mb->button != sf::Mouse::Button::Left) return;

        auto& rw = m_windowPtr->getInternal();
        sf::Vector2i pix = sf::Mouse::getPosition(rw);
        sf::Vector2f world = rw.mapPixelToCoords(pix);
        s_editPts.push_back(world);
      }
      });

    // Tiempo
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();
    if (!gui.isPaused())
      raceTimer += dt * gui.getSpeedMultiplier();

    // ================= LÓGICA DE CARRERA =================

    // 1) Aplica input del jugador primero (mueve su Transform manualmente)
    if (!gui.isPaused() && m_playerIdx >= 0 && m_playerIdx < (int)m_racers.size()) {
      updatePlayerControl(dt * gui.getSpeedMultiplier());
    }

    // 2) Actualiza corredores (IA o sincronización de sprite)
    for (std::size_t i = 0; i < m_racers.size(); ++i) {
      auto& r = m_racers[i];
      if (!r) continue;

      if ((int)i == m_playerIdx) {
        // Jugador: no avanza por IA, pero sí sincroniza su sprite con el Transform
        r->update(0.f);
      }
      else {
        if (!gui.isPaused())
          r->update(dt * gui.getSpeedMultiplier());

        if (r->getPlace() == 0 && r->isFinished()) {
          int p = int(m_finishedOrder.size()) + 1;
          r->setPlace(p);
          m_finishedOrder.push_back(r);
        }
      }
    }

    // Reset pedido por GUI
    if (gui.shouldResetWaypoints()) {
      for (auto& r : m_racers) if (r) r->reset();
      m_finishedOrder.clear();
      raceTimer = 0.f;
      // También resetea estado del jugador
      s_playerVel = { 0.f,0.f };
      s_playerInit = false;
    }

    // GUI: principal
    gui.setRacers(m_racers);
    gui.update(m_windowPtr, m_windowPtr->deltaTime, raceTimer);
    if (gui.shouldQuit()) m_windowPtr->close();

    // Ventana de herramientas de path
    {
      ImGui::Begin("Path Tools", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Edit mode: %s  (press 'E' to toggle)", s_editMode ? "ON" : "OFF");
      ImGui::Text("Points: %d", (int)s_editPts.size());
      if (ImGui::Button("Finalize (F)")) {
        if (s_editPts.size() >= 3) {
          if (vlen(s_editPts.front() - s_editPts.back()) > 5.f)
            s_editPts.push_back(s_editPts.front());

          m_path = densifyClosed(s_editPts, 30.f);
          std::vector<std::vector<sf::Vector2f>> lanes;
          lanes.push_back(m_path);
          lanes.push_back(offsetClosed(m_path, +12.f));
          lanes.push_back(offsetClosed(m_path, -12.f));
          lanes.push_back(offsetClosed(m_path, +24.f));

          for (std::size_t i = 0; i < m_racers.size(); ++i) {
            auto lane = lanes[std::min<std::size_t>(i, lanes.size() - 1)];
            m_racers[i]->setPath(lane);
            if (auto xf = m_racers[i]->getComponent<Transform>())
              xf->setPosition(lane.front());
          }
        }
      }
      if (ImGui::Button("Save path")) {
        savePathTxt("Paths/track.path", s_editPts.empty() ? m_path : s_editPts);
      }
      ImGui::SameLine();
      if (ImGui::Button("Load path")) {
        std::vector<sf::Vector2f> tmp;
        if (loadPathTxt("Paths/track.path", tmp)) {
          s_editPts = tmp; // deja ver los puntos en magenta
        }
      }
      ImGui::Separator();
      ImGui::Text("Click izq: add point | Z: undo | C: clear | F: finish");
      ImGui::End();
    }

    // Ventana de Player Control (selección + parámetros)
    {
      ImGui::Begin("Player Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Selecciona corredor (1..4, 0 = ninguno)");
      int idx = (m_playerIdx < 0) ? 0 : (m_playerIdx + 1);
      if (ImGui::RadioButton("Ninguno", idx == 0)) idx = 0;
      ImGui::SameLine();
      if (ImGui::RadioButton("Mario", idx == 1)) idx = 1;
      ImGui::SameLine();
      if (ImGui::RadioButton("Luigi", idx == 2)) idx = 2;
      ImGui::SameLine();
      if (ImGui::RadioButton("Peach", idx == 3)) idx = 3;
      ImGui::SameLine();
      if (ImGui::RadioButton("Yoshi", idx == 4)) idx = 4;

      int newPlayerIdx = (idx == 0) ? -1 : (idx - 1);
      if (newPlayerIdx != m_playerIdx) {
        m_playerIdx = newPlayerIdx;
        s_playerVel = { 0.f,0.f };
        s_playerInit = false;
      }

      ImGui::Separator();
      ImGui::Text("W/Up: acelerar | S/Down: frenar");
      ImGui::Text("A/Left: girar izq | D/Right: girar der");
      ImGui::SliderFloat("Aceleracion", &s_pcAccel, 50.f, 1200.f);
      ImGui::SliderFloat("Giro", &s_pcTurnRad, 0.5f, 6.0f);
      ImGui::SliderFloat("VelMax", &s_pcMaxSpeed, 50.f, 1200.f);
      ImGui::SliderFloat("Friccion", &s_pcFriction, 0.80f, 0.999f);
      ImGui::End();
    }

    // Render
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull())
      m_trackActor->render(m_windowPtr);

    // Ruta activa (cian)
    if (m_path.size() >= 2) drawClosedPath(*m_windowPtr, m_path, sf::Color(0, 255, 255));
    // Ruta en edición (magenta)
    if (!s_editPts.empty()) drawClosedPath(*m_windowPtr, s_editPts, sf::Color(255, 0, 255));

    // Puntitos amarillos = posición real de cada corredor
    {
      sf::CircleShape dot(5.f);
      dot.setFillColor(sf::Color::Yellow);
      for (auto& r : m_racers) {
        if (!r) continue;
        if (auto xf = r->getComponent<Transform>()) {
          dot.setPosition(xf->getPosition());
          m_windowPtr->draw(dot);
        }
      }
    }

    // Sprites de los racers
    for (auto& r : m_racers)
      if (r) r->render(m_windowPtr);

    gui.render(m_windowPtr);
    m_windowPtr->display();
  }

  destroy();
  return 0;
}

bool BaseApp::init()
{
  // Ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window");
    return false;
  }

  // GUI
  gui.init(m_windowPtr);

  // Pista (Track.png)
  if (!resourceMan.loadTexture("Sprites/Track", "png"))
    MESSAGE("BaseApp", "init", "Cannot load Track.png");

  auto trackTex = resourceMan.getTexture("Sprites/Track");
  if (trackTex.isNull()) {
    ERROR("BaseApp", "init", "Track texture null");
    return false;
  }

  m_trackActor = EngineUtilities::MakeShared<Actor>("Track");
  {
    auto sh = m_trackActor->getComponent<CShape>();
    if (!sh) {
      ERROR("BaseApp", "init", "Missing CShape on track actor");
      return false;
    }
    sh->createShape(ShapeType::RECTANGLE);
    sh->setFillColor(sf::Color::White);

    auto sz = trackTex->getTexture().getSize();
    if (auto r = dynamic_cast<sf::RectangleShape*>(sh->getShape())) {
      r->setSize({ float(sz.x), float(sz.y) });
      r->setOrigin({ 0.f, 0.f });
    }
    float sx = 1920.f / float(sz.x);
    float sy = 1080.f / float(sz.y);
    sh->setScale({ sx, sy });
  }
  m_trackActor->setTexture(trackTex);
  m_trackActor->getComponent<Transform>()->setPosition({ 0.f, 0.f });

  // Intenta cargar una ruta por defecto si existe
  {
    std::vector<sf::Vector2f> loaded;
    if (loadPathTxt("Paths/track.path", loaded)) {
      s_editPts = loaded;
      if (!s_editPts.empty() && vlen(s_editPts.front() - s_editPts.back()) > 5.f)
        s_editPts.push_back(s_editPts.front());
      m_path = densifyClosed(s_editPts, 30.f);
    }
    else {
      m_path.clear(); // sin ruta inicial: usa el editor
    }
  }

  // Corredores (Mario, Luigi, Peach, Yoshi)
  auto r1 = EngineUtilities::MakeShared<A_Racer>("Mario", 1);
  auto r2 = EngineUtilities::MakeShared<A_Racer>("Luigi", 2);
  auto r3 = EngineUtilities::MakeShared<A_Racer>("Peach", 3);
  auto r4 = EngineUtilities::MakeShared<A_Racer>("Yoshi", 4);

  // Si hay ruta, genera carriles y asigna
  if (!m_path.empty()) {
    auto base = m_path;
    auto laneA = offsetClosed(base, +12.f);
    auto laneB = offsetClosed(base, -12.f);
    auto laneC = offsetClosed(base, +24.f);

    r1->setPath(base);
    r2->setPath(laneA);
    r3->setPath(laneB);
    r4->setPath(laneC);

    if (auto xf = r1->getComponent<Transform>()) xf->setPosition(base.front());
    if (auto xf = r2->getComponent<Transform>()) xf->setPosition(laneA.front());
    if (auto xf = r3->getComponent<Transform>()) xf->setPosition(laneB.front());
    if (auto xf = r4->getComponent<Transform>()) xf->setPosition(laneC.front());
  }

  // Texturas
  resourceMan.loadTexture("Sprites/Mario", "png");
  resourceMan.loadTexture("Sprites/Luigi", "png");
  resourceMan.loadTexture("Sprites/Peach", "png");
  resourceMan.loadTexture("Sprites/Yoshi", "png");

  auto texMario = resourceMan.getTexture("Sprites/Mario");
  auto texLuigi = resourceMan.getTexture("Sprites/Luigi");
  auto texPeach = resourceMan.getTexture("Sprites/Peach");
  auto texYoshi = resourceMan.getTexture("Sprites/Yoshi");

  if (!texMario.isNull()) r1->setTexture(texMario);
  if (!texLuigi.isNull()) r2->setTexture(texLuigi);
  if (!texPeach.isNull()) r3->setTexture(texPeach);
  if (!texYoshi.isNull()) r4->setTexture(texYoshi);

  m_racers = { r1, r2, r3, r4 };

  // Meta y vueltas
  m_finishLine = sf::FloatRect{ {1800.f,500.f}, {50.f,200.f} };
  for (auto& r : m_racers) {
    if (!r) continue;
    r->setFinishLine(m_finishLine);
    r->setTotalLaps(3);
  }

  // Escala uniforme de sprites (48 px)
  auto fitSprite = [&](const EngineUtilities::TSharedPointer<A_Racer>& racer,
    const EngineUtilities::TSharedPointer<Texture>& texComp,
    float targetPx)
    {
      if (texComp.isNull()) return;
      auto texSize = texComp->getTexture().getSize();
      float w = static_cast<float>(texSize.x);
      float h = static_cast<float>(texSize.y);
      float s = targetPx / std::max(w, h);
      if (auto xf = racer->getComponent<Transform>()) {
        xf->setScale({ s, s });
      }
    };
  fitSprite(r1, texMario, 48.f);
  fitSprite(r2, texLuigi, 48.f);
  fitSprite(r3, texPeach, 48.f);
  fitSprite(r4, texYoshi, 48.f);

  // Estado inicial: sin jugador seleccionado
  m_playerIdx = -1;
  s_playerVel = { 0.f,0.f };
  s_playerInit = false;

  // GUI arranque
  gui.setRacers(m_racers);
  return true;
}

void BaseApp::destroy()
{
  // Nada especial ahora (sin audio). Limpieza básica si quieres.
  m_racers.clear();
  m_finishedOrder.clear();
  m_trackActor.reset();
}
