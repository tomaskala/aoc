import scala.util.matching.Regex

trait Node
case class In() extends Node
case class Root(left: String, right: String) extends Node
case class Op(
  op: (Long, Long) => Long,
  leftInv: (Long, Long) => Long,
  rightInv: (Long, Long) => Long,
  left: String,
  right: String,
) extends Node
case class Num(value: Long) extends Node

object Solution {
  val inPattern: Regex = """^(humn): .*$""".r
  val rootPattern: Regex = """^(root): (\w+) . (\w+)$""".r
  val addPattern: Regex = """^(\w+): (\w+) \+ (\w+)$""".r
  val subPattern: Regex = """^(\w+): (\w+) - (\w+)$""".r
  val mulPattern: Regex = """^(\w+): (\w+) \* (\w+)$""".r
  val divPattern: Regex = """^(\w+): (\w+) / (\w+)$""".r
  val numPattern: Regex = """^(\w+): (\d+)$""".r

  def known(nodes: Map[String, Node], current: String): Boolean =
    nodes(current) match {
      case In() => false
      case Op(_, _, _, left, right) => known(nodes, left) && known(nodes, right)
      case Num(_) => true
    }

  def evaluate(nodes: Map[String, Node], current: String): Long =
    nodes(current) match {
      case Op(op, _, _, left, right) =>
        op(evaluate(nodes, left), evaluate(nodes, right))
      case Num(value) => value
    }

  def solve(nodes: Map[String, Node], current: String, other: Long): Long =
    nodes(current) match {
      case In() => other
      case Op(_, leftInv, rightInv, left, right) =>
        if (known(nodes, left)) {
          val subResult = evaluate(nodes, left)
          val unknown = rightInv(other, subResult)
          solve(nodes, right, unknown)
        } else {
          val subResult = evaluate(nodes, right)
          val unknown = leftInv(other, subResult)
          solve(nodes, left, unknown)
        }
      case Num(value) => value
    }

  def solve(nodes: Map[String, Node], current: String): Long =
    nodes(current) match {
      case Root(left, right) =>
        if (known(nodes, left)) {
          val subResult = evaluate(nodes, left)
          solve(nodes, right, subResult)
        } else {
          val subResult = evaluate(nodes, right)
          solve(nodes, left, subResult)
        }
    }

  def main(args: Array[String]): Unit = {
    val nodes = io.Source.stdin.getLines().map { line =>
      line match {
        case inPattern(id) =>
          (id, In())
        case rootPattern(id, left, right) =>
          (id, Root(left, right))
        case addPattern(id, left, right) =>
          (id, Op(_ + _, _ - _, (other, subResult) => other - subResult, left, right))
        case subPattern(id, left, right) =>
          (id, Op(_ - _, _ + _, (other, subResult) => subResult - other, left, right))
        case mulPattern(id, left, right) =>
          (id, Op(_ * _, _ / _, (other, subResult) => other / subResult, left, right))
        case divPattern(id, left, right) =>
          (id, Op(_ / _, _ * _, (other, subResult) => subResult / other, left, right))
        case numPattern(id, value) =>
          (id, Num(value.toLong))
      }
    }.toMap

    val result = solve(nodes, "root")
    println(result)
  }
}
