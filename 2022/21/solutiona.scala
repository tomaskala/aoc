import scala.util.matching.Regex

trait Node
case class Op(op: (Long, Long) => Long, left: String, right: String) extends Node
case class Num(value: Long) extends Node

object Solution {
  val addPattern: Regex = """^(\w+): (\w+) \+ (\w+)$""".r
  val subPattern: Regex = """^(\w+): (\w+) - (\w+)$""".r
  val mulPattern: Regex = """^(\w+): (\w+) \* (\w+)$""".r
  val divPattern: Regex = """^(\w+): (\w+) / (\w+)$""".r
  val numPattern: Regex = """^(\w+): (\d+)$""".r

  def evaluate(nodes: Map[String, Node], current: String): Long =
    nodes(current) match {
      case Op(op, left, right) =>
        op(evaluate(nodes, left), evaluate(nodes, right))
      case Num(value) => value
    }

  def main(args: Array[String]): Unit = {
    val nodes = io.Source.stdin.getLines().map { line =>
      line match {
        case addPattern(id, left, right) =>
          (id, Op(_ + _, left, right))
        case subPattern(id, left, right) =>
          (id, Op(_ - _, left, right))
        case mulPattern(id, left, right) =>
          (id, Op(_ * _, left, right))
        case divPattern(id, left, right) =>
          (id, Op(_ / _, left, right))
        case numPattern(id, value) =>
          (id, Num(value.toLong))
      }
    }.toMap

    val result = evaluate(nodes, "root")
    println(result)
  }
}
