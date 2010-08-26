#include <stdio.h>
#include <badgewars.h>

int main(int argc, char **argv)
{
  struct bw_world world;
  
  printf("Starting BadgeWars server...\n");
  printf("Core size:        %d cells\n", BW_CORE_SIZE);
  printf("World image size: %d bytes\n", (int)sizeof(struct bw_world));
  
  bw_init(&world);
  
  return 0;
}