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

BaseApp::~BaseApp() {}

int BaseApp::run()
{
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed");
    return -1;
  }

  float raceTimer = 0.f;

  while (m_windowPtr->isOpen()) {
    // ── Eventos ─────────────────────────────────────────────────────────────
    m_windowPtr->handleEvents([&](const sf::Event& e) {
      gui.processEvent(m_windowPtr, e);

      if (e.is<sf::Event::Closed>()) {
        m_windowPtr->close();
      }
      if (e.is<sf::Event::KeyPressed>()) {
        auto kp = e.getIf<sf::Event::KeyPressed>();
        if (kp && kp->scancode == sf::Keyboard::Scancode::Escape) {
          m_windowPtr->close();
        }
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

      // Podio cuando el corredor terminó sus vueltas
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

    // ── Render ──────────────────────────────────────────────────────────────
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull())
      m_trackActor->render(m_windowPtr);

    // DEBUG (#5): puntitos amarillos en la posición de cada racer
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

  // 4) Ruta (waypoints) — añade más puntos cuando quieras
  m_path = {
      {100.f,150.f}, {300.f,140.f}, {500.f,160.f}, {700.f,300.f},
      {900.f,280.f}, {1100.f,500.f}, {1300.f,480.f}, {1500.f,450.f}
  };

  // 5) Corredores (Mario, Luigi, Peach, Yoshi)
  auto r1 = EngineUtilities::MakeShared<A_Racer>("Mario", 1);
  auto r2 = EngineUtilities::MakeShared<A_Racer>("Luigi", 2);
  auto r3 = EngineUtilities::MakeShared<A_Racer>("Peach", 3);
  auto r4 = EngineUtilities::MakeShared<A_Racer>("Yoshi", 4);

  for (auto* r : { r1.get(), r2.get(), r3.get(), r4.get() }) {
    r->setPath(m_path);
  }

  // Grid de salida
  r1->getComponent<Transform>()->setPosition(m_path[0] + sf::Vector2f{ 0.f,  0.f });
  r2->getComponent<Transform>()->setPosition(m_path[0] + sf::Vector2f{ 0.f, 30.f });
  r3->getComponent<Transform>()->setPosition(m_path[0] + sf::Vector2f{ 30.f,  0.f });
  r4->getComponent<Transform>()->setPosition(m_path[0] + sf::Vector2f{ 30.f, 30.f });

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

  // Pasa meta, nº de vueltas y escala a cada corredor
  for (auto& r : m_racers) {
    if (!r) continue;
    r->setFinishLine(m_finishLine);
    r->setTotalLaps(3);
    if (auto xf = r->getComponent<Transform>()) {
      xf->setScale({ 2.5f, 2.5f }); // para que se vean bien
    }
  }

  // 7) GUI arranque
  gui.setRacers(m_racers);

  return true;
}

// (Si tenías BaseApp::update/render/destroy) puedes dejarlas vacías o no definirlas aquí.
