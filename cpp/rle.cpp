#include <iostream>
#include <string>
#include <fstream>

std::string compress(std::string data)
{
    std::string compressed = "";

    char c_seen = data.back();
    data.pop_back();
    int count = 1;

    char c_next;
    while (!data.empty())
    {
        c_next = data.back();
        data.pop_back();
        if (c_next == c_seen)
        {
            count++;
            if (count == 254)
            {
                compressed.push_back(c_seen);
                compressed.push_back(count);
                count = 0;
            }
        }
        else
        {
            compressed.push_back(c_seen);
            compressed.push_back(count);

            c_seen = c_next;
            count = 1;
        }
    }

    compressed.push_back(c_seen);
    compressed.push_back(count);

    return compressed;
}
std::string decompress(std::string data)
{
    std::string decompressed = "";
    char c;
    int count;
    while (!data.empty())
    {
        count = static_cast<unsigned char>(data.back());
        data.pop_back();

        c = data.back();
        data.pop_back();

        for (int i = 0; i < count; i++)
        {
            decompressed.push_back(c);
        }
    
    }

    return decompressed;
}
