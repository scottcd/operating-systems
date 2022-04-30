files = main.c queue.c station_services.c file_services.c threads_services.c
flags = -g -pthread -o 
outputFile = bonserver 
deleteFiles = bonserver


$(outputFile): $(files)
	gcc $(flags) $(outputFile) $(files)
clean:
	rm $(deleteFiles)
