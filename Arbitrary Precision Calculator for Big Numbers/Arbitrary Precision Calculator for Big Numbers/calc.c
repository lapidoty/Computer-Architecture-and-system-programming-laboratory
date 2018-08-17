#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bigint.h"
#include "bigIntStack.h"



bigint stack[1024];
bigint x; // The stack , the push bigint
int top=0; // The top of the stack
int n=1024; // The size of the stack
bigint operand1;
bigint operand2;
int next_num_is_negative=0;
int main(void){


    inputString(stdin, 10);

    clear_stack();

    return 0;
}

bigint * prepare_operation(int opperation){
    
    operand2 = pop();
    operand1 = pop();
 
    int len1;

    if(operand1.length > operand2.length)
        len1 = operand1.length;
    else
        len1 = operand2.length;


    switch(opperation){

        //Add
        case 1:
        len1 = len1 + 1;
        break;

        // Sub
        case 2:
        len1 = len1;
        break;

        // Mult
        case 3:
        len1 = (len1 + 1)*2 ;
        break;

        //Div
        case 4:

        len1 = len1;
        break;

        case 5:
        len1=len1-1;
        break;

        default:
        break;

    }
    
    //Choose the longest number to send as prepartion for result
    
    return  alloc_bigint(len1);

}

void trim_zeros(bigint* num , int sign_to_add){
    
    int len = num->length;
    digit *s = num->number;
    while (len > -1) {
        
        if(s[len]!=0 && (s[len]==1 || s[len]==2 ||s[len]==3 ||s[len]==4 ||s[len]==5 ||s[len]==6 ||s[len]==7 ||s[len]==8 ||s[len]==9)){
            break;
        }
        len--;
    }
if(len==-1)
{
    len=0;
}

    num->length = len+1;
    num->sign = sign_to_add;

}

int check_for_signs(){

    if(operand1.sign== 1 && operand2.sign == 0){
        return 1;
    }
    if(operand1.sign== 0 && operand2.sign == 1){
        return 2;
    }
    if(operand1.sign== 1 && operand2.sign == 1){
        return 3;
    }
    if(operand1.sign== 0 && operand2.sign == 0){
        return 4;
    }
    return 0;
}

int Plus_Operation(bigint* result , int signs){

    int sign_to_return;
    int compare;
    compare = _compare(operand1.number , operand2.number , operand1.length , operand2.length);
    switch(signs){
        case 1:

        if(compare==1){
           _sub( operand1 , operand2 , result );
           sign_to_return = 1;
       }

       else if(compare==(-1)){
        _sub( operand2 , operand1 , result );
        sign_to_return = 0;
    }

    else if(compare==0){
        _sub( operand2 , operand1 , result );
        sign_to_return = 0;
        result->length=1;
    }

    break;

    case 2:
    if(compare==1){
       _sub( operand1 , operand2 , result );
       sign_to_return = 0;
   }

   else if(compare==(-1)){
    _sub( operand2 , operand1 , result );
    sign_to_return = 1;
}

else if(compare==0){
    _sub( operand2 , operand1 , result );
    sign_to_return = 0;
    result->length=1;
}
break;

case 3:
if(compare==1){
   _add( operand1 , operand2 , result );
   sign_to_return = 1;
}

else{
    _add( operand2 , operand1 , result );
    sign_to_return = 1;
}
break;

case 4:
if(compare==1){
   _add( operand1 , operand2 , result );
   
   sign_to_return = 0;
}
else{
    _add( operand2 , operand1 , result );
    sign_to_return = 0;
}
break;

default:
break;


}

return sign_to_return;
}

