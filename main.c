#include <stdio.h>										//Included some extra libraries
#include <stdlib.h>										//Like the standard library
#include <string.h>										//And strings
double conv2bin(int num);								//Function declarations, I'll explain them later
char * tohex(char * num);
char hexcomp(char * temp);
char * lui(int num, char * luiArray);
char * ori(int num, char * luiArray);
int multiply(int first, int second);
int divide(int first, int second, int * remainder);
int logicops(double firstb, double secondb, char * andb, char * orb, char * xorb, char * notb); /////////CHANGED FUNCTION PROTOTYPE/////////////////
int main(int argc, char *argv[])						
{
	FILE *afile;										//Not necessary but naming the files is convenient
	FILE *bfile;
	afile = fopen(argv[1],"r");							//We first try opening the first file
	if (afile == NULL)									//Making sure it is something valid
	{
		puts("error this is not a valid file");			//Then exiting out with an error if it isn't
		return 1;
	}
	bfile = fopen(argv[2],"w");							//For b file we just just check if we can create it
	if (bfile == NULL)
	{
		puts("error when attempting to make a new file");	//Not sure when it would actually occur that you couldn't...
		return 1;										//But just in case we check
	}	
	char str[100];										//First we create a string... generally long enough to handle a big integer
	float first;										//We create two floats... could have been ints but I got confused early on...
	float second;										//...as to what type of numbers we could be getting
	if (fgets(str, 100, afile) == NULL)					//I used fgets to put the contents of one line into the string
	{
		puts("error in reading file");					//Display an error if we can't read the file correctly
		return 1;
	}
	sscanf(str,"%f",&first);							//Then we try putting the float number into the first
	if (fgets(str, 100, afile) == NULL)					//We call fgets to get another float
	{
		puts("error in reading file");
		return 1;
	}
	sscanf(str,"%f",&second);							//Sscanf to put that float into the variable
	printf("%f\t",first);								//Throughout the program I print to stdout just to make my life easier
	printf("%f\n",second);								//So I don't need to check the output file all the time
	fprintf(bfile, "input numbers in decimal: %f\t%f\n",first,second);	//Here is where I actually output the line to the file

	double firstb;										//next we create two doubles to hold the binary patterns
	double secondb;										//looking back choosing strings would have been much better
	firstb = conv2bin(first);							//but I'm all for challenges... here we set the double equal to the output
	printf("%016.0f\t",firstb);							//of the conv2bin function and output the double with no decimals and 16 places
	secondb = conv2bin(second);							//then we do the same with the second binary
	printf("%016.0f\n",secondb);
	fprintf(bfile, "16-bit binary numbers: %016.0f\t%016.0f\n",firstb,secondb);	//output to the file

	char * luiArray;									//here we make an array of chars aka a string
	luiArray = (char*) calloc(32, sizeof(char));		//we use calloc cause you told us too to make space for 32 chars (32 bits)
	luiArray = lui(firstb, luiArray);					//then we use the lui function to store the first integer in the upper 16
	printf("%s\n",luiArray);							//and print it
	fprintf(bfile, "\"lui\" of num_1: %s\n",luiArray);

	luiArray = ori(secondb, luiArray);					//and use ori to put the second in the lower 16
	printf("%s\n",luiArray);							//and print it
	fprintf(bfile, "\"lui\" and \"ori\": %s\n",luiArray);

	int product;										//we create an int for the product
	product = multiply(first, second);					//call the product function
	printf("%d\n", product);							//print it
	fprintf(bfile, "Multiplication: %f * %f = %d\n", first, second, product);

	int quotient;										//same sort of deal with the quotient
	int * ptr;											//I used a pointer to get the remainder
	ptr = (int*) malloc(sizeof(int));					//allocated the size of one int for it
	quotient = divide(first, second, ptr);
	printf("%d (%d)\n", quotient, *ptr);				//then we print the quotient
	fprintf(bfile, "Division: %f / %f = %d (%d)\n",second,first,quotient,*ptr);
	char * andb = (char*) calloc (16, sizeof(char));		//////////////////////////////////////////////////////
	char * orb = (char*) calloc (16, sizeof(char));			//HERE I MOVED THE POINTERS INTO THE MAIN FUNCTION	//
	char * xorb = (char*) calloc (16, sizeof(char));		//**************************************************//
	char * notb = (char*) calloc (16, sizeof(char));		//////////////////////////////////////////////////////
	logicops(firstb, secondb, andb, orb, xorb, notb);		////////CHANGED FUNCTION CALL//////////////////
	char * andh = (char*) calloc (4, sizeof(char));		//then we get crazy, I put all the logical operator calculations into one function
	char * orh = (char*) calloc (4, sizeof(char));		
	char * xorh = (char*) calloc (4, sizeof(char));		//then we allocate some hexadecimal string pointers
	char * noth = (char*) calloc (4, sizeof(char));
	andh = tohex(andb);									//and use the tohex function to convert the binary strings to hex
	orh = tohex(orb);									//*note that the binary strings were declared global earlier so that is
	xorh = tohex(xorb);									//why you don't see them declared here
	noth = tohex(notb);
	printf("%s\t%s\n",andb,andh);						//then lots of printing
	printf("%s\t%s\n",orb,orh);
	printf("%s\t%s\n",xorb,xorh);
	printf("%s\t%s\n",notb,noth);
	fprintf(bfile, "num_1 & num_2: %s\t%s\n",andb,andh);
	fprintf(bfile, "num_1 | num_2: %s\t%s\n",orb,orh);
	fprintf(bfile, "num_1 ^ num_2: %s\t%s\n",xorb,xorh);
	fprintf(bfile, "~num_2: %s\t%s\n",notb,noth);

	fclose(afile);										//then close the files
	fclose(bfile);
	free(andb);											//free up memory for a gazillion things
	free(orb);
	free(xorb);
	free(notb);
	free(andh);
	free(orh);
	free(xorh);
	free(noth);
	free(ptr);
	free(luiArray);
	return 0;											//and we are done!
}
char * tohex ( char * num)								//my functions are in no particular order and not in a separate file as they probably should (sorry)
{														//this converts binary to hex
	char * temp = (char*) calloc (4, sizeof(char));		//first we create a bunch of temporary strings to hold 4 bits
	char * temp2 = (char*) calloc (4, sizeof(char));
	char * temp3 = (char*) calloc (4, sizeof(char));
	char * temp4 = (char*) calloc (4, sizeof(char));
	char * numh = (char*) calloc (4, sizeof(char));
	int i;
	temp[4] = '\0';										
	for (i = 0; i < 4; i++)								//then we repeat for 4 times
	{
		temp[i] = num[i];								//and divide up the 16 bit binary into 4 shorter bit patterns
		temp2[i] = num[(i+4)];
		temp3[i] = num[(i+8)];
		temp4[i] = num[(i+12)];
	}
	printf("%s\t%s\t%s\t%s\n",temp,temp2,temp3,temp4);
	char c;
	numh[0] = hexcomp(temp);							//then we secretly call another function to calculate the proper hex value
	numh[1] = hexcomp(temp2);
	numh[2] = hexcomp(temp3);
	numh[3] = hexcomp(temp4);

	free(temp);											//then we free up all the temporary variables
	free(temp2);
	free(temp3);
	free(temp4);										
	return numh;										//finally returning the correct hex number
	
}
char hexcomp(char * temp)								//this is a long but very simple and redundant function
{
	char c;
	if (strcmp(temp,"0000") == 0)						//literally we compare the string of 4 binary bits
	{													//to another string... and figure out what number or letter represents it
		c = '0';
	}
	else if (strcmp(temp,"0001") == 0)
	{
		c = '1';
	}
	else if (strcmp(temp,"0010") == 0)					//again and again...
	{
		c = '2';
	}
	else if (strcmp(temp,"0011") == 0)
	{
		c = '3';
	}
	else if (strcmp(temp,"0100") == 0)
	{
		c = '4';
	}
	else if (strcmp(temp,"0101") == 0)
	{
		c = '5';
	}
	else if (strcmp(temp,"0110") == 0)
	{
		c = '6';
	}
	else if (strcmp(temp,"0111") == 0)
	{
		c = '7';
	}
	else if (strcmp(temp,"1000") == 0)
	{
		c = '8';
	}
	else if (strcmp(temp,"1001") == 0)					//and again...
	{
		c = '9';
	}
	else if (strcmp(temp,"1010") == 0)
	{
		c = 'A';
	}
	else if (strcmp(temp,"1011") == 0)
	{
		c = 'B';
	}
	else if (strcmp(temp,"1100") == 0)
	{
		c = 'C';
	}
	else if (strcmp(temp,"1101") == 0)
	{
		c = 'D';
	}
	else if (strcmp(temp,"1110") == 0)
	{
		c = 'E';
	}
	else if (strcmp(temp,"1111") == 0)
	{
		c = 'F';
	}
	return c;											//until we have exhausted all options and return the answer
}
int logicops(double firstb, double secondb, char * andb, char * orb, char * xorb, char * notb) /////////CHANGED FUNCTION TYPE/////////////////
{														
	char temp[16];										//anyways we make two strings to hold 16 bit binary patterns
	char temp2[16];										//this function calculates logical functions btw
	sprintf(temp,"%016.0f",firstb);						//since the patterns passed in are in the form of doubles, we must rip them into strings
	sprintf(temp2,"%016.0f",secondb);					//sounds painful but its not...
	if (firstb > 10000000000000000)						//this is really weird... idk why I had to do this but when I ran my function the first sprintf
	{													//would not for the life of me put in a character in the temp[0] spot... it would for the temp2
		temp[0] = '1';									//just not temp one so idk what the hell is goin on but I had to manually check for just that
	}													//one character which is why this is here
	else
	{
		temp[0] = '0';
	}
	int i;		
	for (i = 0; i < 16; i++)							//after we have our converted strings of bits
	{
		if (temp[i] == '1' && temp2[i] == '1')			//for each bit in each string we check what it is
		{
			andb[i] = '1';								//depending on how it compares, we make the output strings
			orb[i] = '1';								//a one or a zero
			xorb[i] = '0';
			notb[i] = '0';
		}
		else if (temp[i] == '0' && temp2[i] == '0')
		{
			andb[i] = '0';
			orb[i] = '0';
			xorb[i] = '0';
			notb[i] = '1';
		}
		else if (temp[i] == '0' && temp2[i] == '1')
		{
			andb[i] = '0';
			orb[i] = '1';
			xorb[i] = '1';
			notb[i] = '0';
		}
		else if (temp[i] == '1' && temp2[i] == '0')		//sort of self explanatory
		{
			andb[i] = '0';
			orb[i] = '1';
			xorb[i] = '1';
			notb[i] = '1';
		}
	}
	return 0;											//we don't need to return anything cause all the stuff is in global pointers
}
int divide(int first, int second, int * remainder)		//here is the divide function... had hellofa time figuring this out
{
	if (first > second)									//if first is greater than second (obviously dividing 6 by 7 doesn't take much thought)
	{
		*remainder = second;							//the remainder is equal to second
		return 0;										//and the answer is 0
	}
	int divisor = first;								//otherwise we actually have to do work
	int quotient = 0;							
	while (second >= first)								//first we have to shift the divisor left until it is bigger than the number being divided
	{
		first <<= 1;
	}
	while (first >= divisor)							//then while the divisor is bigger
	{
		second = second - first;						//we subtract the divisor from the remaining number
		if (second >= 0)								//if the answer is larger than zero, then it divided succesfully
		{
			quotient <<= 1;								//we can shift the quotient left
			quotient++;									//and add one
			first >>= 1;								//then shift the divisor right
		}
		else
		{
			second = second + first;					//otherwise it didn't divide correctly and we return the divided number to what it was
			first >>= 1;								//and we shift the quotient and divisor without adding one to either
			quotient <<=1;								//this is all very confusing but think about long division of binary numbers
		}												//if you can divide it you put a one up top and subtract, otherwise you
	}													//put a zero up top and bring down the next number and compare again
	*remainder = second;								//what is left is the remainder
	return quotient;									//and we return the quotient
}
int multiply(int first, int second)						//multiply is 5 times easier
{
	int product = 0;									//exactly how mips does it
	while (first > 0)									//while the first number is greater than zero
	{
		if(first % 2 == 1)								//if it can't be divided by two (the last bit is 1)
		{			
			product = product + second;					//then we add the second number to the product
		}
		first >>=1;										//then we always shift the first number left
		second <<= 1;									//and second right
	}	
	return product;										//finally returning the product
}
char * ori(int num, char * luiArray)					//ori is easy
{
	char temp[16];										//create a temporary string
	sprintf(temp,"%016d", num);							//rip the integer binary pattern into it
	int i;
	for (i = 16; i < 32; i++)							//then put it into the luiArray
	{
		luiArray[i] = temp[i-16];
	}
	return luiArray;
}
char * lui (int num, char * luiArray)					//lui isn't that bad either
{
	char temp[32];										//create a 32 char string
	sprintf(temp, "%016d", num);						//rip the first integer into the first 16 bits
	char x = '0';	
	int i;
	for (i = 16; i < 32; i++)							//fill the rest with zeros
	{
		temp[i] = x;
	}
	for (i = 0; i < 32; i++)							//then put the whole thing into the luiArray
	{
		luiArray[i] = temp[i];
	}
	return luiArray;									//returning the array
}
double conv2bin(int num)								//finally this is the bread and butter, converting base 10 into binary
{
	double twop;										//I had to use doubles, cause I wanted to store the "binary" string as a number
	double count;										//but integers couldn't hold large enough numbers (I mean 16 places is a lot)
	double bin;
	bin = 0;
	while (num >= 2)									//while the number being converted is greater than 2
	{
		twop = 2;										//twop stands for power of 2
		count = 10;										//count is really a power of 10
		while ((twop*2) <= num)							//while twop is less than the number
		{
			twop = twop*2;								//we multiply it by two over and over
			count = count*10;							//and count by 10 over and over
		}
		num = num - twop;								//until we have the largest number smaller than or equal to the original number
		bin = bin + count;								//and we add count to the binary number, a power of 10 will be all ones or zeros
	}
	if (num >= 1 && num < 2)							//this is all unnecessary but the last bit is either a one or a zero
	{
		bin++;
		num--;
	}
	if (num == 0)										//and below this point, I made it so my program could convert decimals too
	{													//but the TA said it wasn't necessary, so I commented it out
		return bin;										//but if I needed to I could convert like 72.125 into binary
	}													//so that's pretty neat... 
	else
	{/*
		count = 1;
		while (num != 0)
		{
			count = count * .1;
			num = num*2;
			if (num >= 1)
			{
				bin = bin+count;
				num--;
			}
		}*/
		return bin;										//the end... What did sushi A say to sushi B................... Wasabi (wassup B)
	}
}