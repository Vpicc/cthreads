# cthreads
A biblioteca cthread oferece uma interface de programação (API) que permitem o seu uso para o desenvolvimento de programas.

O escalonador utilizado é do tipo preemptivo por prioridade, ou seja, se uma thread em execução criar outra com prioridade maior que a sua, a mesma deverá perder o processador (preempção) para a thread recém criada. Importante: a preempção só ocorre se a prioridade for superior; não ocorrerá caso for inferior ou igual.

# int ccreate (void *(*start)(void *), void *arg, int prio);
Parâmetros:
start: ponteiro para a função que a thread executará.
arg: um parâmetro que pode ser passado para a thread na sua criação. (Obs.: é um único parâmetro. Se for necessário passar mais de um valor deve-se empregar um ponteiro para uma struct)
prio: Valores numéricos válidos 0, 1 e 2 (0 = alta prioridade, 1 = média prioridade, 2 = baixa prioridade). Demais valores são considerados inválidos e a função deve retornar com código de erro.
Retorno:
Quando executada corretamente: retorna um valor positivo, que representa o identificador da thread criada
Caso contrário, retorna um valor negativo.

# Liberando voluntariamente a CPU:
uma thread pode liberar a CPU de forma voluntária com o auxílio da primitiva cyield. Se isso acontecer, a thread que executou cyield retorna ao estado apto. Então, o escalonador será chamado para selecionar a thread que receberá a CPU.

# int cyield(void);
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Alterando a prioridade das threads:
uma thread pode alterar a sua própria prioridade. Se essa operação resultar que exista no estado apto uma thread com prioridade maior do que a thread em execução (estado executando), esta deverá ser preemptada em favor daquela no estado apto. Ou seja, uma thread PODE perder o processador quando realiza uma chamada à função csetprio.
# int csetprio(int tid, int prio);
Parâmetros:
tid: identificador da thread cuja prioridade será alterada (deixar sempre esse campo como NULL em 2018/02)
prio: nova prioridade da thread.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Sincronização de término:
uma thread pode ser bloqueada até que outra termine sua execução usando a função cjoin. A função cjoin recebe como parâmetro o identificador da thread cujo término está sendo aguardado. Quando essa thread terminar, a função cjoin retorna com um valor inteiro indicando o sucesso ou não de sua execução. Uma determinada thread só pode ser esperada por uma única outra thread. Se duas ou mais threads fizerem cjoin para uma mesma thread, apenas a primeira que realizou a chamada será bloqueada. As outras chamadas retornarão imediatamente com um código de erro e seguirão sua execução. Se cjoin for feito para uma thread que não existe (não foi criada ou já terminou), a função retornará imediatamente com um código de erro.

# int cjoin(int tid);
Parâmetros:
tid: identificador da thread cujo término está sendo aguardado.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Sincronização de controle:
o sistema prevê o emprego de uma variável especial para realizar a sincronização de acesso a recursos compartilhados (por exemplo, uma seção crítica). As primitivas existentes são csem_init, cwait e csignal, e usam uma variável especial que recebe o nome específico de semáforo. A primitiva csem_init é usada para inicializar a variável csem_t e deve ser chamada, obrigatoriamente, antes que essa variável possa ser usada com as primitivas cwait e csignal.

# typedef struct s_sem {
int count; // indica se recurso está ocupado ou não (livre > 0, ocupado ≤ 0)
PFILA2 fila; // ponteiro para uma fila de threads bloqueadas no semáforo.
} csem_t;

# Inicialização de semáforo:
a função csem_init inicializa uma variável do tipo csem_t e consiste em fornecer um valor inteiro (count), positivo ou negativo, que representa a quantidade existente do recurso controlado pelo semáforo. Para realizar exclusão mútua, esse valor inicial da variável semáforo deve ser 1 (semáforo binário). Ainda, cada variável semáforo deve ter associado uma estrutura que registre as threads que estão bloqueadas, esperando por sua liberação. Na inicialização essa lista deve estar vazia.

# int csem_init (csem_t *sem, int count);
Parâmetros:
sem: ponteiro para uma variável do tipo csem_t. Aponta para uma estrutura de dados que representa a variável semáforo.
count: valor a ser usado na inicialização do semáforo. Representa a quantidade de recursos controlados pelo semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Solicitação (alocação) de recurso:
a primitiva cwait será usada para solicitar um recurso. Se o recurso estiver livre, ele é atribuído a thread, que continuará a sua execução normalmente; caso contrário a thread será bloqueada e posta a espera desse recurso na fila. Se na chamada da função o valor de count for menor ou igual a zero, a thread deverá ser posta no estado bloqueado e colocada na fila associada a variável semáforo. Para cada chamada a cwait a variável count da estrutura semáforo é decrementada de uma unidade.
# int cwait (csem_t *sem);
Parâmetros:
sem: ponteiro para uma variável do tipo semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Liberação de recurso:
a chamada csignal serve para indicar que a thread está liberando o recurso. Para cada chamada da primitiva csignal, a variável count deverá ser incrementada de uma unidade. Se houver mais de uma thread bloqueada a espera desse recurso a primeira delas, segundo uma política de FIFO, deverá passar para o estado apto e as demais devem continuar no estado bloqueado.
Atente para o fato que o escalonar é preemptivo, assim, se a thread em executando realizar um csignal que libere (desbloqueie) uma thread com uma prioridade maior que a sua, a thread em execução deverá ser preemptada.
Ainda, as threads que esperam por um mesmo semáforo devem ser liberadas de acordo com a sua prioridade, isso é, se houver uma thread de média prioridade e de alta prioridade esperando pelo mesmo semáforo, a de alta prioridade é que deve ser desbloqueada. Threads de mesma prioridade são desbloqueadas na ordem em que foram bloqueadas (fila).
# int csignal (csem_t *sem);
Parâmetros:
sem: ponteiro para uma variável do tipo semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

# Identificação do grupo:
Além das funções de manipulação das threads e de sincronização a biblioteca deverá prover a implementação de uma função que forneça o nome dos alunos integrantes do grupo que desenvolveu a biblioteca chtread. O protótipo dessa função é:
# int cidentify (char *name, int size);
Parâmetros:
name: ponteiro para uma área de memória onde deve ser escrito um string que contém os nomes dos componentes do grupo e seus números de cartão. Deve ser uma linha por componente.
size: quantidade máxima de caracteres que podem ser copiados para o string de identificação dos componentes do grupo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.
