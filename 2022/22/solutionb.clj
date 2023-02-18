(ns solution (:require [clojure.string]))

(def facing {\> 0 \v 1 \< 2 \^ 3})
(def moves {\> [0 1] \v [1 0] \< [0 -1] \^ [-1 0]})
(def directions
  {\> {"L" \^ "R" \v}
   \v {"L" \> "R" \<}
   \< {"L" \v "R" \^}
   \^ {"L" \< "R" \>}})
(def sides
	{[0 1] {\^ [[3 0] \< \>] \< [[2 0] \< \>]}
	 [0 2] {\^ [[3 0] \v \^] \> [[2 1] \> \<] \v [[1 1] \> \<]}
	 [1 1] {\> [[0 2] \v \^] \< [[2 0] \^ \v]}
	 [2 0] {\^ [[1 1] \< \>] \< [[0 1] \< \>]}
	 [2 1] {\> [[0 2] \> \<] \v [[3 0] \> \<]}
	 [3 0] {\> [[2 1] \v \^] \v [[0 2] \^ \v] \< [[0 1] \^ \v]}})

(defn read-board []
  (loop [line (read-line) board []]
    (if (clojure.string/blank? line) board
      (recur (read-line) (conj board (vec line))))))

(defn read-path []
  (let [path (read-line)]
    (map #(if (re-find #"\d+" %)
            (parse-long %) %)
         (re-seq #"\d+|[LR]" path))))

(defn find-start [board]
  (let [top-row (board 0)]
    [0 (count (take-while #(= % \space) top-row))]))

(defn recalculate-coords [[face-x face-y] [y x] origin-side dir]
  (let [min-y (* 50 face-x)
        max-y (dec (* 50 (inc face-x)))
        min-x (* 50 face-y)
        max-x (dec (* 50 (inc face-y)))
        pair  [dir origin-side]]
    (cond
      (= pair [\> \>]) [(- max-y (mod y 50)) max-x]
      (= pair [\> \v]) [max-y (+ min-x (mod y 50))]
      (= pair [\> \<]) [(+ min-y (mod x 50)) min-x]
      (= pair [\> \^]) [min-y (- max-x (mod y 50))]
      (= pair [\v \>]) [(+ min-y (mod x 50)) max-x]
      (= pair [\v \v]) [max-y (- max-x (mod x 50))]
      (= pair [\v \<]) [(- max-y (mod x 50)) min-x]
      (= pair [\v \^]) [min-y (+ min-x (mod x 50))]
      (= pair [\< \>]) [(+ min-y (mod y 50)) max-x]
      (= pair [\< \v]) [max-y (- max-x (mod y 50))]
      (= pair [\< \<]) [(- max-y (mod y 50)) min-x]
      (= pair [\< \^]) [min-y (+ min-x (mod y 50))]
      (= pair [\^ \>]) [(- min-y (mod x 50)) max-x]
      (= pair [\^ \v]) [max-y (+ min-x (mod x 50))]
      (= pair [\^ \<]) [(+ min-y (mod x 50)) min-x]
      (= pair [\^ \^]) [min-y (- max-x (mod x 50))])))

(defn wrap [board pos dir]
  (let [[face origin-side new-dir] (get-in sides [(mapv #(quot % 50) pos) dir])
        new-pos (recalculate-coords face pos origin-side dir)]
    (if (= \# (get-in board new-pos)) [pos dir] [new-pos new-dir])))

(defn move [board pos dir steps]
  (loop [step 0 pos pos dir dir]
    (if (= step steps) [pos dir]
      (let [new-pos (mapv + pos (moves dir))]
        (case (get-in board new-pos)
          \# (recur (inc step) pos dir)
          \. (recur (inc step) new-pos dir)
          (let [[wrapped-pos wrapped-dir] (wrap board pos dir)]
            (recur (inc step) wrapped-pos wrapped-dir)))))))

(defn walk [board path start]
  (loop [[p & ps] path dir \> pos start]
    (cond
      (nil? p) (conj pos (facing dir))
      (number? p) (let [[new-pos new-dir] (move board pos dir p)]
                    (recur ps new-dir new-pos))
      :else (recur ps (get-in directions [dir p]) pos))))

(defn score [row col facing]
  (+ (* (inc row) 1000) (* (inc col) 4) facing))

(let [board (read-board) path (read-path)]
  (->>
    (find-start board)
    (walk board path)
    (apply score)
    (println)))
