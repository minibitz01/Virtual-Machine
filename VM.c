/*
Assignment:
vm.c - Implement a P-machine virtual machine
Authors: Naseem Elbram and Ana Paula Ramos Lopez 
Language: C (only)
To Compile:
gcc -O2 -Wall -std=c11 -o vm vm.c
To Execute (on Eustis):
./vm input.txt
where:
input.txt is the name of the file containing PM/0 instructions;
each line has three integers (OP L M)
Notes:
- Implements the PM/0 virtual machine described in the homework
instructions.
- No dynamic memory allocation, no pointer arithmetic, and
no function-like macros.
- Does not implement any VM instruction using a separate function.
- Runs on Eustis.
Class: COP 3402 - Systems Software - Spring 2026
Instructor : Dr. Jie Lin
Due Date: Monday, February 9th, 2026
*/
#include <stdio.h>
#include <stdlib.h>
#define PAS_SIZE 500
int PAS[PAS_SIZE];
int OP, L, M, isIt, SL, RAflag = 0;

//struct for VM
typedef struct
{
    int PC; 
    int BP;
    int SP;
} VirtualMachine;

//base function (2)
int base(int BP, int L);

//print function (3)
void print(int L, int M, int SPP, int BPP, int PCC, int isIt); 

int main(int argc, char *argv[])
{
    //instance of VM
    VirtualMachine VM; 
    VM.PC = 0;  
    VM.BP = 480;
    VM.SP = 481; 

    if(argc != 2)
    {
        printf("Error \n");
        return 1; 
    }
    
    FILE *file = fopen(argv[1], "r"); //check file 
    if (file == NULL)
    {
        printf("Error: Could not open file\n");
        return 1;
    }

    printf("%-10s", "          L  M  PC BP  SP      stack \n");
    printf("Initial values: %-2d %-2d %-2d \n", VM.PC, VM.BP, VM.SP);

    //read in file text into instructions 
    while (fscanf(file, "%d %d %d", &OP, &L, &M) == 3)
    {
        //text segment->> Instruction Storage 
        PAS[VM.PC] = OP; 
        PAS[VM.PC + 1] = L;
        PAS[VM.PC + 2] = M;
        VM.PC += 3;
    }

    VM.PC = 0; 

    int halt = 0;

    while(halt != 1)
    {
        // Fetch per instruction line 
        OP = PAS[VM.PC];
        L  = PAS[VM.PC + 1];
        M  = PAS[VM.PC + 2];
        VM.PC += 3;

        // Execute
        switch (OP)
        {
            // 01 LIT -> push value onto the stack 
            case 1: 
                VM.SP--; //decrease first 
                PAS[VM.SP] = M; //then allocate also M is a number 
                printf("LIT "); 
                print(L, M, VM.SP, VM.BP, VM.PC, isIt); 
                break;

            // 02 OPR ---> Arithmetic and Logical Operations 
            case 2:
                switch (M)
                {
                    case 0: // RTN -> in tandem with CAL restores what has been done, reads from stack
                        printf("RTN ");
                        RAflag = 0;
                        
                        int temp = VM.BP; //where frame begins 
                        //use BP to find and remove frame 

                        int DL = temp-1; 
                        VM.BP = PAS[DL];

                        int RA = temp-2; 
                        VM.PC = PAS[RA]; 
                        
                        VM.SP = temp+1; //throw frame created away 
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 1: // NEG
                        printf("NEG ");
                        PAS[VM.SP] = -PAS[VM.SP];
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 2: // ADD -> pop 1x here M is 2 
                        printf("ADD ");
                        PAS[VM.SP + 1] = PAS[VM.SP + 1] + PAS[VM.SP];
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 3: // SUB
                        printf("SUB ");
                        PAS[VM.SP + 1] = PAS[VM.SP + 1] - PAS[VM.SP];
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);   
                        break;

                    case 4: // MUL here M is 4 
                        printf("MUL ");
                        PAS[VM.SP + 1] = PAS[VM.SP + 1] * PAS[VM.SP];
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 5: // DIV
                        printf("DIV ");
                        PAS[VM.SP + 1] = PAS[VM.SP + 1] / PAS[VM.SP];
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 6: // EQL
                        printf("EQL ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] == PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 7: // NEQ
                        printf("NEQ ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] != PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 8: // LSS
                        printf("LSS ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] < PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 9: // LEQ
                        printf("LEQ ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] <= PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 10: // GTR
                        printf("GTR ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] > PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;

                    case 11: // GEQ 
                        printf("GEQ ");
                        PAS[VM.SP + 1] = (PAS[VM.SP + 1] >= PAS[VM.SP]);
                        VM.SP++;
                        print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                        break;
                }
                break;

            // 03 LOD -> push
            // Push from location at offset M in current AR 
            // in other words, finds value in another location in the stack and pushes it where SP points to 
            case 3:
                printf("LOD ");
                VM.SP--;
                // Find the frame L levels up + M (how much its offset to BP) and store it in stack 
                PAS[VM.SP] = PAS[base(VM.BP, L) + M]; //M is a data addres
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 04 STO -> pop 
            // Store top of stack at offset M in current AR.
            // In other words, move the values 
            case 4: 
                printf("STO "); 
                PAS[base(VM.BP, L) - M] = PAS[VM.SP]; //saves value and M is a data addres
                VM.SP++; //-> top of stack
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 05 CAL 
            case 5: 
            // Call procedure at M (generates new AR and pc = M).
            // Creates a new activation record when a function is called (creates a frame pointer)
            // Use SP to create frame and mark it with BP (manipulated twice)
                printf("CAL ");
                //Store static link in stack
               
                int SL = VM.SP-1; 
                PAS[SL] = base(VM.BP, L); 

                //store DL in stack under frame
                
                int DL = VM.SP-2; 
                PAS[DL] = VM.BP;  

                //store the return address
               
                int RA = VM.SP-3; 
                PAS[RA] = VM.PC;

                //update the BP to where the frame begins 
                VM.BP = SL; //BP & SL point the same 

                //update the PC by jumping to func 
                VM.PC = M; //go to procedure and M is a program address here so jump to M 

                isIt = 1; //flag to see if frame is created
                RAflag = 1; //BP and SP have been moved 
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 06 INC works in tandem with RTN AND CAL to allocate space for variables inside created frame by CAL
            case 6:
                VM.SP -= M; //M is a number so make space for M number of variables 
                printf("INC ");
                isIt = 1;
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 07 JMP
            case 7:
                VM.PC = M; //M is a program address
                printf("JMP ");
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 08 JPC -> pop
            case 8:
                if (PAS[VM.SP] == 0)
                    VM.PC = M; //M is a program address
                VM.SP++;
                printf("JPC ");
                print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                break;

            // 09 SYS 
            case 9:
                RAflag = 0; 
                if (M == 1)
                {
                    printf("Output result is: %d\n", PAS[VM.SP]);
                    printf("SYS ");
                    VM.SP++;
                    print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                }
                else if (M == 2)
                {
                    int input;
                    printf("Please Enter an Integer: ");

                    if(scanf("%d", &input) != 1)
                    {
                        printf("Invalid input"); 
                        input = 0; 
                    }

                    VM.SP--;
                    PAS[VM.SP] = input;
                    printf("SYS ");
                    print(L, M, VM.SP, VM.BP, VM.PC, isIt);
                    
                }
                else if (M == 3)
                {
                    halt = 1;
                    printf("SYS ");
                    print(L, M, VM.SP, VM.BP, VM.PC, isIt); 
                }
                break;
        } 
    } 

    fclose(file);
    return 0;
}

int base(int BP, int L) 
{
    //this function is about the static link which stores the address of the parent frame.
    //we see how back we go, return that number
    //then we use that number as an index in the PAS and push the value found unto the stack 
    int arb = BP; // activation record base

    while(L > 0) 
    {
        arb = PAS[arb]; 
        L--;
    }
    
    return arb;
}


void print(int L, int M, int SPP, int BPP, int PCC, int isIt)
{
    printf("%7d %3d %2d %2d %2d", L,M,PCC,BPP,SPP);
    printf("    ");
    
    //implememnt stack print (is it an activation record?)
    if(isIt == 1)
    {
        for(int i = 480; i >= SPP; i--) 
        {
            printf("%d ", PAS[i]);
            if(BPP+1 == i && RAflag == 1)
            {
                printf("| ");
            } 
        }  
    }
    printf("\n"); 
}