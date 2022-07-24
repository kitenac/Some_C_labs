#include <stdio.h>
#include <stdlib.h>//for malloc()
#include <string.h>//for strlen() 

//PLAN: Char str ---> 5-clustered Bin str, with tailing(by needance) --->
//      --->[each 5-bit str ---> 10-base str] ---> 29-base char str (4 symbols of possible 32(5-bit cluster codes special)


int main()
{
//making string array for filling later
 int i,j;//arrays` counters
 int t;//subcounter for 2nd array
 int num;
 int deg;//simulator of base2`s degree
 int n10;//size of string below
 char* str_10;//pointer to 10-based string
 str_10 = (char*) malloc( n10*sizeof(char) );

//filling it for encrypt 
 printf("\nUsername, u are to write your text without spacebar(use anyth else u want to devide) \nor your massage will be losst and don`t worry about register :)\n");
 scanf("%s",str_10);
 
 n10 = strlen(str_10);
 
//------------Encrypting---------------
 //preparing binary array of str above
 int* str_bin;//pointer to spoken array
 str_bin = (int*)malloc( (n10*8) * sizeof(int) );

 //getting bin string
 j=-1;//(-1) is for 1-st loop`s incrimentation be zero  
 for(i=0;i<n10;++i)
  {
   num=str_10[i];
   deg=128;
   for(t=0;t<8;++t) // num base10---> 8bits in base2  
    {
     j++;
     str_bin[j]=num/deg;
     num=num%deg;
     deg=deg/2;
    }
  }
  
 
 //cycle-5bit-looking-&-translating of bin string to 10base
 int* str2_10;
 int N;
 if ((n10*8)/5 == 0) N=(n10*8)/5;
 else N = (n10*8)/5 + 1;//if num of bits not devides by 5 we add tail not to miss some bits of info in bin str 
 str2_10 = (int*)malloc(N*sizeof(int));
   
 i=-1;//to become 0 in first loop
 for(j=0; (j+5)<n10*8;j+=5)//listing each 5-bite claster per loop
  //j+5 compares not jump out n10*8 length str on loop
 { 
  i++;//i-now index in transated to 10base str
  deg=16;//max bit of 5bit-substring means 2*2*2*2
  for(t=j;t<j+5;t++) //5-step translating of cluster to number(in 10base)
   { 
    str2_10[i]+=deg*str_bin[t];
    deg=deg/2;
   }  //this place we`ve got str2_10[i], now compare it with stinkiest values
//(4 ones that(29=0a,30=0b,31=0c,) costs 2 additinal diget)
   
 } //for-ended and all the 5-bities are over, but who knows if there is a tail<5?
 
   
 int tail=(n10*8)%5;//if tail exist, it equals to  (n10*8)%5
 if( j != (n10*8)-1)//"producing" tale(adding ..000. till 5-devideable bin str) if need
 //this 'if' trigs if number of bits in bin str not devides by 5
  {
    j=n10*8-1;//so it means that virtualy we continued 'listing-for'
              //like we had a str which devided by 5 
    deg=16;
    str2_10[i+1]=0;//previosly we stopped on i-st symbol,
    //so cleannig trash in (i+1)-st before filling
    for(t=j-(tail-1);t<n10*8;++t)// (j-(tail-1))-to watch previously
           //is no need because there`re already watched bits
           //and to watch forward (n10*8-1) too(there`re only zeros) 
     {
      str2_10[i+1]+=deg*str_bin[t];
      deg=deg/2;
     }
      
  }

/*
 Now we need to translate 10-based string by ASCII-29Base`s table
 0=a 1=b 2=c 3=d ... 25=z 26=1 27=2 28 = 0
  This 4 enodes by 2 digits of 29-base-table
   29=0a 30=0b 31=0c,
 but we can have situation 000b -  how to
 decode it to 10base: 0,0,0,1 or 0,0,30 ?

 !!!for cases like this we count number of 
 going line zeros-if it`s odd( x mod 2 = 1)we decode 
 assotiating last 0 in the line with letter(it can be a,b or c)
 as two digets in 29base.
 If number of zeroes is even we decode every letter in line separatly
 by the table.
*/



//Below we scope all the situations that requaires additional zero(stinky)
//than we can allocate memory for encoded string and encode.
//P.S. two huge for-if-...-if are very simulary 
 int zeroes_line = 0;
 int stinky = 0;//ammount of additional digets in encrypted str
 for(j=0;j<N;++j)
  { 
    if(str2_10[j]==28)//'0' in ASCII
     {
      zeroes_line++;//we have one more 0 above
      if(str2_10[j+1]==28)
       {
        zeroes_line++;//we have one more 0 above
         if(str2_10[j+2]==0 || str2_10[j+2]==1
            || str2_10[j+2]==2  )//'a','b','c' in b29
          {
           if (zeroes_line%2==1)//not even, but must be 
             stinky++;//we add one digit 0 to encode correctly

           zeroes_line=0;//the line of zeroes ended on [j+2]-st
           j+=2;//actually j+=3 ([j+2]-st != 0)  but we have incriment in end 
          }      //of 1st 'if' of this 'if-tree'  

          else if(str2_10[j+2]==29 || str2_10[j+2]==30
                  || str2_10[j+2]==31 )
           {
            stinky++;//any of abowe requaires additional zero didget by left

            if(zeroes_line%2==0)//not odd,but must be
             stinky++;//adding one zero left for correct encoding

            j+=2;//actually j+=3 ([j+2]-st != 0)  but we have incriment in end 
           } 
        j++;//we have allready checked that [j-1] is zero
       }
     } //1-st if ended
  }//for ended


 char *encrpt_str;
 int N2=N+stinky;//ammount of encrypted symbols
 encrpt_str =(char*)malloc(N2*sizeof(char));
 j=0;
//here we go with final encrypting 
 zeroes_line=0;
 for (i=0;i<N;++i)
 {
  if(str2_10[i]<=25) 
   {
     encrpt_str[j] = str2_10[i] + 97;//look at ASCII table
     j++;
   }

  if(str2_10[i]==26 || str2_10[i]==27) 
   {
     encrpt_str[j] = str2_10[i] + 23;
     j++;
   }


  if(str2_10[i]==28) 
   {
     encrpt_str[j]='0';
     zeroes_line++;//we have one more 0 above
      if(str2_10[i+1]==28)
       {
        zeroes_line++;//we have one more 0 above
        encrpt_str[j+1]='0';
        
         if(str2_10[i+2]==0 || str2_10[i+2]==1
            || str2_10[i+2]==2  ) //'a','b','c' in b29
          {
           if (zeroes_line%2==1)//not even, but must be 
           {
            str2_10[j+2] = 0;//adding zero for correct encoding
            str2_10[j+3] = str2_10[i+2] + 97;
            j+=2;//watched [j+2]-th,[j+3]-th
           }
           if (zeroes_line%2==0)//even, as must be 
           {
            str2_10[j+2] = str2_10[i+2] + 97;
            j++;//watched [j+2]-th
           }

           zeroes_line=0;//the line of zeroes ended on [j+2]-st
           i++;//watched [i+2]-th
          }   
           
         else if(str2_10[i+2]==29 || str2_10[i+2]==30
                 || str2_10[i+2]==31 )
          {
           if(zeroes_line%2==0)//not odd,but must be
            {
             str2_10[j+2] = 0;//adding zero for correct encoding
             str2_10[j+3] = str2_10[i+2] + 97;
             j+=2;//watched [j+2]-th,[j+3]-th
            }
           if(zeroes_line%2==1)//odd, as must be 
           {
            str2_10[j+2] = str2_10[i+2] + 97;
            j++;//watched [j+2]-th
           }
             
            zeroes_line = 0;
            i++;//watched [i+2]-th
           }

         
        j++;//watched [j+1]-th
        i++;//watched [i+1]-th
       }
     j++;//watched [i]-th     
    }

   if(str2_10[i]>28) 
   {
     encrpt_str[j]='0';
     encrpt_str[j+1]= str2_10[i]%29 + 97;//ASCII alpf-step = 97 like for nums<=25
     j+=2;
   } 
 }//last 'for' ended
 
 printf("------------------\nEncoded text: ");
 for (i=0;i<N2;++i) { printf("%c",encrpt_str[i]); }//printing encrypted str in base29
 
 printf("\nGrowing-coifficent: %f\n", (float)N2/(float)n10 );//helps to know efficiency factor
 
 return 0;
}
