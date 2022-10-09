#include "window.hpp"
#include <string>

void Window::onCreate() {
  auto const filename{abcg::Application::getAssetsPath() + "Roboto-Medium.ttf"};

  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};

  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 32.0f);

  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  restartGame("Fácil");
}

void Window::restartGame(string difficult) {
  resetWord(difficult);
  m_gameState = GameState::Play;
}

void Window::onPaintUI() {
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse};

    ImGui::Begin("Forca", nullptr, flags);

    {
      std::string text;
      auto color = IM_COL32(255, 255, 255, 255);
      switch (m_gameState) {
      case GameState::Play:
        text = fmt::format("[{}] Tente adivinhar a seguinte palavra:", getCurrentDifficult());
        break;
      case GameState::Lose:
        color = IM_COL32(255, 0, 0, 255);
        text = "Você perdeu :(";
        break;
      case GameState::Win:
        color = IM_COL32(0, 255, 0, 255);
        text = "Parabéns :)";
        break;
      }
      // Center text
      ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::PopStyleColor();

      ImGui::Spacing();
    }

    ImGui::Spacing();

    {
      ImGui::PushFont(m_font);

      for (size_t i = 0; i < strlen(getWord()); i++) {
        auto ch{getWord()[i]};

        if (ch != 0) {
          if (!getRevealed()[i]) {
            ch = ' ';
          }

          auto buttonText{fmt::format("{}##{}", ch, 0)};

          ImGui::Button(buttonText.c_str(), ImVec2(40, 40));
          ImGui::SameLine();
        }
      }

      ImGui::PopFont();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (m_gameState == GameState::Play) {
      char guess = '\0';

      {
        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
          ImGui::SetKeyboardFocusHere(0);
        if (ImGui::InputText("##Guess", &guess, 2, ImGuiInputTextFlags_EnterReturnsTrue)) {
          makeGuess(guess);
          checkEndCondition();
          guess = '\0';
        }

        if (m_gameState == GameState::Play) {
          std::string tryText = "Palpites errados restantes: " + std::to_string(maxError);
          ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(tryText.c_str()).x) / 2);
          ImGui::Text("%s", tryText.c_str());
        }

        if (getErrorGuess()) {
          ImGui::Spacing();
          ImGui::Spacing();
          ImGui::Spacing();

          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
          std::string text = "Palpite errado!";
          ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
          ImGui::Text("%s", text.c_str());
          ImGui::PopStyleColor();
        }


        if (getRepeatedGuess()) {
          ImGui::Spacing();
          ImGui::Spacing();
          ImGui::Spacing();

          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
          std::string text = "Palpite repetido!";
          ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
          ImGui::Text("%s", text.c_str());
          ImGui::PopStyleColor();
        }


        if (!isalpha(getCurrentGuess()) && getCurrentGuess() != '\0') {
          ImGui::Spacing();
          ImGui::Spacing();
          ImGui::Spacing();

          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
          std::string text = "O palpite deve ser uma letra!";
          ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
          ImGui::Text("%s", text.c_str());
          ImGui::PopStyleColor();
        }
      }
    }


    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    std::string text = "Começar uma nova partida:";
    ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
    ImGui::Text("%s", text.c_str());

    if (ImGui::Button("Fácil", ImVec2((appWindowWidth / 3) - 5, 30))) {
      restartGame("Fácil");
    }
    ImGui::SameLine();
    if (ImGui::Button("Médio", ImVec2((appWindowWidth / 3) - 5, 30))) {
      restartGame("Médio");
    }
    ImGui::SameLine();
    if (ImGui::Button("Difícil", ImVec2((appWindowWidth / 3) - 5, 30))) {
      restartGame("Difícil");
    }
    ImGui::SameLine();

    ImGui::Spacing();

    ImGui::End();
  }
}

void Window::checkEndCondition() {
  if (m_gameState != GameState::Play) {
    return;
  }

  bool winGame{true};

  for (size_t i = 0; i < strlen(m_word); i++) {
    if (m_revealed[i] == false) {
      winGame = false;
    }
  }

  if (winGame) {
    m_gameState = GameState::Win;
  } else if (maxError <= 0) {
    m_gameState = GameState::Lose;
  }

  return;
}
