const std = @import("std");
const fmt = std.fmt;
const heap = std.heap;
const io = std.io;
const math = std.math;
const mem = std.mem;

const time_limit = 30;

const Valve = struct {
    idx: usize,
    rate: i32,
};

const State = struct {
    current: u16,
    opened: std.AutoHashMap(u16, void),
    remaining_time: i32,
    utility: i32,
};

const Graph = struct {
    const Self = @This();

    mapping: std.AutoHashMap(u16, Valve),
    adjacency: std.AutoHashMap(u16, std.ArrayList(u16)),
    idx_to_label: std.AutoHashMap(usize, u16),

    pub fn floyd_warshall(self: Self, allocator: mem.Allocator) ![][]i32 {
        const n = self.adjacency.count();
        var dist = try allocator.alloc([]i32, n);
        var iter = self.adjacency.iterator();

        for (dist) |*row| {
            row.* = try allocator.alloc(i32, n);
            for (row.*) |*d| d.* = math.maxInt(i32);
        }

        while (iter.next()) |kv| {
            const label = kv.key_ptr.*;
            const neighbors = kv.value_ptr.*;
            const i = self.mapping.get(label).?.idx;
            dist[i][i] = 0;

            for (neighbors.items) |neighbor| {
                const j = self.mapping.get(neighbor).?.idx;
                dist[i][j] = 1;
            }
        }

        var k: usize = 0;
        while (k < n) : (k += 1) {
            var i: usize = 0;
            while (i < n) : (i += 1) {
                var j: usize = 0;
                while (j < n) : (j += 1) {
                    const new_dist = dist[i][k] +| dist[k][j];
                    if (dist[i][j] > new_dist) dist[i][j] = new_dist;
                }
            }
        }

        return dist;
    }
};

const Neighborhood = struct {
    const Self = @This();

    state: State,
    graph: Graph,
    dist: [][]i32,
    index: usize = 0,

    pub fn next(self: *Self) !?State {
        const n = self.graph.adjacency.count();
        if (self.index >= n) return null;
        defer self.index += 1;

        const state = self.state;
        const current_idx = self.graph.mapping.get(state.current).?.idx;

        while (self.index < n) : (self.index += 1) {
            const neighbor = self.graph.idx_to_label.get(self.index).?;
            const neighbor_rate = self.graph.mapping.get(neighbor).?.rate;
            const dist = self.dist[current_idx][self.index];
            const remaining_time = state.remaining_time - dist - 1;

            if (neighbor_rate > 0 and remaining_time >= 0 and !state.opened.contains(neighbor)) {
                var opened = try state.opened.clone();
                try opened.put(neighbor, {});

                return State{
                    .current = neighbor,
                    .opened = opened,
                    .remaining_time = remaining_time,
                    .utility = state.utility + remaining_time * neighbor_rate,
                };
            }
        }

        return null;
    }
};

fn utilityGreaterThan(context: void, a: State, b: State) math.Order {
    _ = context;
    return math.order(b.utility, a.utility);
}

fn search(graph: Graph, start_label: u16, allocator: mem.Allocator) !i32 {
    var max_utility: i32 = 0;
    var queue = std.PriorityQueue(State, void, utilityGreaterThan).init(allocator, {});
    const dist = try graph.floyd_warshall(allocator);
    const initial = State{
        .current = start_label,
        .opened = std.AutoHashMap(u16, void).init(allocator),
        .remaining_time = time_limit,
        .utility = 0,
    };
    try queue.add(initial);

    while (queue.len > 0) {
        const current = queue.remove();
        var neighborhood = Neighborhood{
            .state = current,
            .graph = graph,
            .dist = dist,
        };
        if (current.utility > max_utility) max_utility = current.utility;
        while (try neighborhood.next()) |neighbor| try queue.add(neighbor);
    }

    return max_utility;
}

fn readInput(allocator: mem.Allocator) !Graph {
    const stdin = io.getStdIn();
    defer stdin.close();

    var mapping = std.AutoHashMap(u16, Valve).init(allocator);
    var adjacency = std.AutoHashMap(u16, std.ArrayList(u16)).init(allocator);
    var idx_to_label = std.AutoHashMap(usize, u16).init(allocator);

    var reader = io.bufferedReader(stdin.reader());
    var stream = reader.reader();
    var buf: [128]u8 = undefined;
    var idx: usize = 0;

    while (try stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        const label = (@as(u16, line[6]) << 8) | @as(u16, line[7]);
        const rate_offset = mem.indexOf(u8, line, ";").?;
        const rate = try fmt.parseInt(i32, line[23..rate_offset], 10);
        const valve = Valve{ .idx = idx, .rate = rate };

        try mapping.put(label, valve);
        try idx_to_label.put(idx, label);
        idx += 1;

        var neighbors = std.ArrayList(u16).init(allocator);
        const neighbors_offset = if (mem.indexOf(u8, line, "valves ")) |offset|
            offset + 7
        else
            mem.indexOf(u8, line, "valve ").? + 6;
        var neighbors_labels = mem.tokenize(u8, line[neighbors_offset..], ", ");

        while (neighbors_labels.next()) |neighbor| {
            std.debug.assert(neighbor.len == 2);
            const n = (@as(u16, neighbor[0]) << 8) | @as(u16, neighbor[1]);
            try neighbors.append(n);
        }

        try adjacency.put(label, neighbors);
    }

    return Graph{
        .mapping = mapping,
        .adjacency = adjacency,
        .idx_to_label = idx_to_label,
    };
}

pub fn main() !void {
    const stdout = io.getStdOut().writer();
    const stderr = io.getStdErr().writer();
    var arena = heap.ArenaAllocator.init(heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const graph = try readInput(allocator);
    const start = (@as(u16, 'A') << 8) | @as(u16, 'A');

    if (search(graph, start, allocator)) |utility| {
        try stdout.print("{d}\n", .{utility});
    } else |err| {
        try stderr.print("{}\n", .{err});
    }
}
