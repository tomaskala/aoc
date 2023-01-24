(use-modules (ice-9 hash-table))
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

(define step
  (lambda (cave jets rocks)
    (let*
      ((spawned-rock (spawn-rock cave (car rocks)))
       (dropped-state (drop-rock cave jets spawned-rock)))
      (list
        (car dropped-state)
        (cdr dropped-state)
        (cdr rocks)))))

(define profile
  (lambda (cave)
    (let*
      ((keys (hash-fold (lambda (k v acc) (cons k acc)) '() cave))
       (xs (iota 7 0))
       (groups (map (lambda (x) (filter (lambda (p) (= x (car p))) keys)) xs))
       (ys (map (lambda (ps) (map cdr ps)) groups))
       (non-empty-ys (map (lambda (group) (if (null? group) '(0) group)) ys))
       (top-ys (map (lambda (group) (apply max group)) non-empty-ys))
       (top (find-top cave)))
      (map (lambda (y) (- y top)) top-ys))))

(define states-equal?
  (lambda (x y)
    (let
      ((profile0 (profile (car x)))
       (profile1 (profile (car y)))
       (jets0 (cadr x))
       (jets1 (cadr y))
       (rocks0 (caddr x))
       (rocks1 (caddr y)))
      (and (equal? jets0 jets1)
           (equal? (car rocks0) (car rocks1))
           (equal? profile0 profile1)))))

(define tortoise-hare0
  (lambda (f jets rocks)
    (letrec
      ((x0 (make-state jets rocks))
       (x1 (make-state jets rocks))
       (go (lambda (tortoise hare)
             (if (states-equal? tortoise hare) hare
               (go (apply f tortoise) (apply f (apply f hare)))))))
      (go (apply f x0) (apply f (apply f x1))))))

(define tortoise-hare1
  (lambda (f jets rocks h)
    (letrec
      ((x0 (make-state jets rocks))
       (go (lambda (mu tortoise hare)
             (if (states-equal? tortoise hare)
               (cons mu tortoise)
               (go (+ 1 mu) (apply f tortoise) (apply f hare))))))
      (go 0 x0 h))))

(define tortoise-hare2
  (lambda (f t)
    (letrec ((go (lambda (lam hare)
                   (if (states-equal? t hare) lam
                     (go (+ 1 lam) (apply f hare))))))
      (go 1 (apply f t)))))

(define make-state
  (lambda (jets rocks)
    (list (make-hash-table) jets rocks)))

(define clone-state
  (lambda (cave jets rocks)
    (list (alist->hash-table (hash-map->list cons cave)) jets rocks)))

(define find-cycle
  (lambda (jets rocks)
    (let*
      ((hare (tortoise-hare0 step jets rocks))
       (mu-tortoise (tortoise-hare1 step jets rocks hare))
       (lam (tortoise-hare2 step (cdr mu-tortoise))))
        (cons lam (- (car mu-tortoise) 1)))))

(define simulate
  (lambda (n . x0)
    (if (= n 0) x0 (apply simulate (- n 1) (apply step x0)))))

(define find-solution
  (lambda (jets rocks)
    (let*
      ((n 1000000000000)
       (lam-mu (find-cycle jets rocks))
       (rem (remainder (- n (cdr lam-mu)) (car lam-mu)))
       (quot (quotient (- n (cdr lam-mu)) (car lam-mu)))
       (x1 (apply simulate (cdr lam-mu) (make-state jets rocks)))
       (x2 (apply simulate (car lam-mu) (apply clone-state x1)))
       (x3 (apply simulate rem (apply clone-state x1))))
      (+
        (* quot (- (find-top (car x2)) (find-top (car x1))))
        (find-top (car x3))))))

(let*
  ((input-line (string->list (read-line)))
   (directions (map convert-direction input-line)))
  (begin
    (set-cdr! (last-pair directions) directions)
    (set-cdr! (last-pair shapes) shapes)
    (display (find-solution directions shapes))
    (newline)))
