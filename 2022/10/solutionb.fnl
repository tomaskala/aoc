(fn decode-inst [inst-line]
  (match (inst-line:match "(%a+) ?(%-?%d*)")
    ("addx" arg) {:inst "addx" :arg (tonumber arg)}
    "noop" {:inst "noop" :arg 0}))

(fn fetch-inst [prev-inst]
  (match prev-inst
    {:inst "noop" :arg _} (decode-inst (io.read "l"))
    {:inst "addx" :arg arg} {:inst "noop" :arg arg}))

(fn overlap? [x cycle]
  (let [pos (% cycle 40)
        min (math.max (- x 1) 0)
        max (math.min (+ x 1) 39)]
    (and (<= min pos) (<= pos max))))

(fn advance-state [state]
  {:inst (fetch-inst state.inst)
   :pixels state.pixels
   :x (if (= state.inst.inst "noop")
        (+ state.x state.inst.arg)
        state.x)
   :cycle (+ state.cycle 1)})

(fn execute [state]
  (if (= state.cycle 239)
    state.pixels
    (let [state (advance-state state)]
      (table.insert
        state.pixels
        (if (overlap? state.x state.cycle) "#" "."))
    (execute state))))

(fn draw-pixels [pixels]
  (each [index value (ipairs pixels)]
    (io.write value)
    (when (= (% index 40) 0)
      (io.write "\n"))))

(local state
  {:inst {:inst "noop" :arg 0}
   :pixels []
   :x 1
   :cycle -1})
(draw-pixels (execute state))
