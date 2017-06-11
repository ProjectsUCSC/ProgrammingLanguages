/*Program to implement a POSIX based regex parser which performs the following operations

.   Any single character. Example: h.t matches hat, hit, hot and hut.

[ ] Any one of the characters in the brackets, or any of a range of characters separated by a hyphen (-), or a character class operator
    (see below). Examples: h[aeiou][a-z] matches hat, hip, hit, hop, and hut; [A-Za-z] matches any single letter; x[0-9] matches x0, x1,
    …, x9.

[^] Any characters except for those after the caret "^". Example: h[^u]t matches hat, hit, and hot, but not hut.

^   The start of a line (column 1).

$   The end of a line (not the line break characters). Use this for restricting matches to characters at the end of a line. Example: end$
    only matches "end" when it's the last word on a line, and ^end only matches "end" when it's the first word on a line.

\<  The start of a word.

\>  The end of a word.

\t  The tab character.

\f  The page break (form feed) character.

\n  A new line character, for matching expressions that span line boundaries. This cannot be followed by operators '*', '+' or {}. Do not
    use this for constraining matches to the end of a line. It's much more efficient to use "$".

*   Matches zero or more of the preceding characters or expressions. Example: ho*p matches hp, hop and hoop.

?   Matches zero or one of the preceding characters or expressions. Example: ho?p matches hp, and hop, but not hoop.

+   Matches one or more of the preceding characters or expressions. Example: ho+p matches hop, and hoop, but not hp.

{count}   Matches the specified number of the preceding characters or expressions. Example: ho\{2\}p matches hoop, but not hop.

{min,}    Matches at least the specified number of the preceding characters or expressions. Example: ho\{1,\}p matches hop and hoop, but
            not hp.

{min,max} Matches between min and max of the preceding characters or expressions. Example: ho\{1,2\}p matches hop and hoop, but not hp or hooop.

\|  Matches either the expression to its left or its right. Example: hop\|hoop matches hop, or hoop.

\   "Escapes" the special meaning of the above expressions, so that they can be matched as literal characters. Hence, to match a literal "
    \", you must use "\\". Example: \< matches the start of a word, but \\< matches "\<".

Character Class Operators "[: ... :]":

These can be used in class expressions as an alternative way of representing classes of characters. For example, [a-z0-9] is equivalent to [[:lower:][:digit:]]. (Note the extra pairs of brackets.) The defined classes are:
Expression: 	Description:
[:alpha:] 	Any letter.
[:lower:] 	Any lower case letter.
[:upper:] 	Any upper case letter.
[:alnum:] 	Any digit or letter.
[:digit:] 	Any digit.
[:xdigit:] 	Any hexadecimal digit (0-9, a-f or A-F).
[:blank:] 	Space or tab.
[:space:] 	Space, tab, vertical tab, return, line feed, form feed.
*/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SPACE 32
#define LINE 10
#define TAB 9
#define FORM 12
#define SLASH 92
#define MAX_REGEX_LENGTH 1024 + 400
#define MAX_STRING_LENGTH 2048 + 400

//The largest regex string would be 1024 and the largest compare string would be 2048.

/* match: search for regexp anywhere in text */

int extra_length = 0;

int match(char *regexp, char *text)
{
    /*
        The main part of the algorithm which keeps calling matchhere for every character of regex with the entered string
    */

    char newtext[MAX_STRING_LENGTH + 1], newregex[MAX_REGEX_LENGTH + 1];
    int index = 0;

    if (regexp[0] == '^')
        return matchhere(regexp+1, text);

    if (regexp[0] == '/' && regexp[1] == '<')//Used to match the start of a word
    {
        newtext[0] = ' ';/*        The first word in the text is prefixed with a space*/
        strcpy(newtext + 1, text);
        newregex[0] = ' ';/*        The regex is also prefixed with a space so that it only matches words prefixed with a space(i.e start of words)*/
        strcpy(newregex + 1, regexp + 2);
        //printf("regex=%s\nstring=%s\n", newregex, newtext);
        regexp = newregex;
        text = newtext;
    }

    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
        {
            printf("Preprocessed regular expression %s\n", regexp);
            printf("Part of the string matched - > ");
            for (int iter=0; iter < strlen(text) - extra_length; iter ++)
                printf("%c", *(text + iter));            
            printf("\n");
            return 1;
        }
        index ++;
    } while (*text++ != '\0');

    return 0;
}

