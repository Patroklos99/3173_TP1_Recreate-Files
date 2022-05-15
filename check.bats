#!/usr/bin/env bats

load test_helper

@test "fichier" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/biblo_le_hobit 
	checki 0 <<FIN 
-rw-r--r-- 3173 'tests/ici/biblo_le_hobit'
FIN
}

@test "fichier-exists" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run mkdir tests/ici
	run bash -c "echo bijour li wourld > tests/ici/biblo_le_hobit"
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/biblo_le_hobit 
	checki 0 <<FIN 
-rw-r--r-- 3173 'tests/ici/biblo_le_hobit'
FIN
}

@test "symlink" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/lien
	checki 0 <<FIN 
lrwxrwxrwx 11 'tests/ici/lien' -> 'bouboulogie'
FIN
}

@test "symlink-exists" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run mkdir tests/ici
	run ln -s nada tests/ici/lien
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/lien
	checki 0 <<FIN 
lrwxrwxrwx 11 'tests/ici/lien' -> 'bouboulogie'
FIN
}

@test "dir" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/rep.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/solution
	checki 0 <<FIN 
drwxr-xr-x 4096 'tests/ici/solution'
FIN
}

@test "dir-exists" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run mkdir -p tests/ici/solution
	run ./wall-e tests/rep.stat tests/ici
	check 1 ""
}

@test "full" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/output.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/*
	checki 0 <<FIN
-rw-r--r-- 416 'tests/ici/biblo.c'
lrwxrwxrwx 6 'tests/ici/lien' -> 'loulou'
-rw-r--r-- 0 'tests/ici/loulou'
drwxr-xr-x 4096 'tests/ici/rep'
FIN
}
