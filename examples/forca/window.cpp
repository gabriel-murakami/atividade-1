#include "window.hpp"
#include <string>

void Window::onCreate() {
  auto const filename{abcg::Application::getAssetsPath() + "Roboto-Medium.ttf"};

  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 32.0f);

  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  restartGame();
}

void Window::restartGame() {
  // m_board.fill('\0');
  resetWord();
  m_gameState = GameState::Play;
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Forca" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse};

    ImGui::Begin("Forca", nullptr, flags);

    // TODO: Add static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = fmt::format("Escolha uma letra");
        break;
      case GameState::Lose:
        text = "Você perdeu!";
        break;
      case GameState::Win:
        text = "Parabéns!";
        break;
      }
      // Center text
      ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());

      if (m_gameState == GameState::Play) {
        std::string tryText = "Tentativas restantes: " + std::to_string(maxError);
        ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(tryText.c_str()).x) / 2);
        ImGui::Text("%s", tryText.c_str());
      }

      ImGui::Spacing();
    }

    ImGui::Spacing();
    // TODO: Add game board

    {
      // Use custom font
      ImGui::PushFont(m_font);

      for (int i = 0; i < strlen(getWord()); i++) {
        auto ch{getWord()[i]};

        if (ch != 0) {
          if (!getRevealed()[i]) {
            ch = ' ';
          }

          auto buttonText{fmt::format("{}##{}", ch, 0)};

          ImGui::Button(buttonText.c_str(), ImVec2(50, 50));
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
      bool successGuess = true;

      // ImGui::SetKeyboardFocusHere(0);
      {
        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
          ImGui::SetKeyboardFocusHere(0);
        if (ImGui::InputText("##Guess", &guess, 2, ImGuiInputTextFlags_EnterReturnsTrue)) {
          successGuess = makeGuess(guess);
          checkEndCondition();
          guess = '\0';
        }
        ImGui::SameLine();
        // if (ImGui::Button("Enviar")) {
        //   makeGuess(guess);
        //   checkEndCondition();
        //   guess = ' ';
        // }

        if (!successGuess) {
          ImGui::Spacing();
          ImGui::Spacing();
          ImGui::Spacing();

          std::string text = "Letra repetida!! Tente novamente";
          ImGui::SetCursorPosX((appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
          ImGui::Text("%s", text.c_str());
        }
      }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Começar uma nova partida", ImVec2(-1, 30))) {
      restartGame();
    }

    ImGui::Spacing();

    ImGui::End();
  }
}

void Window::checkEndCondition() {
  if (m_gameState != GameState::Play) {
    return;
  }

  bool winGame{true};

  for (int i = 0; i < strlen(m_word); i++) {
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
