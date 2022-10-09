#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  enum class GameState { Play, Win, Lose };
  GameState m_gameState;
  ImFont *m_font{};

  char* words[3] = {"cachorro", "gato", "flamingo"};
  char* m_word = new char[100];
  bool* m_revealed = new bool[100];
  int maxError{5};

  char* guessedChars = new char[100];

  void checkEndCondition();
  void restartGame();

public:
  char* getWord() {
    return m_word;
  }

  bool* getRevealed() {
    return m_revealed;
  }

  void resetWord() {
    int randIndex = rand() % (sizeof(words) / sizeof(char*));
    char* selectedWord = words[randIndex];

    m_word = selectedWord;

    maxError = 5;

    resetRevelead();
    resetGuessed();
  }

  void resetGuessed() {
    for (size_t i = 0; i < strlen(guessedChars); i++) {
      guessedChars[i] = '\0';
    }
  }

  void resetRevelead() {
    for (size_t i = 0; i < strlen(m_word); i++) {
      m_revealed[i] = false;
    }
  }

  bool makeGuess(char guess) {
    bool anyHits = false;

    for (size_t i = 0; i < strlen(guessedChars); i++) {
      if (guessedChars[i] == guess) {
        return false;
      } else if (guessedChars[i] == '\0') {
        guessedChars[i] = guess;
        break;
      }
    }

    for (size_t i = 0; i < strlen(m_word); i++) {
      if (m_word[i] == guess) {
        m_revealed[i] = true;
        anyHits = true;
      }
    }

    if (!anyHits)
      maxError -= 1;

    return true;
  }
};

#endif
