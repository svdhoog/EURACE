/**
 * \file  xparser.c
 * \code
 *     Author:       		Simon Coakley (USFD)
 *     Added to and Modified: 	David Worth (STFC)
 *				Shawn Chin (STFC)
 *				Chris Greenough (STFC)
 *     Copyright (c) 2006 Simon Coakley
 *     License:      n/a
 * \endcode
 * \brief Architecture for agent-based modelling based on X-machines.
 */

/** \mainpage Xparser - parser tool for the X-agents framework
 *
 * \section intro_sec Introduction
 *
 * This program is the FLAME XMML parser. It accepts a model description 
 * in X-agent XML and produces source code for the model in either serial 
 * (default) or in parallel (via MPI libraries).
 *
 * The command line is:
 *
 * 	xparser [-p | -s] [XMML file]
 *
 * xparser without any arguments will produce:
 *
 * xparser: Version \<version number\>
 *
 * Usage: xparser [XMML file] [-s | -p]
 *
 * By default xparser will generate a serial program.
 *
 * The code xparser generates, although not pretty, should compile on most
 * systems that have a compiler conforming to the C99 standard.
 *
 * \section install_sec Installation
 *
 * Compile using a C compiler. A Make file is provided. xparser has been tested
 * on a large number of UNIX based systems. 
 *
 * \section changelog
 *
 * 0.11.0 Can handle arrays in agent memory in serial (not parallel)
 */

#include "header.h"

/** \fn int main(int argc, char * argv[])
 * \brief Main program function.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector
 * \return system int
 */
