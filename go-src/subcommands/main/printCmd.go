package main

import (
	"context"
	"flag"
	"github.com/google/subcommands"
)

type printCmd struct{

}

// Name returns the name of the command.
func(p *printCmd)Name()string{
	return "printCmd"
}



// Synopsis returns a short string (less than one line) describing the command.
func(p *printCmd)Synopsis() string{
	return "cmd"
}

// Usage returns a long string explaining the command and giving usage
// information.
func(p *printCmd)Usage() string{
	return "this is printCmd command"
}

// SetFlags adds the flags for this command to the specified set.
func(p *printCmd)SetFlags(set *flag.FlagSet){

}

// Execute executes the command and returns an ExitStatus.
func(p *printCmd)Execute(ctx context.Context, f *flag.FlagSet, args ...interface{}) subcommands.ExitStatus{
	return 0
}