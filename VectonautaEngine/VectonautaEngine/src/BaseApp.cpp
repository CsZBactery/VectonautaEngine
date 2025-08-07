#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "A_Racer.h"
#include "ECS/Transform.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

// Helper para la línea de meta
static bool crossedFinishLine(const sf::Vector2f& pos,
  const sf::FloatRect& finish)
{
  return finish.contains(pos);
}

BaseApp::~BaseApp() {}

int BaseApp::run()
{
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed");
    return -1;
  }

  float raceTimer = 0.f;

  while (m_windowPtr->isOpen()) {
    // — Eventos —
    m_windowPtr->handleEvents([&](const sf::Event& e) {
      gui.processEvent(m_windowPtr, e);
      if (e.is<sf::Event::Closed>()) m_windowPtr->close();
      });

    // — Tiempo —
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();
    if (!gui.isPaused())
      raceTimer += dt * gui.getSpeedMultiplier();

    // — Lógica de carrera —
    for (auto& r : m_racers) {
      if (!r) continue;
      if (!gui.isPaused())
        r->update(dt * gui.getSpeedMultiplier());

      // chequeo de meta
      if (r->getPlace() == 0) {
        auto xf = r->getComponent<Transform>();
        if (xf && crossedFinishLine(xf->getPosition(), m_finishLine)) {
          int p = int(m_finishedOrder.size()) + 1;
          r->setPlace(p);
          m_finishedOrder.push_back(r);
        }
      }
    }

    // — Reset pedido por GUI —
    if (gui.shouldResetWaypoints()) {
      for (auto& r : m_racers) if (r) r->reset();
      m_finishedOrder.clear();
      raceTimer = 0.f;
    }

    // — Pasar estado a GUI y actualizarlo —
    gui.setRacers(m_racers);
    gui.update(m_windowPtr, m_windowPtr->deltaTime, raceTimer);
    if (gui.shouldQuit())
      m_windowPtr->close();

    // — Render —
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull())
      m_trackActor->render(m_windowPtr);

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
  // 1) Crear ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window");
    return false;
  }

  // 2) Inicializar GUI
  gui.init(m_windowPtr);

  // 3) Cargar y configurar pista
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

  // 4) Definir ruta (central de la pista)
  m_path = {
    {100.f,150.f}, {300.f,140.f}, {500.f,160.f}, {700.f,300.f},
    {900.f,280.f}, {1100.f,500.f}, {1300.f,480.f}, {1500.f,450.f}
  };

  // 5) Crear dos racers de ejemplo
  auto r1 = EngineUtilities::MakeShared<A_Racer>("Racer 1", 1);
  auto r2 = EngineUtilities::MakeShared<A_Racer>("Racer 2", 2);
  r1->setPath(m_path);
  r2->setPath(m_path);
  r1->getComponent<Transform>()->setPosition(m_path[0]);
  r2->getComponent<Transform>()->setPosition(m_path[0] + sf::Vector2f{ 0.f,30.f });

  if (!resourceMan.loadTexture("Sprites/Mario", "png"))
    MESSAGE("BaseApp", "init", "Cannot load Mario.png");
  auto marioTex = resourceMan.getTexture("Sprites/Mario");
  if (!marioTex.isNull()) {
    r1->setTexture(marioTex);
    r2->setTexture(marioTex);
  }
  m_racers = { r1, r2 };

  // 6) Meta (SFML 3 syntax)
  m_finishLine = sf::FloatRect{ {1800.f,500.f}, {50.f,200.f} };

  // 7) GUI arranque
  gui.setRacers(m_racers);

  return true;
}

// **Eliminamos** aquí las definiciones secundarias de update/render/destroy:
// void BaseApp::update()  { /*...*/ }
// void BaseApp::render()  { /*...*/ }
// void BaseApp::destroy() { /*...*/ }