int getvaluelabel(char *c, int j,  char *label, int mask)
{
    /*
        Used for [:expressions:]
        Input: c containing the expression label
        Result: c is replaced with the content denoted by the label
        Output: returns the length of expanded c
    */

    int k = 0;
    if (!strcmp(label, "alpha"))
    {
        j = getvaluelabel(c, j, "lower", mask);
        j = getvaluelabel(c, j, "upper", mask);
    }
    else if(!strcmp(label, "lower"))
    {
        if(!mask)
            c[j] = 97;
        else
            c[j] = c[0];
        k = 0;
        while(k < 26)
            if(!mask)
                c[++j] = 97 + (++k);
            else
                c[++j] = c[0];
    }
    else if(!strcmp(label, "upper"))
    {
        if(!mask)
            c[j] = 65;
        else
            c[j] = 0;
        k = 0;
        while(k < 26)
            if(!mask)
                c[++j] = 65 + (++k);
            else
                c[++j] = c[0];
    }
    else if(!strcmp(label, "alnum"))
    {
        j = getvaluelabel(c, j, "digit", mask);
        j = getvaluelabel(c, j, "alpha", mask);
    }
    else if(!strcmp(label, "digit"))
    {
        if(!mask)
            c[j] = 48;
        else
            c[j] = c[0];
        k = 0;
        while(k < 10)
            if(!mask)
                c[++j] = 48 + (++k);
            else
                c[++j] = c[0];
    }
    else if(!strcmp(label, "xdigit"))
    {
        j = getvaluelabel(c, j, "digit", mask);
        if(!mask)
            c[j] = 65;
        else
            c[j] = c[0];
        k = 0;
        while(k < 6)
            if(!mask)
                c[++j] = 65 + (++k);
            else
                c[++j] = c[0];
        c[j] = 97;
        k = 0;
        while(k < 6)
            if(!mask)
                c[++j] = 97 + (++k);
            else
                c[++j] = c[0];
    }
    else if(!strcmp(label, "blank"))
    {
        if(!mask)
        {
            c[j] = SPACE;
            c[++j] = TAB;
        }
        else
            c[j] = c[++j] = c[0];
    }
    else if(!strcmp(label, "space"))
    {
        j = getvaluelabel(c, j, "blank", mask);
        if(!mask)
            c[++j] = FORM;
        else
            c[++j] = c[0];
    }
    else if(!strcmp(label, "cntrl"))
    {
        k = 0;
        while( k < SPACE)
            if(!mask)
                c[j++] = k;
            else
                c[j++] = c[0];
    }
    return j;
}

