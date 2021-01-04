#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//enum gate{AND, NOR, NAND, OR, XOR, NOT};

typedef struct IOStruc{
    char var[100];
    int val;
}IOStruc;

typedef struct tempStruc{
    char var[100];
    int val;
    struct tempStruc* next;
}tempStruc;

//tempStruc* head = NULL;

int AND(int a, int b)
{
    if(a == 1 && b == 1)
    {
        return 1;
    }
    return 0;
}

int OR(int a, int b)
{
    if(a == 0 && b == 0)
    {
        return 0;
    }
    return 1;
}

int NOT(int a)
{
    if(a == 1)
    {
        return 0;
    }
    if(a == 0)
    {
        return 1;
    }
    return -1;
}

int NAND(int a, int b)
{
    if(a == 1 && b == 1)
    {
        return 0;
    }
    return 1;
}

int NOR(int a, int b)
{
    if(a == 0 && b == 0)
    {
        return 1;
    }
    return 0;
}

int XOR(int a, int b)
{
    if(a == b || (a == 1 && b == 1) || (a == 0 && b == 0))
    {
        return 0;
    }
    return 1;
}

int power2(int a)
{
    int result = 1;
    for(int i = 0; i < a; i++)
    {
        result *= 2;
    }
    return result;
}

int** truthtableM(int n, int o)
{
    int row = power2(n);
    //printf("Row: %d\n", row);
    int** t = (int **)malloc(row * sizeof(int *));
    for(int i = 0; i < row; i++)
    {
        t[i] = malloc((n + o) * sizeof(int));
    }
    return t;
}

void truthtable(int** t, int change, int c, int numR, int numC)
{
    if(c == numC)
    {
        return;
    }
    int count = 0;
    int num = 0;
    for(int i = 0; i < numR; i++)
    {
        t[i][c] = num;
        if(count == (change - 1) && num == 0)
        {
            count = 0;
            num = 1;
            continue;
        }
        if(count == change - 1 && num == 1)
        {
            count = 0;
            num = 0;
            continue;
        }
        count++;
    }
    truthtable(t, change/2, c + 1, numR, numC);
}

int checkOutput(char* out, IOStruc* output, int numOutput)
{
    for(int i = 0; i < numOutput; i++)
    {
        //printf("Output Method: %s\n", output[i].var);
        if(strcmp(output[i].var, out) == 0)
        {
            //printf("True\n");
            return 0;
        }
    }
    return 1;
}

tempStruc* createTemp(char* name)
{
    tempStruc* temp = malloc(sizeof(tempStruc));
    int len = strlen(name) + 1;
    memcpy(temp->var, name, len);
    temp->val = -1;
    temp->next = NULL;
    return temp;
}

