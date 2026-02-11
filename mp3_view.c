#include "mp3_header.h"

void view(struct MP3 *mp3)
{
    mp3->org_mp3_fptr = fopen(mp3->mp3_filename, "r");
    if(mp3->org_mp3_fptr == NULL)
    {
        printf("File doesnot exist\n");
        return;
    }
    printf("It is mp3 file\n");
    printf("-------------------------------------------------------------------------------------\n");
    printf("MP3 Tag Reader and Editor for ID3v2\n");
    printf("-------------------------------------------------------------------------------------\n");
    
    //skip 10 bytes of header data
    fseek(mp3->org_mp3_fptr, 10, SEEK_CUR);

    //since we have 6 tags info i = 6
    int i=6;
    while(i)
    {
        //Tag information is being collected and stored in tag
        char tag[5];
        fread(tag, 4, 1, mp3->org_mp3_fptr);
        //adding last character as null character 
        tag[4] = '\0';
        //holds the size of data that file information is storing
        int frame_size;
        //4 bytes of information
        fread(&frame_size, sizeof(int), 1, mp3->org_mp3_fptr);
        //since file data is in bigendian 
        toggle_endianess(mp3, &frame_size);
        //3 bytes are for flag not required
        fseek(mp3->org_mp3_fptr, 3, SEEK_CUR);

        //to get tag name
        int ret = tag_to_name(tag);
        
        if(ret ==0)
        {
            printf("Invalid tag found\n");
            return;
        }
        char ch;
        for(int i=0;i<frame_size-1;i++)
        {
            fread(&ch, 1,1,mp3->org_mp3_fptr);
            if(ch!=0)
                putchar(ch);
        }
        printf("\n");
        i--;
    }
    printf("-------------------------------------------------------------------------------------\n");
}


