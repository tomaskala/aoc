solve(P) :-
  open(P,read,F),
  process_lines(F,S),
  close(F), !,
  writeln(S).

process_lines(F,S) :-
  process_lines(F,S,0).
process_lines(F,A,A) :-
  at_end_of_stream(F).
process_lines(F,S,A) :-
  \+ at_end_of_stream(F),
  read_line_to_codes(F,C1),
  read_line_to_codes(F,C2),
  read_line_to_codes(F,C3),
  ascii_to_alphabet(C1,V1),
  ascii_to_alphabet(C2,V2),
  ascii_to_alphabet(C3,V3),
  find_item(V1,V2,V3,X),
  N is A+X,
  process_lines(F,S,N).

ascii_to_alphabet([],[]).
ascii_to_alphabet([H|T],[X|Y]) :-
  H>=65,
  H=<90,
  X is H-65+27,
  ascii_to_alphabet(T,Y).
ascii_to_alphabet([H|T],[X|Y]) :-
  H>=97,
  H=<122,
  X is H-96,
  ascii_to_alphabet(T,Y).

find_item(L1,L2,L3,C) :-
  member(C,L1),
  member(C,L2),
  member(C,L3).

partition(L,0,[],L).
partition([H|T],N,[H|F],S) :-
  M is N-1,
  partition(T,M,F,S).
