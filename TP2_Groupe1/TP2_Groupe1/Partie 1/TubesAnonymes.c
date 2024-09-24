#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int main() {
    
    

    if(fork() == 0){ //P3
        int fd1[2];
        pipe(fd1);

        if(fork() == 0){ //P2
            
            int fd2[2];
            pipe(fd2);

            if(fork() == 0){ //P1
                close(fd2[0]);
                
                dup2(fd2[1], 1);

                execlp("cat", "cat", "In.txt", NULL);
            }


            close(fd2[1]);
            close(fd1[0]);

            dup2(fd2[0], 0);
            dup2(fd1[1], 1);

            execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
        }


        close(fd1[1]);
        dup2(fd1[0], 0);

        int output = open("Out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);

        dup2(output, 1);
        printf("test\n");



        execlp("diff", "diff", "-", "In.txt", NULL);
    }

    wait(NULL);



    return(0);
}
