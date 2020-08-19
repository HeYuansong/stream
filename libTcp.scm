(load-shared-object "./libTcp.so")
(case (machine-type)
  [(i3le ti3le a6le ta6le) (load-shared-object "libc.so.6")]
  [(i3osx ti3osx a6osx ta6osx) (load-shared-object "libc.dylib")]
  [else (load-shared-object "libc.so")])
(define c-close (foreign-procedure "c_close" (int) int))
(define tcp-listen (foreign-procedure "tcp_listen" (int) int))
(define tcp-accept (foreign-procedure "tcp_accept" (int) int))
(define connect (foreign-procedure "tcp_connect" (string int) int))
(define is-socket-closed (foreign-procedure "isSocketClosed" (int) int))

(define c-read
  (foreign-procedure "c_read" (int)
    string))
(define c-write
  (foreign-procedure "c_write" (string int)
    ssize_t))
