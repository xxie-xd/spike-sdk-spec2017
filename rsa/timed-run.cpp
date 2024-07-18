// run a benchmark with limited number of retired instructions

// CSR_CYCLE 0xc00
// CSR_TIME 0xc01
// CSR_INSTRET 0xc02

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define read_cycle()   read_csr(0xc00)
#define read_time()    read_csr(0xc01)
#define read_instret() read_csr(0xc02)

#include <chrono>
#include <iostream>
#include <thread>
#include <cstdint>
#include <spawn.h>
#include <sys/wait.h>

#include "flexicas/flexicas-pfc.h"

class PFCRecord {
public:
  void start() { write_csr(0x8F0, FLEXICAS_PFC_START); }
  void end() { write_csr(0x8F0, FLEXICAS_PFC_STOP); }
};

static PFCRecord pfc;

using namespace std::chrono_literals;

void run_cmd(char *argv[], char* envp[], uint64_t max_instret) {
  pid_t pid;
  int rv = posix_spawnp(&pid, argv[0], NULL, NULL, argv, envp);

  if(rv) {
    if(rv == ENOSYS) {
      std::cerr << "posix_spawn() is NOT supported in this system!" << std::endl;
      exit(1);
    } else exit(rv);
  }

  uint64_t instret_start = read_instret();
  uint64_t instret_now = instret_start;
  //std::cerr << "[timed-run] " << read_time() << ": successfully started with initial reading of " << instret_start << " instructions." << std::endl;
  pfc.start();
  int s, status;
  while(max_instret == 0 || instret_now - instret_start < max_instret) {
    s = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
    if(0 == s) { // child is running
      std::this_thread::sleep_for(100ms);
      instret_now = read_instret();
      //std::cerr << "[timed-run] " << read_time() << ": successfully run " << instret_now - instret_start << " instructions." << std::endl;
    } else break;
  }

  if(s != 0) { // somthing is wrong
     if(s == -1)
       std::cerr << "waitpid() is NOT supported in this system!" << std::endl;
     else
       std::cerr << "process exit early!" << std::endl;
     exit(1);
  }

  pfc.end();

  kill(pid, SIGKILL);
  waitpid(pid, NULL, WNOHANG);
}

int main(int argc, char* argv[], char* envp[]) {
  uint64_t max_instret = 1000000ull * std::stoll(std::string(argv[1]));
  argv += 2;
  run_cmd(argv, envp, max_instret);
  return 0;
}
