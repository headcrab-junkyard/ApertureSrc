Multithreading support

	* Job manager - detect hardware concurrency and set max number of threads
	* Set max jobs count
	* threads will get jobs and do it
	* thread: getjob->dojob->getjob->dojob (in loop for each thread)
	
	* dedicated thread for rendering? or render in set of available threads?