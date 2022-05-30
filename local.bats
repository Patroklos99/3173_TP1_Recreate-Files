#!/usr/bin/env bats

load test_helper

@test "gratuit" {
	true
}

@test "nb_arguments_invalide" {
        run bash -c "test -d tests/ici && rm -fr tests/ici"
        run mkdir tests/ici
        run ./wall-e tests/reg.stat tests/ici john
        [ "$status" -eq 1 ]
}

@test "fichier_non_existant" {
        run bash -c "test -d tests/ici && rm -fr tests/ici"
        run mkdir tests/ici
        run bash -c "echo bijour li wourld > tests/ici/biblo_le_hobit"
        run ./wall-e tests/notext.stat tests/ici
        [ "$status" -eq 1 ]
        check 1 ""
}

