# Problema do Posto

# Descrição 
Este trabalho visa ilustrar a programação multithread e mostrar o funcionamento de um código multithread em tempo real.<br>
Para isso elaboramos o problema de um posto de gasolina, que funciona de maneira muito semelhante ao problema com múltiplos barbeiros dorminhocos mostrado em sala.<br>
Neste posto de gasolina, existem 4 bombas de gasolina e 4 bombas de espera em que os carros para para esperar uma bomba ficar vazia. <br>
Para manter um funcionamento mais justo, quando um carro novo chega ao posto, ele não tenta ir direto a uma bomba, mas sim procura se tem uma vaga de espera livre. Caso não tenha, ele vai embora do posto sem abastecer. <br>
Ao chegar a uma vaga de espera, em determinado momento o carro passa a desejar abastecer. Quando isso ocorre, ele espera uma as bombas ficar vazia e vai até ela. <br>
Após abastecer, o carro libera a bomba e vai embora do posto. <br>
Todo esse controle de sincronização e comunicação entre threads é feito por múltiplos semáforos. <br>

![Imagem Posto](https://github.com/XaviRCX/MC504/blob/main/Projeto1/Imagens/Imagem%20Posto.png)

# Equipe
* Filipe Franco Ferreira - 251027
* Mateus de Brito Guirardello - 241540
* Lucas Xavier - 182278


# Estrutura de Arquivos e Pastas

~~~
├─── README.md                                         <- Apresentação do projeto
│
├─── Imagens           
|     ├── Imagem Posto.png    <- Imagem mostrada acima
|
├─── posto.c         <- Arquivo principal com multithread que chama as animações
|
├─── screen.h                        <- Implementação das animações no terminal
~~~

# Link para os slides usados na apresentação

https://docs.google.com/presentation/d/1BZFTuBNSvu41Y8FaGe5mSQjCI6GCowWOGy4tkswAttU/edit?pli=1#slide=id.p 

# Link do vídeo da apresentação
