# compilation
## LL1 analyser with C
A small LL1 analyser that can remove recusrsion, calculate the first and follow of a given production rules then you can enter a word to analyse it.
## how to use
### syntax
- terminal must be lower case
- non-terminals must be upper case
- terminals and non-terminals should be one character at most
- epsilon are represented by the character '&'
### install
``` 
gcc LL1.c -o LL1 
./LL1
``` 
### exemple
#### remove recursion ,calculate first and follow
after running you can type the following production rules as an exemple :

E->E+T|T

T->T\*F|F

F->(E)|d

.

when u finish typing your prodution rules you need to type '.' then press enter

![capt](https://user-images.githubusercontent.com/59932913/167090016-2d3569d5-cf66-44b5-9325-1bc709091db1.PNG)

- this is the result of the first step of the execution 

#### analyse a given word
now we jump to next step wich is verifying a given word matching our production rules (grammar) or not.
so we type this word for exemple:
(d+d)\*(d+d)

*note*: non-terminal caracters must be the same as the non-terminals entred in the production rules ~~(5+5)*5~~ this word will not be accepted despite it is correct instead you should type (d+d)\*d 

![cap](https://user-images.githubusercontent.com/59932913/167091717-4eb2ddb1-b2fe-4194-a76b-bbb9b2fd1317.PNG)

as we can see it tell's as that this word is accepted !.



sorry for the one file code 😕

