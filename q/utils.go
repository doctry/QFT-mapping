package main

import "log"

func checkAssign[T any](value T, err error) T {
	if err != nil {
		log.Fatalln(err)
	}

	return value
}
