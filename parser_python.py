#Python script making use of the regular expression library
def parser(regex, string):
    import subprocess
    from subprocess import call
    executable = "./parser"
    process = subprocess.Popen([executable, regex, string] ,stdout = subprocess.PIPE)
    
    (output, err) = process.communicate()
    exit_code = process.wait()

    if exit_code == 0:
        return output.decode('UTF-8')
    elif exit_code == 1:
        return "No match found!"
    else:
        return err
