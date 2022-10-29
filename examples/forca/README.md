## Forca

**Nome: Gabriel Murakami Alves**

**RA: 11071916**

Aplicação em WebAssembly:
https://gabriel-murakami.github.io/atividades-comp-grafica/forca/forca

Essa aplicação utilizando a biblioteca ABCg simula o funcionamento de um jogo da forca.

- Ao iniciar o jogo escolhe uma palavra previamente definida na difuculdade fácil e monta a sua representação na tela.
- O jogador deve então utilizar a caixa de texto para entrar com o seu palpite, sendo que o mesmo possui uma
quantidade limitada de tentativas (erros).
- O jogador pode também clicar para recomeçar o jogo a qualquer momento.
- Os palpites devem ser dados digitando a letra escolhida e dando Enter. O foco sempre estará no campo de Input.
- O campo só irá aceitar letras, dando uma mensagem de erro caso contrário.

Ao recomeçar o jogo, o jogador pode escolher entre três difuculdades disponíveis. A variação de cada difuculdade
diz respeito a quantidade de letras que a palavra a ser adivinhada possui. Sendo as variações contendo 6, 8 e 10 letras.

O jogo termina ao acertar a palavra ou gastar todos os palpites disponíveis.

## Código (Lógica)

Ao iniciar o programa é selecionado um array de palavras dependendo da dificuldade, e sorteado uma dentre elas.
Ao sortear a palavra é criado um array de booleanos com exatamente a mesma quantidade de caracteres da palavra com todos os seus elementos como false.
Ao renderizar na tela a palabra a ser acertada, a palavra é iterada e é feito uma validação se, dada uma posição na palavra, a posição
correspondente no array de booleanos (array de revelação) é true ou false. Se for false quer dizer que a letra ainda não foi descoberta.

Quando o jogador fizer um palpite, o mesmo passa por algumas validações:
- Primeiro o palpite é normalizado para minúscula
- É verificado se o mesmo não é algo além de uma letra
- Então, a palavra é iterada para encontrar se há correspondência de letra. Se houver, a posição no array de revelação é setado com true
- Caso contrário, o limite máximo de erros é decrementado e o jogo continua

A cada palpite é rodado a função de verificação do estado do jogo. Onde ele confere ou se a palavra está completamente revelada (vitória), ou se
o número máximo de erros chegou a zero (derrota). Se nenhum dos dois for satisfeito o jogo continua.

Os botões de dificuldade chamam o método de redefinição passando uma string que define qual o array de palavras que será usado no sorteio, fazedo assim
a diferenciação da dificuldade atual do jogo.

![image](https://user-images.githubusercontent.com/56936297/194776371-31e03dd3-afc8-44e9-b750-54fe2f87bac2.png)