int Minus_Operation(bigint* result , int signs){
    int sign_to_return;
    int compare;
    compare = _compare(operand1.number , operand2.number , operand1.length , operand2.length);
    switch(signs){
        case 1:

        if(compare==1){
           _add( operand1 , operand2 , result );
           sign_to_return = 1;
       }

       else {
        _add( operand2 , operand1 , result );
        sign_to_return = 1;
    }

    break;

    case 2:
    if(compare==1){
       _add( operand1 , operand2 , result );
       sign_to_return = 0;
   }

   else {
    _add( operand2 , operand1 , result );
    sign_to_return = 0;
}

break;

case 3:
if(compare==1){
   _sub( operand1 , operand2 , result );
   sign_to_return = 1;
}

else {
    _sub( operand2 , operand1 , result );
    sign_to_return = 0;
}
break;

case 4:
if(compare==1){
   _sub( operand1 , operand2 , result );
   sign_to_return = 0;
}
else if(compare==(-1)){
    _sub( operand2 , operand1 , result );
    sign_to_return = 1;
}
else if(compare==0){
    _sub( operand2 , operand1 , result );
    sign_to_return = 0;
    result->length=1;
}
break;

default:
break;


}
return sign_to_return;
}

int Mult_Operation(bigint* result , int signs){
    int sign_to_return;
    int compare;
    compare = _compare(operand1.number , operand2.number , operand1.length , operand2.length);
    switch(signs){
        case 1:

        if(compare==1){
           _mult( operand1 , operand2 , result );
           sign_to_return = 1;
       }

       else {
        _mult( operand2 , operand1 , result );
        sign_to_return = 1;
    }

    break;

    case 2:
    if(compare==1){
       _mult( operand1 , operand2 , result );
       sign_to_return = 1;
   }

   else {
    _mult( operand2 , operand1 , result );
    sign_to_return = 1;
}

break;

case 3:
if(compare==1){
   _mult( operand1 , operand2 , result );
   sign_to_return = 0;
}

else {
    _mult( operand2 , operand1 , result );
    sign_to_return = 0;
}
break;

case 4:
if(compare==1){
   _mult( operand1 , operand2 , result );
   sign_to_return = 0;
}

else {
    _mult( operand2 , operand1 , result );
    sign_to_return = 0;
}
break;

default:
break;


}
return sign_to_return;
}

int Divide_Operation(bigint* result , int signs){

    bigint* Help_Vector;
    bigint* Copy_Orign_B_Vector;
    Help_Vector = alloc_bigint((*result).length + 1);
    Copy_Orign_B_Vector = alloc_bigint((*result).length + 1);
    int sign_to_return;
    int compare;
    compare = _compare(operand1.number , operand2.number , operand1.length , operand2.length);

    switch(signs){
        case 1:
        if(compare==1){
           _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
           sign_to_return = 1;
       }

       else if(compare==0){
        _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
        sign_to_return = 1;
    }

    break;

    case 2:
    if(compare==1){
       _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
       sign_to_return = 1;
   }

   else if(compare==0){
    _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
    sign_to_return = 1;
}

break;

case 3:
if(compare==1){
   _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
   sign_to_return = 0;
}

else if(compare==0){
    _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
    sign_to_return = 0;
}

break;

case 4:

if(compare==1){
   _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
   sign_to_return = 0;

   
}

else if(compare==0){
    _div( operand1 , operand2 , result, Help_Vector ,Copy_Orign_B_Vector );
    sign_to_return = 0;
}

break;

default:
break;


}
return sign_to_return;
}

void do_operation(bigint* result , int opperation){

    int signs=0;
    int sign_to_add=0;
    signs = check_for_signs();

    switch(opperation){
        case 1:
        sign_to_add= Plus_Operation(result, signs);
        trim_zeros(result , sign_to_add);
        x = *result; 
        push();
        break;


        case 2:
        sign_to_add= Minus_Operation(result, signs);
        trim_zeros(result , sign_to_add);
        x = *result; 
        push();
        break;


        case 3:
        sign_to_add= Mult_Operation(result, signs);
        trim_zeros(result , sign_to_add);
        x = *result; 
        push();
        break;


        case 4:
        sign_to_add= Divide_Operation(result, signs);
        trim_zeros(result , sign_to_add);
        x = *result; 
        push();
        break;

        default:
        break;
    }

}

void inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    bigint * to_print;
    bigint * result_add;
    bigint * result_sub;
    bigint * result_mult;
    bigint * result_div;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    
    while(EOF!=(ch=fgetc(fp)) && ch != 'q'){
    // Case of digit    
     if(ch > 47 && ch < 58){
       str[len++]=ch;
       if(len==size){
        str = realloc(str, sizeof(char)*(size+=16));
    }
}

    // Case of delimiter    
else if(ch < 33 || (ch==42 || ch==43 || ch==45 || ch==47 || ch==99|| ch==112 || ch==113)){
  if(strlen(str) > 0 && isdigit(str[0])){
    str[len++]='\0';
                        bigint* a = newnumc( str, (len-1)); //Initiliaze bignum

                        

                        len=0;
                        x = *a;
                        push();

                       //Initiliaze next str to hold the next bignum
                        int tmp_len = sizeof(str);
                        memset(str, 1, tmp_len);
                    }   
                }

    // Case of operator
                switch (ch) {
                   case '_': 
                   next_num_is_negative=1;
                   break;

                   case '+': 
                   result_add = prepare_operation(1);
                   do_operation(result_add , 1);
                   break;

                   case '-':
                   result_sub = prepare_operation(2);
                   do_operation(result_sub , 2);
                   break;

                   case '*':
                   result_mult = prepare_operation(3);
                   do_operation(result_mult , 3);
                   break;


                   case '/':
                   
                   result_div = prepare_operation(4);
                   do_operation(result_div , 4);
                   break;

                   case 'c':
                   clear_stack();
                   break;

                   case 'p':
                   if(top==-1){ //Do nothing
                              }
                   else{
                    to_print = &stack[0];
                    print(to_print);
                    }
                   break;


                   default:
                   break;
               }


           }



           return;
       }


//this function allocates and returns a bigint pointer, with the number numb allocated
  bigint* newnumc(char* numb , int len)
  {
   //allocate storage for the bigint
     bigint* num = (bigint*)malloc(sizeof(bigint));

     int numbl = len;

     int numbll = numbl - 1;
   //allocate storage for the number, in the right length
     digit* out = (digit*)malloc(numbl);
     for(int i = 0; i < numbl; i++)
     {
      out[i] = numb[numbll - i] - CHARZERO_ABOVEINTZERO;
  }
  if(next_num_is_negative==1){
    (*num).sign=1;
    next_num_is_negative=0;
        }
    else
        {
    (*num).sign=0;
        }
  num->number = out;
  num->length = numbl;

   //return the new bigint
  return num;
}

bigint* alloc_bigint(int len)
{
   //allocate storage for the bigint
 bigint* num = (bigint*)malloc(sizeof(bigint));

   //allocate storage for the number, in the right length
 digit* out = (digit*)malloc(len);
 for(int i = 0; i < len; i++)
 {
  out[i] = 0;
}

num->number = out;
num->length = len;

   //return the new bigint
return num;
}


//clear a bigint that was created by newnumc
void delnum(bigint* num)
{
  
   //free the bigint's number
 free(num->number);


}

//print out a bigint's value
void print(bigint* num)
{
   
   digit* numc = num->number; //NOT A CHAR ARRAY
   int numl = num->length;
  
   int numll = numl - 1;
   char* out = (char*)malloc(numl + 1);

   if(num->sign==1){
    putchar('-');
}

for(int i = 0; i < numl; i++)
{

  out[i] = numc[numll - i] + CHARZERO_ABOVEINTZERO;
}
out[numl] = '\0';

puts(out);
free(out);
}

void push()
{
    if(top>=n-1)
    {
        //STACK is over flow

    }
    else
    {
        if(top==-1){
            top=0;
            stack[top]=x;
            top++;
        }
        else{

        stack[top]=x;
        top++;
    }
    }
}
bigint pop()
{
bigint to_return;
    if(top<=-1)
    {
        //Stack is under flow
    }
    else
    {

        top--;
        return stack[top];
    }
    return to_return;
}
void display()
{
 int i=0;
 while ( i < top ) {
    bigint * to_print = &stack[i];
    print(to_print);


    i++;
}

}

void clear_stack()
{
 int i=0;

 while ( i <= top ) {
   bigint * to_clear = &stack[i];
    delnum(to_clear);

    i++;
}
top=-1;

}
