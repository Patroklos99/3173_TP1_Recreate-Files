all: wall-e

check: wall-e
	bats check.bats
clean:
	rm wall-e
