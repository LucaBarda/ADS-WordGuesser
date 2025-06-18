# WordGuesser

Application developed for the "Algorithms and Data Structures" course @ Politecnico di Milano.

## Goal
The main goal of the project is to create a program that checks the correspondence between two words of same length by using some optimized algorithms and data structures.  
The quality of the application is measured using some test cases making sure to complete the execution within predefined time and space limits.

## Description
The application takes in input the words length and the set of words (dictionary) that will be used during the execution.  
A user can create new games using the command ```+nuova_partita```.  
For each game you need to insert the reference word (word that will need to be guessed) and the maximum number of tries to get the right word. A user can submit the tries to guess the reference word and the system will reply with a report on the matching between the inserted word and the correct one. This information reports whether a letter belongs to the reference word and it's in the correct spot, whether it belongs to it but it's in the wrong position or if it doesn't belong to it.  
The user can insert other commands during the execution:
- ```+inserisci_inizio``` followed by ```+inserisci_fine```: to insert new words in the dictionary
- ```+stampa_filtrate```: to print the possible words in the dictionary that could be the reference word given the information acquired during previous failed attempts.

The information one can acquire from each attempt's character is:
1. Whether character belongs to the reference word
2. Positions where character must belong
3. Positions where character cannot belong
4. Minimum number of occurrences of character
5. Exact number of occurrences of character 
