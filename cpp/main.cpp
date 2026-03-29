#include <rle.h>
#include <pce.h>
#include <iostream>

int main(int argc, char *argv[])
{

    if (argc == 4)
    {
        if (std::strcmp(argv[1], "rle") == 0)
        {
            if (std::strcmp(argv[2], "compress") == 0)
            {
                FILE *fptr;

                fptr = fopen(argv[3], "rb");
                
                if (fptr == NULL) {
                    std::cerr << "Error: Could not open the file " << argv[3]<< std::endl;
                    return 1; 
                }



                fseek(fptr, 0, SEEK_END);
                long fileSize = ftell(fptr);
                rewind(fptr);

                // Tek seferde oku
                std::string content(fileSize, '\0');
                fread(&content[0], 1, fileSize, fptr);

                fclose(fptr);


                std::cout << compress(content);
            }
            else if (std::strcmp(argv[2], "decompress") == 0)
            {
                FILE *fptr;

                fptr = fopen(argv[3], "rb");
                
                if (fptr == NULL) {
                    std::cerr << "Error: Could not open the file " << argv[3]<< std::endl;
                    return 1; 
                }



                fseek(fptr, 0, SEEK_END);
                long fileSize = ftell(fptr);
                rewind(fptr);

                // Tek seferde oku
                std::string content(fileSize, '\0');
                fread(&content[0], 1, fileSize, fptr);

                fclose(fptr);

                
                std::cout << decompress(content);
            }
            else
            {
                std::cout << "Usage: [compress|decompress] <filename>" << std::endl;
            }
        }
        else if(std::strcmp(argv[1], "pce")==0)
        {
            if (std::strcmp(argv[2], "compress") == 0)
            {
                FILE *fptr;

                fptr = fopen(argv[3], "rb");
                
                if (fptr == NULL) {
                    std::cerr << "Error: Could not open the file " << argv[3]<< std::endl;
                    return 1; 
                }



                fseek(fptr, 0, SEEK_END);
                long fileSize = ftell(fptr);
                rewind(fptr);

                // Tek seferde oku
                std::string content(fileSize, '\0');
                fread(&content[0], 1, fileSize, fptr);

                fclose(fptr);


                std::cout << pce_compress(content);
            }
            else if (std::strcmp(argv[2], "decompress") == 0)
            {
                FILE *fptr;

                fptr = fopen(argv[3], "rb");
                
                if (fptr == NULL) {
                    std::cerr << "Error: Could not open the file " << argv[3]<< std::endl;
                    return 1; 
                }



                fseek(fptr, 0, SEEK_END);
                long fileSize = ftell(fptr);
                rewind(fptr);

                // Tek seferde oku
                std::string content(fileSize, '\0');
                fread(&content[0], 1, fileSize, fptr);

                fclose(fptr);

                
                std::cout << decompress(content);
            }
            else
            {
                std::cout << "Usage: [compress|decompress] <filename>" << std::endl;
            }

        }
        else
        {
        std::cout << "Usage: [rle|dct] [compress|decompress] <filename>" << std::endl;

        }
        
    }
    else
    {
        std::cout << "Usage: [rle|dct] [compress|decompress] <filename>" << std::endl;
    }
}