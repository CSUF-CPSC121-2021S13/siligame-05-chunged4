#include "game.h"

// constructors
Game::Game() : Game(800, 600) {}
Game::Game(int width, int height) : score_(0) { gameScreen_.Initialize(width, height); }

// getters/setters
graphics::Image &Game::GetGameScreen() { return gameScreen_; }
std::vector<std::unique_ptr<Opponent>> &Game::GetOpponents() {
  return enemies_;
}
std::vector<std::unique_ptr<OpponentProjectile>>
    &Game::GetOpponentProjectiles() {
  return balls_;
}
std::vector<std::unique_ptr<PlayerProjectile>> &Game::GetPlayerProjectiles() {
  return lBolts_;
}
Player &Game::GetPlayer() { return thePlayer_; }
int Game::GetScore() const { return score_; }
bool Game::HasLost() const { return playing_; }

// Member Functions
void Game::CreateOpponents() {
  std::unique_ptr<Opponent> opponent = std::make_unique<Opponent>();
  int xPos = rand() % (gameScreen_.GetWidth() - opponent->GetWidth() / 2) +
                       opponent->GetWidth() / 2;
  int yPos = rand() % (gameScreen_.GetHeight() - opponent->GetHeight() / 2) +
                       opponent->GetHeight() / 2;
  opponent->SetX(xPos);
  opponent->SetY(yPos);
  GetOpponents().push_back(std::move(opponent));
}
void Game::Init() {
  int xPos = gameScreen_.GetWidth() / 2;
  int yPos = gameScreen_.GetWidth() * .75;
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i]->LaunchProjectile() != nullptr) {
      balls_.push_back(std::move(enemies_[i]->LaunchProjectile()));
    }
  }
  thePlayer_.SetX(xPos);
  thePlayer_.SetY(yPos);
  gameScreen_.AddMouseEventListener(*this);
  gameScreen_.AddAnimationEventListener(*this);
}
void Game::UpdateScreen() {
  graphics::Color black(0, 0, 0);
  if (HasLost()) {
    gameScreen_.DrawText(gameScreen_.GetWidth() / 2,
                         gameScreen_.GetHeight() / 2, "GAME OVER", 200, black);
  }
  gameScreen_.DrawRectangle(0, 0, gameScreen_.GetWidth(),
                            gameScreen_.GetHeight(), lightBlue);
  std::string scoreMsg = "Score: ";
  scoreMsg.append((char *)score_);
  gameScreen_.DrawText(0, 0, scoreMsg, 30, black);
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i]->GetIsActive() && enemies_[i]->GetToggle() == 1) {
      enemies_[i]->Draw(gameScreen_);
    } else if (enemies_[i]->GetIsActive() && enemies_[i]->GetToggle() == 2) {
      enemies_[i]->DrawBackwords(gameScreen_);
    }
  }
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i]->GetIsActive()) {
      balls_[i]->Draw(gameScreen_);
    }
  }
  for (int i = 0; i < lBolts_.size(); i++) {
    if (lBolts_[i]->GetIsActive()) {
      lBolts_[i]->Draw(gameScreen_);
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
    if (enemies_[i]->GetIsActive()) {
      enemies_[i]->Move(gameScreen_);
    }
  }
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i]->GetIsActive()) {
      balls_[i]->Move(gameScreen_);
    }
  }
  for (int i = 0; i < lBolts_.size(); i++) {
    if (lBolts_[i]->GetIsActive()) {
      lBolts_[i]->Move(gameScreen_);
    }
  }
  if (thePlayer_.GetIsActive()) {
    thePlayer_.Move(gameScreen_);
  }
}
void Game::FilterIntersections() {
  // player vs opponent intersections
  for (int i = 0; i < enemies_.size(); i++) {
    if (enemies_[i]->IntersectsWith(&thePlayer_)) {
      enemies_[i]->SetIsActive(false);
      thePlayer_.SetIsActive(false);
      playing_ = false;
    }
  }
  // player vs opponentprojectile intersections
  for (int i = 0; i < balls_.size(); i++) {
    if (balls_[i]->IntersectsWith(&thePlayer_)) {
      balls_[i]->SetIsActive(false);
      thePlayer_.SetIsActive(false);
      playing_ = false;
    }
  }
  // playerprojectile vs opponent intersections
  for (int i = 0; i < lBolts_.size(); i++) {
    for (int j = 0; j < enemies_.size(); j++) {
      if (lBolts_[i]->IntersectsWith(enemies_[j].get())) {
        lBolts_[i]->SetIsActive(false);
        enemies_[j]->SetIsActive(false);
        if (thePlayer_.GetIsActive()) {
          score_++;
          playing_ = true;
        }
      }
    }
  }
}
void Game::RemoveInactive() {
  for (int i = enemies_.size(); i > 0; i--) {
    if (!(enemies_[i]->GetIsActive())) {
      enemies_.erase(enemies_.begin() + i);
    }
  }
  for (int i = balls_.size(); i > 0; i--) {
    if (!(balls_[i]->GetIsActive())) {
      balls_.erase(balls_.begin() + i);
    }
  }
  for (int i = lBolts_.size(); i > 0; i--) {
    if (!(lBolts_[i]->GetIsActive())) {
      lBolts_.erase(lBolts_.begin() + i);
    }
  }
}
void Game::OnAnimationStep() {
  if (enemies_.size() == 0) {
    CreateOpponents();
    CreateOpponents();
    CreateOpponents();
  }
  MoveGameElements();
  for (int i = 0; i < enemies_.size(); i++) {
    enemies_[i]->LaunchProjectile();
  }
  FilterIntersections();
  // RemoveInactive();
  UpdateScreen();
  gameScreen_.Flush();
}
void Game::OnMouseEvent(const graphics::MouseEvent &event) {
  if (event.GetX() > 0 || event.GetX() < gameScreen_.GetWidth() ||
      event.GetY() > 0 || event.GetY() < gameScreen_.GetHeight()) {
    if (event.GetMouseAction() == graphics::MouseAction::kMoved ||
        event.GetMouseAction() == graphics::MouseAction::kDragged) {
      if (event.GetX() - (thePlayer_.GetX() + thePlayer_.GetWidth() / 2) > 0) {
        thePlayer_.SetToggle(2);
      } else if (event.GetX() - (thePlayer_.GetX() + thePlayer_.GetWidth() / 2) <
               0) {
        thePlayer_.SetToggle(1);
      }
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
      std::unique_ptr<PlayerProjectile> bolt =
          std::make_unique<PlayerProjectile>();
      bolt->SetX(thePlayer_.GetWidth() / 2);
      lBolts_.push_back(std::move(bolt));
    }
  }
}