import glob
import os
import shutil

cwd = f"{os.getcwd()}/"

os.mkdir("template")
os.mkdir("template/romfs")
os.mkdir("template/source")
os.mkdir("template/source/dsge")
shutil.copy("examples/template/Makefile", "template/Makefile")
shutil.copy("examples/template/run.bat", "template/run.bat")
shutil.copy("examples/template/source/main.cpp", "template/source/main.cpp")

for file in glob.glob("source/**pp"):
    file2 = "template/" + file.replace("\\", "/dsge/")
    shutil.copy(file, file2)