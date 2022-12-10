(fn decode-inst [inst-line]
  (match (inst-line:match "(%a+) ?(%-?%d*)")
    ("addx" arg) {:inst "addx" :arg (tonumber arg)}
    "noop" {:inst "noop" :arg 0}))

(fn fetch-inst [prev-inst]
  (match prev-inst
    {:inst "noop" :arg _} (decode-inst (io.read "l"))
    {:inst "addx" :arg arg} {:inst "noop" :arg arg}))

(fn advance-state [state]
  {:inst (fetch-inst state.inst)
   :check (if (= state.cycle state.check)
            (+ state.check 40)
            state.check)
   :strength (if (= state.cycle state.check)
               (+ state.strength (* state.cycle state.x))
               state.strength)
   :x (if (= state.inst.inst "noop")
               (+ state.x state.inst.arg)
               state.x)
   :cycle (+ state.cycle 1)})

(fn execute [state]
  (if (= state.cycle 221)
    state.strength
    (execute (advance-state state))))

(local state
  {:inst {:inst "noop" :arg 0}
   :check 20
   :strength 0
   :x 1
   :cycle 0})
(print (execute state))
