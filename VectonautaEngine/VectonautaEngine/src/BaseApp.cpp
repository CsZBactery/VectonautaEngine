// BaseApp.cpp
#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "A_Racer.h"
#include "ECS/Transform.h"
#include "CShape.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>   // std::max
#include <fstream>     // save/load path

namespace { // ------- helpers de geometría / path -------

  inline float vlen(const sf::Vector2f& v) { return std::sqrt(v.x * v.x + v.y * v.y); }
  inline sf::Vector2f vnorm(const sf::Vector2f& v) {
    float L = vlen(v); return (L > 1e-6f) ? sf::Vector2f{ v.x / L, v.y / L } : sf::Vector2f{ 0.f,0.f };
  }
  inline sf::Vector2f vperp(const sf::Vector2f& v) { return sf::Vector2f{ -v.y, v.x }; }

  // Densifica una polilínea cerrada para que los segmentos no superen maxSegLen px
  std::vector<sf::Vector2f> densifyClosed(const std::vector<sf::Vector2f>& pts, float maxSegLen) {
    std::vector<sf::Vector2f> out;
    if (pts.size() < 2) return out;
    const int N = (int)pts.size();
    out.reserve(N * 4);
    for (int i = 0;i < N;++i) {
      const sf::Vector2f A = pts[i];
      const sf::Vector2f B = pts[(i + 1) % N];
      out.push_back(A);
      const float d = vlen(B - A);
      if (d > maxSegLen) {
        int steps = std::max(1, (int)std::floor(d / maxSegLen));
        sf::Vector2f dir = (B - A) * (1.f / (float)(steps + 1));
        for (int k = 1;k <= steps;++k) out.push_back(A + dir * (float)k);
      }
    }
    return out;
  }

