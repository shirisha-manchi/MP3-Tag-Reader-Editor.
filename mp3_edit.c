#include "mp3_header.h"

void edit(struct MP3 *mp3, char * argv[])
{
    //Creating temporary mp3 file
    mp3->dup_mp3_fptr = fopen("temp.mp3", "w");

    //if creating temp.mp3 fails
    if(mp3->dup_mp3_fptr == NULL)
    {
        printf("FILE DOES NOT EXIST\n");
        return;
    }
        
    //opening sample.mp3 in read mode
    mp3->org_mp3_fptr = fopen(mp3->mp3_filename, "r");

    //copy 10 bytes of header from sample.mp3 to temp.mp3
    char header[10];
    fread(header,10,1,mp3->org_mp3_fptr);
    fwrite(header,10,1,mp3->dup_mp3_fptr);

    //transfering remaining 6 tags information
    int i=6;
    while(i)
    {
        //copy 4 bytes of tag from sample.mp3 to temple.mp3
        char tag[5];
        fread(tag,4,1,mp3->org_mp3_fptr);
        fwrite(tag,4,1,mp3->dup_mp3_fptr);
        tag[4] = '\0'; 

        //if tag is not same as user wants to edit
        if(strcmp(mp3->given_tag, tag) != 0)
        {
            //4 bytes of frames size from sample.mp3 to temp.mp3
            int frame_size;
            fread(&frame_size, sizeof(int), 1, mp3->org_mp3_fptr);
            fwrite(&frame_size, sizeof(int), 1,mp3->dup_mp3_fptr);

            //since mp3 has bigendianess data toggling it to littleendian
            toggle_endianess(mp3, &frame_size);

            //3 bytes of flag from sample.mp3 to temp.mp3
            char flag[3];
            fread(flag,3,1,mp3->org_mp3_fptr);
            fwrite(flag,3,1,mp3->dup_mp3_fptr);

            //transfering the frame content based on frame size
            char ch;
            for(int i=0;i<frame_size-1;i++)
            {
                fread(&ch, 1,1,mp3->org_mp3_fptr);
                if(ch!=0)
                    fwrite(&ch,1,1,mp3->dup_mp3_fptr);
            }
        }

        //tag found
        else
        {
            //tag size is given by user (argv[3])
            int new_size = strlen(argv[3])+1;

            //storing frame_size info in bigendianess formatte
            toggle_endianess(mp3, &new_size);
            fwrite(&new_size, sizeof(int), 1,mp3->dup_mp3_fptr);


            int frame_size;
            fread(&frame_size, sizeof(int), 1, mp3->org_mp3_fptr);
            toggle_endianess(mp3, &frame_size);

            //transfering 3 bytes of flag from sample.mp3 to temp.mp3
            char flag[3];
            fread(flag,3,1,mp3->org_mp3_fptr);
            fwrite(flag,3,1,mp3->dup_mp3_fptr);

            //moving offset to framesize times
            fseek(mp3->org_mp3_fptr, frame_size-1, SEEK_CUR);


            //writing user frame content from argv[3] to temp.mp3
            fwrite(argv[3],strlen(argv[3]),1,mp3->dup_mp3_fptr);
            break;
        }
        i--;
    }

    //copying remaining audio data from sample.mp3 to temp.mp3
    copy_remaining_data(mp3);

    //removing sample.mp3
    remove("sample.mp3");
    //renaming temp.mp3 to sample.mp3
    rename("temp.mp3", "sample.mp3");

    //printing successfull editing information
    print_edit(mp3, argv);

    //close the files
    fclose(mp3->dup_mp3_fptr);
    fclose(mp3->org_mp3_fptr);
}