void circuit(char* in1, char* in2, char* out1, char* operation, int** t, IOStruc* input, IOStruc* output, int numInput, int numOutput, int rowNum, tempStruc* head)
{
    int a = -1;
    int b = -1;
    int opp = 0;
    if(in2[0] == '\0')
    {
        opp = 1;
    }
    int aCheck = 0;
    int bCheck = 0;
    int cCheck = 0;
    int dCheck = 0;
    if(strcmp(in1, "0") == 0)
    {
      a = 0;
      cCheck = 1;
    } 
    if(strcmp(in1, "1") == 0)
    {
      a = 1;
      cCheck = 1;
    }
    if(strcmp(in2, "0") == 0)
    {
      b = 0;
      dCheck = 1;
    }
    if(strcmp(in2, "1") == 0)
    {
      b = 1;
      dCheck = 1;
    }
    if(in1[0] >= 'A' && in1[0] <= 'Z')
    {
        aCheck = 1;
    }
    if(in2[0] >= 'A' && in2[0] <= 'Z')
    {
        bCheck = 1;
    }
    for(int i = 0; i < numInput; i++)
    {
        if(aCheck == 1 && strcmp(input[i].var, in1) == 0)
        {
            cCheck = 1;
            a = input[i].val;
            aCheck = 0;
        }
        if(opp)
        {
            dCheck = 1;
            continue;
        }
        if(bCheck == 1 && strcmp(input[i].var, in2) == 0)
        {
            dCheck = 1;
            b = input[i].val;
            bCheck = 0;
        }
        if(aCheck == 0 && bCheck == 0)
        {
            break;
        }
    }

    if(cCheck == 0)
    {
        tempStruc* ptr = head;
        while(ptr != NULL)
        {
            if(strcmp(ptr->var, in1) == 0)
            {
                //printf("TRUE1\n");
                a = ptr->val;
                break;
            }
            ptr = ptr->next;
        }
    }
    if(dCheck == 0)
    {
        tempStruc* ptr = head;
        while(ptr != NULL)
        {
            if(strcmp(ptr->var, in2) == 0)
            {
                //printf("TRUE2\n");
                b = ptr->val;
                break;
            }
            ptr = ptr->next;
        }
    }
    //printf("%s\n", out1);
    if(checkOutput(out1, output, numOutput) == 0)
    {
        for(int i = 0; i < numOutput; i++)
        {
            if(strcmp(output[i].var, out1) == 0)
            {
                //printf("Op: %s\n", operation);
                if(strcmp(operation, "AND") == 0)
                {
                    output[i].val = AND(a, b);
                    //printf("FINAL: %d\n", output[i].val);
                    t[rowNum][numInput + i] = output[i].val;
                }
                if(strcmp(operation, "NAND") == 0)
                {
                    output[i].val = NAND(a, b);
                    t[rowNum][numInput + i] = output[i].val;
                }
                if(strcmp(operation, "NOR") == 0)
                {
                    output[i].val = NOR(a, b);
                    t[rowNum][numInput + i] = output[i].val;
                }
                if(strcmp(operation, "OR") == 0)
                {
                    output[i].val = OR(a, b);
                    t[rowNum][numInput + i] = output[i].val;
                }
                if(strcmp(operation, "XOR") == 0)
                {
                    output[i].val = XOR(a, b);
                    t[rowNum][numInput + i] = output[i].val;
                }
                if(strcmp(operation, "NOT") == 0)
                {
                    output[i].val = NOT(a);
                    t[rowNum][numInput + i] = output[i].val;
                }
                break;
            }
        }
    }
    if(checkOutput(out1, output, numOutput) == 1)
    {
        tempStruc* current = head;
        while(current != NULL)
        {
            if(strcmp(current->var, out1) == 0)
            {
                if(strcmp(operation, "AND") == 0){
                    current->val = AND(a, b);
                }
                if(strcmp(operation, "NAND") == 0)
                {
                    current->val = NAND(a, b);
                }
                if(strcmp(operation, "NOR") == 0)
                {
                    current->val = NOR(a, b);
                }
                if(strcmp(operation, "OR") == 0)
                {
                    current->val = OR(a, b);
                }
                if(strcmp(operation, "XOR") == 0)
                {
                    current->val = XOR(a, b);
                }
                if(strcmp(operation, "NOT") == 0)
                {
                    current->val = NOT(a);
                }
                break;
            }
            current = current->next;
        }
    }
}

