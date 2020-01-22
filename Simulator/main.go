package main

import (
	"fmt"
	"os"
	"os/exec"
	"time"
)

//type wind struct {
//	direction string
//}
type Car struct {
	yaw string
}

var car Car
var direction = "NORTH"

func main() {
	go changeWindDir()
	go controller()

	for {
		fmt.Printf("WindDir: %s, TurbineDir: %s.\n", direction, car.yaw)
		time.Sleep(5 * time.Second)
	}
}

func getWindDir() string {
	return direction
}

func controller() {
	for {
		if car.yaw != getWindDir() {
			fmt.Println("Yaw not in wind!")
			time.Sleep(6 * time.Second)
			/* Call change yaw task */
			car.yaw = getWindDir()
		}
	}
}

func changeWindDir() {
	// disable input buffering
	exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1").Run()
	// do not display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "-echo").Run()

	var b []byte = make([]byte, 1)
	for {
		os.Stdin.Read(b)
		switch b[0] {
		case byte('A'):
			{
				direction = "NORTH"
				fmt.Println("NORTH")
			}

		case byte('B'):
			{
				direction = "SOUTH"
				fmt.Println("SOUTH")
			}
		case byte('C'):
			{
				direction = "EAST"
				fmt.Println("EAST")
			}
		case byte('D'):
			{
				direction = "WEST"
				fmt.Println("WEST")
			}
		}
	}
}
