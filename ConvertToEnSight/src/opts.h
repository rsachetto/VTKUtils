/*
 * opts.h
 *
 *  Created on: 27/05/2011
 *      Author: sachetto
 */

#ifndef OPTS_H_
#define OPTS_H_

#include <getopt.h>

enum type {ALG, PETSC, ERROR};
enum dim {D2, D3, DERROR};

struct globalArgs_t {
	int inType;				/* -t option */
	double sideLenght;		/* -l option */
	const char *inDirName;	/* -i option */
	const char *meshName;	/* -m option */
	int dim;		/* -d option */
} globalArgs;

static const char *optString = "i:t:l:d:m:h?";

/* Display program usage, and exit.
 */
void display_usage( )
{
	puts( "ConvertToVTK - Convert Monodomain text output to VTK" );
	puts( "Usage: ConvertToVTK -t inputType -l sideLenght -i inDirName -m meshFile -d dimension" );
	puts( "inputType: ALG or PETSC" );
	puts( "sideLenght: size of the mesh (Only used with ALG)" );
	puts( "inDirName: Dir containing the text output" );
	puts( "meshFile: File containing the mesh parameters (Only used for PETSC)" );
	puts( "dimension: Dimension of the problem (2D or 3D)" );
	exit( EXIT_FAILURE );
}

void parseOptions(int argc, char**argv) {
	int opt = 0;

	/* Initialize globalArgs before we get to work. */
	globalArgs.inType = ERROR;
	globalArgs.inDirName = NULL;
	globalArgs.meshName = NULL;
	globalArgs.sideLenght = 0.0;
	globalArgs.dim = D2;

	opt = getopt( argc, argv, optString );

	while( opt != -1 ) {
		switch( opt ) {
		case 't':
			if ( strcasecmp( "ALG", optarg ) == 0 ) {
				globalArgs.inType = ALG;
			}
			else if ( strcasecmp( "PETSC", optarg ) == 0 ) {
				globalArgs.inType = PETSC;
			}
			else {
				globalArgs.inType = ERROR;
			}

			break;
		case 'd':
			if ( strcasecmp( "2D", optarg ) == 0 ) {
				globalArgs.dim = D2;
			}
			else if ( strcasecmp( "3D", optarg ) == 0 ) {
				globalArgs.dim = D3;
			}
			else {
				globalArgs.dim = DERROR;
			}

			break;

		case 'l':
			globalArgs.sideLenght = atof(optarg);
			break;

		case 'i':
			globalArgs.inDirName = optarg;
			break;

		case 'm':
			globalArgs.meshName = optarg;
			break;

		case 'h':	/* fall-through is intentional */
		case '?':
			display_usage();
			break;

		default:
			/* You won't actually get here. */
			break;
		}

		opt = getopt( argc, argv, optString );
	}

	if(!globalArgs.inDirName) {
		display_usage();
	}

	if(globalArgs.inType == ERROR) {
		puts("ERROR: Invalid input type!");
		display_usage();
	}

	if(globalArgs.inType == ALG && globalArgs.sideLenght <= 0) {
		puts("ERROR: You need to supply the mesh side lenght (-l option) name to convert a ALG output ");
		display_usage();
	}

	if((globalArgs.inType == PETSC) && (globalArgs.meshName == NULL)) {
		puts("ERROR: You need to supply the mesh (-d option) name to convert a petsc output ");
		display_usage();
	}

}

#endif /* OPTS_H_ */
