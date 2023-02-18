(ns solution (:require [clojure.string]))

(def facing {\> 0 \v 1 \< 2 \^ 3})
(def moves {\> [0 1] \v [1 0] \< [0 -1] \^ [-1 0]})
(def directions
  {\> {"L" \^ "R" \v}
   \v {"L" \> "R" \<}
   \< {"L" \v "R" \^}
   \^ {"L" \< "R" \>}})

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

(defn get-y [board start delta x]
  (loop [y start]
    (if (= (get-in board [y x]) \space)
      (recur (+ y delta))
      y)))

(defn wrap [board pos dir]
  (let [[y x] pos
        new-pos (case dir
                  \> [y (count (take-while #(= % \space) (board y)))]
                  \v [(get-y board 0 1 x) x]
                  \< [y (dec (count (board y)))]
                  \^ [(get-y board (dec (count board)) -1 x) x])]
    (if (= \# (get-in board new-pos)) pos new-pos)))

(defn move [board pos dir steps]
  (let [delta (moves dir)]
    (loop [step 0 pos pos]
      (if (= step steps) pos
        (let [new-pos (mapv + pos delta)]
          (case (get-in board new-pos)
            \# (recur (inc step) pos)
            \. (recur (inc step) new-pos)
            (recur (inc step) (wrap board pos dir))))))))

(defn walk [board path start]
  (loop [[p & ps] path dir \> pos start]
    (cond
      (nil? p) (conj pos (facing dir))
      (number? p) (recur ps dir (move board pos dir p))
      :else (recur ps (get-in directions [dir p]) pos))))

(defn score [row col facing]
  (+ (* (inc row) 1000) (* (inc col) 4) facing))

(let [board (read-board) path (read-path)]
  (->>
    (find-start board)
    (walk board path)
    (apply score)
    (println)))