/* matchhere: search for regexp at beginning of text */
int matchhere(char *regexp, char *text)
{
    /*
        A recursive function which recurses, consuming the text and matching everything with the regex
    */

    char c[MAX_REGEX_LENGTH + 500], temp, label[8];
    int i, j, k;
    if (regexp[0] == '\0')
    {
        extra_length = strlen(text);
        return 1;
    }
    if (regexp[0] == '[') //Dealing with square brackets
    {
        printf("\nEncounter open\n");
        j = 0;
        i = 1;
        if(regexp[1] != '^')
        {
            while (regexp[i] != ']')
            {
                if (regexp[i] == ':')
                {
                    k = 0;
                    i++; // skip the :
                    while( regexp[i] != ':')
                        label[k++] = regexp[i++];
                    label[k] = '\0';
                    i++;
/*                    printf("label=%s\n", label);*/
                    j = getvaluelabel(c, j, label, 0);
                }
                else if(regexp[i] == '[')
                {
                    i += 2; //skip the : and [
                    k = 0;
                    while( regexp[i] != ':')
                        label[k++] = regexp[i++];
                    label[k] = '\0';
                    i+=2; //skip the : and ]
                    j = getvaluelabel(c, j, label, 0);
                }
                else if (regexp[i] == '-') //When we want a range
                {
                    temp = regexp[i-1] + 1;
                    while(temp < regexp[i+1])
                        c[j++] = temp++;
                    i += 1;
                }
                else
                    c[j++] = regexp[i++];
            }
            printf("\nEncountered close");
            c[j] = '\0';
        }
        else if(regexp[1] == '^') //Dealing with complements inside brackets
        {
            j = 0;
            //First take all the ascii values into c, all these are allowed
            while(j <= 255)
                if ((char)j != '\0' && (char)j != '.')
                    c[j] = (char)j++;
                else
                    c[j] = j++ - 1;

            c[j] = '\0';

            j = 0;
            i = 2;
            while (regexp[i] != ']')
            {
                //Replace some junk character(smiley) in place of those characters which are not allowed (as marking, don't consider)
                if(regexp[i] == '[')
                {
                    i += 2; //skip the :
                    k = 0;
                    while( regexp[i] != ':')
                        label[k++] = regexp[i++];
                    label[k] = '\0';
                    i+=2; //skip the : and ]
                    j = getvaluelabel(c, j, label, 1);
                }
                else if (regexp[i] == '-')
                {
                    temp = regexp[i-1] + 1;
                    while(temp < regexp[i+1])
                        c[temp++] = c[0];
                    i += 1;
                }
                else
                    c[regexp[i++]] = c[0];
            }
        }
        if (regexp[i + 1] == '*')
        {
            extra_length = strlen(text);
            return matchstarbrackets(c, regexp + i + 2, text);
        }
        else if (regexp[i + 1] == '+')
        {
            extra_length = strlen(text);
            return matchplusbrackets(c, regexp + i + 2, text);
        }
        else if (regexp[i + 1] == '?')
        {
            extra_length = strlen(text);
            return matchquebrackets(c, regexp + i + 2, text);
        }
        else if (isin(text++, c))
        {
            extra_length = strlen(text);
            return matchhere(regexp + i + 1, text);
        }       
        else
            return 0;
    }
    if (regexp[1] == '?')
    {
        extra_length = strlen(text);
        return matchquemark(regexp[0], regexp+2, text);
    }
    if (regexp[1] == '*')
    {
        extra_length = strlen(text);
        return matchstar(regexp[0], regexp+2, text);
    }
    if (regexp[1] == '+')
    {
        extra_length = strlen(text);
        return matchplus(regexp[0], regexp+2, text);
    }
    if (regexp[0] == '$' && regexp[1] == '\0')
    {
        extra_length = strlen(text);
        return *text == '\0';
    }
    if (regexp[0] == '/' && regexp[1] == '>' && regexp[2] == '\0')
    {
        extra_length = strlen(text);
        return *text == ' ' || *text == '\0';
    }
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
    {
/*        printf("till %s\n", text+1);*/
        extra_length = strlen(text + 1);
        return matchhere(regexp+1, text+1);
    }
    return 0;
}

int matchquemark(int c, char *regexp, char *text)
{
    if(matchhere(regexp, text))
        return 1;
    else if((*text++ == c || c == '.'))
        return matchhere(regexp, text);
    return 0;
}

int matchquebrackets(char *c, char *regexp, char *text)
{
    if(matchhere(regexp, text))
        return 1;
    else if(isin(text++, c))
        return matchhere(regexp, text);
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text)
{
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

int isin( char *text, char *c)
{
    int i = 0, l;
    l = strlen(c);
    while(i < l)
    {
        if (c[i] == '.')
            return 1;
        else if((int)*text == (int)c[i])
            return 1;
        i++;
    }
    return 0;
}

int matchstarbrackets(char *c, char *regexp, char *text)
{
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (isin(text++, c)));
    return 0;
}


