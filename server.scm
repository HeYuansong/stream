(load "libTcp.scm")
(define (message-loop cfd)
  (if (= (is-socket-closed cfd) 1)
      (display "client exit\n")
      (let* ([buf (c-read cfd)])
	(display "recvd : ") (display buf) (newline)
	(if (string=? buf  "ping") (c-write "pong" cfd))
	(if (string=? buf "hello")
	    (c-write "world" cfd)
	    (c-write "none" cfd))
	(message-loop cfd))))

(define (connect-loop lfd)
  (let ([cfd (tcp-accept lfd)])
    (display "connect success !\n")
    (fork-thread (lambda () (message-loop cfd)))
    (connect-loop lfd)))

(define lfd (tcp-listen 8080))

(display "wait port : 8080\n")

(connect-loop lfd)
