<h1>Explicando o arquivo posto.c 1</h1>	
O arquivo posto.c implementa toda a parte de programacao paralela em si do trabalho.<br>
A versao atual do codigo contem muitos comentarios para explicar todo o seu funcionamento, mas podemos retira-los no futuro.<br>
<br>
<h2>Impressao de coisas no terminal</h2>	
Para ilustrar o que esta acontecendo, é so mudar as partes do codigo em que a funcao imprimeCineminha() é chamada.<br>
A definição dela que está lá é completamente provisória e retirada de um dos codigos da professor. Ela serve pra ver o que esta acontecendo mas obviamente não é a que vamos usar.<br>
Vamos deletar toda aquela definição e substituir pelo que nos interessa.<br>
Não precisamos substituir por uma função só, podemos usar várias, mas deixei assim so pra ndicar os pontos de impressao mesmo.<br>
Em cada chamada desta funcao existe um comentario ao lado indicando o que de interessante acabou de acontecer para demandar uma nova impressão.<br>
Se houverem mais coisas podemos adicionar novas chamadas, se alguma delas nao interessar podemos retirar, não tem problema.<br>
**Nota importante:** Como todas as chamadas da funcao imprimeCineminha() estao dentro de blocos de controle de estado (sem_wait(&sem_estados) -- AQUI -- sem_post(&sem_estados)) apenas uma versao delas é chamada de cada vez.<br>

<h2>Coisas no codigo que carregam informacoes importantes para impressao</h2>
O vetor estado_vagas[N_VAGAS_ESTAC] indica se cada vaga esta B ou F (Busy ou Free).<br>
O vetor conteudo_vagas[N_VAGAS_ESTAC] indica qual carro esta em qual vaga, caso tenha alguem la, mas para saber se tem alguem precisamos olhar estado_vagas[N_VAGAS_ESTAC].<br>
<br>
O vetor estado_bombas[N_BOMBAS]; indica se cada bomba esta BAB ou D (Bomba ABastecendo ou Desocupada).<br>
O vetor carros_em_bombas[N_BOMBAS] indica qual carro esta em qual bomba, caso tenha alguem la, mas para saber se tem alguem precisamos olhar estado_bombas[N_VAGAS_ESTAC].<br>
<br>
O vetor estado_carros[N_CARROS] indica o que cada carro esta fazendo. Aqui estão todas as opcoes:<br>
A - arrived           (carro);  chegou ao posto, mas nao necessariamente entrou em uma vaga<br>
CAB - abastecendo  (carro); carro abastecendo<br>
E - exit  		 (carro); saindo do posto<br>
I - idle          (carro); nao chegou ao posto ainda<br>
L - left    		 (carro); ja foi embora do posto completamente<br>
W - waiting		 (carro); esta na vaga esperando uma bomba<br>
<br>
Imagino que com tudo isso de pra imprimir tudo o que a gente quer.
