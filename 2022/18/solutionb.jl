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

function expand(points :: Set{Point}, pmin :: Point, pmax :: Point)
  function is_valid_point(p :: Point)
    return p.x >= pmin.x && p.y >= pmin.y && p.z >= pmin.z &&
           p.x <= pmax.x && p.y <= pmax.y && p.z <= pmax.z &&
           p ∉ exterior
  end

  exterior = Set{Point}([pmin])
  surface = Set{Point}()
  stack = Point[pmin]

  while !isempty(stack)
    current = pop!(stack)

    for n in filter(is_valid_point, neighborhood(current))
      if n ∈ points
        push!(surface, n)
      else
        push!(exterior, n)
        push!(stack, n)
      end
    end
  end

  return exterior, surface
end

points = read_input(eachline())
xmin, xmax = extrema(map(p -> p.x, points))
ymin, ymax = extrema(map(p -> p.y, points))
zmin, zmax = extrema(map(p -> p.z, points))

pmin = Point(xmin - 1, ymin - 1, zmin - 1)
pmax = Point(xmax + 1, ymax + 1, zmax + 1)

exterior, surface = expand(Set{Point}(points), pmin, pmax)
result = 0

for p in surface
  global result += count(n -> n ∈ exterior, neighborhood(p))
end

println(result)
