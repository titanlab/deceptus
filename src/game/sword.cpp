#include "sword.h"

#include <iostream>

#include "constants.h"
#include "debugdraw.h"
#include "framework/tools/stopwatch.h"
#include "player/player.h"
#include "worldquery.h"

namespace
{
constexpr auto sword_damage = 10;
}

using namespace std::chrono_literals;

Sword::Sword() : _duration_from_swing_start_to_hit(200ms), _duration_from_hit_start_to_end(120ms)
{
   _type = WeaponType::Sword;
}

void Sword::draw(sf::RenderTarget& target [[maybe_unused]])
{
   if (!checkHitWindowActive())
   {
      return;
   }

   // DebugDraw::drawRect(target, _hit_rect_px, sf::Color{255, 0, 0});
}

void Sword::update(const sf::Time& /*time*/)
{
   if (!_cleared_to_attack)
   {
      return;
   }

   if (checkHitWindowActive())
   {
      _cleared_to_attack = false;

      updateHitbox();

      // this ought to go to a separate class for hit/damage management
      auto hit_nodes = WorldQuery::findNodes(_hit_rect_px);
      for (auto& node : hit_nodes)
      {
         std::cout << "hit: " << node->_script_name << " " << node->_id << std::endl;
         node->luaHit(sword_damage);
      }
   }
}

void Sword::use(const std::shared_ptr<b2World>& /*world*/, const b2Vec2& dir)
{
   _cleared_to_attack = true;
   _timepoint_swing_start = StopWatch::getInstance().now();
   _dir_m = dir;
}

bool Sword::checkHitWindowActive() const
{
   const auto now = StopWatch::getInstance().now();
   const auto start = _timepoint_swing_start + _duration_from_swing_start_to_hit;
   const auto end = _timepoint_swing_start + _duration_from_swing_start_to_hit + _duration_from_hit_start_to_end;
   const auto within_active_time_window = (now >= start && now <= end);
   return within_active_time_window;
}

void Sword::updateHitbox()
{
   constexpr auto hitbox_width_px = 30.0f;
   constexpr auto hitbox_height_px = 20.0f;
   auto player = Player::getCurrent();
   const auto center = player->getPixelPositionFloat();
   const auto crouching = player->getBend().isCrouching();

   const auto hitbox_pos = sf::Vector2f{
      center.x + 0.2f * PPM * _dir_m.x + ((_dir_m.x < 0.0f) ? -hitbox_width_px : 0.0f),
      center.y + (crouching ? -0.1f * PPM : -0.6f * PPM),
   };

   _hit_rect_px = sf::FloatRect(hitbox_pos, sf::Vector2f(hitbox_width_px, hitbox_height_px));
}
