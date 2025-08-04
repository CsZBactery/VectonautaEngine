#include "BaseApp.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "A_Racer.h"
#include "ECS/Transform.h"
#include <SFML/Graphics.hpp>
#include <cmath>

// Helper para detectar cruce de meta
static bool crossedFinishLine(const sf::Vector2f& position, const sf::FloatRect& finishRect) {
  return finishRect.contains(position);
}

BaseApp::~BaseApp() {}

int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initialization failed", "Check init()");
    return -1;
  }

  float raceTimer = 0.f;

  while (m_windowPtr->isOpen()) {
    // Eventos
    m_windowPtr->handleEvents([&](const sf::Event& event) {
      gui.processEvent(m_windowPtr, event);
      if (event.is<sf::Event::Closed>()) {
        m_windowPtr->close();
      }
      });

    // Tiempo
    m_windowPtr->update();
    float dt = m_windowPtr->deltaTime.asSeconds();
    if (!gui.isPaused()) {
      raceTimer += dt * gui.getSpeedMultiplier();
    }

    // Actualizar racers
    for (auto& racer : m_racers) {
      if (!racer) continue;

      if (!gui.isPaused()) {
        racer->update(dt * gui.getSpeedMultiplier());
      }

      if (racer->getPlace() == 0) {
        if (auto xf = racer->getComponent<Transform>()) {
          if (crossedFinishLine(xf->getPosition(), m_finishLine)) {
            int nextPlace = static_cast<int>(m_finishedOrder.size()) + 1;
            racer->setPlace(nextPlace);
            m_finishedOrder.push_back(racer);
          }
        }
      }
    }

    // Reset global si se pidió
    if (gui.shouldResetWaypoints()) {
      for (auto& racer : m_racers) {
        if (racer) racer->reset();
      }
      m_finishedOrder.clear();
      raceTimer = 0.f;
    }

    // Pasar estado a GUI
    gui.setRacers(m_racers);
    gui.update(m_windowPtr, m_windowPtr->deltaTime);
    if (gui.shouldQuit()) {
      m_windowPtr->close();
    }

    // Render
    m_windowPtr->clear(sf::Color::Black);

    if (!m_trackActor.isNull()) {
      m_trackActor->render(m_windowPtr);
    }

    for (auto& racer : m_racers) {
      if (racer)
        racer->render(m_windowPtr);
    }

    gui.render(m_windowPtr);
    m_windowPtr->display();
  }

  destroy();
  return 0;
}

bool BaseApp::init() {
  // Crear ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "VectonautaEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window", "");
    return false;
  }

  // Inicializar GUI
  gui.init(m_windowPtr);

  // Cargar pista
  if (!resourceMan.loadTexture("Sprites/Track", "png")) {
    MESSAGE("BaseApp", "init", "Cannot load Track.png");
  }
  auto trackTex = resourceMan.getTexture("Sprites/Track");
  if (trackTex.isNull()) {
    ERROR("BaseApp", "init", "Track texture null", "");
    return false;
  }

  m_trackActor = EngineUtilities::MakeShared<Actor>("Track");
  if (auto shape = m_trackActor->getComponent<CShape>()) {
    shape->createShape(ShapeType::RECTANGLE);
    shape->setFillColor(sf::Color::White);

    auto texSize = trackTex->getTexture().getSize();
    if (auto rect = dynamic_cast<sf::RectangleShape*>(shape->getShape())) {
      rect->setSize({ static_cast<float>(texSize.x), static_cast<float>(texSize.y) });
      rect->setOrigin({ 0.f, 0.f });
    }

    float scaleX = 1920.f / static_cast<float>(texSize.x);
    float scaleY = 1080.f / static_cast<float>(texSize.y);
    shape->setScale({ scaleX, scaleY });
  }
  else {
    ERROR("BaseApp", "init", "Failed to get CShape from track actor", "");
    return false;
  }
  m_trackActor->setTexture(trackTex);
  if (auto xf = m_trackActor->getComponent<Transform>()) {
    xf->setPosition({ 0.f, 0.f });
  }

  // Definir ruta (ajústala a tu pista real)
  m_path = {
    {100.f, 150.f}, {300.f, 140.f}, {500.f, 160.f}, {700.f, 300.f},
    {900.f, 280.f}, {1100.f, 500.f}, {1300.f, 480.f}, {1500.f, 450.f}
  };

  // Crear racers
  m_racers.clear();
  auto racer1 = EngineUtilities::MakeShared<A_Racer>("Racer 1", 1);
  auto racer2 = EngineUtilities::MakeShared<A_Racer>("Racer 2", 2);

  racer1->setPath(m_path);
  racer2->setPath(m_path);

  if (auto xf1 = racer1->getComponent<Transform>()) xf1->setPosition(m_path[0]);
  if (auto xf2 = racer2->getComponent<Transform>()) xf2->setPosition(m_path[0] + sf::Vector2f(0.f, 30.f));

  if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
    MESSAGE("BaseApp", "init", "Cannot load Mario.png");
  }
  auto marioTex = resourceMan.getTexture("Sprites/Mario");
  if (!marioTex.isNull()) {
    racer1->setTexture(marioTex);
    racer2->setTexture(marioTex);
  }

  m_racers.push_back(racer1);
  m_racers.push_back(racer2);

  // Línea de meta
  m_finishLine:(1800.f, 500.f, 50.f, 200.f);

  // Estado inicial GUI
  gui.setRacers(m_racers);
  return true;
}

void BaseApp::update() {
  // Lógica ya en run()
}

void BaseApp::render() {
  // Ya integrado
}

void BaseApp::destroy() {
  // Limpieza adicional si aplica
}
