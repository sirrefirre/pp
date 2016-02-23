palindromic.c is a function to find all palindromes and palindromic word from an input file and write them to a new file palindromes.
It can be run with "./pal workers file". Default values are 20 max workers and file "words.small".

program seems to scale good with threads if you consider error margins with time and speedup.

run at avril.it.kth.se
number of palindromes calculated by palindromic.c
words.small: ~25k words 299 palindromes
threads:	10	4	2	1/0
time in sec:	1.17	2.94	5.83	11.45
speedup:	9.8	3.9	2	1
	
words.medium: ~53k words 516 palindromes
threads:	10	4	2	1/0
timein sec:	4.26	10.95	21.28	42.62
speedup:	10	3.9	2	1

words.large: ~114k words 885 palindromes
threads:	10	4	2	1/0
timein sec:	26	65	129	257
speedup:	9.9	4	2	1
words.large was run a reduced number of times