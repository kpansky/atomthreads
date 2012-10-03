define run_atomthreads_test
  shell sleep 5
  disconnect
  shell sleep 1
  target remote :1234
  file build/$arg0.elf
  load
  printf "Running "
  echo $arg0
  printf "\n"
  printf "When test completes, break (Ctrl-C) or reset the system\n"
  c
end

set endian little
target remote :1234
run_atomthreads_test kern1
run_atomthreads_test kern2
run_atomthreads_test kern3
run_atomthreads_test kern4
run_atomthreads_test mutex1
run_atomthreads_test mutex2
run_atomthreads_test mutex3
run_atomthreads_test mutex4
run_atomthreads_test mutex5
run_atomthreads_test mutex6
run_atomthreads_test mutex7
run_atomthreads_test mutex8
run_atomthreads_test mutex9
run_atomthreads_test queue1
run_atomthreads_test queue2
run_atomthreads_test queue3
run_atomthreads_test queue4
run_atomthreads_test queue5
run_atomthreads_test queue6
run_atomthreads_test queue7
run_atomthreads_test queue8
run_atomthreads_test queue9
run_atomthreads_test queue10
run_atomthreads_test sem1
run_atomthreads_test sem2
run_atomthreads_test sem3
run_atomthreads_test sem4
run_atomthreads_test sem5
run_atomthreads_test sem6
run_atomthreads_test sem7
run_atomthreads_test sem8
run_atomthreads_test sem9
run_atomthreads_test timer1
run_atomthreads_test timer2
run_atomthreads_test timer3
run_atomthreads_test timer4
run_atomthreads_test timer5
run_atomthreads_test timer6
run_atomthreads_test timer7

q

