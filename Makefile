SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
DST_DIR = dist
TST_DIR = test
BIN_DIR = bin
LOG_DIR = logs

CC      = g++
CCFLAGS  = -I${INC_DIR}
LDFLAGS = -lrt

TARGET  = tracktimestamps
TEST    = test_tracktimestamps
DIST	= libtracktimestamps.a

all: ${TARGET}.o distributable  ${TEST}.o ${TEST}

${TEST}: ${OBJ_DIR}/${TEST}.o ${OBJ_DIR}/${TARGET}.o
	$(CC) -o $@ $^ -L${DST_DIR} $(LDFLAGS) -ltracktimestamps
	mv ${TEST} ${BIN_DIR}/

${TARGET}.o: ${SRC_DIR}/${TARGET}.cpp ${INC_DIR}/${TARGET}.h
	$(CC) -Wall -c $(CCFLAGS) $<
	mv *.o ${OBJ_DIR}/

${TEST}.o: ${TST_DIR}/${TEST}.cpp ${OBJ_DIR}/${TARGET}.o
	$(CC) -Wall -c $(CCFLAGS) $<
	mv *.o ${OBJ_DIR}/

distributable:
	ar -cvq ${DIST} ${OBJ_DIR}/${TARGET}.o
	mv *.a ${DST_DIR}/

clean:
	rm ${OBJ_DIR}/*.o
	rm ${DST_DIR}/*.a

cleantest: clean
	rm ${BIN_DIR}/${TEST}
	rm *.dat
	rm ${LOG_DIR}/*.log
	rm *.png

plot:
	gnuplot -e "set term png; set output 'times.png'; set autoscale; set title 'Measured Times'; set xlabel 'Test Nr'; set ylabel 'Time (ns)';  plot 'times.dat' with linespoints"

displayplot: plot
	eog times.png

runtest:
	valgrind --tool=memcheck --leak-check=yes --log-file=${TEST}-valgrind.log  -v ${BIN_DIR}/${TEST}
	mv ${TEST}-valgrind.log ${LOG_DIR}/

runreapetedtest:
	for x in 1 2 3 4 5 ; do \
		${BIN_DIR}/${TEST} 10 ; \
	done
