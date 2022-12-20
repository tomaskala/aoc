package aoc

import kotlin.math.abs
import kotlin.system.exitProcess

data class Point(val x: Int, val y: Int) {
  fun dist(p: Point): Int = abs(x - p.x) + abs(y - p.y)
}

data class Sensor(val sensorPos: Point, val beaconPos: Point) {
  val radius = sensorPos.dist(beaconPos)
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

fun calculateSkip(sensors: List<Sensor>, current: Point): Int? = sensors
  .map { it.radius - it.sensorPos.dist(current) }
  .filter { it >= 0 }
  .maxOrNull()

tailrec fun searchRow(sensors: List<Sensor>, current: Point, bound: Int): Point? {
  if (current.x < bound) {
    return when (val skip = calculateSkip(sensors, current)) {
      null -> current
      else -> searchRow(sensors, Point(current.x + skip + 1, current.y), bound)
    }
  }

  return null
}

tailrec fun search(sensors: List<Sensor>, current: Point, bound: Int): Point? {
  if (current.y < bound) {
    return when (val point = searchRow(sensors, current, bound)) {
      null -> search(sensors, Point(0, current.y + 1), bound)
      else -> point
    }
  }

  return null
}

fun main(args: Array<String>) {
  if (args.isEmpty()) {
    println("Provide the search bound")
    exitProcess(1)
  }

  val bound = args[0].toInt()
  val sensors = generateSequence(::readLine).map(::parseSensor).toList()

  when (val beacon = search(sensors, Point(0, 0), bound)) {
    null -> println("No solution found")
    else -> println(beacon.x.toLong() * 4_000_000 + beacon.y.toLong())
  }
}
