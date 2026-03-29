// Probable combinations encoding
#include <iostream>
#include <string>
/*
void appendCompressed(std::string& compressed,std::string possible_comb,int count)
{

    //compressed veriye ekle
    compressed.push_back(static_cast<unsigned char>(possible_comb.length()));//ilk önce kombinasyonun uzunluğu
    compressed.push_back(static_cast<unsigned char>(count));//2.si tekrar sayısı
    for (size_t i = 0; i < possible_comb.length(); i++)
    {
        compressed.push_back(possible_comb[i]);
    }
    std::cout << compressed << std::endl;
}

std::string pce_compress(std::string data)
{
    std::string compressed = "";
    std::string possible_comb = "";
    std::string buffer = "";
    char found_buffer = 31;

    possible_comb.push_back(data.back());
    data.pop_back();
    std::cout << possible_comb << std::endl;

    char c;
    int count = 1;
    while (!data.empty())
    {
        c = data.back();
        data.pop_back();
    std::cout << possible_comb << std::endl;

        if (c == possible_comb.back())
        {
            buffer.clear();
            for (int i = 0; i < strlen(possible_comb.c_str()) - 1; i++)
            {
                c = data.back();
                data.pop_back();

                buffer.push_back(c);
                if (c != possible_comb[possible_comb.length() - 1 - i])
                {
                    for (size_t i = 0; i < buffer.length(); i++)
                    {
                       possible_comb.push_back(buffer[i]);
                    }
                    buffer.clear();
                    break; //while'a geri döndü
                }
            }
            if (!buffer.empty()) //buffer doluysa kombinasyon buldu
            {
                count++;
            }
        }
        else if(possible_comb.find(c) != std::string::npos)
        {
            found_buffer = c;
            for (size_t i = possible_comb.find(c); i < possible_comb.length(); i++)
            {
                c = data.back();
                data.pop_back();

                buffer.push_back(c);
                if (c != possible_comb[i])
                {
                    for (size_t i = 0; i < buffer.length(); i++)
                    {
                       possible_comb.push_back(buffer[i]);
                    }
                    buffer.clear();
                    break; //while'a geri döndü
                }
            }
            
            if (!buffer.empty())
            {
                size_t found_index = possible_comb.find(found_buffer);
                std::string new_comb = "";
                for (size_t i = 0; i < possible_comb.length(); i++)
                {
                    if (i >= found_index) new_comb.push_back(possible_comb.back());
                    possible_comb.pop_back();
                }
                possible_comb = new_comb;

                appendCompressed(compressed,possible_comb,count);
                possible_comb.clear();
                count = 0;
                buffer.clear();
            }
            
        }
        else
        {
            if (!buffer.empty())
            {
                appendCompressed(compressed,possible_comb,count);
                std::cout << compressed << std::endl;
                possible_comb.clear();
                count = 0;
                buffer.clear();
            }
            else
            {
                possible_comb.push_back(c);
            }

        }

        
    }
    std::cout << compressed << std::endl;
    return compressed;
}*/


char getChar(std::string data, size_t &itr)
{
    char c = data[itr];
    itr--;
    return c;
}
std::string pce_compress(std::string data)
{
    int count = 1;
    bool found = false;
    size_t itr = data.length() - 1;
    size_t comp_len = 0;
    std::string possible_comp = "";
    std::string found_comp = "";
    std::string compressed = "";

    char c;

    while (!data.empty())
    {
        c = getChar(data,itr);
        if (c == data[itr + comp_len])
        {
            found = true;
        }


        if (found)
        {
            found_comp.push_back(c);
            while (possible_comp.length() != found_comp.length())
            {
                c = getChar(data, itr);
                found_comp.push_back(c);

                if ( c != data[itr + comp_len] )
                {
                    //does it contain it
                    std::string buffer = "";
                    for (size_t i = 1; i <= possible_comp.length(); i++)
                    {
                        buffer = buffer + data[itr + i];
                        if (c == data[itr + i])
                        {
                            if (count == 1)
                            {
                                std::string buffer2 = "";
                                for (size_t k = i + 1; k < possible_comp.length(); k++)
                                {
                                    buffer2 = buffer2 + possible_comp[k];
                                }
                                if (buffer2.length() > 0)
                                {
                                    compressed.push_back(static_cast<unsigned char>(count));
                                    compressed.push_back(static_cast<unsigned char>(buffer2.length()));
                                    for (size_t l = 0; l < buffer2.length(); l++)
                                    {
                                        compressed.push_back(buffer2[l]);

                                    }
                                    
                                }
                                possible_comp = buffer2;
                                comp_len = i + 1;
                                
                            }
                            else if(count > 1)
                            {
                                compressed.push_back(static_cast<unsigned char>(count));
                                compressed.push_back(static_cast<unsigned char>(possible_comp.length()));
                                for (size_t l = 0; l < possible_comp.length(); l++)
                                {
                                    compressed.push_back(possible_comp[l]);

                                }

                                possible_comp.clear();
                                comp_len = 0;
                            }
                        }
                    }
                    

                    //en son
                    for (size_t i = 0; i < found_comp.length(); i++)
                    {
                        possible_comp.push_back(found_comp.back());
                        comp_len++;
                    }
                    found_comp.clear();

                    break;
                }
            }
            if (possible_comp.length() != found_comp.length())
            {
                
                count++;
                found_comp.clear();


                found = false;
            }

        }
        else
        {
            possible_comp.push_back(c);
            comp_len++;
        }
    }
    

    return compressed;
}