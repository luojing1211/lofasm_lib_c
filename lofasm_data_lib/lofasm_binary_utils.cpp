void swap2(char *word)
/* Swap 2 bytes data */
{
	char tmp;
	tmp = word[0];
	word[0] = word[1];
	word[1] = tmp;
}

void swap4(char *word)
/* Swap 4 bytes data */
{
	char tmp;
  	tmp = word[0]; word[0] = word[3]; word[3] = tmp;
  	tmp = word[1]; word[1] = word[2]; word[2] = tmp;
}

void swap8(char *dword)
/* Swap 8 bytes data */
{
  	char tmp;
  	tmp = dword[0]; dword[0] = dword[7]; dword[7] = tmp;
  	tmp = dword[1]; dword[1] = dword[6]; dword[6] = tmp;
  	tmp = dword[2]; dword[2] = dword[5]; dword[5] = tmp;
  	tmp = dword[3]; dword[3] = dword[4]; dword[4] = tmp;
}
