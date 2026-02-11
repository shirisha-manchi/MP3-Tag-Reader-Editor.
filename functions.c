#include "mp3_header.h"

//CLA VALIDATION
int validate_cla(int argc, char *argv[], struct MP3 *mp3)
{
	//if other than ./a.out if only one CLA is there
    if( argc == 1 )
    {
		printf("------------------------------------------------------------------------------\n");
		printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
		printf("USAGE : To view please pass like: ./a.out -v mp3filename\n");
		printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
		printf("To get help pass like : ./a.out --help\n");
		printf("------------------------------------------------------------------------------\n");
		return 0;
    }

	//if user needs help to know tag information
    if( (strcmp(argv[1], "--help") == 0))
    {
		printf("------------------------------->HELP<-----------------------------------------\n");
		printf("1. -v -> to view mp3 file contents\n");
		printf("2. -e -> to edit mp3 file contents\n");
		printf("\t\t2.1. -t -> to edit song title\n");
		printf("\t\t2.2. -a -> to edit artist name\n");
		printf("\t\t2.3. -A -> to edit album name\n");
		printf("\t\t2.4. -y -> to edit year\n");
		printf("\t\t2.5. -m -> to edit content\n");
		printf("\t\t2.6. -c -> to edit comment\n");
		printf("------------------------------------------------------------------------------\n");
		return 0; 
    }

	// VIEW
    if(!strcmp(argv[1], "-v"))
    {
		if(argc == 3) //CLA must have 3 inputs
		{
			char * ptr = strstr(argv[2],".mp3");
    		if(ptr == NULL)
        		return 0;
    		else
    		{
        		if(strcmp(".mp3",ptr) != 0)
            		return 0;
    		}
			mp3->mp3_filename = argv[2];
			return VIEW;
		}
		else
		{
			printf("------------------------------------------------------------------------------\n");
			printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
			printf("USAGE : To view please pass like: ./a.out -v mp3filename\n");
			printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
			printf("To get help pass like : ./a.out --help\n");
			printf("------------------------------------------------------------------------------\n");
			return 0;
		}
    }
    
	//EDIT
    if(!strcmp(argv[1], "-e"))
    {
		//must have 5 CLA 
		if(argc!=5)
			return 0;
		
		//wrong tag
		if((strcmp(argv[2],"-t")!= 0 )&& (strcmp(argv[2],"-a")!= 0) &&(strcmp(argv[2],"-A")!= 0) && (strcmp(argv[2],"-y")!= 0) && (strcmp(argv[2],"-m")!= 0) && (strcmp(argv[2],"-c")!= 0) )
		{
			return 0;
		}

		//mp3 file validation
		char * ptr = strstr(argv[4],".mp3");
    	if(ptr == NULL)
        	return 0;
    	else
    	{
        	if(strcmp(".mp3",ptr) != 0)
            	return 0;
    	}

		if(strcmp(argv[2],"-t")== 0)
		{
			strcpy(mp3->given_tag, "TIT2");
		}
		else if(strcmp(argv[2],"-a") == 0)
		{
			strcpy(mp3->given_tag, "TPE1");
		}
		else if(strcmp(argv[2],"-A")== 0)
		{
			strcpy(mp3->given_tag, "TALB");
		}
		else if(strcmp(argv[2],"-y")== 0)
		{
			strcpy(mp3->given_tag, "TYER");
		}
		else if(strcmp(argv[2],"-m")== 0)
		{
			strcpy(mp3->given_tag, "TCON");
		}
		else if(strcmp(argv[2],"-c")== 0)
		{
			strcpy(mp3->given_tag, "COMM");
		}
		mp3->mp3_filename = argv[4];
		return EDIT;
	}
	else
	{
		printf("------------------------------------------------------------------------------\n");
		printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
		printf("USAGE : To view please pass like: ./a.out -v mp3filename\n");
		printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
		printf("To get help pass like : ./a.out --help\n");
		printf("------------------------------------------------------------------------------\n");
		return 0;
	}

}

//TOGGLE ENDIANESS
void toggle_endianess(struct MP3 *mp3, int *size)
{
	char * cptr;
	cptr = (char *) size;
	//since always toggling only 4 bytes information
	for(int i=0;i<2;i++)
	{
		char temp = cptr[i];
		cptr[i] = cptr[4-i-1];
		cptr[4-i-1] = temp;
	}

}

//TAG TO NAME
int tag_to_name(char * tag)
{
	if(strcmp(tag,"TIT2")==0)
	{
		printf("Title		:		");
	}
	else if(strcmp(tag,"TPE1")==0)
	{
		printf("Artist		:		");	
	}
	else if(strcmp(tag,"TALB")==0)
	{
		printf("Album		:		");		
	}
	else if(strcmp(tag,"TYER")==0)
	{
		printf("Year		:		");	
	}
	else if(strcmp(tag,"TCON")==0)
	{
		printf("Music		:		");		
	}
	else if(strcmp(tag,"COMM")==0)
	{
		printf("Comment		:		");	
	}
	else
	{
		return 0;
	}
	return 1;
}

//to copy remaining contents
void copy_remaining_data(struct MP3 *mp3)
{
	char ch;
	//scans unit it reaches EOF
	while(fread(&ch,1,1,mp3->org_mp3_fptr) == 1)
	{
		fwrite(&ch,1,1,mp3->dup_mp3_fptr);
	}
}

//After edit to confirm successful editing
void print_edit(struct MP3 *mp3,char * argv[])
{
	printf("-----------------------Select Edit Option -----------------------------\n");
	
	if (strcmp(mp3->given_tag, "TIT2") == 0)
    	printf("-------------------Select Title Change Option -------------------------\n");
	else if (strcmp(mp3->given_tag, "TPE1") == 0)
    	printf("-------------------Select Artist Change Option ------------------------\n");
	else if (strcmp(mp3->given_tag, "TALB") == 0)
    	printf("---------------------Select Album Change Option ----------------------\n");
	else if (strcmp(mp3->given_tag, "TYER") == 0)
    	printf("-------------------Select Year Change Option -------------------------\n");
	else if (strcmp(mp3->given_tag, "TCON") == 0)
    	printf("-------------------Select Music Change Option ------------------------\n");
	else if (strcmp(mp3->given_tag, "COMM") == 0)
    	printf("-------------------Select Comment Change Option ----------------------\n");

	tag_to_name(mp3->given_tag);  
	printf("%s\n", argv[3]);

	if (strcmp(mp3->given_tag, "TIT2") == 0)
    	printf("----------------------Title Changed Successfully------------------------\n");
	else if (strcmp(mp3->given_tag, "TPE1") == 0)
    	printf("-----------------------Artist Changed Successfully----------------------\n");
	else if (strcmp(mp3->given_tag, "TALB") == 0)
    	printf("--------------------Album Changed Successfully--------------------------\n");
	else if (strcmp(mp3->given_tag, "TYER") == 0)
   		printf("--------------------Year Changed Successfully---------------------------\n");
	else if (strcmp(mp3->given_tag, "TCON") == 0)
    	printf("---------------------Music Changed Successfully-------------------------\n");
	else if (strcmp(mp3->given_tag, "COMM") == 0)
    	printf("---------------------Comment Changed Successfully-----------------------\n");
}
