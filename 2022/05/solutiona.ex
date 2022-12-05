defmodule SolutionA do
  def read_stacks do
    IO.stream(:stdio, :line)
      |> Enum.take_while(fn line -> line != "\n" end)
      |> Enum.reduce(%{}, fn line, stacks -> String.codepoints(line)
        |> Enum.chunk_every(4)
        |> Enum.with_index
        |> Enum.reduce(stacks, fn {crate, n}, acc ->
          case crate do
            ["[", char, "]", _] ->
              Map.update(acc, n + 1, [char], fn x -> [char|x] end)
            _ -> acc
          end
        end)
      end)
      |> Enum.map(fn {k, v} -> {k, Enum.reverse(v)} end)
      |> Map.new
  end

  def stack_crates(stacks) do
    IO.stream(:stdio, :line) |> Enum.reduce(stacks, &move(&1, &2))
  end

  defp move(line, stacks) do
    [_, n, _, s, _, d] = String.split(line, " ") |> Enum.map(&String.trim(&1))
    num = String.to_integer(n)
    src = String.to_integer(s)
    dst = String.to_integer(d)

    Enum.reduce(1..num, stacks, fn _, acc ->
      [head|tail] = acc[src]
      Map.put(acc, src, tail) |> Map.update!(dst, fn x -> [head|x] end)
    end)
  end
end

SolutionA.read_stacks
  |> SolutionA.stack_crates
  |> Map.to_list
  |> Enum.sort(fn {k1, _}, {k2, _} -> k1 <= k2 end)
  |> Enum.map(fn {_, v} -> hd(v) end)
  |> IO.puts
