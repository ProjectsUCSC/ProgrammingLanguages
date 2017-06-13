# RegularExpressionParser
An implementation of a regular exression parser, which checks whether a given string matches a given regular expression or not. This regular expression metacharacters follow the POSIX standards.

An implementation of a regex parser accepting POSIX based regular expressions as given here http://jlsoper.tripod.com/regExp/Use_Regular_Expressions.html as input.

Instructions for running the C parser:

1. gcc parser.c
2. ./a.out
3. Enter the regular expression,
4. Enter the test string,

Instructions for running the Python Interactive Parser 
1. python3 interactive_parser.py


Instructions for running the Scala Interactive Parser 
1.  cd scala/src
2.  pass the full path to parser.c as a commandline argument :
    eg : scala Parser.scala /Users/shubhi/Public/ProgrammingLanguages/parser
    
Instructions for running the Swift Parser
1. swift parser_swift.swift
2. To check for different inputs, open parser_swift.swift in an editor and pass the regex and the string to "shell".
   eg: shell("./parser","a+bc", "aabc"), where a+bc is the regex and aabc is the test string.
   
Instructions for running the GO Parser
1. Create a workspace for GO
2. Create a folder named src in the workspace
3. Create a folder "parser_go" in the src folder
4. Copy the parser_go.go provided in the main folder to the parser_go folder
5. Open parser_go.go in an editor and change the path of the compiled parser on line 10
   eg: command := "/Users/avaniarora/Desktop/ProgrammingLanguages/./parser" + " " + regex + " " + text
6. After saving the changes, on the terminal 
    a) go build  //compiles the go program
    b) ./parser_go


