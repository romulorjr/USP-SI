#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int main(void) {
    int pid;
    int fork_count, getpid_count, write_count;

    printf(1, "--- Teste da syscall getsyscallcount ---\n");

    // Chama getpid() algumas vezes
    getpid();
    getpid();

    getpid_count = getsyscallcount(SYS_getpid);
    printf(1, "Pai: getpid() foi chamado %d vezes.\n", getpid_count);

    pid = fork();

    if (pid == 0) { // Processo Filho
        printf(1, "\n--- No processo filho ---\n");

        fork_count = getsyscallcount(SYS_fork);
        getpid_count = getsyscallcount(SYS_getpid);
        write_count = getsyscallcount(SYS_write);

        printf(1, "Filho: fork() foi chamado %d vezes.\n", fork_count);
        printf(1, "Filho: getpid() foi chamado %d vezes.\n", getpid_count);
        printf(1, "Filho: write() foi chamado %d vezes.\n", write_count);

        // A chamada a printf acima conta como uma chamada a write.
        // Vamos chamar getpid() no filho.
        getpid();
        getpid_count = getsyscallcount(SYS_getpid);
        printf(1, "Filho: apos chamar getpid(), a contagem eh %d.\n", getpid_count);

        exit();
    } else { // Processo Pai
        wait(); // Espera o filho terminar

        printf(1, "\n--- De volta ao processo pai ---\n");

        fork_count = getsyscallcount(SYS_fork);
        getpid_count = getsyscallcount(SYS_getpid);

        printf(1, "Pai: fork() foi chamado %d vezes.\n", fork_count);
        printf(1, "Pai: getpid() ainda foi chamado %d vezes.\n", getpid_count);
    }

    exit();
}