int main(int argc, char * argv[])
{
	/* Variables for parsing directories */
	int lastd;
	int i;
	
	/* Variable to read in command line input */
	char inputfile[1000];
	char directory[1000];
	char filename[1000];
	char templatename[1000];
	char templatedirectory[1000];
	
	/* List of input files */
	input_file ** p_files;
	input_file * temp_input_file;
	input_file * current_input_file;
	p_files = &temp_input_file;
	temp_input_file = NULL;
	
	model_data * modeldata;
	
	/* Hold model data */
	xmachine * xmachines;
	/*xmachine_memory * xmemory;*/
	xmachine_message * xmessage;
	/*xmachine_state * xstates;*/
	/*xmachine_function * xfunctions;*/
	variable * envvar;
	env_func * envfunc;
	variable * envdefine;
	variable * allvars;
	f_code * it_end_code;
	layer * layers;
	flame_communication * communications;
	model_datatype * datatypes;
	time_data * time_units;
	
	modeldata = (model_data *)malloc(sizeof(model_data));
	/* Variable for code type */
	/* 0=serial(default) 1=parallel 2=grid */
	modeldata->code_type = 0;
	/* 0=dgraph.dot 1=stategraph.dot */
	modeldata->depends_style = 0;

	inputfile[1]='\0';
	
	/* Initialise pointers */
	modeldata->p_xmachines = &xmachines;
	xmachines = NULL;
	/*modeldata->p_xmemory = &xmemory;*/
	/*xmemory = NULL;*/
	modeldata->p_xmessages = &xmessage;
	xmessage = NULL;
	/*modeldata->p_xstates = &xstates;*/
	/*xstates = NULL;*/
	/*modeldata->p_xfunctions = &xfunctions;*/
	/*xfunctions = NULL;*/
	modeldata->p_envvars = &envvar;
	envvar = NULL;
	modeldata->p_envfuncs = &envfunc;
	envfunc = NULL;
	modeldata->p_envdefines = &envdefine;
	envdefine = NULL;
	modeldata->p_allvars = &allvars;
	allvars = NULL;
	modeldata->p_it_end_code = &it_end_code;
	it_end_code = NULL;
	modeldata->p_layers = &layers;
	layers = NULL;
	modeldata->p_communications = &communications;
	communications = NULL;
	modeldata->p_datatypes = &datatypes;
	datatypes = NULL;
	modeldata->p_time_units = &time_units;
	time_units = NULL;
	
	printf("xparser: Version %d.%d.%d\n", VERSIONMAJOR, VERSIONMINOR, VERSIONMICRO);
	
	/* Must be at least the input file name */
	if(argc < 2) {
		printf("Usage: xparser [XMML file] [-s | -p]\n");
		return 0;
	}
	
	/* Copy location of xparser */
	strcpy(templatedirectory,argv[0]);
	
	/* parser command line */
	while(argc >1){
		if(argv[1][0] == '-') {
			switch (argv[1][1]){
			case 's': modeldata->code_type = 0;
				  break;
			case 'p': modeldata->code_type = 1;
				  break;
			default:  printf("xparser: Error - unknown option %s\n",argv[1]);
				  return 0;
			}
		}
		else
		{
			strcpy(inputfile,argv[1]);
			printf("XMML input file: %s\n",argv[1]);
		}
		argc--;
		argv++;
	}
	
	if(inputfile[1] == '\0') {
		printf("xparser: Error - XMML must be specified\n");
		return 0;
	}
	
	/* Print what type of code writing */
	if(modeldata->code_type == 0) printf("code-type   : Serial\n");
	if(modeldata->code_type == 1) printf("code-type   : Parallel\n");
	
	/* Calculate directory to write files to */
	i = 0;
	lastd = 0;
	while(inputfile[i] != '\0')
	{
	/* For windows directories */
		if(inputfile[i] == '\\') lastd=i;
	/* For unix directories */
		if(inputfile[i] == '/') lastd=i;
		i++;
	}
	/* If a directory is in the path */
	if(lastd != 0)
	{
		strcpy(directory, inputfile);
		directory[lastd+1] = '\0';
	}
	else directory[0] = '\0';
	
	/* Calculate directory where xparser and template files are */
	i = 0;
	lastd = 0;
	while(templatedirectory[i] != '\0')
	{
		/* For windows directories */
		if(templatedirectory[i] == '\\') lastd=i;
		/* For unix directories */
		if(templatedirectory[i] == '/') lastd=i;
		i++;
	}
	/* If a directory is in the path */
	if(lastd != 0)
	{
		templatedirectory[lastd+1] = '\0';
	}
	else templatedirectory[0] = '\0';
	
	printf("inputfile: %s\n", inputfile);
	printf("directory: %s\n", directory);
	printf("templates: %s\n", templatedirectory);
	
	current_input_file = add_input_file(p_files);
	current_input_file->fullfilepath = copystr(inputfile);
	current_input_file->fulldirectory = copystr(directory);
	current_input_file->localdirectory = copystr("");
	
	/* Read model from model xml file */
	current_input_file = * p_files;
	while(current_input_file)
	{
		if(current_input_file->enabled == 1)
			readModel(current_input_file, directory, modeldata, p_files);
		
		current_input_file = current_input_file->next;
	}
	checkmodel(modeldata);
	
	/* Calculate dependency graph for model functions */
	create_dependency_graph(directory, modeldata);
	
	strcpy(filename, directory); strcat(filename, "Makefile");
	strcpy(templatename, templatedirectory); strcat(templatename, "Makefile.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "xml.c");
	strcpy(templatename, templatedirectory); strcat(templatename, "xml.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "main.c");
	strcpy(templatename, templatedirectory); strcat(templatename, "main.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "header.h");
	strcpy(templatename, templatedirectory); strcat(templatename, "header.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "memory.c");
	strcpy(templatename, templatedirectory); strcat(templatename, "memory.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "low_primes.h");
	strcpy(templatename, templatedirectory); strcat(templatename, "low_primes.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "messageboards.c");
	strcpy(templatename, templatedirectory); strcat(templatename, "messageboards.tmpl");
	parseTemplate(filename, templatename, modeldata);
	strcpy(filename, directory); strcat(filename, "partitioning.c");
	strcpy(templatename, templatedirectory); strcat(templatename, "partitioning.tmpl");
	parseTemplate(filename, templatename, modeldata);
	if(modeldata->code_type == 1)
	{
		strcpy(filename, directory); strcat(filename, "propagate_messages.c");
		strcpy(templatename, templatedirectory); strcat(templatename, "propagate_messages.tmpl");
		parseTemplate(filename, templatename, modeldata);
		strcpy(filename, directory); strcat(filename, "propagate_agents.c");
		strcpy(templatename, templatedirectory); strcat(templatename, "propagate_agents.tmpl");
		parseTemplate(filename, templatename, modeldata);
	}
	strcpy(filename, directory); strcat(filename, "Doxyfile");
	strcpy(templatename, templatedirectory); strcat(templatename, "Doxyfile.tmpl");
	parseTemplate(filename, templatename, modeldata);
	parseAgentHeaderTemplate(directory, modeldata);
	parseRuleFunctionsTemplate(directory, modeldata);
	
	freexmachines(modeldata->p_xmachines);
	freexmessages(modeldata->p_xmessages);
	freeenvfunc(modeldata->p_envfuncs);
	freevariables(modeldata->p_envvars);
	freevariables(modeldata->p_envdefines);
	freelayers(modeldata->p_layers);
	freedatatypes(modeldata->p_datatypes);
	free_flame_communications(modeldata->p_communications);
	free_time_units(modeldata->p_time_units);
	free(modeldata);
	free_input_files(p_files);
	
	printf("xparser finished\n");
	
	/* Exit successfully by returning zero to Operating System */
	return 0;
}
