test : queue_test PriorityQueue.hh
	./queue_test

queue_test : queue_test.cc PriorityQueue.hh
	g++ queue_test.cc -o queue_test

clean:
	rm -rf *.o *~