int matchplusbrackets(char *c, char *regexp, char *text)
{
    while (*text != '\0' && (isin(text++, c)))
    {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    }
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int matchplus(int c, char *regexp, char *text)
{
    while (*text != '\0' && (*text++ == c || c == '.'))
        if (matchhere(regexp, text))
            return 1;
    return 0;
}

void preprocessfreq(char *reg, int length)
{
    /*
        To deal with {count}, {min, }, {min, max} operators we follow the logic as per the below example,
        Assume the regex is abc
        For abc\{2\} we transform regex to abcc
        For abc\{2,\} we transform it to abcc+
        For abc\{2,5\} we transform it to abccc?c?c?
        The above logic is implemented as follows:
            Implement the part for {min, max}
            For count, do min = max and use the same logic as {min,max}
            For {min}, do max = -1 and make it *

        This function does the above transform
        Input: The regex and its length
        Result: The regex is transformed
    */

    int i, j, k, x, ib, jb, iter, min, max, expand, level;
    char temp[MAX_REGEX_LENGTH], num[10];
    i = 0;
    j = 0;
    k = 0;
    x = 0;
    ib = 0;
    jb = 0;
    iter = 0;
    while(i < length)
    {
        //printf("came %d\n", i);
        if(i < (length - 1) && reg[i] == SLASH)
        {
            if(reg[i + 1] == '{') // Found a pattern \{
            {
                //We have entered the part where
                expand = i - 1;
                i+=2; //skip slash and {

                //Obtain the number in num
                k = 0;
                while(reg[i] != ',' && reg[i] != SLASH)
                    num[k++] = reg[i++];
                num[k] = '\0';

                if(reg[i] == SLASH) //This means we are dealing with count
                    min = max = atoi(num);
                else
                {
                    min = atoi(num);
                    if(reg[i + 1] != SLASH) //This means we are dealing with {min,max}
                    {
                        i++;//skip the comma

                        //Obtain the number in num
                        k = 0;
                        while(reg[i] != SLASH)
                            num[k++] = reg[i++];
                        num[k] = '\0';

                        max = atoi(num);
                    }
                    else //This means we are dealing with {min,}
                    {
                        i++;//skip the comma
                        max = -1;
                    }

                }
                i+=2; //skipping slash and }

                //We are done finding the values of min and max, now we proceed to transform the regex as per the logic

                if(reg[expand] != ']') //When the character before the {min, max} type function is not ']' => It is a single character
                {
                    //first expand min no. of times
                    for(iter=0;iter<min-1;iter++)
                        temp[x++] = reg[expand];

                    //Expanding with question mark
                    for(;iter<max-1;iter++)
                    {
                        temp[x++] = reg[expand];
                        temp[x++] = '?';
                    }

                    //Expand with asterix for {min, }
                    if(max == -1)
                        temp[x++] = '+';
                }
                else
                {
                    /*
                        When the character preceeding the {min, max} type function is a ']', we first find the opening brace corresponding
                        to it. Next we repeat the entire string from the '[' till ']' of that pattern
                    */

                    level = 1;//This is used to keep track of level of '['

                    //Find the opening brace
                    ib = expand;
                    while(level != 0)
                    {
                        ib--;
                        if(reg[ib] == '[')
                            level--;
                        else if(reg[ib] == ']')
                            level++;
                    }
                    //now ib is pointing at [ corresponding to the ']'

                    //Expanding min times
                    for(iter=0;iter<min-1;iter++)
                    {
                        for(jb=ib;jb<=expand;jb++)
                            temp[x++] = reg[jb];
                    }

                    //Expanding with question mark for {min, max}
                    for(;iter<max-1;iter++)
                    {
                        for(jb=ib;jb<=expand;jb++)
                            temp[x++] = reg[jb];
                        temp[x++] = '?';
                    }

                    //Expand with asterix for {min, }
                    if(max == -1)
                        temp[x++] = '+';
                }
            }
        }
        else
                temp[x++] = reg[i++]; //When that function is absent, we simply copy
    }
    temp[x] = '\0';
    
    //Now we copy the entire message to regex
    i = 0;
    while(temp[i] != '\0')
        reg[i] = temp[i++];
    reg[i] = '\0';
}

int get_hex(char *str)
{
    return (int)strtol(str, 0, 16);
}

void preprocess(char *str, int length, int flag)
{
    /*
        If the string contains characters \n,\t,\f,\\ they are replaced with their ASCII values -> Type-1
        Occurences of \{, \<, \>, \} are retained as is. -> Type-2
        Input: Unfiltered string, its length, flag
        flag = 1 => the string is a regex, else it is a string
        Result: The string is filtered
    */

    int i, j, dec_flag, k;
    char temp[MAX_STRING_LENGTH], num[10];
    i = 0;
    j = 0;
    
    //We create the filtered regex in variable temp and later copy the value of temp to regex variable reg
    while(i < length)
    {   
        if(i < (length - 1) && str[i] == SLASH)
        {
            if(str[i+1] == 'n') //If it is a \n
            {
                temp[j++] = LINE;
                i+=2;//skip \ and the character after it
            }
            else if(str[i+1] == 't') // If it is \t
            {
                temp[j++] = TAB;
                i+=2;//skip \ and the character after it
            }
            else if(str[i+1] == 'f') // \f
            {
                temp[j++] = FORM;
                i+=2;//skip \ and the character after it
            }
            else if((str[i+1] == 'd' || str[i+1] == 'x') && flag) //This preprocessing has to be done ony for regex
            { 
                dec_flag = (str[i+1] == 'd'); //sets dec_flag to true if it is a decimal we are dealing with
                i += 2; //Go to the character after 'x' or 'd'
                
                //Now we store the character to obtain the ascii value
                k = 0;
                while(str[i] != SLASH)
                    num[k++] = str[i++];
                num[k] = '\0';
                
                i+=1;//There is a line doing i += 
                
                //We convert the string to its equivalent ascii
                if(dec_flag)
                    temp[j++] = (char)atoi(num);
                else
                    temp[j++] = (char)get_hex(num);
            }
            else if(str[i+1] == SLASH && flag == 1) //When it is a regex, \\ has to be replaced with ASCII of slash
            {
                temp[j++] = SLASH;
                i+=2;//skip \ and the character after it
            }
            else
            {
                //When it is of type-2
                i--; // We decrement so that i+=2 does not skip the character after \ too
                temp[j++] = SLASH;
                i+=2;//skip \ and the character after it
            }
             
        }
        else
            temp[j++] = str[i++]; //Any other case just copy the regex
    }
    temp[j] = '\0';
    i = 0;

    //We replace reg with temp
    while(temp[i] != '\0')
        str[i] = temp[i++];
    str[i] = '\0';
    
    if(flag)//If its a regex, we need an extra step of preprocessing to be done
        preprocessfreq(str, (int)strlen(str));
}

int buildarray(char regarray[][MAX_REGEX_LENGTH], char *str, int n)
{
    /*
        Divdes the regex string into groups depending on location of \|
        Takes as input string str= reg1 \| reg2
        Operation: makes array regarray= {reg1, reg2}
        Returns output: No. of elements in regarray
    */

    int i, j, k;
    i = 0;
    j = 0;
    k = 0;
    while(i < n)
    {
        if(i < (n - 1) && str[i] == SLASH && str[i + 1] == '|') // checking for \|
        {
            i += 2;
            j++;
            regarray[j][k] = '\0';

            k = 0;
        }
        else
            regarray[j][k++] = str[i++];
    }

    return (j + 1); // returning number of groups
}



int CompareString(char temp_reg[], char str[])
{
    char regarray[50][MAX_REGEX_LENGTH];
    int numgroups, i, j;
//    printf("enter regex and string:\n");
//    scanf("%s%s", temp_reg, str);

    /*
        To deal with \| operator, we input the regex string, check for the presence of \|, if present, it implies that we have to check
        whether the string matches either of regex. We call the match function on each regex and do a boolean OR of the result.
        Eg: reg1 \| reg2, forms an array such that regarray = {reg1, reg2}, numgroups=2 and match function is called on reg1 and if it
        failes then on reg2. If one of them passes "found" is printed
    */

    numgroups = buildarray(regarray, temp_reg, (int)strlen(temp_reg));//Gives the number of regexes to implement OR operation
    i = 0;

/*    Multiple regexes separated by or \|*/
    while(i<numgroups)
    {
        //We do a bunch of preprocessing
        preprocess(regarray[i], (int)strlen(regarray[i]), 1);
        preprocess(str, (int)strlen(str), 0);
        
        if(match(regarray[i], str))
        {
            return 1;
        }
        else
        {
            i++;
        }
    }
    //printf("Not found\n");
    return 0;
}

int main()
{
	char strCompare[MAX_STRING_LENGTH], regexCompare[MAX_REGEX_LENGTH];
	printf("Enter the regex expression:");
	gets(regexCompare);

	while(1)
	{
		printf("\nEnter the test string:");
		gets(strCompare);

		if (CompareString(regexCompare, strCompare) == 1)
			printf("Match!");
		else
			printf("no match");
		//break;
	}
	return 0;
}
