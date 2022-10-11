#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
using namespace std;

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  enum class GameState { Play, Win, Lose };
  GameState m_gameState;
  ImFont *m_font{};

  char* easy_words[5] = {"adorno", "casual", "hostil", "isento", "exceto"};
  char* medium_words[5] = {"eminente", "reiterar", "alienado", "complexo", "prudente"};
  char* hard_words[5] = {"prescindir", "diligencia", "intemperie", "corroborar", "incipiente"};

  char* m_word = new char[100];
  bool* m_revealed = new bool[100];
  int maxError{5};

  char* guessedChars = new char[100];

  std::string guessed = "";

  char currentGuess = '\0';
  char repeatedGuess = false;
  char errorGuess = false;
  string currentDificult = "Fácil";

  void checkEndCondition();
  void restartGame(string difficult);

public:
  char* getWord() {
    return m_word;
  }

  char* getGuessed() {
    return guessedChars;
  }

  string getGuessedString() {
    return guessed;
  }

  bool* getRevealed() {
    return m_revealed;
  }

  void setGuessed(int i, char value) {
    guessedChars[i] = value;
  }

  char getCurrentGuess() {
    return currentGuess;
  }

  char getRepeatedGuess() {
    return repeatedGuess;
  }

  char getErrorGuess() {
    return errorGuess;
  }

  string getCurrentDifficult() {
    return currentDificult;
  }

  void resetWord(string difficult = "Fácil") {
    char* selectedWord;

    currentDificult = difficult;

    if (difficult == "Fácil") {
      int randIndex = rand() % (sizeof(easy_words) / sizeof(char*));
      selectedWord = easy_words[randIndex];
    } else if (difficult == "Médio") {
      int randIndex = rand() % (sizeof(medium_words) / sizeof(char*));
      selectedWord = medium_words[randIndex];
    } else {
      int randIndex = rand() % (sizeof(hard_words) / sizeof(char*));
      selectedWord = hard_words[randIndex];
    }

    m_word = selectedWord;

    maxError = 5;
    currentGuess = '\0';
    repeatedGuess = false;
    errorGuess = false;
    guessed = "";

    resetRevelead();
  }

  void resetRevelead() {
    for (size_t i = 0; i < strlen(m_word); i++) {
      m_revealed[i] = false;
    }
  }

  void makeGuess(char value) {
    bool anyHits = false;
    errorGuess = false;
    repeatedGuess = false;

    if (!isalpha(value)) {
      currentGuess = value;
      return;
    }

    char guess = tolower(value);

    currentGuess = guess;

    // for (size_t i = 0; i < strlen(getGuessed()); i++) {
    //   if (getGuessed()[i] == guess) {
    //     return;
    //   }
    // }

    guessed += toupper(guess);
    guessed += ' ';

    for (size_t i = 0; i < strlen(m_word); i++) {
      if (m_word[i] == guess) {
        if (m_revealed[i]) {
          repeatedGuess = true;
          anyHits = true;
        } else {
          m_revealed[i] = true;
          anyHits = true;
        }
      }
    }

    if (!anyHits) {
      errorGuess = true;
      maxError -= 1;
    }
  }
};

#endif
