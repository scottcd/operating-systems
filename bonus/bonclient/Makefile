files = main.c file_services.c threads_services.c queue.c
flags = -g -pthread -o 
outputFile = bonclient 
deleteFiles = bonclient order-files/out order-files/out1 order-files/out2 order-files/out3


$(outputFile): $(files)
	gcc $(flags) $(outputFile) $(files)
clean:
	rm $(deleteFiles)
