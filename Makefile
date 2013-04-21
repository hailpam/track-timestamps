SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
DST_DIR = dist
TST_DIR = test

CC      = g++
CCFLAGS  = -I${INC_DIR}
LDFLAGS = -lrt

TARGET  = tracktimestamps
TEST    = test_tracktimestamps
DIST	= libtracktimestamps.a

all: ${TARGET}.o distributable  ${TEST}.o ${TEST}

${TEST}: ${OBJ_DIR}/${TEST}.o ${OBJ_DIR}/${TARGET}.o
	$(CC) -o $@ $^ -L${DST_DIR} $(LDFLAGS) -ltracktimestamps

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
	rm ${TEST}
	rm *.dat
	rm *.png

plot:
	gnuplot -e "set term png; set output 'times.png'; plot 'times.dat'"

displayplot:
	eog times.png
