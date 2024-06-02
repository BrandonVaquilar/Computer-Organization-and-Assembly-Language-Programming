.global ssepia

//void sepia(unsigned char 0*in, unsigned char 1*out, int 2width, int 3height);

ssepia:
    PUSH {LR}
    PUSH {R4-R12}
    MOV R4, #0           //int i = 0;
    MUL R6, R2, R3       //Stride = Width * Height;
width:
    CMP R4, R2
    BGE return
    MOV R5, #0           //int j = 0, declare it in width so we can reset height after every iteration
height:
    CMP R5, R3           //j < height
    ADDGE R4, #1         //i++
    BGE width            //if j >= height, continue width for loop
    MUL R8, R5, R2       // j*width
    ADD R8, R4           // (j*width) + i
    MOV R12, #0          // k < 3 //third counter
widen:
    LDRB R9, [R0, R8]    // R[i,j] value
    ADD R7, R6, R8       //stride + (j*width+i)
    LDRB R10, [R0, R7]   //G[i,j] value
    ADD R7, R6           //add stride again
    LDRB R11, [R0, R7]   //B[i,j] value
    ADD R12, #1          //K++
    CMP R12, #1          //1 = red = branch to red
    BEQ red              //jump to red
    CMP R12, #2          //2 = green = branch to green
    BEQ green
    CMP R12, #3          //3 = blue = branch to blue
    BEQ blue
red:
    MOV R6, #101        //float * 256 (8 bits)
    MUL R9, R6, R9      //multiply R[i,j] value
    ASR R9, #8          //shift right back 8 times
    MOV R6, #197        //repeat for G[i,j]
    MUL R10, R6, R10
    ASR R10, #8
    MOV R6, #48         //repeat for B[i,j]
    MUL R11, R6, R11
    ASR R11, #8
    MUL R6, R2, R3       //Remake Stride for top of for loop (WAR pipeline rip)
    ADD R7, R9, R10  //r + g
    ADD R7, R7, R11  //(r+g) + b
    B clamp
green:
    MOV R6, #90
    MUL R9, R6, R9
    ASR R9, #8
    MOV R6, #176
    MUL R10, R6, R10
    ASR R10, #8
    MOV R6, #43
    MUL R11, R6, R11
    ASR R11, #8
    MUL R6, R2, R3
    ADD R7, R9, R10
    ADD R7, R7, R11
    ADD R8, R6, R8   //location at array + stride for green plane
    B clamp
blue:
    MOV R6, #70
    MUL R9, R6, R9
    ASR R9, #8
    MOV R6, #137
    MUL R10, R6, R10
    ASR R10, #8
    MOV R6, #34
    MUL R11, R6, R11
    ASR R11, #8
    MUL R6, R2, R3
    ADD R7, R9, R10
    ADD R7, R7, R11
    ADD R8, R6, R8
    ADD R8, R6       //2 strides this time
clamp:
    CMP R7, #255
    MOVGT R7, #255  //Char cannot go higher than 255
    B save
save:
    CMP R12, #3
    BGT height2             //if (K > 3) height is done
    STRB R7, [R1, R8]       //Store
    MUL R8, R5, R2          // remake j*width + i
    ADD R8, R4
    B widen
height2:
    ADD R5, #1           //j++
    B height
return:
    POP {R4-R12}
    POP {PC}


