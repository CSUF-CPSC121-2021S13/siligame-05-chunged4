#include "game.h"

// constructors
Game::Game() : Game(800, 600) {}
Game::Game(int width, int height) { gameScreen_.Initialize(width, height); }

// getters/setters
graphics::Image &Game::GetGameScreen() { return gameScreen_; }
std::vector<Opponent> &Game::GetOpponents() { return enemies_; }
std::vector<OpponentProjectile> &Game::GetOpponentProjectiles() {
  return balls_;
}
std::vector<PlayerProjectile> &Game::GetPlayerProjectiles() { return lBolts_; }
Player &Game::GetPlayer() { return thePlayer_; }

// Member Functions
void Game::CreateOpponents() {
  Opponent opponent;
  int xPos = rand() % (gameScreen_.GetWidth() - opponent.GetWidth()) +
             opponent.GetWidth();
  int yPos = rand() % ((gameScreen_.GetHeight() / 2) - opponent.GetHeight()) +
             opponent.GetHeight();
  opponent.SetX(xPos);
  opponent.SetY(yPos - opponent.GetHeight());
  GetOpponents().push_back(opponent);
}
void Game::CreateOpponentProjectiles() {
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i].GetIsActive()) {
      OpponentProjectile ball(enemies_[i].GetWidth() / 2 + enemies_[i].GetX(),
                              enemies_[i].GetHeight() + enemies_[i].GetY());
      GetOpponentProjectiles().push_back(ball);
    }
  }
}
void Game::CreatePlayerProjectiles() {
  PlayerProjectile bolt(thePlayer_.GetWidth() / 2 + thePlayer_.GetX(),
  thePlayer_.GetY());
  GetPlayerProjectiles().push_back(bolt);
}
void Game::Init() {
  int xPos = gameScreen_.GetWidth() / 2;
  int yPos = gameScreen_.GetWidth() * .75;
  thePlayer_.SetX(xPos);
  thePlayer_.SetY(yPos);
  CreateOpponents();
  CreateOpponentProjectiles();
  CreatePlayerProjectiles();
  gameScreen_.AddMouseEventListener(*this);
  gameScreen_.AddAnimationEventListener(*this);
}
void Game::UpdateScreen() {
  gameScreen_.DrawRectangle(0, 0, gameScreen_.GetWidth(),
                            gameScreen_.GetHeight(), lightBlue);
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i].GetIsActive()) {
      enemies_[i].Draw(gameScreen_);
    }
  }
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i].GetIsActive()) {
      balls_[i].Draw(gameScreen_);
    }
  }
  for (int i = 0; i < lBolts_.size(); i++) {
    if (lBolts_[i].GetIsActive()) {
      lBolts_[i].Draw(gameScreen_);
    }
  }
  if (thePlayer_.GetIsActive() && thePlayer_.GetToggle() == 1) {
    thePlayer_.Draw(gameScreen_);
  } else if (thePlayer_.GetIsActive() && thePlayer_.GetToggle() == 2) {
    thePlayer_.DrawBackwords(gameScreen_);
  }
}
void Game::Start() { gameScreen_.ShowUntilClosed(); }
void Game::MoveGameElements() {
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i].GetIsActive()) {
      enemies_[i].Move(gameScreen_);
    }
  }
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i].GetIsActive()) {
      balls_[i].Move(gameScreen_);
    }
  }
  for (int i = 0; i < lBolts_.size(); i++) {
    if (lBolts_[i].GetIsActive()) {
      lBolts_[i].Move(gameScreen_);
    }
  }
  if (thePlayer_.GetIsActive()) {
    thePlayer_.Move(gameScreen_);
  }
}
void Game::FilterIntersections() {
  // player vs opponent intersections
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i].IntersectsWith(&thePlayer_)) {
      enemies_[i].SetIsActive(false);
      thePlayer_.SetIsActive(false);
    }
  }
  // player vs opponentprojectile intersections
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i].IntersectsWith(&thePlayer_)) {
      balls_[i].SetIsActive(false);
      thePlayer_.SetIsActive(false);
    }
  }
  // playerprojectile vs opponent intersections
  for (int i = 0; i < lBolts_.size(); i++) {
    for (int j = 0; j < enemies_.size(); j++) {
      if (lBolts_[i].IntersectsWith(&enemies_[j])) {
        lBolts_[i].SetIsActive(false);
        enemies_[j].SetIsActive(false);
      }
    }
  }
}
void Game::OnAnimationStep() {
  MoveGameElements();
  FilterIntersections();
  UpdateScreen();
  gameScreen_.Flush();
}
void Game::OnMouseEvent(const graphics::MouseEvent &event) {
  if (event.GetMouseAction() == graphics::MouseAction::kMoved ||
      event.GetMouseAction() == graphics::MouseAction::kDragged) {
    if (event.GetX() - (thePlayer_.GetX() + thePlayer_.GetWidth() / 2) > 0) {
      thePlayer_.SetToggle(2);
    } else if (event.GetX() - (thePlayer_.GetX() + thePlayer_.GetWidth() / 2) < 0) {
      thePlayer_.SetToggle(1);
    }
    if (event.GetX() > 0 || event.GetX() < gameScreen_.GetWidth() ||
        event.GetY() > 0 || event.GetY() < gameScreen_.GetHeight()) {
      thePlayer_.SetX(event.GetX() - (thePlayer_.GetWidth() / 2));
      thePlayer_.SetY(event.GetY() - (thePlayer_.GetHeight() / 2));
      if (thePlayer_.IsOutOfBounds(gameScreen_)) {
        if (thePlayer_.GetX() < 0) {
          thePlayer_.SetX(event.GetX());
        }
        if (thePlayer_.GetY() < 0) {
          thePlayer_.SetY(event.GetY());
        }
        if (thePlayer_.GetX() + thePlayer_.GetWidth() >
            gameScreen_.GetWidth()) {
          thePlayer_.SetX(event.GetX() - thePlayer_.GetWidth());
        }
        if (thePlayer_.GetY() + thePlayer_.GetHeight() >
            gameScreen_.GetHeight()) {
          thePlayer_.SetY(event.GetY() - thePlayer_.GetHeight());
        }
      }
    }
  }
  if ((event.GetMouseAction() == graphics::MouseAction::kPressed &&
       event.GetMouseAction() != graphics::MouseAction::kReleased) ||
       event.GetMouseAction() == graphics::MouseAction::kDragged) {
    if (thePlayer_.GetIsActive()) {
      CreatePlayerProjectiles();
    }
  }
}
