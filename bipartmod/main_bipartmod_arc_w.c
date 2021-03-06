#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include <gsl/gsl_rng.h>
#include "tools.h"
#include "graph.h"
#include "modules.h"
#include "bipartite.h"


main(int argc, char **argv)
{
  FILE *outF, *inF;
  int rgm;
  char *dbs;
  int seed = 1111;
  struct binet *binet = NULL;
  struct group *part = NULL;
  gsl_rng *randGen;
  double Ti, Tf, Ts, fac;
  char file_name[100];
  int invert;
  
  seed=atoi(argv[1]);
  strcpy(file_name,argv[2]);

  /*
  Parameters that don't need to change:
  */
  
  fac=1.0;
  Ts=0.995;
  invert=0;  

  /*
    ------------------------------------------------------------
    Prompt for user-defined parameters
    ------------------------------------------------------------
  
  printf("\n# Enter random number seed (POSITIVE integer): ");
  rgm = scanf("%d", &seed);

  printf("\n# Enter the name of the network file: ");
  dbs = scanf("%s", &file_name);  
  
  printf("\n# Enter iteration factor (recommended 1.0): ");
  rgm = scanf("%lf", &fac);
  
  printf("\n# Enter the cooling factor (recommended 0.950-0.995): ");
  rgm = scanf("%lf", &Ts);

  printf("\n# Find modules from first column (0) or second columnd (1): ");
  rgm = scanf("%d", &invert);
  

    ------------------------------------------------------------------
    Initialize the random number generator
    ------------------------------------------------------------------
  */
  randGen = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(randGen, seed);

  /*
    ------------------------------------------------------------------
    Build the network
    ------------------------------------------------------------------
  */
  inF = fopen(file_name, "r");
  binet = FBuildNetworkBipart(inF, 1, 0);
  fclose(inF);
  if (invert == 1)
    InvertBipart(binet);

  /*
    ------------------------------------------------------------------
    Find the modules using the bipartite network
    ------------------------------------------------------------------
  */
  Ti = 1. / (double)CountNodes(binet->net1);
  Tf = 0.;

  part = SACommunityIdentBipartWeighted(binet,
				Ti, Tf, Ts, fac,
				0, 'o', 1, 'm',
				randGen);
  outF = fopen("modules_bipart_w.dat", "w");
  FPrintPartition(outF, part, 0);
  fclose(outF);
  RemovePartition(part);

  // Free memory
  // ------------------------------------------------------------
  gsl_rng_free(randGen);
  RemoveBipart(binet);
}
