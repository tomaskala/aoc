using System;
using System.Collections.Generic;
using System.Linq;

struct Point
{
  public int X { get; }
  public int Y { get; }

  public Point(int x, int y)
  {
    X = x;
    Y = y;
  }
}

class Solution
{
  public static void AddOrAppend(Dictionary<Point, List<Point>> dict, Point key, Point p)
  {
    if (dict.ContainsKey(key))
    {
      dict[key].Add(p);
    }
    else
    {
      var list = new List<Point>() {p};
      dict.Add(key, list);
    }
  }

  public static void AddOrIncrement(Dictionary<Point, int> dict, Point key)
  {
    if (dict.ContainsKey(key))
    {
      dict[key]++;
    }
    else
    {
      dict.Add(key, 1);
    }
  }

  private static HashSet<Point> ReadInput()
  {
    var board = new HashSet<Point>();
    string line;
    var i = 0;

    while ((line = Console.ReadLine()) != null)
    {
      for (var j = 0; j < line.Length; j++)
      {
        if (line[j] == '#')
        {
          board.Add(new Point(j, i));
        }
      }

      i++;
    }

    return board;
  }

  private static bool Lonely(HashSet<Point> board, Point p)
  {
    for (int y = -1; y <= 1; y++)
    {
      for (int x = -1; x <= 1; x++)
      {
        if (x == 0 && y == 0)
        {
          continue;
        }

        var neighbor = new Point(p.X + x, p.Y + y);

        if (board.Contains(neighbor))
        {
          return false;
        }
      }
    }

    return true;
  }

  private static void Simulate(HashSet<Point> board, int round)
  {
    var proposals = new Dictionary<Point, List<Point>>();
    var occupied = new Dictionary<Point, int>();

    foreach (var p in board)
    {
      if (Lonely(board, p))
      {
        continue;
      }

      for (int i = 0; i < 4; i++)
      {
        bool proposed = false;

        if ((round + i) % 4 == 0 && !board.Contains(new Point(p.X - 1, p.Y - 1)) && !board.Contains(new Point(p.X, p.Y - 1)) && !board.Contains(new Point(p.X + 1, p.Y - 1)))
        {
          var proposal = new Point(p.X, p.Y - 1);
          AddOrAppend(proposals, p, proposal);
          AddOrIncrement(occupied, proposal);
          proposed = true;
        }

        if ((round + i) % 4 == 1 && !board.Contains(new Point(p.X - 1, p.Y + 1)) && !board.Contains(new Point(p.X, p.Y + 1)) && !board.Contains(new Point(p.X + 1, p.Y + 1)))
        {
          var proposal = new Point(p.X, p.Y + 1);
          AddOrAppend(proposals, p, proposal);
          AddOrIncrement(occupied, proposal);
          proposed = true;
        }

        if ((round + i) % 4 == 2 && !board.Contains(new Point(p.X - 1, p.Y - 1)) && !board.Contains(new Point(p.X - 1, p.Y)) && !board.Contains(new Point(p.X - 1, p.Y + 1)))
        {
          var proposal = new Point(p.X - 1, p.Y);
          AddOrAppend(proposals, p, proposal);
          AddOrIncrement(occupied, proposal);
          proposed = true;
        }

        if ((round + i) % 4 == 3 && !board.Contains(new Point(p.X + 1, p.Y - 1)) && !board.Contains(new Point(p.X + 1, p.Y)) && !board.Contains(new Point(p.X + 1, p.Y + 1)))
        {
          var proposal = new Point(p.X + 1, p.Y);
          AddOrAppend(proposals, p, proposal);
          AddOrIncrement(occupied, proposal);
          proposed = true;
        }

        if (proposed)
        {
          break;
        }
      }
    }

    foreach (var kv in proposals)
    {
      foreach (var proposed in kv.Value)
      {
        if (occupied[proposed] == 1)
        {
          board.Remove(kv.Key);
          board.Add(proposed);
        }
      }
    }
  }

  public static void Main(string[] args)
  {
    var board = ReadInput();

    for (var i = 0; i < 10; i++)
    {
      Simulate(board, i);
    }

    var xMin = board.Min(p => p.X);
    var xMax = board.Max(p => p.X);
    var yMin = board.Min(p => p.Y);
    var yMax = board.Max(p => p.Y);
    var result = (xMax - xMin + 1) * (yMax - yMin + 1) - board.Count;

    Console.WriteLine(result);
  }
}
