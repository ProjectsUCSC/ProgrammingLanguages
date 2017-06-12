package main

import (
	"fmt"
	"os/exec"
     //	"strings"
	//"sync"
)
func parser(regex string, text string) (out []byte,_ int){
	command := "/Users/avaniarora/Desktop/ProgrammingLanguages/./parser" + " " + regex + " " + text
	out, _ = exec.Command("sh", "-c",command).Output() //"/Users/avaniarora/Desktop/ProgrammingLanguages/./parser" + " " + regex + " " + text).Output()

//	fmt.Printf("%s\n",out)
	return
}

func main() {
	
	output,status := parser("aa", "aaa")
	if(status == 0){
		fmt.Printf("Matched with %s\n",output)
	} else{
		fmt.Printf("No Match\n")
	}
}
