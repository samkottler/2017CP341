#! /usr/bin/python3
from subprocess import call

sbatchDir = "./sbat/"
outDir = "./out/"

def doTest(type,c,p,num):
    filename = sbatchDir+type+","+str(c)+","+str(p)+"("+str(num)+").sbatch"
    text = "#! /bin/bash\n#SBATCH --nodes=1\n#SBATCH --exclude=node[0-1]\n#SBATCH --output="+outDir+type+","+str(c)+","+str(p)+"("+str(num)+").txt\n\n./bin/mandelbrot_"+type+" " + str(c) + " " + str(p)+"\n"
    f = open(filename,"w")
    f.write(text)
    f.close()
    call(["sbatch",filename])

def doMPI(nodes,c,p,num):
    filename = sbatchDir+"mpi"+str(nodes)+","+str(c)+","+str(p)+"("+str(num)+").sbatch"
    text = "#! /bin/bash\n#SBATCH --nodes="+str(nodes)+"\n#SBATCH --ntasks="+str(nodes)+"\n#SBATCH --exclude=node[0-1]\n#SBATCH --output="+outDir+"mpi"+str(nodes)+","+str(c)+","+str(p)+"("+str(num)+").txt\n\n./bin/mpi " + str(c) + " " + str(p)+"\n"
    f = open(filename,"w")
    f.write(text)
    f.close()
    call(["sbatch",filename])

if __name__ == "__main__":
    call(["mkdir", "-p", sbatchDir])
    call(["mkdir", "-p", outDir])
    for c in range(1000,2000,1000):
        for p in range(100000,120000,20000):
            for i in range(1):
                doMPI(1,c,p,i)
                doMPI(2,c,p,i)
                for type in ["serial","cilk","cilk2","openmp","nonrecursive","nonrecursive2"]:
                    doTest(type,c,p,i)
