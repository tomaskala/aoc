using Graphs

struct Point
  x :: Int
  y :: Int
  z :: Int
end

function read_input(lines)
  return [Point(map(c -> parse(Int, c), split(l, ","))...) for l in lines]
end

function neighborhood(p)
  return [Point(p.x - 1, p.y, p.z), Point(p.x + 1, p.y, p.z),
          Point(p.x, p.y - 1, p.z), Point(p.x, p.y + 1, p.z),
          Point(p.x, p.y, p.z - 1), Point(p.x, p.y, p.z + 1)]
end

function build_graph(points)
  g = SimpleGraph(length(points))
  point_map = Dict{Point, Int}()

  for (i, p) in enumerate(points)
    point_map[p] = i

    for n in filter(n -> n ∈ keys(point_map), neighborhood(p))
      add_edge!(g, i, point_map[n])
    end
  end

  return g
end

points = read_input(eachline())
g = build_graph(points)

result = 6 * nv(g) - sum(degree(g))
println(result)
