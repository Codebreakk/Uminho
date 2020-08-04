Notas finais:
 - Alguns métodos poderiam ter sido divididos em métodos mais pequenos de forma
a tornar o código mais legivel e compreensivel;
 - Os locks implementados na classe Usuario não são eficientes devido à forma como
fazemos a execução da actualização para informar todos os usuários da nova média.
Quando alguém actualiza o seu número de infectados conhecidos, os seguintes passos
são executados:
    1) Obtenção de ReadLock da HashMap da classe Registos;
    2) Actualização do número de infectados que o Usuário conhece;
    3) Cálculo da nova média;
    4) Obtenção do lock de cada usuário (1 a 1);
    5) Escrita da mensagem para cada usuário;
    6) Libertação do lock de cada usuário (1 a 1);
Uma forma melhor de implementar os locks seria fazer lock/unlock à classe Usuário
por completo, uma vez que sempre que fazemos lock/unlock a 1 elemento desta classe
já sabemos que iremos fazer lock/unlock a todos os elementos. Da forma que ficou
implementada acaba por ser menos eficiente.
 - A implementação de locks Read/Write na classe Registos está bem feita e é uma
 forma interessante de implementar estes locks neste trabalho.
