#include "game_element.h"

// constructors
GameElement::GameElement()
    : x_(0), y_(0), kWidth_(50), kHeight_(50), is_active_(true) {}
GameElement::GameElement(int x, int y, int width, int height)
    : x_(x), y_(y), kWidth_(width), kHeight_(height), is_active_(true) {}
// getters/setters
int GameElement::GetX() const { return x_; }
void GameElement::SetX(int x) { x_ = x; }
int GameElement::GetY() const { return y_; }
void GameElement::SetY(int y) { y_ = y; }
int GameElement::GetWidth() const { return kWidth_; }
int GameElement::GetHeight() const { return kHeight_; }
bool GameElement::GetIsActive() const { return is_active_; }
void GameElement::SetIsActive(bool isActive) { is_active_ = isActive; }
// member functions
bool GameElement::IntersectsWith(GameElement* gameElem_ptr) {
  if (gameElem_ptr->GetX() + gameElem_ptr->GetWidth() > x_ &&
      gameElem_ptr->GetX() < x_ + kWidth_ &&
      gameElem_ptr->GetY() + gameElem_ptr->GetHeight() > y_ &&
      gameElem_ptr->GetY() < y_ + kHeight_) {
    return true;
  } else {
    return false;
  }
}
bool GameElement::IsOutOfBounds(const graphics::Image& image) {
  if (x_ < 0 || x_ + kWidth_ > image.GetWidth() || y_ < 0 ||
      y_ + kHeight_ > image.GetHeight()) {
    return true;
  } else {
    return false;
  }
}