files = order_processing.c file_services.c station_services.c pipeline_services.c threads_services.c
flags = -g -pthread -o 
outputFile = hw4 
deleteFiles = hw4 order-files/out order-files/out1 order-files/out2 order-files/out3


$(outputFile): $(files)
	gcc $(flags) $(outputFile) $(files)
clean:
	rm $(deleteFiles)
