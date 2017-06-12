package main

import (
	"fmt"
	"os/exec"
	"strings"
	//"sync"
)

func exe_cmd(cmd string) {
	fmt.Println("command is ", cmd)
	// splitting head => g++ parts => rest of the command
	parts := strings.Fields(cmd)
	head := parts[0]
	parts = parts[1:len(parts)]

	out, err := exec.Command(head, parts...).Output()
	if err != nil {
		fmt.Printf("%s", err)
	}
	fmt.Printf("%s", out)
	//wg.Done() // Need to signal to waitgroup that this goroutine is done
}

func main() {
	//out, _ := exec.Command("sh", "-c", "date +\"%Y-%m-%d %H:%M:%S %Z\"").Output()
	//out, _ := exec.Command("sh", "-c", "./../parser ab ab ").Output()
	out, _ := exec.Command("sh", "-c", "./parser bv aaaa").Output()
	fmt.Printf("%s\n\n", out)
}
/*
#Python script making use of the regular expression library
def parser(regex, string):
    import subprocess
    from subprocess import call
    executable = "./parser"
    process = subprocess.Popen([executable, regex, string] ,stdout = subprocess.PIPE)

    (output, err) = process.communicate()
    exit_code = process.wait()

    if exit_code == 0:
        return output
    elif exit_code == 1:
        return "No match found!"
    else:
        return err

 */