Notas finais:

 - A funcionalidade tempo-execucao não foi implementada da forma especificada;
 - A funcionalidade adicional não foi implementada;
 - O relatório poderia ter sido mais  trabalhado;
 - A escrita do output de cada funcionalidade deveria ter sido feito de outra forma:
na forma actual cada processo filho escreve o seu output, o que pode levar a que
múltiplos outputs sejam escritos ao mesmo tempo. Uma solução possível seria o próprio
servidor receber o output de cada filho e escrever para o ficheiro, garantindo que
apenas 1 processo tem acesso ao ficheiro para escrita (dentro do servidor);
 - O ciclo principal de leitura do servidor também podia ser melhorado, uma vez que
causa Segmentation Fault em sistemas MacOS. Em vez de criar 1 ciclo que está cons-
tantemente a abrir e fechar os pipes, os pipes são abertos quando o servidor
inicia e apenas são fechados quando este termina;
 - As restantes funcionalidades estão implementadas de acordo com o especificado;
