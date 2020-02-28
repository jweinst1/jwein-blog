from __future__ import print_function
import sys
import os
from subprocess import check_output, CalledProcessError

if __name__ == '__main__':
    pass
    user_args = sys.argv[1:]
    if len(user_args) != 1:
        print("Need name of the C file")
        sys.exit(1)
    c_file = user_args[0]
    exec_name = c_file.split(".")[0]
    compile_args = ["cl", c_file]
    run_args = [exec_name + ".exe"]
    
    try:
        compile_output = check_output(compile_args)
        print("Got the output from compiler: " + compile_output)
    except CalledProcessError as exc:
        print("Got compiling error with output: " + exc.output);
        sys.exit(3)
    except Exception as exc:
        print("Got unkown error: " + str(exc))
        sys.exit(2)
        
    print("Running compiled executable....")
    print(check_output(run_args))
    print("Removing result files now...")
    os.remove(exec_name + ".exe")
    os.remove(exec_name + ".obj")
    print("Compilation completed!")
    