package aoc

import kotlin.math.abs
import kotlin.system.exitProcess

data class Point(val x: Int, val y: Int) {
  fun dist(p: Point): Int = abs(x - p.x) + abs(y - p.y)
}

data class Sensor(val sensorPos: Point, val beaconPos: Point) {
  val radius = sensorPos.dist(beaconPos)

  fun coveredRow(y: Int): IntRange? {
    val width = radius - abs(y - sensorPos.y)
    return (sensorPos.x - width .. sensorPos.x + width).takeUnless(IntRange::isEmpty)
  }
}

fun parseSensor(line: String) =
  Sensor(
    Point(
      line.substringAfter("x=").substringBefore(",").toInt(),
      line.substringAfter("y=").substringBefore(":").toInt(),
    ),
    Point(
      line.substringAfterLast("x=").substringBefore(",").toInt(),
      line.substringAfterLast("y=").toInt(),
    ),
  )

fun main(args: Array<String>) {
  if (args.isEmpty()) {
    println("Provide the y coordinate")
    exitProcess(1)
  }

  val y = args[0].toInt()
  val sensors = generateSequence(::readLine).map(::parseSensor).toList()

  val beaconsOnRow = sensors
    .filter { it.beaconPos.y == y }
    .map { it.beaconPos.x }
    .distinct()
    .count()

  val coveredRows = sensors.mapNotNull { it.coveredRow(y) }
  val rangeStarts = coveredRows
    .map { it.first }
    .filter { start -> coveredRows.none { start - 1 in it } }
  val rangeEnds = coveredRows
    .map { it.last }
    .filter { end -> coveredRows.none { end + 1 in it } }
  val totalCovered = rangeStarts
    .map { start -> start .. rangeEnds.filter { start < it }.min() }
    .sumOf { it.last - it.first + 1 }

  println(totalCovered - beaconsOnRow)
}
