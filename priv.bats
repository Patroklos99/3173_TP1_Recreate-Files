#!/usr/bin/env bats

load test_helper

@test "priv-vide" {
	run bash -c "test -d tests/la-bas && rm -fr tests/la-bas"
	run touch tests/walou.stat
	run ./wall-e tests/walou.stat tests/la-bas
	check 0 ""
	run bash -c "ls tests/la-bas | wc -l"
	check 0 "0"
}

@test "priv-file" {
	run bash -c "test -d tests/la-bas && rm -fr tests/la-bas"
	run ./wall-e tests/file-full.stat tests/la-bas
	check 0 ""
	run stat -c "%A %s %N" tests/la-bas/*
	checki 0 <<FIN
-rw-r--r-- 41600 'tests/la-bas/beaucoup'
-rw-r--r-- 2 'tests/la-bas/deux'
-rw-r--r-- 4096 'tests/la-bas/trois'
-rw-r--r-- 1 'tests/la-bas/un'
FIN
}

@test "priv-idem-file-bis" {
	run bash -c "test -d tests/la-bas && rm -fr tests/la-bas"
	run ./wall-e tests/idem-reg.stat tests/la-bas
	check 0 ""
	run bash -c "ls tests/la-bas | wc -l"
	check 0 "1"
}

@test "priv-idem-file" {
	run bash -c "test -d tests/la-bas && rm -fr tests/la-bas"
	run ./wall-e tests/idem-reg.stat tests/la-bas
	check 0 ""
	run stat -c "%A %s %N" tests/la-bas/*
	checki 0 <<FIN
-rw-r--r-- 3173 'tests/la-bas/inf'
FIN
}

@test "priv-rep" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/rep.stat tests/ici/la-bas/puis/ici
	check 1 ""
}

@test "priv-symlink" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run mkdir tests/ici
	run bash -c "echo 'Traitement des traumatismes sans gravité, comme les égratignures et les bleus.' > tests/ici/bouboulogie"
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/*
	checki 0 <<FIN 
-rw-r--r-- 81 'tests/ici/bouboulogie'
lrwxrwxrwx 11 'tests/ici/lien' -> 'bouboulogie'
FIN
}

@test "priv-fich-fi" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run ./wall-e tests/reg.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/biblo_le_hobit 
	checki 0 <<FIN 
-rw-r--r-- 3173 'tests/ici/biblo_le_hobit'
FIN
}

@test "priv-sym-fi" {
	run bash -c "test -d tests/ici && rm -fr tests/ici"
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run ./wall-e tests/symlink.stat tests/ici
	check 0 ""
	run stat -c "%A %s %N" tests/ici/lien
	checki 0 <<FIN 
lrwxrwxrwx 11 'tests/ici/lien' -> 'bouboulogie'
FIN
}
