import subprocess

mingw_x64 = "mingw-w64"
pip = "req.txt"
subprocess.run(["sudo", "apt", "install", "-y", mingw_x64], check=True)
subprocess.run(["pip", "install", "-r", pip], check=True)
