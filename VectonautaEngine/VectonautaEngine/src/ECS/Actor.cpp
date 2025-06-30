#include "ECS/Actor.h"

Actor::Actor(const std::string& actorName) {
  //Setup Actor Name
  m_name = actorName;

  //Setup Transform
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
}

void Actor::start()
{
}





void Actor::update(float deltaTime)
{
}

void
Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {


}

void Actor::destroy()
{
}
