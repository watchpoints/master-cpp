#include <stdio.h>      // printf
#include <sys/types.h>  // pid_t
#include <unistd.h>     // fork
#include <sys/wait.h>   // wait

int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      //https://coolshell.cn/articles/7965.html
      //printf(“-“);把“-”放到了缓存中，并没有真正的输出
      printf("-");
   }

   wait(NULL);
   wait(NULL);

   return 0;
}
