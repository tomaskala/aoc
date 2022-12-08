-module(solutiona).
-import(lists, [append/1, foldl/3, map/2, reverse/1, sum/1, zipwith/3]).

read_lines() ->
  case io:get_line("") of
    eof -> [];
    Line ->
      Heights = [C - 48 || C <- Line, 48 =< C, C =< 57],
      [Heights|read_lines()]
  end.

map_reverse(Matrix) -> map(fun(R) -> reverse(R) end, Matrix).

transpose([[]|_]) -> [];
transpose(Matrix) ->
  [map(fun erlang:hd/1, Matrix) | transpose(map(fun erlang:tl/1, Matrix))].

visible_trees([Height|Tail]) -> [true | visible_trees(Height, Tail)].
visible_trees(Height1, [Height2]) -> [Height1 < Height2];
visible_trees(Height1, [Height2|Tail]) when Height1 < Height2 ->
  [true | visible_trees(Height2, Tail)];
visible_trees(Height1, [_|Tail]) -> [false | visible_trees(Height1, Tail)].

or_matrices([Matrix|Matrices])
  -> foldl(fun(M, Acc) -> or_matrices(M, Acc) end, Matrix, Matrices).
or_matrices(Matrix, Acc) ->
  zipwith(fun(Row1, Row2) ->
              zipwith(fun(X, Y) ->
                          X or Y
                      end, Row1, Row2)
          end, Matrix, Acc).

bool_to_int(B) ->
  case B of
    true -> 1;
    false -> 0
  end.

main(_) ->
  L = read_lines(),

  VL = [visible_trees(R) || R <- L],
  VLR = map_reverse([visible_trees(R) || R <- map_reverse(L)]),
  VT = transpose([visible_trees(R) || R <- transpose(L)]),
  VTR = reverse(transpose(
   [visible_trees(R) || R <- transpose(reverse(L))]
  )),

  V = or_matrices([VL, VLR, VT, VTR]),
  N = sum(map(fun(B) -> bool_to_int(B) end, append(V))),
  io:format("~w~n", [N]).
