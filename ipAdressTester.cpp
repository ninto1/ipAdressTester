#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;

bool oneFile;
string exec(string command)
{
    char buffer[128];
    string result = "";

    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe))
    {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}
string getIp(int parts)
{
    string result = "";
    int num = 0;
    for (int i = 0; i < parts; i++)
    {
        printf("Please enter the %d. part of the IP:", i + 1);
        scanf("%d", &num);
        stringstream ss;
        ss << num;
        string str = ss.str();
        result += str + ".";
    }
    return result;
}
void writeResultsToFile(int replys[], int currentRep, string ip)
{
    if (!oneFile)
    {
        string filename = ip + "x.dat";
        char fileNameC[17] = {'a'};
        strcpy(fileNameC, filename.c_str());
        FILE *fp = fopen(fileNameC, "w");
        for (int i = 0; i < currentRep; i++)
        {
            string buff = ip;
            for (int k = 0; k < buff.length(); k++)
            { // DEBUG ONLY
                printf("%c", buff[k]);
                fprintf(fp, "%c", buff[k]);
            }
            printf("%d\n", replys[i]);
            fprintf(fp, "%d\n", replys[i]);
        }
        fclose(fp);
    }
    else{
        FILE* bufferFile = fopen("buffer.txt","a+");
        for (int i = 0; i < currentRep; i++)
        {
            string buff = ip;
            for (int k = 0; k < buff.length(); k++)
            { // DEBUG ONLY
                printf("%c", buff[k]);
                fprintf(bufferFile, "%c", buff[k]);
            }
            printf("%d\n", replys[i]);
            fprintf(bufferFile, "%d\n", replys[i]);
        }
        fclose(bufferFile);
    }
}
void finalizeSingleFile(string ip){
    string filename = ip + ".txt";
        char fileNameC[17] = {'a'};
        strcpy(fileNameC, filename.c_str());
    string line;
    //Code from the internet. source: https://pencilprogrammer.com/cpp-programs/copy-one-file-to-another/
    ifstream ini_file {"buffer.dat"};
    ofstream out_file {fileNameC};
 
    if(ini_file && out_file){
 
        while(getline(ini_file,line)){
            out_file << line << "\n";
        }
 
        #//cout << "Copy Finished \n";
 
    } else {
        //Something went wrong
        printf("Cannot read File");
    }
 
    //Closing file
    ini_file.close();
    out_file.close();
    remove("buffer.dat");
}
void ipTester(string ip)
{
    int replys[255] = {0};
    int currentRep = 0;
    string ls;
    for (int i = 1; i < 255; i++)
    {
        stringstream ss;
        ss << i;
        string str = ss.str();
        string buff = ip;
        ls = exec("ping " + buff + str + " -n 1 -w 500");
        if (!(ls.find("Request timed out.") != std::string::npos))
        {
            cout << ip;
            cout << str;
            printf(" succeeded\n", i);
            replys[currentRep] = i;
            currentRep++;
        }
        else
        {
            cout << ip;
            cout << str;
            printf(" failed\n", i);
        }
    }
    writeResultsToFile(replys, currentRep, ip);
}
void ipTestLoop(int ttl, string ip)
{
    if (ttl > 1)
        for (int i = 1; i < 255; i++)
        {
            stringstream ss;
            ss << i;
            string str = ss.str();
            string buff = ip;
            buff += str;
            buff += ".";
            ipTestLoop(ttl - 1, buff);
        }
    else
    {
        ipTester(ip);
    }
}
int main()
{
    printf("Save to seperate files, or one? (1 = one, 0 = multiple):");
    int files = -1;
    while (files < 0 || files > 1)
        scanf("%d",&files);
   if(files)oneFile = true;
   else oneFile = false;
    printf("How many octets of the IP to input (the rest is generated):");
    int ipParts = 0;
    while (ipParts < 1 || ipParts > 3)
        scanf("%d", &ipParts);
    string ip = getIp(ipParts);
    ipTestLoop(4 - ipParts, ip);
    string nameBuff = ip;
    for(int i = 0;i<4-ipParts;i++)nameBuff+="x.";
    if(oneFile)finalizeSingleFile(nameBuff);
    int usefulInt;
    scanf("%d",&usefulInt);
    return 0;
}