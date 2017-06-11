from parser_python import  *

def main():
    answer = 'y'
    while(answer == 'y'):
        regex = str(input("Enter the regex to be evaluated "))
        print(type(regex))
        string = str(input("Enter the input string "))
        parser(regex, string)

        answer = str(input("Do you want to continue? (y/N)"))

main()
