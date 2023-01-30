SJF:
	g++ SJF.cpp -o SJF.exe
	./SJF.exe ./test/process1.dat
	./SJF.exe ./test/process2.dat
	./SJF.exe ./test/process3.dat
	./SJF.exe ./test/process_SJF_fine.dat
	./SJF.exe ./test/process_SJF_bad.dat
RR:
	g++ RR.cpp -o RR.exe
	./RR.exe ./test/process1.dat
	./RR.exe ./test/process2.dat
	./RR.exe ./test/process3.dat
	./RR.exe ./test/process_RR_fine.dat
	./RR.exe ./test/process_RR_bad.dat

clean:
	if [ '-f *.exe' ] ; then rm -r *.exe; fi
