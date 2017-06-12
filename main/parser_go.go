package main

import (
	"fmt"
	"os/exec"
	"strings"
	//"sync"
)

func parser(regex string, text string) (out string){

	out, _ := exec.Command("sh", "-c", "./parser" + " " + regex + " " + text).Output()
    return
}

func main() {
	
	out = parser("a+bc", "aaabc")
	fmt.Printf("%s\n\n", out)
}
