TIME_LIMIT = 24
BLUEPRINT_FORMAT = Regexp.new("[^\\d]+(\\d+)" * 7 + "[^\\d]+")

class Items
  attr_accessor :ore, :clay, :obsidian, :geode

  def initialize(ore, clay, obsidian, geode)
    @ore = ore
    @clay = clay
    @obsidian = obsidian
    @geode = geode
  end

  def increment(items)
    self.ore += items.ore
    self.clay += items.clay
    self.obsidian += items.obsidian
    self.geode += items.geode
  end
end

class State
  attr_accessor :time
  attr_reader :robots, :materials

  def initialize(time, robots, materials)
    @time = time
    @robots = robots
    @materials = materials
  end

  def neighborhood(blueprint)
    neighbors = []

    ore = self.build_ore(blueprint)
    neighbors << ore unless ore.nil?

    clay = self.build_clay(blueprint)
    neighbors << clay unless clay.nil?

    obsidian = self.build_obsidian(blueprint)
    neighbors << obsidian unless obsidian.nil?

    geode = self.build_geode(blueprint)
    neighbors << geode unless geode.nil?

    neighbors << self.mine if neighbors.empty?
    neighbors
  end

  private
    def clone
      State.new(
        self.time,
        Items.new(
          self.robots.ore,
          self.robots.clay,
          self.robots.obsidian,
          self.robots.geode,
        ),
        Items.new(
          self.materials.ore,
          self.materials.clay,
          self.materials.obsidian,
          self.materials.geode,
        ),
      )
    end

    def build_ore(blueprint)
      return nil if self.robots.ore >= blueprint.max_ore
      state = self.clone

      while state.materials.ore < blueprint.ore_robot_ore
        state.materials.increment(state.robots)
        state.time += 1
      end

      return nil if state.time >= TIME_LIMIT

      state.materials.ore -= blueprint.ore_robot_ore
      state.materials.increment(state.robots)
      state.robots.ore += 1
      state.time += 1

      state
    end

    def build_clay(blueprint)
      return nil if self.robots.clay >= blueprint.obsidian_robot_clay
      state = self.clone

      while state.materials.ore < blueprint.clay_robot_ore
        state.materials.increment(state.robots)
        state.time += 1
      end

      return nil if state.time >= TIME_LIMIT

      state.materials.ore -= blueprint.clay_robot_ore
      state.materials.increment(state.robots)
      state.robots.clay += 1
      state.time += 1

      state
    end

    def build_obsidian(blueprint)
      return nil if self.robots.clay == 0 ||
        self.robots.obsidian >= blueprint.geode_robot_obsidian
      state = self.clone

      while state.materials.ore < blueprint.obsidian_robot_ore ||
        state.materials.clay < blueprint.obsidian_robot_clay
        state.materials.increment(state.robots)
        state.time += 1
      end

      return nil if state.time >= TIME_LIMIT

      state.materials.ore -= blueprint.obsidian_robot_ore
      state.materials.clay -= blueprint.obsidian_robot_clay
      state.materials.increment(state.robots)
      state.robots.obsidian += 1
      state.time += 1

      state
    end

    def build_geode(blueprint)
      return nil if self.robots.obsidian == 0
      state = self.clone

      while state.materials.ore < blueprint.geode_robot_ore ||
        state.materials.obsidian < blueprint.geode_robot_obsidian
        state.materials.increment(state.robots)
        state.time += 1
      end

      return nil if state.time >= TIME_LIMIT

      state.materials.ore -= blueprint.geode_robot_ore
      state.materials.obsidian -= blueprint.geode_robot_obsidian
      state.materials.increment(state.robots)
      state.robots.geode += 1
      state.time += 1

      state
    end

    def mine
      state = self.clone

      while state.time < TIME_LIMIT
        state.materials.increment(state.robots)
        state.time += 1
      end

      state
    end
end

Blueprint = Struct.new(
  :id,
  :ore_robot_ore,
  :clay_robot_ore,
  :obsidian_robot_ore,
  :obsidian_robot_clay,
  :geode_robot_ore,
  :geode_robot_obsidian,
) do
  def max_ore
    [self.ore_robot_ore,
     self.clay_robot_ore,
     self.obsidian_robot_ore,
     self.geode_robot_ore,
    ].max
  end
end

def simulate(blueprint)
  max_geodes = 0
  visited = {}
  frontier = [State.new(1, Items.new(1, 0, 0, 0), Items.new(0, 0, 0, 0))]

  until frontier.empty?
    state = frontier.pop
    visited[state] = true

    if state.time == TIME_LIMIT
      total_geodes = state.materials.geode + state.robots.geode
      max_geodes = total_geodes if total_geodes > max_geodes
    else
      state.neighborhood(blueprint).each do |n|
        frontier << n unless visited.has_key? n
      end
    end
  end

  blueprint.id * max_geodes
end

result = 0

ARGF.each do |line|
  line.match(BLUEPRINT_FORMAT) do |m|
    result += simulate(Blueprint.new(*m.captures.map do |s| Integer(s) end))
  end
end

puts result
