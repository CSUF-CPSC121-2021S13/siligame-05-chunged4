#include <iostream>
#include <memory>

#include "cpputils/graphics/image.h"

#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

class GameElement {
 public:
  //  Constructors
  GameElement();
  GameElement(int x, int y, int width, int height);
  //  Getters/Setters
  int GetX() const;
  void SetX(int x);
  int GetY() const;
  void SetY(int y);
  int GetWidth() const;
  int GetHeight() const;
  bool GetIsActive() const;
  void SetIsActive(bool isActive);

  //   Member Functions
  virtual void Draw(graphics::Image &image) = 0;
  virtual void Move(const graphics::Image &image) = 0;
  bool IntersectsWith(GameElement *gameElem_ptr);
  bool IsOutOfBounds(const graphics::Image &image);

 private:
  int x_;
  int y_;
  int kWidth_;
  int kHeight_;
  bool is_active_;
};

#endif