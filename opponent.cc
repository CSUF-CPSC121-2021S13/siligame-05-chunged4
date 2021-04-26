#include "opponent.h"

// Opponent constructors
Opponent::Opponent() : Opponent(0, 0) {}
Opponent::Opponent(int x, int y) : GameElement(x, y, 50, 46), shootCounter_(0) {}

// getters/setters
int Opponent::GetToggle() const { return drawToggle_; }
void Opponent::SetToggle(int drawToggle) { drawToggle_ = drawToggle; }
bool Opponent::GetXToggle() const { return XToggle_; }
void Opponent::SetXToggle(bool xToggle) { XToggle_ = xToggle; }
bool Opponent::GetYToggle() const { return YToggle_; }
void Opponent::SetYToggle(bool yToggle) { YToggle_ = yToggle; }

// Opponent member functions
void Opponent::Draw(graphics::Image &image) {
  graphics::Image oImage(GetWidth(), GetHeight());
  oImage.Load("ash-ketchup.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = 0; i < oImage.GetWidth(); i++) {
    for (int j = 0; j < oImage.GetHeight(); j++) {
      if (oImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + i, GetY() + j, oImage.GetColor(i, j));
      }
    }
  }
}
void Opponent::DrawBackwords(graphics::Image &image) {
  graphics::Image pImage(GetWidth(), GetHeight());
  pImage.Load("ash-ketchup.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = pImage.GetWidth() - 1; i >= 0; i--) {
    for (int j = pImage.GetHeight() - 1; j >= 0; j--) {
      if (pImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + (GetWidth() - i), GetY() + j, pImage.GetColor(i, j));
      }
    }
  }
}
void Opponent::Move(const graphics::Image &image) {
  if (GetWidth() + GetX() >= image.GetWidth() - GetWidth() / 2) {
    SetXToggle(true);
  } else if (GetX() <= GetWidth() / 2) {
    SetXToggle(false);
  }
  if (GetHeight() + GetY() >= image.GetHeight() / 2) {
    SetYToggle(true);
  } else if (GetY() <= GetHeight() / 2) {
    SetYToggle(false);
  }
  int xSpeed = 2;
  int ySpeed = 2;
  if (GetXToggle()) {
    SetX(GetX() - xSpeed);
    SetToggle(1);
  } else {
    SetX(GetX() + xSpeed);
    SetToggle(2);
  }
  if (GetYToggle()) {
    SetY(GetY() - ySpeed);
  } else {
    SetY(GetY() + ySpeed);
  }
  if (IsOutOfBounds(image)) {
    SetIsActive(false);
  }
}
std::unique_ptr<OpponentProjectile> Opponent::LaunchProjectile() {
  shootCounter_++;
  std::unique_ptr<OpponentProjectile> oProj_ptr = std::make_unique<OpponentProjectile>();
  oProj_ptr->SetX(GetWidth() / 2);
  oProj_ptr->SetY(GetHeight());
  if (shootCounter_ % 10 == 0) {
    return oProj_ptr;
  } else {
    return nullptr;
  }
}

// OpponentProjectile constructors
OpponentProjectile::OpponentProjectile() : OpponentProjectile(0, 0) {}
OpponentProjectile::OpponentProjectile(int x, int y)
    : GameElement(x, y, 12, 12) {}

// OpponentProjectile member functions
void OpponentProjectile::Draw(graphics::Image &image) {
  graphics::Image opImage(GetWidth(), GetHeight());
  opImage.Load("pokeball.bmp");
  graphics::Color green(0, 255, 0);
  for (int i = 0; i < opImage.GetWidth(); i++) {
    for (int j = 0; j < opImage.GetHeight(); j++) {
      if (opImage.GetColor(i, j) != green) {
        image.SetColor(GetX() + i, GetY() + j, opImage.GetColor(i, j));
      }
    }
  }
}
void OpponentProjectile::Move(const graphics::Image &image) {
  SetY(GetY() + 3);
  if (IsOutOfBounds(image)) {
    SetIsActive(false);
  }
}