int main(int argc, char* argv[])
{
    FILE* fileR = fopen(argv[1], "r");
    if(fileR == NULL)
    {
      printf("error\n");
      exit(0);
    }
    int numInput = 0;
    fscanf(fileR, "INPUTVAR %d", &numInput);
    //printf("Number of Inputs: %d\n", numInput);
    IOStruc* inputVar = malloc(numInput * sizeof(IOStruc));
    for(int i = 0; i < numInput; i++)
    {
      //printf("Hey\n");
        char str[100];
        fscanf(fileR, " %s", str);
	//printf("%s ", str);
        int len = strlen(str) + 1;
        memcpy(inputVar[i].var, str, len);
    }
    //printf("\n");
    for(int i = 0; i < numInput; i++)
    {
      //printf("%s ", inputVar[i].var);
    }
    //printf("\n");
    int numOutput = 0;
    fscanf(fileR, "\nOUTPUTVAR %d", &numOutput);
    //printf("Number of Outputs: %d\n", numOutput);
    IOStruc* outputVar = malloc(numOutput * sizeof(IOStruc));
    for(int i = 0; i < numOutput; i++)
    {
        char str[100];
        fscanf(fileR, " %s", str);
        int len = strlen(str) + 1;
        //printf("%s ", str);
        memcpy(outputVar[i].var, str, len);
    }
    for(int i = 0; i < numOutput; i++)
    {
        //printf("%s\n", outputVar[i].var);
    }
    //printf("Welcome: %d\n", numInput);
    char cir[5];
    //int num = -1;
    //int count = 0;
    int** truthT = truthtableM(numInput, numOutput);
    int row = power2(numInput);
    int ch = row / 2;
    truthtable(truthT, ch, 0, row, numInput);

    int tracker = 0;
    tempStruc* front = NULL;
    while(tracker < row)
    {
        for(int i = 0; i < numInput; i++)
        {
            //printf("%d", numInput);
            inputVar[i].val = truthT[tracker][i];
        }
        //printf("-> ");
        for(int i = 0; i < numInput; i++)
        {
            //printf("%d ", inputVar[i].val);
        }
        //printf("\n");
        int boolean = 1;
        while(boolean)
        {
            if(fscanf(fileR, "%s", cir) == 1){
                boolean = 1;
            }else{
                //printf("_--------------------------------------------_\n");
	        tempStruc* ptr = NULL;
                while(front != NULL)
                {
                    ptr = front;
                    front = front->next;
                    free(ptr);
                }
                free(front);
                front = NULL;
                fclose(fileR);
                fileR = fopen(argv[1], "r");
                fscanf(fileR, "%*[^\n]");
                fscanf(fileR, "\n");
                fscanf(fileR, "%*[^\n]");
                fscanf(fileR, "\n");
                break;
            }
            char op1[100];
            char op2[100];
            char op3[100];
            if(strcmp(cir, "AND") == 0)
            {
                //printf("AND\n");
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op2);
                fscanf(fileR, " %s", op3);
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
            if(strcmp(cir, "NAND") == 0)
            {
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op2);
                fscanf(fileR, " %s", op3);
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
            if(strcmp(cir, "OR") == 0)
            {
                //printf("OR\n");
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op2);
                fscanf(fileR, " %s", op3);
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
            if(strcmp(cir, "NOR") == 0)
            {
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op2);
                fscanf(fileR, " %s", op3);
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
            if(strcmp(cir, "XOR") == 0)
            {
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op2);
                fscanf(fileR, " %s", op3);
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
            if(strcmp(cir, "NOT") == 0)
            {
                fscanf(fileR, " %s", op1);
                fscanf(fileR, " %s", op3);
                op2[0] = '\0';
                fscanf(fileR, "\n");
                if(checkOutput(op3, outputVar, numOutput)){
                    tempStruc* tmp = createTemp(op3);
                    tmp->next = front;
                    front = tmp;
                }
                circuit(op1, op2, op3, cir, truthT, inputVar, outputVar, numInput, numOutput, tracker, front);
                continue;
            }
        }
        tracker++;
    }
    for(int i = 0; i < row; i++)
    {
        //printf("%d. ", i);
        for(int j = 0; j < (numInput + numOutput); j++)
        {
	  printf("%d ", truthT[i][j]);
        }
        printf("\n");
    }
    for(int i = 0; i < row; i++)
    {
      free(truthT[i]);
    }
    free(truthT);
    free(inputVar);
    free(outputVar);
    fclose(fileR);
    return 0;
}
