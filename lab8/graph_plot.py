import subprocess

proc = subprocess.Popen(["./LRU.out", "60", "20", "60", "req1.dat"],stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout, stderr = proc.communicate()

if proc.returncode != 0:
    print("Execution failed:", stderr.decode())
else:
    print("Output:", stdout.decode())
