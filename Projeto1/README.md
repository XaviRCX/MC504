# Problema do Posto

# Descrição 
Este trabalho visa ilustrar a programação multithread e mostrar o funcionamento de um código multithread em tempo real.<br>
Para isso elaboramos o problema de um posto de gasolina, que funciona de maneira muito semelhante ao problema com múltiplos barbeiros dorminhocos mostrado em sala.<br>
Neste posto de gasolina, existem 4 bombas de gasolina e 4 vagas de espera em que os carros param para esperar uma bomba ficar vazia. <br>
Para manter um funcionamento mais justo, quando um carro novo chega ao posto, ele não tenta ir direto a uma bomba, mas sim procura se tem uma vaga de espera livre. Caso não tenha, ele vai embora do posto sem abastecer. <br>
Ao chegar a uma vaga de espera, em determinado momento o carro passa a desejar abastecer. Quando isso ocorre, ele espera uma das bombas ficar vazia e vai até ela. <br>
Após abastecer, o carro libera a bomba e vai embora do posto. <br>
Todo esse controle de sincronização e comunicação entre threads é feito por múltiplos semáforos. <br>

![Imagem Posto](https://github.com/XaviRCX/MC504/blob/main/Projeto1/Imagens/Imagem%20Posto.png)

# Equipe
* Filipe Franco Ferreira - 251027
* Mateus de Brito Guirardello - 241540
* Lucas Xavier - 182278


# Estrutura de Arquivos e Pastas

~~~
├─── Imagens           
|     ├── Imagem Posto.png    <- Imagem mostrada acima
|
├─── README.md                <- Apresentação do projeto
|
├─── posto                    <- Arquivo principal executável
|
├─── posto.c                  <- Arquivo fonte principal 
|
├─── screen.c                 <- Implementação das animações no terminal
|
├─── screen.h                 <- Header das animações no terminal
~~~

# Link para os slides usados na apresentação

https://docs.google.com/presentation/d/1BZFTuBNSvu41Y8FaGe5mSQjCI6GCowWOGy4tkswAttU/edit?pli=1#slide=id.p 

# Link do vídeo da apresentação
https://www.canva.com/design/DAFjmlBzpN4/jvcXoqN_lWBZjNVGu6VCmg/watch?utm_content=DAFjmlBzpN4&utm_campaign=celebratory_first_publish&utm_medium=link&utm_source=celebratory_first_publish
