files = order_processing.c file_services.c station_services.c pipeline_services.c
flags = -g -o
outputFile = hw3 
deleteFiles = hw3 order-files/out order-files/out1 order-files/out2 order-files/out3


$(outputFile): $(files)
	gcc $(flags) $(outputFile) $(files)
clean:
	rm $(deleteFiles)
