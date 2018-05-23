# Exercícios

Exercícios do livro: _Introduction to Parallel Programming_.
O _source code_ requiridos por algumas das questões foi encontrado [aqui](https://www.cs.usfca.edu/~peter/ipp/).

### Exercício 3.1
#### Questão
What happens in the greetings program if, instead of strlen(greeting) + 1,
we use strlen(greeting) for the length of the message being sent by processes 1, 2,..., comm\_sz−1?
What happens if we use MAX_STRING instead of strlen(greeting) + 1?
Can you explain these results?
#### Resposta
A função `strlen` retorna o tamanho da _string_.
Porém, em C todo final de _string_ deve ser marcado pelo caractere `'\0'`, o _string terminator_,
e esse último caractere não é contato pelo **strlen**.
Ao enviar a mensagem de um processo para outro sem adicionar espaço para o _string terminator_,
o caractere ao fim da _string_ fica subjetivo ao estado de execução do processo que recebeu a mensagem.

Assim, usando apenas `strln(greeting)` a _string_ recebida pode não ter o _string terminator_,
e então o comportamento do programa é incerto,
podendo resultar numa falta de segmentação,
na mensagem sendo impressa com lixo de memória,
ou no texto de **greeting** sendo impresso normalmente.
Se `MAX STRING` for usado como tamanho mais caracteres seriam copiados do que é realmente necessário,
mas o programa teria o comportamento correto e esperado.

### Exercício 3.2
#### Questão
Modify the trapezoidal rule so that it will correctly estimate the integral even if comm_sz doesn’t evenly divide n.
(You can still assume that n ≥ comm\_sz.)
#### Resposta
Reposta: [3.2_exercicio.c](3.2_exercicio.c)  
Exemplo: `make all test FILE=3.2_exercicio.c PROCESS=3`

### Exercício 3.3
#### Questão
Determine which of the variables in the trapezoidal rule program are local and which are global.
#### Resposta
Como MPI é um padrão para memória distribuída,
todos os identificadores referenciam endereços de memória que são locais ao processo.
O MPI oferece algumas ferramentas para lidar com memória compartilhada, mas nenhuma delas é usada no programa.

### Exercício 3.4
#### Questão
Modify the program that just prints a line of output from each process
( mpi output.c ) so that the output is printed in process rank order: process
0s output first, then process 1s, and so on.
#### Resposta
Reposta: [3.4_exercicio.c](3.4_exercicio.c)  

### Exercício 3.6
#### Questão
Suppose comm\_sz = 4 and suppose that x is a vector with n = 14 components.
a. How would the components of x be distributed among the processes in a
program that used a block distribution?
b. How would the components of x be distributed among the processes in a
program that used a cyclic distribution?
c. How would the components of x be distributed among the processes in a
program that used a block-cyclic distribution with blocksize b = 2?
You should try to make your distributions general so that they could be used regardless of what comm\_sz and n are.
You should also try to make your distributions “fair” so that if q and r are any two processes,
the difference between the number of components assigned to q and the number of components assigned to r is as small as possible.
#### Resposta
a. Os elementos seriam distribuídos em blocos sequenciais com algum processo recebendo o resto da divisão, ex:
	- comm\_sz 0 <- n = [0, 1, 2, 3, 4]
	- comm\_sz 1 <- n = [5, 6, 7]
	- comm\_sz 2 <- n = [8, 9, 10]
	- comm\_sz 3 <- n = [11 12, 13]
b. Os elementos seriam distribuídos de maneira intercalada, ex:
	- comm\_sz 0 <- n = [0, 4, 8, 12, 13]
	- comm\_sz 1 <- n = [1, 5, 9]
	- comm\_sz 2 <- n = [2, 6, 10]
	- comm\_sz 3 <- n = [3, 7, 11]
c. Os processos receberiam um ou mais blocos sequenciais de tamanho 2, ex:
	- comm\_sz 0 <- n = [0, 1, 8, 9]
	- comm\_sz 1 <- n = [2, 3, 10, 11]
	- comm\_sz 2 <- n = [4, 5, 12, 13]
	- comm\_sz 3 <- n = [6, 7]

### Exercício 3.7
#### Questão
What do the various MPI collective functions do if the communicator contains single process?
#### Resposta
A primitivas coletivas de MPI funcionam mesmo se na execução houver apenas um processo,
já que o requisito principal dessas primitivas é que sejam chamadas de maneira equivalente em todos os processos.

<!-- ### Exercício x.x -->
<!-- #### Questão -->
<!-- #### Resposta -->
