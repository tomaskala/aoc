-module(solutionb).
-import(lists, [append/1, foldl/3, map/2, max/1, reverse/1, zipwith/3]).

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

enumerate(List) -> enumerate(0, List).
enumerate(_, []) -> [];
enumerate(I, [H|T]) -> [{I, H}|enumerate(I + 1, T)].

find_index([], _) -> 0;
find_index([{I, H}|T], P) ->
  case P(H) of
    true -> I;
    false -> find_index(T, P)
  end.

scores(List) -> scores([], enumerate(List)).
scores(_, []) -> [];
scores(Left, [{I, Height}|Tail]) ->
  N = find_index(Left, fun(X) -> X >= Height end),
  [I - N | scores([{I, Height}|Left], Tail)].

multiply_matrices([Matrix|Matrices])
  -> foldl(fun(M, Acc) -> multiply_matrices(M, Acc) end, Matrix, Matrices).
multiply_matrices(Matrix, Acc) ->
  zipwith(fun(Row1, Row2) ->
              zipwith(fun(X, Y) ->
                          X * Y
                      end, Row1, Row2)
          end, Matrix, Acc).

main(_) ->
  L = read_lines(),

  VL = [scores(R) || R <- L],
  VLR = map_reverse([scores(R) || R <- map_reverse(L)]),
  VT = transpose([scores(R) || R <- transpose(L)]),
  VTR = reverse(transpose([scores(R) || R <- transpose(reverse(L))])),

  S = multiply_matrices([VL, VLR, VT, VTR]),
  N = max(append(S)),
  io:format("~w~n", [N]).
