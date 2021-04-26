#include "game_element.h"

#ifndef OPPONENT_H
#define OPPONENT_H

class Opponent : public GameElement {
 public:
  //  Constructors
  Opponent();
  Opponent(int x, int y);
  // getters/setters
  bool GetXToggle() const;
  void SetXToggle(bool xToggle);
  bool GetYToggle() const;
  void SetYToggle(bool yToggle);
  //  Member Functions
  void Draw(graphics::Image &image) override;
  void DrawBackwords(graphics::Image &image);
  void Move(const graphics::Image &image) override;
  std::unique_ptr<OpponentProjectile> LaunchProjectile();

 private:
  bool XToggle_;
  bool YToggle_;
  int shootCounter_;
};

class OpponentProjectile : public GameElement {
 public:
  //  Constructors
  OpponentProjectile();
  OpponentProjectile(int x, int y);
  //  Member Functions
  void Draw(graphics::Image &image) override;
  void Move(const graphics::Image &image) override;

 private:
};

#endif
