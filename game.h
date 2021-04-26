#include <vector>

#include "cpputils/graphics/image.h"
#include "cpputils/graphics/image_event.h"
#include "opponent.h"
#include "player.h"

const graphics::Color lightBlue(173, 216, 230);

#ifndef GAME_H
#define GAME_H

class Game : public graphics::AnimationEventListener,
             public graphics::MouseEventListener {
 public:
  //  Constructors
  Game();
  Game(int width, int height);

  //  Getters/Setters
  graphics::Image &GetGameScreen();
  std::vector<Opponent> &GetOpponents();
  std::vector<OpponentProjectile> &GetOpponentProjectiles();
  std::vector<PlayerProjectile> &GetPlayerProjectiles();
  Player &GetPlayer();

  //  Member Functions
  void CreateOpponents();
  void CreateOpponentProjectiles();
  void CreatePlayerProjectiles();
  void Init();
  void UpdateScreen();
  void Start();
  void MoveGameElements();
  void FilterIntersections();
  void OnAnimationStep() override;
  void OnMouseEvent(const graphics::MouseEvent &event) override;

 private:
  graphics::Image gameScreen_;
  std::vector<Opponent> enemies_;
  std::vector<OpponentProjectile> balls_;
  std::vector<PlayerProjectile> lBolts_;
  Player thePlayer_;
};

#endif