  // Offset lateral de una polilínea cerrada usando bisectriz (más suave en curvas)
  std::vector<sf::Vector2f> offsetClosed(const std::vector<sf::Vector2f>& path, float offsetPx) {
    const int N = (int)path.size();
    if (N < 2 || std::abs(offsetPx) < 1e-6f) return path;
    std::vector<sf::Vector2f> res(N);
    for (int i = 0;i < N;++i) {
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

  // Debug: dibuja una polilínea cerrada con puntos
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

} // namespace

// ----- Estado de editor de ruta (file-scope para no tocar BaseApp.h) -----
static bool s_editMode = false;
static std::vector<sf::Vector2f> s_editPts;

// Guarda/lee una ruta simple (x y por línea)
static bool savePathTxt(const std::string& path, const std::vector<sf::Vector2f>& pts) {
  std::ofstream f(path); if (!f) return false;
  for (auto& p : pts) f << p.x << " " << p.y << "\n";
  return true;
}
static bool loadPathTxt(const std::string& path, std::vector<sf::Vector2f>& out) {
  std::ifstream f(path); if (!f) return false;
  out.clear();
  float x, y; while (f >> x >> y) out.push_back({ x,y });
  return !out.empty();
}

// ---------------- BaseApp ----------------

BaseApp::~BaseApp() {}

int BaseApp::run()
{
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed");
    return -1;
  }

  float raceTimer = 0.f;

  // ⚙️ Lambda para finalizar el trazado (sin usar sf::Event por defecto)
  auto finalizePath = [&]() {
    if (!s_editMode || s_editPts.size() < 3) return;

    // Cerrar el lazo si falta
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
    };

  while (m_windowPtr->isOpen()) {
    // ── Eventos ─────────────────────────────────────────────────────────────
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
        if (s_editMode && kp->scancode == sf::Keyboard::Scancode::F)
          finalizePath(); // ✅ sin sf::Event falso
      }
      if (e.is<sf::Event::MouseButtonPressed>()) {
        if (!s_editMode) return;
        auto mb = e.getIf<sf::Event::MouseButtonPressed>();
        if (!mb || mb->button != sf::Mouse::Button::Left) return;

        // Mapeo pixel->coords (usa la view actual)
        auto& rw = m_windowPtr->getInternal();
        sf::Vector2i pix = sf::Mouse::getPosition(rw);
        sf::Vector2f world = rw.mapPixelToCoords(pix);

        s_editPts.push_back(world);
      }
      });

    // ── Tiempo ──────────────────────────────────────────────────────────────
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();
    if (!gui.isPaused())
      raceTimer += dt * gui.getSpeedMultiplier();

    // ── Lógica de carrera ───────────────────────────────────────────────────
    for (auto& r : m_racers) {
      if (!r) continue;

      if (!gui.isPaused())
        r->update(dt * gui.getSpeedMultiplier());

      if (r->getPlace() == 0 && r->isFinished()) {
        int p = int(m_finishedOrder.size()) + 1;
        r->setPlace(p);
        m_finishedOrder.push_back(r);
      }
    }

    // Reset pedido por GUI
    if (gui.shouldResetWaypoints()) {
      for (auto& r : m_racers) if (r) r->reset();
      m_finishedOrder.clear();
      raceTimer = 0.f;
    }

    // ── GUI ─────────────────────────────────────────────────────────────────
    gui.setRacers(m_racers);
    gui.update(m_windowPtr, m_windowPtr->deltaTime, raceTimer);
    if (gui.shouldQuit()) m_windowPtr->close();

    // Ventana chiquita de Path Tools
    {
      ImGui::Begin("Path Tools", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Edit mode: %s  (press 'E' to toggle)", s_editMode ? "ON" : "OFF");
      ImGui::Text("Points: %d", (int)s_editPts.size());
      if (ImGui::Button("Finalize (F)")) {
        finalizePath(); // ✅ llamar directo
      }
      if (ImGui::Button("Save path")) {
        savePathTxt("bin/Paths/track.path", s_editPts);
      }
      ImGui::SameLine();
      if (ImGui::Button("Load path")) {
        std::vector<sf::Vector2f> tmp;
        if (loadPathTxt("bin/Paths/track.path", tmp)) s_editPts = tmp;
      }
      ImGui::Separator();
      ImGui::Text("Click izq: add point | Z: undo | C: clear | F: finish");
      ImGui::End();
    }

    // ── Render ──────────────────────────────────────────────────────────────
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull())
      m_trackActor->render(m_windowPtr);

    // Ruta activa (cian)
    if (m_path.size() >= 2) drawClosedPath(*m_windowPtr, m_path, sf::Color(0, 255, 255));
    // Ruta en edición (magenta)
    if (!s_editPts.empty()) drawClosedPath(*m_windowPtr, s_editPts, sf::Color(255, 0, 255));

    // Puntitos amarillos (posición real)
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
  // 1) Ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window");
    return false;
  }

  // 2) GUI
  gui.init(m_windowPtr);

  // 3) Pista (Track.png)
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

  // 4) Ruta inicial mínima (puedes borrarla y dibujar la tuya)
  m_path = {
      {100.f,150.f}, {300.f,140.f}, {500.f,160.f}, {700.f,300.f},
      {900.f,280.f}, {1100.f,500.f}, {1300.f,480.f}, {1500.f,450.f}
  };
  m_path = densifyClosed(m_path, 30.f);

  // 5) Corredores (Mario, Luigi, Peach, Yoshi)
  auto r1 = EngineUtilities::MakeShared<A_Racer>("Mario", 1);
  auto r2 = EngineUtilities::MakeShared<A_Racer>("Luigi", 2);
  auto r3 = EngineUtilities::MakeShared<A_Racer>("Peach", 3);
  auto r4 = EngineUtilities::MakeShared<A_Racer>("Yoshi", 4);

  // Carriles por offset (valores moderados; ajusta según ancho de pista)
  auto base = m_path;
  auto laneA = offsetClosed(base, +8.f);
  auto laneB = offsetClosed(base, -8.f);
  auto laneC = offsetClosed(base, +16.f);

  r1->setPath(base);
  r2->setPath(laneA);
  r3->setPath(laneB);
  r4->setPath(laneC);

  // Grid de salida
  r1->getComponent<Transform>()->setPosition(base.front() + sf::Vector2f{ 0.f,  0.f });
  r2->getComponent<Transform>()->setPosition(laneA.front() + sf::Vector2f{ 0.f, 16.f });
  r3->getComponent<Transform>()->setPosition(laneB.front() + sf::Vector2f{ 16.f,  0.f });
  r4->getComponent<Transform>()->setPosition(laneC.front() + sf::Vector2f{ 16.f, 16.f });

  // Texturas de personajes
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

  // 6) Línea de meta (posición + tamaño)
  m_finishLine = sf::FloatRect{ {1800.f,500.f}, {50.f,200.f} };

  // Meta + laps
  for (auto& r : m_racers) {
    if (!r) continue;
    r->setFinishLine(m_finishLine);
    r->setTotalLaps(3);
  }

  // Auto-escala de sprites a tamaño "kart"
  auto fitSprite = [&](const EngineUtilities::TSharedPointer<A_Racer>& racer,
    const EngineUtilities::TSharedPointer<Texture>& texComp,
    float targetPx)
    {
      if (texComp.isNull()) return;
      auto texSize = texComp->getTexture().getSize(); // Vector2u
      float w = static_cast<float>(texSize.x);
      float h = static_cast<float>(texSize.y);
      float s = targetPx / std::max(w, h);            // escala uniforme

      if (auto xf = racer->getComponent<Transform>()) {
        xf->setScale({ s, s });
      }
    };
  fitSprite(r1, texMario, 48.f);
  fitSprite(r2, texLuigi, 48.f);
  fitSprite(r3, texPeach, 48.f);
  fitSprite(r4, texYoshi, 48.f);

  // 7) GUI arranque
  gui.setRacers(m_racers);

  return true;
}
