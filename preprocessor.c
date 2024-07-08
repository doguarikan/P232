
#include "P_header.h"
// tamamlandı
void declare(char *line, FILE *output_file)
{
    ArrayCount++;
    if (sscanf(line, "@int %[^()](%[^,],%[^)])", AT[ArrayCount].name, AT[ArrayCount].size1, AT[ArrayCount].size2) == 3)
    {
        AT[ArrayCount].dim = 2;
    }
    // It's a 1D array
    else if (sscanf(line, "@int %[^()](%[^)])", AT[ArrayCount].name, AT[ArrayCount].size1) == 2)
    {
        AT[ArrayCount].dim = 1;
    }

    // Generate the declaration code
    if (AT[ArrayCount].dim == 1)
    {
        fprintf(output_file, "\tint %s[%s];\n", AT[ArrayCount].name, AT[ArrayCount].size1);
    }
    else if (AT[ArrayCount].dim == 2)
    {
        fprintf(output_file, "\tint %s[%s][%s];\n", AT[ArrayCount].name, AT[ArrayCount].size1, AT[ArrayCount].size2);
    }
}
// tamamlandı
void read(char *line, FILE *output_file) 
{
    // Parse the @read directive line
    sscanf(line, "@%s %s < %s", PT.oper, PT.lhs, PT.rhs1);

    int i;
    for (i=0; i < 20; i++){
        if(!(strcmp(AT[i].name, PT.lhs)))
            break;
    }

    // Generate code to read the array elements from the file
    if (AT[i].dim == 1) {
        fprintf(output_file, "	FILE *%s = fopen(\"%s\", \"r\");\n",PT.rhs1, PT.rhs1);
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "	    fscanf(%s, \"%%d\", &%s[i]);\n",PT.rhs1, AT[i].name);
        fprintf(output_file, "	}\n");
        fprintf(output_file, "	fclose(%s);\n", PT.rhs1);

    } else if (AT[i].dim == 2) {
        fprintf(output_file, "	FILE *%s = fopen(\"%s\", \"r\");\n", PT.rhs1, PT.rhs1);
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "    	for (int j = 0; j < %s; ++j) {\n", AT[i].size2);
        fprintf(output_file, "        	fscanf(%s, \"%%d\", &%s[i][j]);\n",PT.rhs1, AT[i].name);
        fprintf(output_file, "    	}\n");
        fprintf(output_file, "	}\n");
        fprintf(output_file, "	fclose(%s);\n", PT.rhs1);
    }
} 
// tamamlanmadı
void copy(char *line, FILE *output_file) {
    char src[10];
    char dest[10];
    int i, j;

    // Parse the @copy directive line
    sscanf(line, "@copy %s = %s", dest, src);

    // Find the source and destination arrays in the ArrayTable
    int srcIndex = -1, destIndex = -1;
    for (i = 0; i < 20; ++i) {
        if (strcmp(AT[i].name, src) == 0) {
            srcIndex = i;
        }
        if (strcmp(AT[i].name, dest) == 0) {
            destIndex = i;
        }
        if (srcIndex != -1 && destIndex != -1) {
            break;
        }
    }

    if (srcIndex == -1 || destIndex == -1) {
        fprintf(stderr, "Source or destination array not found in ArrayTable\n");
        return;
    }

    if (AT[srcIndex].dim != AT[destIndex].dim) {
        fprintf(stderr, "Source and destination arrays have different dimensions\n");
        return;
    }

    // Generate code to copy the array elements
    if (AT[srcIndex].dim == 1) {
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[srcIndex].size1);
        fprintf(output_file, "    	%s[i] = %s[i];\n", AT[destIndex].name, AT[srcIndex].name);
        fprintf(output_file, "	}\n");
    } else if (AT[srcIndex].dim == 2) {
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[srcIndex].size1);
        fprintf(output_file, "    	for (int j = 0; j < %s; ++j) {\n", AT[srcIndex].size2);
        fprintf(output_file, "        	%s[i][j] = %s[i][j];\n", AT[destIndex].name, AT[srcIndex].name);
        fprintf(output_file, "    	}\n");
        fprintf(output_file, "	}\n");
    } else {
        fprintf(stderr, "Array %s has invalid dimension %d\n", src, AT[srcIndex].dim);
    }
}
// tamamlandı
void initialize(char *line, FILE *output_file) {

    int i;

    // Parse the @init directive line
    sscanf(line, "@init %s = %s", PT.lhs, PT.rhs1);

    // Find the array in the ArrayTable
    for (i = 0; i < 20; ++i) {
        if (!strcmp(AT[i].name, PT.lhs)) {
            break;
        }
    }

    // Generate code to initialize the array elements
    if (AT[i].dim == 1) {
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "		%s[i] = %s;\n", PT.lhs, PT.rhs1);
        fprintf(output_file, "	}\n");
    } 
	else if (AT[i].dim == 2) {
        fprintf(output_file, "	for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "	    for (int j = 0; j < %s; ++j) {\n", AT[i].size2);
        fprintf(output_file, "	        %s[i][j] = %s;\n", PT.lhs, PT.rhs1);
        fprintf(output_file, "	    }\n");
        fprintf(output_file, "	}\n");
    }
}
// tamamlandı
void preprint(char *line, FILE *output_file)
{
    sscanf(line, "@%s %s", PT.oper, PT.lhs);

    int i;
    for (i=0; i < 20; i++){
        if(!(strcmp(AT[i].name, PT.lhs)))
            break;
    }

    if (AT[i].dim == 1)
    {
        fprintf(output_file, "    for (int i = 0; i < %s; i++)\n", AT[i].size1);
        fprintf(output_file, "        printf(\"%%d \", %s[i]);\n", AT[i].name);
        fprintf(output_file, "	  printf(\"\\n\");\n");
    }
    else if (AT[i].dim == 2)
    {
        fprintf(output_file, "    for (int i = 0; i < %s; i++){\n", AT[i].size1);
        fprintf(output_file, "        for(int j = 0; j < %s; j++){\n", AT[i].size2);
        fprintf(output_file, "            printf(\"%%d \", %s[i][j]);\n", AT[i].name);
        fprintf(output_file, "        }printf(\"\\n\");\n");
        fprintf(output_file, "    printf(\"\\n\");\n");
        fprintf(output_file, "    }\n");
    }
}
// tamamlnadı
void dot_product(char *line, FILE *output_file) {
// @dotp C = A . B

    sscanf(line, "@%s %s = %s . %s", PT.oper, PT.lhs, PT.rhs1, PT.rhs2);
    fprintf(output_file, "    int dot = 0;\n");
    fprintf(output_file, "    int size = sizeof(%s) / sizeof(%s[0]);\n", PT.rhs1, PT.rhs1);
    fprintf(output_file, "    for (int i = 0; i<size; i++){\n");
    fprintf(output_file, "        dot += %s[i]*%s[i];\n", PT.rhs1,PT.rhs2);
    fprintf(output_file, "    }\n");
    fprintf(output_file, "    P_dot = dot;");

}
// tamamlandı
void preaddition(char *line, FILE *output_file)
{   

    sscanf(line, "@%s %s = %s + %s", PT.oper, PT.lhs, PT.rhs1, PT.rhs2);

    int i;
    for (i=0; i < 20; i++){
        if(!(strcmp(AT[i].name, PT.lhs)))
            break;
    }

    if (AT[i].dim == 1)
    {
        fprintf(output_file, "    for(int i = 0; i < %s; i++)\n", AT[i].size1);
        fprintf(output_file, "	  {\n");
		fprintf(output_file, "        %s[i] = %s[i] + %s[i];\n", PT.lhs, PT.rhs1, PT.rhs2);
        fprintf(output_file, "    }\n");
    }
    else if (AT[i].dim == 2)
    {
        fprintf(output_file, "    for(int i = 0; i < %s; i++)\n", AT[i].size1);
        fprintf(output_file, "	  {\n");
		fprintf(output_file, "        for(int j = 0; j < %s; j++)\n", AT[i].size2);
        fprintf(output_file, "		  {\n");
		fprintf(output_file, "		      %s[i][j] = %s[i][j] + %s[i][j];\n", PT.lhs, PT.rhs1, PT.rhs2);
        fprintf(output_file, "        }\n");
        fprintf(output_file, "    }\n");
    }
            
    
}
// tamamlandı
void matrix_multiplication(char *line, FILE *output_file) {
// @mmult C = A * B

    sscanf(line, "@%s %s = %s * %s", PT.oper, PT.lhs, PT.rhs1, PT.rhs2);

    int i;

    for (i = 0; i < 20; i++) {
        if (!(strcmp(AT[i].name, PT.rhs1)))
            break;
    }

    int j;

    for (j = 0; j < 20; j++) {
        if (!(strcmp(AT[j].name, PT.rhs2)))
            break;
    }

    if (AT[i].dim == 1) {
        fprintf(output_file, "    int mult;\n");
        fprintf(output_file, "    for (int i = 0; i < %s; i++) {\n", AT[j].size1);
        fprintf(output_file, "        mult = 0;\n");  
        fprintf(output_file, "        for (int j = 0; j < %s; j++) {\n", AT[j].size2);
        fprintf(output_file, "            mult += %s[j] * %s[j][i];\n", PT.rhs1, PT.rhs2);
        fprintf(output_file, "        }\n");
        fprintf(output_file, "        %s[i] = mult;\n", PT.lhs);
        fprintf(output_file, "    }\n");

    } else if (!(strcmp(AT[i].size2, AT[j].size1))) {
        fprintf(output_file, "    int mult;\n");
        fprintf(output_file, "    for (int i = 0; i < %s; i++) {\n", AT[i].size1);  
        fprintf(output_file, "        for (int j = 0; j < %s; j++) {\n", AT[j].size2);  
        fprintf(output_file, "            mult = 0;\n");  
        fprintf(output_file, "            for (int k = 0; k < %s; k++) {\n", AT[i].size2); 
        fprintf(output_file, "               mult += %s[i][k] * %s[k][j];\n", PT.rhs1, PT.rhs2);
        fprintf(output_file, "            }\n");
        fprintf(output_file, "            %s[i][j] = mult;\n", PT.lhs);
        fprintf(output_file, "        }\n");
        fprintf(output_file, "    }\n");
    }
}
// tamamlandı
void sum(char *line, FILE *output_file) {
    
    sscanf(line, "@%s %s", PT.oper, PT.lhs);
    int i;
    for (i = 0; i < 20; ++i) {
        if (!(strcmp(AT[i].name, PT.lhs))) {
            break;
        }
    }

    if (AT[i].dim == 1) {
        fprintf(output_file, "    P_sum = 0;\n");        
        fprintf(output_file, "    for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "        P_sum += %s[i];\n", AT[i].name);
        fprintf(output_file, "    }\n");
    } else if (AT[i].dim == 2) {
        fprintf(output_file, "    P_sum = 0;\n"); 
        fprintf(output_file, "    for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "        for (int j = 0; j < %s; ++j) {\n", AT[i].size2);
        fprintf(output_file, "            P_sum += %s[i][j];\n", AT[i].name);
        fprintf(output_file, "        }\n");
        fprintf(output_file, "    }\n");
    }
}
// tamamlanmadı
void average(char *line, FILE *output_file) {
    sscanf(line, "@%s %s", PT.oper, PT.lhs);
    int i;

    for (i = 0; i < 20; i++) {
        if (!(strcmp(AT[i].name, PT.lhs))) {
            break;
        }
    }

    if (AT[i].dim == 1) {
        fprintf(output_file, "    P_aver = 0;\n");        
        fprintf(output_file, "    for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "        P_aver += %s[i];\n", AT[i].name);
        fprintf(output_file, "    }\n");
        fprintf(output_file, "    P_aver /= %s;\n", AT[i].size1);
    } else if (AT[i].dim == 2) {
        fprintf(output_file, "    P_aver = 0;\n"); 
        fprintf(output_file, "    for (int i = 0; i < %s; ++i) {\n", AT[i].size1);
        fprintf(output_file, "        for (int j = 0; j < %s; ++j) {\n", AT[i].size2);
        fprintf(output_file, "            P_aver += %s[i][j];\n", AT[i].name);
        fprintf(output_file, "        }\n");
        fprintf(output_file, "    }\n");
        fprintf(output_file, "    P_aver /= (%s * %s);\n", AT[i].size1, AT[i].size2);
    }
}


int main(int argument_count, char *argument_vector[])
{
    FILE *inputFile = fopen(argument_vector[1], "r");
    FILE *outputFile = fopen("expanded.c", "w");

    char line[100];
    char tmp[100];
	int i;

	i = 0;
    while (fgets(line, 100, inputFile))
    {
		i = 0;
		while (isspace((unsigned char)line[i]) && line[i] != '\0') {
            i++;
        }
		strcpy(tmp,line+i);
        if (tmp[0] == '@')
        {
            sscanf(tmp, "@%s", PT.oper);
            
            if(strcmp(PT.oper, "int") == 0)
            {
                declare(tmp, outputFile);
            }
            else if(strcmp(PT.oper, "read") == 0)
            {
                read(tmp, outputFile);
            }
			else if(strcmp(PT.oper, "copy") == 0)
            {
                copy(tmp, outputFile);
            }
			else if(strcmp(PT.oper, "init") == 0)
            {
                initialize(tmp, outputFile);
            }
			else if(strcmp(PT.oper, "print") == 0)
            {
                preprint(tmp, outputFile);
            }
			else if(strcmp(PT.oper, "add") == 0)
            {
                preaddition(tmp, outputFile);
            }
            else if(strcmp(PT.oper, "dotp")== 0)
            {
                dot_product(tmp, outputFile);
            }
            else if(strcmp(PT.oper, "mmult")== 0)
            {
                matrix_multiplication(tmp, outputFile);
            }
            else if(strcmp(PT.oper, "sum")== 0)
            {
                sum(tmp, outputFile);
            }
            else if(strcmp(PT.oper, "aver")== 0)
            {
                average(tmp, outputFile);
            }
		}
		else
		{
			fprintf(outputFile, line);
		}
    }
}