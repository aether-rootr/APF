#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    char *path;
    if (argc >= 2) {
      path = argv[1];
    } else {
      path = "./a";
    }
    int cpu_time_used, memory_used, signum;
    pid_t pid;

    pid = fork();
    std::chrono::system_clock::time_point start, end;
    if (pid > 0) {
      start = std::chrono::system_clock::now();
    }
    if(pid < 0){
        printf("fork error.\n");
        return -1;
    }else if(pid == 0){
        printf("========Program output========\n");
        execl(path, NULL);
    }else{
        int status = 0;
        struct rusage ru;
        wait4(pid, &status, 0, &ru);
        end = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cpu_time_used =
                ru.ru_utime.tv_sec * 1000 + ru.ru_utime.tv_usec / 1000 +
                ru.ru_stime.tv_sec * 1000 + ru.ru_stime.tv_usec / 1000;

        memory_used = ru.ru_maxrss;
        
        if (WIFEXITED(status)) {
          signum = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
          signum = WTERMSIG(status);
        } else if (WIFSTOPPED(status)) {
          signum = WSTOPSIG(status);
        } else {
          signum = 0;
        }


        printf("==============================\n");

        printf("time used:     %lf ms\n", double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den * 1000);
        printf("cpu time used: %6d ms\n", cpu_time_used);
        printf("memory used:   %6d kb\n", memory_used);
        printf("exit code:     %6d\n", signum);
    }
    return 0;
}