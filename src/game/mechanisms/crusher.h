#pragma once

#include "constants.h"
#include "gamedeserializedata.h"
#include "gamemechanism.h"
#include "gamenode.h"

#include <array>
#include <filesystem>

#include <Box2D/Box2D.h>


struct TmxObject;

class Crusher : public GameMechanism, public GameNode
{
   public:

      enum class Mode
      {
         Interval,
         Distance
      };

      enum class State
      {
         Idle,
         Extract,
         Retract
      };

      Crusher(GameNode* parent = nullptr);

      void draw(sf::RenderTarget& color, sf::RenderTarget& normal) override;
      void update(const sf::Time& dt) override;

      void setup(const GameDeserializeData& data);


   private:

      void updateTransform();
      void setupBody(const std::shared_ptr<b2World>& world);

      void step(const sf::Time& dt);
      void updateState();
      void updateSpritePositions();

      Mode _mode = Mode::Interval;
      State _state = State::Idle;
      State _state_previous = State::Idle;
      Alignment _alignment = Alignment::PointsDown;

      b2Body* _body = nullptr;
      sf::Vector2f _pixel_position;
      sf::Vector2f _blade_offset;

      sf::Time _idle_time;
      sf::Time _extraction_time;
      sf::Time _retraction_time;

      sf::Sprite _sprite_spike;
      sf::Sprite _sprite_pusher;
      sf::Sprite _sprite_mount;
      sf::Vector2f _pixel_offset_mount;
      sf::Vector2f _pixel_offset_pusher;
      sf::Vector2f _pixel_offset_spike;

      int32_t _instance_id = 0;

      std::shared_ptr<sf::Texture> _texture;
      static int32_t __instance_counter;
};

