Você deve escrever programas em C que implementam uma comunicação ponto a ponto entre dois nós, empregando organização de camadas e protocolos. O enlace (meio físico) dessa conexão simulada será a saída padrão.

Nessa parte do trabalho, você deve implementar o transmissor de mensagens.

A mensagem enviada tem formato string e deve ser lida pelo programa transmissor pela entrada padrão (scanf). Tal informação possui até 80 caracteres (sem acentos), e pode possuir espaços em branco\*. A mensagem sempre é encerrada com uma quebra de linha (caractere '\n').

A pilha de protocolos empregada em tal comunicação possui três camadas. A camada em nível mais alto, recebe mensagens do usuário no formato string. A camada intermediária recebe as mensagens lidas pela camada superior, conta seus caracteres, converte-as em números inteiros, caractere a caractere e repassa tanto o tamanho quanto a representação decimal da mensagem original. A camada inferior, recebe os números inteiros em representação decimal da camada intermediária (inclusive o tamanho) e converte-os em números binários para transmissão.

Por fim, a mensagem original, codificada em representação binária deve ser exibida na saída padrão (printf), para representar a transmissão.

Cada protocolo deve ser implementado como uma função distinta. Apenas o resultado do protocolo de nível mais baixo deve ser exibido por scanf. As únicas bibliotecas permitidas para a resolução dessa parte do trabalho são: stdio.h, stdlib.h e string.h.

O não atendimento a qualquer regra do trabalho pode acarretar em desconto(s) na nota. Os trabalhos entregues serão comparados de modo computadorizado e também manualmente para verificação de plágio/cópia.

Exemplo de mensagem:
Tres tigres tristes.

Em decimal:
84 114 101 115 32 116 105 103 114 101 115 32 116 114 105 115 116 101 115 46

Em binário:
1010100 1110010 1100101 1110011 100000 1110100 1101001 1100111 1110010 1100101 1110011 100000 1110100 1110010 1101001 1110011 1110100 1100101 1110011 101110
