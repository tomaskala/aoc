(use-modules (ice-9 rdelim))

(define shapes
  '(((0 . 0) (1 . 0) (2 . 0) (3 . 0))

    (        (1 . 2)
     (0 . 1) (1 . 1) (2 . 1)
             (1 . 0)               )

    (                (2 . 2)
                     (2 . 1)
     (0 . 0) (1 . 0) (2 . 0)       )

    ((0 . 3)
     (0 . 2)
     (0 . 1)
     (0 . 0)                       )

    ((0 . 1) (1 . 1)
     (0 . 0) (1 . 0)               )))

(define convert-direction
  (lambda (dir)
    (cond
      ((eqv? dir #\<) -1)
      ((eqv? dir #\>) +1)
      (else (error "Unexpected direction" dir)))))

(define translate
  (lambda (rock dx dy)
    (map (lambda (p) (cons (+ (car p) dx) (+ (cdr p) dy))) rock)))

(define all
  (lambda (l)
    (or
      (null? l)
      (and (car l) (all (cdr l))))))

(define can-place-rock?
  (lambda (cave rock)
    (and
      (all (map (lambda (p) (and (>= (car p) 0) (<= (car p) 6))) rock))
      (all (map (lambda (p) (>= (cdr p) 0)) rock))
      (all (map (lambda (p) (not (hash-ref cave p))) rock)))))

(define push-rock
  (lambda (jet rock)
    (translate rock jet 0)))

(define fall-rock
  (lambda (rock)
    (translate rock 0 -1)))

(define place-rock
  (lambda (cave rock)
    (if (null? rock)
      cave
      (begin
        (hash-set! cave (car rock) #t)
        (place-rock cave (cdr rock))))))

(define drop-rock
  (lambda (cave jets rock)
    (let*
      ((pushed-rock (push-rock (car jets) rock))
       (next-rock (if (can-place-rock? cave pushed-rock) pushed-rock rock))
       (fallen-rock (fall-rock next-rock)))
      (if (can-place-rock? cave fallen-rock)
        (drop-rock cave (cdr jets) fallen-rock)
        (cons (place-rock cave next-rock) (cdr jets))))))

(define find-top
  (lambda (cave)
    (+ 1 (hash-fold (lambda (k v acc) (max (cdr k) acc)) -1 cave))))

(define spawn-rock
  (lambda (cave rock)
    (let ((max-y (find-top cave)))
      (translate rock 2 (+ 3 max-y)))))

(define simulate
  (lambda (n cave jets rocks)
    (if (= n 0)
      cave
      (let*
        ((spawned-rock (spawn-rock cave (car rocks)))
         (cave-jets (drop-rock cave jets spawned-rock)))
        (simulate (- n 1) (car cave-jets) (cdr cave-jets) (cdr rocks))))))

(let*
  ((input-line (string->list (read-line)))
   (directions (map convert-direction input-line)))
  (begin
    (set-cdr! (last-pair directions) directions)
    (set-cdr! (last-pair shapes) shapes)
    (display (find-top (simulate 2022 (make-hash-table) directions shapes)))
    (newline)))
