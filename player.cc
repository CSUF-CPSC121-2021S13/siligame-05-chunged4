#include "player.h"

// Player constructors
Player::Player() : Player(0, 0) {}
Player::Player(int x, int y) : GameElement(x, y, 50, 50), drawToggle_(1) {}

// getters/setters
int Player::GetToggle() const { return drawToggle_; }
void Player::SetToggle(int drawToggle) { drawToggle_ = drawToggle; }

// Player member functions
void Player::Draw(graphics::Image &image) {
  graphics::Image pImage(GetWidth(), GetHeight());
  pImage.Load("pikachu.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = 0; i < pImage.GetWidth(); i++) {
    for (int j = 0; j < pImage.GetHeight(); j++) {
      if (pImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + i, GetY() + j, pImage.GetColor(i, j));
      }
    }
  }
}
void Player::DrawBackwords(graphics::Image &image) {
  graphics::Image pImage(GetWidth(), GetHeight());
  pImage.Load("pikachu.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = pImage.GetWidth() - 1; i >= 0; i--) {
    for (int j = pImage.GetHeight() - 1; j >= 0; j--) {
      if (pImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + (GetWidth() - i), GetY() + j, pImage.GetColor(i, j));
      }
    }
  }
}
void Player::Move(const graphics::Image &image) {}

// PlayerProjectile constructors
PlayerProjectile::PlayerProjectile() : PlayerProjectile(0, 0) {}
PlayerProjectile::PlayerProjectile(int x, int y) : GameElement(x, y, 10, 11) {}

// PlayerProjectile member functions
void PlayerProjectile::Draw(graphics::Image &image) {
  graphics::Image ppImage(GetWidth(), GetHeight());
  ppImage.Load("thunderbolt.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = 0; i < ppImage.GetWidth(); i++) {
    for (int j = 0; j < ppImage.GetHeight(); j++) {
      if (ppImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + i, GetY() + j, ppImage.GetColor(i, j));
      }
    }
  }
}
void PlayerProjectile::Move(const graphics::Image &image) {
  SetY(GetY() - 3);
  if (IsOutOfBounds(image)) {
    SetIsActive(false);
  }
}
