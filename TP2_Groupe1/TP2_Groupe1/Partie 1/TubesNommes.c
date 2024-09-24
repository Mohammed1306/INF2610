#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>


int main() {
    mkfifo("tube23", 0060);
    mkfifo("tube12", 0060);
    
    if(fork() == 0){ //P3
        if(fork() == 0){ //P2
            if(fork() == 0){ //P1
                dup2(open("tube12", O_WRONLY), 1);
                execlp("cat", "cat", "In.txt", NULL);
            }

            dup2(open("tube12", O_RDONLY), 0);
            dup2(open("tube23", O_WRONLY), 1);

            execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
        }

        dup2(open("tube23", O_RDONLY), 0);

        int output = open("Outn.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);

        dup2(output, 1);

        execlp("diff", "diff", "-", "In.txt", NULL);
    }

    wait(NULL);



    return(0);
}
