/**
 * @file main.c
 * 
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "slicx.h"
#include <getopt.h>
#include <stdlib.h>


unsigned long GetTickCount()
{
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);
return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}



int main(int argc, char *argv[])
{
    int optc;
	int o;
    const char *optstring = "c:d:t:";
    int lose = 0;

    // TODO::
    int t = 0, n = 0;
    // const char *greeting = NULL;

	char target[16], lictype[64];
	int err;
	cid_t cid;
	lic_dev_id dev_id;
	lic_type_id type_id;
	lic_t lic;

	const char *cid2 = NULL;
	const char *dev_id2 = NULL;
	const char *type_id2 = NULL;

    // ----------

    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'c':
				cid2 = optarg;
//                printf("CID: %s\n", cid2);
                break;
            case 'd':
				dev_id2 = optarg;
//                printf("opt is d, oprarg is: %s\n", dev_id2);
                break;
            case 't':
				type_id2 = optarg;
//                printf("opt is t, oprarg is: %s\n", type_id2);
                break;
            case '?':
				printf("Options:\n");
				printf("\n-c (CID): 11 byte Computer ID like XXXXX-XXXXX\n");
				printf("\n-d (dev_id):\n 1: C51, 2: C251, 3: C166, 4: ARM, 0: NA\n");
				printf("\n-t (type_id):\n 1: Cortex-M0/M0+,\n 2: Cortex-M0/M0+ 128K,\n 3: Cortex-M0/M0+ 256K,\n 4: Macro Assembler Kit,\n 5: Compiler/Assembler Kit,\n 6: Developers Kit,\n 7: Debugger,\n 8: Hitex Extensions,\n 9: Infineon Extensions,\n 10: Essential/Standard Cortex-M only,\n 12: Prof. Developers Kit/Plus,\n 13: Limited (64K),\n 14: Basic (256K)/Community,\n 16: RL/Real-Time OS,\n 17: Professional Cortex-M only(DS5),\n 18: Professional,\n 0: NA\n\n");
                return 0;
				break;
        }
    }

	if (cid2 != NULL) {
		strncpy(cid, cid2, sizeof(cid)); 
	} else {
		printf("Warning! Uncorrect CID\n");
		return 0;
	}

	if (dev_id2 != NULL) {
		dev_id = atoi(dev_id2); 
	} else {
		printf("Warning! Uncorrect dev_id\n");
		return 0;
	}
	
	if (type_id2 != NULL) {
		type_id = atoi(type_id2);  
	} else {
		printf("Warning! Uncorrect type_id\n");
		return 0;
	}

	err = lic_make(lic, cid, dev_id, type_id, GetTickCount());

	switch (err) {
	case 0: 
//		printf(\n);		
		printf("%s\n", lic);
		break;
	case -2:
		printf(
			"Please enter the correct CID! Warnning...");
		break;
	default:
		printf(
			"An unknown error occurred! Warnning...");
		break;
	}
	return 0;
}
