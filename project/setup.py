#! /usr/bin/python3
from subprocess import call

sbatchDir = "./sbat/"
outDir = "./out/"

def doTest(type,c,p,num):
    filename = sbatchDir+type+str(c)+","+str(p)+"("+str(num)+").sbatch"
    text = "#! /bin/bash\n#SBATCH --nodes=1\n#SBATCH --output="+outDir+type+str(c)+","+str(p)+"("+str(num)+").txt\n\n./bin/mandelbrot_serial " + str(c) + " " + str(p)+"\n"
    f = open(filename,"w")
    f.write(text)
    f.close()
    call(["sbatch",filename])
    

if __name__ == "__main__":
    call(["mkdir", "-p", sbatchDir])
    call(["mkdir", "-p", outDir])
    c = 100
    p = 10000
    type = "serial"
    doTest(type,c,p,0)
