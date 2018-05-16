# Exercícios do livro: Introduction to Parallel Programming

### Exercício 3.1
#### Questão
What happens in the greetings program if, instead of strlen(greeting) + 1,
we use strlen(greeting) for the length of the message being sent by processes 1, 2,..., comm sz−1?
What happens if we use MAX STRING instead of strlen(greeting) + 1?
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

<!-- ### Exercício x.x -->
<!-- #### Questão -->
<!-- #### Resposta -->
